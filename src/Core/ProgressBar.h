#pragma once
#include "Core/Kaguya.h"

class ConsoleProgressBar
{
public:
	ConsoleProgressBar() : mBarStr(cBarSize, ' '), mCursorIdx(0) {}

	void reset()
	{
		mBarStr.assign(cBarSize, ' ');
	}

	void print(float percent)
	{
		size_t newIndex = percent < 0 ? 0
			: percent > 1.0f ? cBarSize : percent * 50;

		if (newIndex >= mCursorIdx)
		{
			size_t replaceCount = newIndex - mCursorIdx;
			mBarStr.replace(mCursorIdx, replaceCount, replaceCount, '=');
		}
		else
		{
			size_t replaceCount = mCursorIdx - newIndex;
			mBarStr.replace(newIndex + 1, replaceCount, replaceCount, ' ');
		}
		mBarStr[newIndex] = '>';
		mCursorIdx = newIndex;

		std::cout << "\r" "[" << mBarStr << "] ";
		std::cout.width(6);
		std::cout << std::fixed << std::setprecision(2) << percent * 100 << "%    " << std::flush;
	}

private:
	static const size_t cBarSize = 51;

	std::string mBarStr;
	size_t mCursorIdx;
};
