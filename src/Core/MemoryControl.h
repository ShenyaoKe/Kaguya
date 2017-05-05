#pragma once

#include <iostream>
//using namespace std;

namespace Kaguya
{

/*
a[0]  a[1]  a[2]
0     3     6
1     4     7
2     5     8
*/

template <typename T>
class AlignedArray2D
{
public:
	AlignedArray2D() : mRows(0), mCols(0), mRawPtr(nullptr) {}
	AlignedArray2D(size_t rows, size_t cols, bool runConstructor = false);
	~AlignedArray2D()
	{
		release();
	}

	T* operator[](size_t i)
	{
		return mDataPtr[i];
	}
	const T* operator[](size_t i) const
	{
		return mDataPtr[i];
	}

	T &operator()(size_t xpos, size_t ypos)
	{
		return mDataPtr[ypos][xpos];
	}

	const T &operator()(size_t xpos, size_t ypos) const
	{
		return mDataPtr[ypos][xpos];
	}

	void release();
	void allocate(size_t rows, size_t cols);
	bool cloneData(const AlignedArray2D<T> &other);

private:
	size_t mRows, mCols;
	union
	{
		char* mRawPtr;
		T** mDataPtr;
	};
};


template <typename T>
AlignedArray2D<T>::AlignedArray2D(size_t rows, size_t cols, bool runConstructor)
{
	// Allocate data
	allocate(rows, cols);

	if (runConstructor)
	{
		for (size_t i = 0; i < mRows * mCols; i++)
		{
			mDataPtr[0][i] = T();
		}
	}
}

template <typename T>
void Kaguya::AlignedArray2D<T>::release()
{
	if (!mRawPtr)
	{
		delete[] mRawPtr;
		mRows = mCols = 0;
	}
}

template<typename T>
inline void AlignedArray2D<T>::allocate(size_t rows, size_t cols)
{
	if (rows == mRows && cols == mCols)
	{
		return;
	}
	// Reset allocation
	delete[] mRawPtr;
	mRows = rows;
	mCols = cols;

	size_t ptr_offset = mRows * sizeof(void*);
	size_t data_length = mRows * mCols * sizeof(T);
	mRawPtr = new char[ptr_offset + data_length];

	// Assign 2D pointer to data address
	*mDataPtr = (T*)(mRawPtr + ptr_offset);
	for (size_t i = 1; i < mRows; i++)
	{
		mDataPtr[i] = mDataPtr[i - 1] + mCols;
	}
}

template <typename T>
bool Kaguya::AlignedArray2D<T>::cloneData(const AlignedArray2D<T> &other)
{
	if (mRows != other.mRows || mCols != other.mCols)
	{
		return false;
	}
	memcpy(mDataPtr[0], other.mDataPtr[0], mRows * mCols * sizeof(T));
	return true;
}

}
