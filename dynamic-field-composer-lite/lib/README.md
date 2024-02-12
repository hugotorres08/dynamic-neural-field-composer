# Using discrete Fourier transform (DFT) for kernel convolution

Instead of using the standard nested for loops to compute convolution products we could try and use FFT!

The implementations for the std_convolution and the fft_convolution are taken out of [FFTConvolution](https://github.com/jeremyfix/FFTConvolution/tree/master). 

To use the fft_convolution you must download the libraries here [fftw.org](https://www.fftw.org/).

If you are using Windows (like me) refer to this link [fftw.org/windows](https://www.fftw.org/install/windows.html).

In my case what I had to do was:
- Download the 64-bit version;
- Create a import library .lib using lib /machine:x64 /def:libfftw3l-3.def (I had to define the machine explicitly or else it defaulted to x86);
- Add the necessary configurations to CMakeLists.txt. As follows:
```cmake
# Setup fftw3
set(LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/${DNF_COMPOSER_LIB_PREFIX}")
file(COPY ${CMAKE_SOURCE_DIR}/lib/libfftw3-3.dll DESTINATION ${LIB_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC "libfftw3-3.lib")
```
