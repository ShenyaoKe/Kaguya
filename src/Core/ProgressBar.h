#pragma once
#include "Core/Kaguya.h"

class ConsoleProgressBar
{
public:
	ConsoleProgressBar() : mBarStr(cBarSize, ' '), mCursorIdx(0) {}

	void reset(const char token = ' ')
	{
		mBarStr.assign(cBarSize, token);
		mCursorIdx = 0;
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

	void complete()
	{
		reset('=');
		std::cout << "\r" "[" << mBarStr << "] Completed!    "
			<< std::flush << std::endl;

		reset();
	}

private:
	static const size_t cBarSize = 51;

	std::string mBarStr;
	size_t mCursorIdx;
};
