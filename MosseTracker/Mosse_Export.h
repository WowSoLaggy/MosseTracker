#pragma once

#ifndef MOSSEEXPORTS_H
#define MOSSEEXPORTS_H


#ifdef MOSSETRACKER_EXPORTS
#define MOSSE_API extern "C" __declspec(dllexport)
#else
#define MOSSE_API extern "C" __declspec(dllimport)
#endif


MOSSE_API void Mosse_Init();

MOSSE_API void Mosse_OnFrame();

MOSSE_API void Mosse_Dispose();


#endif // MOSSEEXPORTS_H
