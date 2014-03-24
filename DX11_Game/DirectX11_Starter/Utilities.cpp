#include "Utilities.h"

// Initialize the static device variables
ID3D11Device* DeviceManager::_currentDevice = nullptr;
ID3D11DeviceContext* DeviceManager::_currentDeviceContext = nullptr;

// Initialize timer's variables
__int64 DebugTimer::frequency = 0ULL;
__int64 DebugTimer::start = 0ULL;
__int64 DebugTimer::stop = 0ULL;
std::wstring DebugTimer::name = L"";

// Start the high performance timer
void DebugTimer::Start(std::wstring timerName){
	name = timerName;

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	frequency = freq.QuadPart;

	LARGE_INTEGER s;
	QueryPerformanceCounter(&s);
	start = s.QuadPart;
};

// Stop the timer and log the 
void DebugTimer::Stop(){
	LARGE_INTEGER s;
	QueryPerformanceCounter(&s);
	stop = s.QuadPart;

	__int64 diff = stop - start;
	double seconds = (double)diff / (double)frequency;
	LOG(name, L": ", std::to_wstring(seconds*1000.0f), L"ms");
};

// Debug logging
#if defined(DEBUG) | defined(_DEBUG)
void LogString(std::wstring s){
	std::wcout << s << std::endl;
};
void LogString(std::wstring s1, std::wstring s2){
	std::wcout << s1 << s2 << std::endl;
};
void LogString(std::wstring s1, std::wstring s2, std::wstring s3){
	std::wcout << s1 << s2 << s3 << std::endl;
};
void LogString(std::wstring s1, std::wstring s2, std::wstring s3, std::wstring s4){
	std::wcout << s1 << s2 << s3 << s4 << std::endl;
};
void LogString(std::wstring s1, std::wstring s2, std::wstring s3, std::wstring s4, std::wstring s5){
	std::wcout << s1 << s2 << s3 << s4 << s5 << std::endl;
};
void LogString(std::wstring s1, std::wstring s2, std::wstring s3, std::wstring s4, std::wstring s5, std::wstring s6){
	std::wcout << s1 << s2 << s3 << s4 << s5 << s6 << std::endl;
};
#endif

// Used to generate random numbers
std::random_device RNG::rd;
std::default_random_engine RNG::rnd;

// Initialize the RNG engine
void RNG::Init(){
	rnd = std::default_random_engine(rd());
};

// Returns a random int between min and max, 
// default paramaters are INT_MIN and INT_MAX
int RNG::randInt(int min, int max){
	std::uniform_int_distribution<int> uInt(min, max);
	return uInt(rnd);
};

// Returns a random float between min and max,
// default parameters are FLT_MIN and FLT_MAX
float RNG::randFloat(float min, float max){
	rnd = std::default_random_engine(rd());
	std::uniform_real_distribution<float> flt(min,max);
	return flt(rnd);
};

// Returns a random double between min and max,
// default parameters are DBL_MIN and DBL_MAX
double RNG::randDouble(double min, double max){
	rnd = std::default_random_engine(rd());
	std::uniform_real_distribution<double> dbl(min,max);
	return dbl(rnd);
};