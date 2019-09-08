#pragma once
#include "Kaguya.h"

namespace Kaguya
{

namespace Utils
{

inline bool endsWith(const std::string &src, const std::string &ext,
					 bool isCaseSensitive = true)
{
	// true if both strings are empty
	if (src.length() == 0) return ext.length() == 0;

	size_t srcLen = src.length();
	size_t extLen = ext.length();
	// true if second string is shorter than first
	if (srcLen < extLen)
	{
		return false;
	}
	size_t pos = srcLen - extLen;

	// directly compare two string if case sensitive
	if (isCaseSensitive)
	{
		return src.compare(pos, ext.length(), ext) == 0;
	}
	else
	{
		for (size_t i = 0; i < ext.length(); i++)
		{
			if (tolower(src[pos + i]) != tolower(ext[i]))
			{
				return false;
			}
		}
	}
	return true;
}

inline bool startsWith(const char* src, const char* prefix,
					   bool /*isCaseSensitive*/ = true)
{

	/*if (isCaseSensitive)
	{
	do
	{
	if (*src != *prefix)
	{
	return false;
	}
	} while (*++src && *++prefix);
	}
	else
	{
	do
	{
	if (tolower(*src) != tolower(*prefix))
	{
	return false;
	}
	} while (*++src && *++prefix);
	}

	return *prefix == '\0';*/
	return strcmp(src, prefix) == 0;
}

}

}
