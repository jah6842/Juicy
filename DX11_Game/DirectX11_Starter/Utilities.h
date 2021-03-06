/* Assorted utilities that don't need their own file */

#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <Windows.h>
#include "dxerr.h"
#include <d3d11.h>
#include <string>
#include <iostream>
#include <random>
#include "DeviceManager.h"

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>

	// Visual Leak Detector is much better than the standard leak detector
	// but doesn't compile on most of the lab computers. Uncomment this line
	// and comment the above lines to use VLD. If recommiting this file, leave
	// this line commented out.
	//#include <vld.h>
#endif

// Convenience macro for releasing a COM object
// Use this for releasing GPU resources.
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
		}														\
	}														
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif

// A timer that uses QueryPerformanceCounter
class DebugTimer {
	static __int64 frequency;
	static __int64 start;
	static __int64 stop;
	static std::wstring name;
public:
	// Starts the timer
	static void Start(std::wstring timerName);
	// Stops the timer and prints the time it took to execute
	static void Stop();
};

// Macro for debug logging
// In debug builds will log to console
// In release builds will replace with a NO-OP
#if defined(DEBUG) | defined(_DEBUG)
	#define LOG LogString													
#else
	#define LOG __noop
#endif

// Debug logging
#if defined(DEBUG) | defined(_DEBUG)
void LogString(std::wstring s);
void LogString(std::wstring s1, std::wstring s2);
void LogString(std::wstring s1, std::wstring s2, std::wstring s3);
void LogString(std::wstring s1, std::wstring s2, std::wstring s3, std::wstring s4);
void LogString(std::wstring s1, std::wstring s2, std::wstring s3, std::wstring s4, std::wstring s5);
void LogString(std::wstring s1, std::wstring s2, std::wstring s3, std::wstring s4, std::wstring s5, std::wstring s6);
#endif

// A class to provide easy random number generation
class RNG {
public:
	// Used to generate random numbers
	static std::random_device rd;
	static std::default_random_engine rnd;
	
	// Initialize the RNG engine
	static void Init();

	// Returns a random int between min and max, 
	// default paramaters are INT_MIN and INT_MAX
	static int randInt(int min = INT_MIN, int max = INT_MAX);

	// Returns a random float between min and max,
	// default parameters are FLT_MIN and FLT_MAX
	static float randFloat(float min = FLT_MIN, float max = FLT_MAX);

	// Returns a random double between min and max,
	// default parameters are DBL_MIN and DBL_MAX
	static double randDouble(double min = DBL_MIN, double max = DBL_MAX);
};

#endif // _UTILITIES_H
