#pragma once

#include <iostream>
#include <string>



#ifndef NDEBUG
	#ifndef SE_MALGE_DEBUG
		#define SE_MALGE_DEBUG
	#endif
#else
	#ifndef SE_MALGE_NO_DEBUG
		#define SE_MALGE_NO_DEBUG
	#endif
#endif


#ifdef SE_MALGE_ASSERT
	#undef SE_MALGE_ASSERT
#endif


#ifdef SE_MALGE_DEBUG
	#ifndef SE_MALGE_DISABLE_ASSERT
		#define SE_MALGE_ASSERT(cond, msg) if (!(cond)) {\
		std::cout << "SE-malge assert in " << __FILE__ << ":" << __LINE__ << " : " << msg << std::endl;\
		throw std::string(msg);}
	#else
		#define SE_MALGE_ASSERT(cond, msg)
	#endif
#else
	#define SE_MALGE_ASSERT(cond, msg)
#endif