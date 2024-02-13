#pragma once

#include <cmath>
#include <iostream>
#include <vector>

// https://github.com/jeremyfix/FFTConvolution/blob/master/Convolution/src/convolution_std.h

namespace STD_Convolution
{

    template<typename T>
    std::vector<T> conv_linear_full(std::vector<T> const& src, std::vector<T> const& kernel)
    {
        const int h_src = static_cast<int>(src.size());
        constexpr int w_src = 1; // For 1-dimensional vectors
        const int h_kernel = static_cast<int>(kernel.size());
        constexpr int w_kernel = 1; // For 1-dimensional kernels
        std::vector<T> dst;

        const int h_dst = h_src + h_kernel - 1;
        const int w_dst = w_src + w_kernel - 1;

        if (h_dst > 0 && w_dst > 0)
            dst.resize(h_dst * w_dst);

        // Full linear convolution of size N + M -1
        for (int i = 0; i < h_dst; ++i)
        {
            const int low_k = std::max(0, i - h_kernel + 1);
            const int high_k = std::min(h_src - 1, i);
            for (int j = 0; j < w_dst; ++j)
            {
                const int low_l = std::max(0, j - w_kernel + 1);
                const int high_l = std::min(w_src - 1, j);
            	double temp = 0.0;
                for (int k = low_k; k <= high_k; ++k)
                {
                    for (int l = low_l; l <= high_l; ++l)
                    {
                        temp += src[k * w_src + l] * kernel[(i - k) * w_kernel + (j - l)];
                    }
                }
                dst[i * w_dst + j] = temp;
            }
        }

        return dst;
    }

    template<typename T>
    std::vector<T> conv_linear_same(std::vector<T> const& src, std::vector<T> const& kernel)
    {
        const int h_src = static_cast<int>(src.size());
        constexpr int w_src = 1; // For 1-dimensional vectors
        const int h_kernel = static_cast<int>(kernel.size());
        constexpr int w_kernel = 1; // For 1-dimensional kernels
        std::vector<T> dst;

        const int h_dst = h_src;
        constexpr int w_dst = w_src;

        if (h_dst > 0 && w_dst > 0)
            dst.resize(h_dst * w_dst);

        // Same linear convolution, of size N
        for (int i = 0; i < h_dst; ++i)
        {
            const int low_k = std::max(0, i - static_cast<int>((h_kernel - 1.0) / 2.0));
            const int high_k = std::min(h_src - 1, i + static_cast<int>(h_kernel / 2.0));
            for (int j = 0; j < w_dst; ++j)
            {
                const int low_l = std::max(0, j - static_cast<int>((w_kernel - 1.0) / 2.0));
                const int high_l = std::min(w_src - 1, j + static_cast<int>(w_kernel / 2.0));
                double temp = 0.0;
                for (int k = low_k; k <= high_k; ++k)
                {
                    for (int l = low_l; l <= high_l; ++l)
                    {
                        temp += src[k * w_src + l] * kernel[(i - k + static_cast<int>(h_kernel / 2.0)) * w_kernel + (j - l + static_cast<int>(w_kernel / 2.0))];
                    }
                }
                dst[i * w_dst + j] = temp;
            }
        }

        return dst;
    }

    template<typename T>
    std::vector<T> conv_linear_valid(std::vector<T> const& src, std::vector<T> const& kernel)
    {
        const int h_src = static_cast<int>(src.size());
        constexpr int w_src = 1; // For 1-dimensional vectors
        const int h_kernel = static_cast<int>(kernel.size());
        constexpr int w_kernel = 1; // For 1-dimensional kernels
        std::vector<T> dst;

        const int h_dst = h_src - h_kernel + 1;
        constexpr int w_dst = w_src - w_kernel + 1;

        if (h_dst > 0 && w_dst > 0)
            dst.resize(h_dst * w_dst);

        for (int i = 0; i < h_dst; ++i)
        {
            for (int j = 0; j < w_dst; ++j)
            {
                double temp = 0.0;
                for (int k = i; k <= i + h_kernel - 1; ++k)
                {
                    for (int l = j; l <= j + w_kernel - 1; ++l)
                    {
                        temp += src[k * w_src + l] * kernel[(i + h_kernel - 1 - k) * w_kernel + (j + w_kernel - 1 - l)];
                    }
                }
                dst[i * w_dst + j] = temp;
            }
        }

        return dst;
    }

    template<typename T>
    std::vector<T> conv_circular_same(std::vector<T> const& src, std::vector<T> const& kernel)
    {
        const int h_src = static_cast<int>(src.size());
        constexpr int w_src = 1; // For 1-dimensional vectors
        const int h_kernel = static_cast<int>(kernel.size());
        constexpr int w_kernel = 1; // For 1-dimensional kernels
        std::vector<T> dst;

        const int h_dst = h_src;
        constexpr int w_dst = w_src;

        if (h_dst > 0 && w_dst > 0)
            dst.resize(h_dst * w_dst);

        // We suppose the filter has a size at most the size of the image
        for (int i = 0; i < h_dst; ++i)
        {
            for (int j = 0; j < w_dst; ++j)
            {
                double temp = 0.0;
                // We browse the kernel
                for (int k = 0; k < h_kernel; ++k)
                {
                    int i_src = (i - k) % h_dst;
                    if (i_src < 0)
                        i_src += h_dst;

                    for (int l = 0; l < w_kernel; ++l)
                    {
                        int j_src = (j - l) % w_dst;
                        if (j_src < 0)
                            j_src += w_dst;
                        if (i_src >= 0 && i_src < h_src
                            && j_src >= 0 && j_src < w_src)
                            temp += src[i_src * w_src + j_src] * kernel[k * w_kernel + l];
                    }
                }
                dst[i * w_dst + j] = temp;
            }
        }

        return dst;
    }

    template<typename T>
    std::vector<T> conv_circular_full(std::vector<T> const& src, std::vector<T> const& kernel)
    {
        const int h_src = static_cast<int>(src.size());
        constexpr int w_src = 1; // For 1-dimensional vectors
        const int h_kernel = static_cast<int>(kernel.size());
        constexpr int w_kernel = 1; // For 1-dimensional kernels
        std::vector<T> dst;

        const int h_dst = h_src + h_kernel - 1;
        constexpr int w_dst = w_src + w_kernel - 1;

        if (h_dst > 0 && w_dst > 0)
            dst.resize(h_dst * w_dst);

        // We suppose the filter has a size at most the size of the image
        for (int i = 0; i < h_dst; ++i)
        {
            for (int j = 0; j < w_dst; ++j)
            {
                double temp = 0.0;
                // We browse the kernel
                for (int k = 0; k < h_kernel; ++k)
                {
                    int i_src = (i - k) % h_dst;
                    if (i_src < 0)
                        i_src += h_dst;

                    for (int l = 0; l < w_kernel; ++l)
                    {
                        int j_src = (j - l) % w_dst;
                        if (j_src < 0)
                            j_src += w_dst;
                        if (i_src >= 0 && i_src < h_src
                            && j_src >= 0 && j_src < w_src)
                            temp += src[i_src * w_src + j_src] * kernel[k * w_kernel + l];
                    }
                }
                dst[i * w_dst + j] = temp;
            }
        }

        return dst;
    }
}
