#pragma once

#if !defined(__UTILITY_H__)
#define __UTILITY_H__

// Library Includes
#include <strstream>
#include <Windows.h>

// Local Includes

// Types

// Constants

// Prototypes
#define VALIDATE(a) {if (!a) return (false);}
#define DeleteOBJ(x) {if(x) {delete x; x = nullptr;}}

template<typename T> std::string ToString(const T& _value)
{
	std::strstream theStream;
	theStream << _value << std::ends;

	return (theStream.str());
}

#endif // __UTILITY_H__