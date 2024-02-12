#pragma once

#include <cmath>
#include <iostream>

// https://github.com/jeremyfix/FFTConvolution/blob/master/Convolution/src/convolution_std.h

namespace STD_Convolution
{

    typedef enum
    {
        LINEAR_FULL,
        LINEAR_SAME,
        LINEAR_VALID,
        CIRCULAR_SAME,
        CIRCULAR_FULL
    } Convolution_Mode;

    typedef struct Workspace
    {
        int h_src, w_src;
        int h_kernel, w_kernel;
        Convolution_Mode mode;
        double* dst;
        int h_dst, w_dst;
    } Workspace;

    void init_workspace(Workspace& ws, Convolution_Mode mode, int h_src, int w_src, int h_kernel, int w_kernel);
    void clear_workspace(Workspace& ws);
    void update_workspace(Workspace& ws, Convolution_Mode mode, int h_src, int w_src, int h_kernel, int w_kernel);
    void convolve(Workspace& ws, double* src, double* kernel);
}
