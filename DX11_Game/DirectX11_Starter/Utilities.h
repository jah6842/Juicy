/* Assorted utilities that don't need their own file */

#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <Windows.h>
#include "dxerr.h"
#include <d3d11.h>
#include <string>
#include <iostream>

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

// Static class to manage DirectX devices and contexts
// For our purposes, we'll probably only have one device and one context.
class DeviceManager {
private:
	static ID3D11Device* _currentDevice;
	static ID3D11DeviceContext* _currentDeviceContext;
public:
	static ID3D11Device* GetCurrentDevice(){
		return _currentDevice;
	};
	static ID3D11DeviceContext* GetCurrentDeviceContext(){
		return _currentDeviceContext;
	};

	static void SetCurrentDevice(ID3D11Device* device){
		_currentDevice = device;
	};
	static void SetCurrentDeviceContext(ID3D11DeviceContext* deviceContext){
		_currentDeviceContext = deviceContext;
	};
};

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

#endif // _UTILITIES_H
