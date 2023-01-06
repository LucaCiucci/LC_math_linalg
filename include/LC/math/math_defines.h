#pragma once


// Due to a MSVC bug, we cannot use constexpr function as template parameter for member function whose definition is outside the class, so we need to use macro instead
#ifndef MAX
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#endif // !MAX

#ifndef MIN
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#endif // !MIN

#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

