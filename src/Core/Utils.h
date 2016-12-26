#pragma once
#include "Kaguya.h"

namespace Utils
{

bool endsWith(const string &src, const string &ext, bool isCaseSensitive = true)
{
    // true if both strings are empty
    if (src.length() == 0) return ext.length() == 0;

    int pos = src.length() - ext.length();
    // true if second string is shorter than first
    if (pos < 0) return false;

    // directly compare two string if case sensitive
    if (isCaseSensitive)
    {
        return src.compare(pos, ext.length(), ext) == 0;
    }
    else
    {
        for (int i = 0; i < ext.length(); i++)
        {
            if (tolower(src[pos + i]) != tolower(ext[i]))
            {
                return false;
            }
        }
    }
    return true;
}

}
