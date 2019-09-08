#pragma once
#include "Core/Kaguya.h"

class ConsoleProgressBar
{
public:
	ConsoleProgressBar() : mBarStr{ ' ' }, mCursorIdx(0)
	{
		reset();//mBarStr.back() = '\0';
	}

	void reset(const char token = ' ')
	{
		mBarStr.fill(token);
		//mBarStr.back() = '\0';
		mCursorIdx = 0;
	}

	void print(float percent)
	{
		size_t newIndex = percent < 0 ? 0
			: percent > 1.0f ? cBarSize : percent * 50;

		if (newIndex >= mCursorIdx)
		{
			//size_t replaceCount = newIndex - mCursorIdx;
			//mBarStr.replace(mCursorIdx, replaceCount, replaceCount, '=');
			std::fill(mBarStr.begin() + mCursorIdx, mBarStr.begin() + newIndex, '=');
		}
		else
		{
			//size_t replaceCount = mCursorIdx - newIndex;
			//mBarStr.replace(newIndex + 1, replaceCount, replaceCount, ' ');
			std::fill(mBarStr.begin() + newIndex + 1, mBarStr.begin() + mCursorIdx, ' ');
		}
		mBarStr[newIndex] = '>';
		mCursorIdx = newIndex;

		std::cout << "\r" "[" << mBarStr.data() << "] ";
		std::cout.width(6);
		std::cout << std::fixed << std::setprecision(2) << percent * 100 << "%    " << std::flush;
	}

	void complete()
	{
		reset('=');
		std::cout << "\r" "[" << mBarStr.data() << "] Completed!    "
			<< std::flush << std::endl;

		reset();
	}

private:
	static const size_t cBarSize = 51;

	std::array<char, cBarSize> mBarStr;
	//std::string mBarStr;
	size_t mCursorIdx;
};
