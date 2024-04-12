#pragma once



#ifdef __APPLE__
	#define SE_MALGE_REQUIRES(...)
#else
	#define SE_MALGE_REQUIRES(...) requires __VA_ARGS__
#endif
