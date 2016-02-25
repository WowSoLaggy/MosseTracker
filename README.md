# MosseTracker
MOSSE tracker used to track a single object on the frame sequence

Algorythm is based on the paper: "Visual Object Tracking using Adaptive Correlation Filters" by David S. Bolme, J. Ross Beveridge, Bruce A. Draper, Yui Man Lui, Computer Science Department, Colorado State University. Fast Fourier Transform algorythm was taken from the freeware source (written by Miroslav Voinarovsky) with insignificant modifications by me.

Compiled via MS Visual Studio 2015. Uses std and Windows.h. Should be modified for Lin-support. Uses OpenCV only to output results. The algorythm itself doesn't require OpenCV.

Structure:

1. MosseTracker project (dll)
  1.1. MosseTracker module
    1.1.1. MosseTracker.h
    1.1.2. MosseTracker.cpp       - contains main tracker functions - initialization and onFrame
    1.1.3. MosseTracker_Misc.cpp  - contains misc tracker functions
  1.2. Export section
    1.2.1. Mosse_Export.h   - contains declaration of exported functions
    1.2.2. Mosse_Export.cpp - contains definition of exported functions, controls MosseTracker object lifetime
  1.3. FFT - Fast Fourier Transform
    1.3.1. Fft.h
    1.3.2. Fft.cpp
    1.3.3. Fft_arrays.h - contains large magic arrays used for FFT calculations

2. MosseTest (exe)
  2.1. Tests
    2.1.1. TestFromFile - runs MosseTracker test with the images loaded frimfrom files

Anton Egorov, 2016 dohxehapo@gmail.com
