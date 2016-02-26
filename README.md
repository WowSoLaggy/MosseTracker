# MosseTracker

#### Description

MOSSE tracker used to track a single object on the frame sequence

Algorithm is based on the paper: "Visual Object Tracking using Adaptive Correlation Filters" by David S. Bolme, J. Ross Beveridge, Bruce A. Draper, Yui Man Lui, Computer Science Department, Colorado State University.
Fast Fourier Transform algorithm was taken from the freeware source (written by Miroslav Voinarovsky) with insignificant modifications by me.

Compiled via MS Visual Studio 2015. Uses std and Windows.h. Should be modified for Lin-support.
Uses OpenCV only to output results. The algorithm itself doesn't require OpenCV.
The usage of fftw can be defined instead of FFT by Miroslav Voinarovsky.

FPS is calculated via QueryPerformanceCounter. FPS is about 7000 for the 64x64 rectangle of interest (with fftw) or about 1600 without it.
Warning: FPS calculated in TestFromFile can be inaccurate. It is better to estimate FPS via TestOneFrame. Suggestions for more accurate FPS counting are welcome.

#### Structure

1. MosseTracker project (dll)
  1. MosseTracker module
    1. MosseTracker.h
    2. MosseTracker.cpp       - contains main tracker functions - initialization and onFrame
    3. MosseTracker_Misc.cpp  - contains misc tracker functions
	4. MosseTracker_Debug.cpp - contains some debug functions
  2. Export section
    1. Mosse_Export.h   - contains declaration of exported functions
    2. Mosse_Export.cpp - contains definition of exported functions, controls MosseTracker object lifetime
  3. FFT - Fast Fourier Transform
    1. Fft.h
    2. Fft.cpp
    3. Fft_arrays.h - contains large magic arrays used for FFT calculations

2. MosseTest (exe)
  1. Tests
    1. TestFromFile - runs MosseTracker test with the images loaded from files
	2. TestOneFrame	- runs test with only one frame (and one for the initialization). It processes this frame 5000 times and counts the total time
	
3. fftw distrib
  1. fftw3.h
  2. libfftw3f-3.dll
  3. libfftw3f-3.lib
  
4. Test image sequences


Anton Egorov, 2016 dohxehapo@gmail.com
