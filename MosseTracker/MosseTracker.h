// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MOSSETRACKER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MOSSETRACKER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MOSSETRACKER_EXPORTS
#define MOSSETRACKER_API __declspec(dllexport)
#else
#define MOSSETRACKER_API __declspec(dllimport)
#endif

// This class is exported from the MosseTracker.dll
class MOSSETRACKER_API CMosseTracker {
public:
	CMosseTracker(void);
	// TODO: add your methods here.
};

extern MOSSETRACKER_API int nMosseTracker;

MOSSETRACKER_API int fnMosseTracker(void);
