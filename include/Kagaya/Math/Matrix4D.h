//
//  Matrix4x4.h
//
//  Created by Shenyao Ke on 1/21/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Matrix4x4__
#define __Matrix4x4__

#include "Math/CGVector.h"
#include "Math/Matrix3D.h"

template <typename mat_t>
class Matrix4x4
{
public:
	// Column Major
	mat_t mtx[4][4];
	//mat_t det = 0;

	Matrix4x4() : mtx{}
	{
		//determinant();
	}
	Matrix4x4(const mat_t mat[4][4])
	{
		memcpy(mtx, mat, sizeof(mtx));
	}
	Matrix4x4(const Matrix4x4<mat_t> &mat)
	{
		memcpy(mtx, mat.mtx, sizeof(mat.mtx));
	}
	Matrix4x4(mat_t t00, mat_t t01, mat_t t02, mat_t t03,
		mat_t t10, mat_t t11, mat_t t12, mat_t t13,
		mat_t t20, mat_t t21, mat_t t22, mat_t t23,
		mat_t t30, mat_t t31, mat_t t32, mat_t t33)
	{
		mtx[0][0] = t00; mtx[0][1] = t01; mtx[0][2] = t02; mtx[0][3] = t03;
		mtx[1][0] = t10; mtx[1][1] = t11; mtx[1][2] = t12; mtx[1][3] = t13;
		mtx[2][0] = t20; mtx[2][1] = t21; mtx[2][2] = t22; mtx[2][3] = t23;
		mtx[3][0] = t30; mtx[3][1] = t31; mtx[3][2] = t32; mtx[3][3] = t33;
	}

	Matrix4x4(Vector4D& col0, Vector4D& col1, Vector4D& col2, Vector4D& col3)
	{
		// Column Major
		mtx[0][0] = col0.x;	mtx[0][1] = col0.y;	mtx[0][2] = col0.z;	mtx[0][3] = col0.w;
		mtx[1][0] = col1.x;	mtx[1][1] = col1.y;	mtx[1][2] = col1.z;	mtx[1][3] = col1.w;
		mtx[2][0] = col2.x;	mtx[2][1] = col2.y;	mtx[2][2] = col2.z;	mtx[2][3] = col2.w;
		mtx[3][0] = col3.x;	mtx[3][1] = col3.y;	mtx[3][2] = col3.z;	mtx[3][3] = col3.w;
		//determinant();
	}
	Matrix4x4(mat_t val)
	{
		memset(mtx, val, sizeof(mtx));
		//determinant();
	}
	~Matrix4x4()
	{
	}
	mat_t* operator [] (int i);
	const mat_t* operator [] (int i) const;
	Matrix4x4<mat_t> operator + (const Matrix4x4<mat_t> &mat) const;
	Matrix4x4<mat_t> operator - (const Matrix4x4<mat_t> &mat) const;
	Matrix4x4<mat_t> operator * (const Matrix4x4<mat_t> &mat) const;
	Vector4D<mat_t> operator * (const Vector4D& p) const;
	const Matrix4x4<mat_t> &operator = (const Matrix4x4<mat_t> &mat);
	bool operator == (const Matrix4x4<mat_t> &mat) const;
	bool operator != (const Matrix4x4<mat_t> &mat) const;
	//Vector3D operator * (Vector3D& p) const;
	//Matrix4x4 operator / (const Matrix4x4 &) const;

	void zero();
	void setIdentity();
	void printMat();
	mat_t determinant() const;
	mat_t getMinor(int x, int y) const;
	mat_t getCofactor(int x, int y) const;
	Matrix4x4<mat_t> transposeMat() const;
	Matrix4x4<mat_t> adjointMat() const;
	Matrix4x4<mat_t> inverseMat() const;

	//Set transformation matrix
	friend Matrix4x4<mat_t> identityMat4();
	friend Matrix4x4<mat_t> setTranslation(mat_t tx, mat_t ty, mat_t tz);
	friend Matrix4x4<mat_t> setTranslation(Vector3D& vec);
	friend Matrix4x4<mat_t> setRoationX(mat_t theta);
	friend Matrix4x4<mat_t> setRoationY(mat_t theta);
	friend Matrix4x4<mat_t> setRoationZ(mat_t theta);
	friend Matrix4x4<mat_t> setRotation(mat_t alpha, mat_t beta, mat_t gamma);// in degree
	friend Matrix4x4<mat_t> setScale(mat_t sx, mat_t sy, mat_t sz);
	friend Matrix4x4<mat_t> setScale(mat_t scale);
	friend Matrix4x4<mat_t> setShear(Vector3D& vec);
	friend Matrix4x4<mat_t> setReflection(Vector3D& vec);
	friend Matrix4x4<mat_t> setPerspective(Vector3D& vPnt);
};
template <typename mat_t>
inline mat_t* Matrix4x4::operator[](int i)
{
	return mtx[i];
}
template <typename mat_t>
inline const mat_t* Matrix4x4::operator[](int i)const
{
	return mtx[i];
}
template <typename mat_t>
inline Matrix4x4 Matrix4x4::operator + (const Matrix4x4 &mat) const
{
	Matrix4x4 ret;

	for (int i = 0; i < 16; i++)
	{
		ret.mtx[0][i] = mtx[0][i] + mat.mtx[0][i];
	}

	return ret;
}
template <typename mat_t>
inline Matrix4x4 Matrix4x4::operator - (const Matrix4x4 &mat) const
{
	Matrix4x4 ret;

	for (int i = 0; i < 16; i++)
	{
		ret.mtx[0][i] = mtx[0][i] - mat.mtx[0][i];
	}

	return ret;
}
template <typename mat_t>
inline Matrix4x4 Matrix4x4::operator * (const Matrix4x4 &mat) const
{
	Matrix4x4 buffer;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				buffer.mtx[i][j] += mtx[i][k] * mat.mtx[k][j];
			}
		}
	}
	return buffer;
}
template <typename mat_t>
inline Vector4D Matrix4x4::operator * (const Vector4D& p) const
{
	return Vector4D(
		p.x * mtx[0][0] + p.y * mtx[0][1] + p.z * mtx[0][2] + p.w * mtx[0][3],
		p.x * mtx[1][0] + p.y * mtx[1][1] + p.z * mtx[1][2] + p.w * mtx[1][3],
		p.x * mtx[2][0] + p.y * mtx[2][1] + p.z * mtx[2][2] + p.w * mtx[2][3],
		p.x * mtx[3][0] + p.y * mtx[3][1] + p.z * mtx[3][2] + p.w * mtx[3][3]
		);
}
template <typename mat_t>
inline const Matrix4x4& Matrix4x4::operator = (const Matrix4x4 &mat)
{
	memcpy(mtx, mat.mtx, sizeof(mtx));
	return *this;
}

template <typename mat_t>
inline bool Matrix4x4::operator == (const Matrix4x4 &mat) const
{
	return mtx[0][0] == mat.mtx[0][0]
		&& mtx[0][1] == mat.mtx[0][1]
		&& mtx[0][2] == mat.mtx[0][2]
		&& mtx[0][3] == mat.mtx[0][3]
		&& mtx[1][0] == mat.mtx[1][0]
		&& mtx[1][1] == mat.mtx[1][1]
		&& mtx[1][2] == mat.mtx[1][2]
		&& mtx[1][3] == mat.mtx[1][3]
		&& mtx[2][0] == mat.mtx[2][0]
		&& mtx[2][1] == mat.mtx[2][1]
		&& mtx[2][2] == mat.mtx[2][2]
		&& mtx[2][3] == mat.mtx[2][3]
		&& mtx[3][0] == mat.mtx[3][0]
		&& mtx[3][1] == mat.mtx[3][1]
		&& mtx[3][2] == mat.mtx[3][2]
		&& mtx[3][3] == mat.mtx[3][3];
}

template <typename mat_t>
inline bool Matrix4x4<mat_t>::operator != (const Matrix4x4<mat_t> &mat) const
{
	return mtx[0][0] != mat.mtx[0][0]
		&& mtx[0][1] != mat.mtx[0][1]
		&& mtx[0][2] != mat.mtx[0][2]
		&& mtx[0][3] != mat.mtx[0][3]
		&& mtx[1][0] != mat.mtx[1][0]
		&& mtx[1][1] != mat.mtx[1][1]
		&& mtx[1][2] != mat.mtx[1][2]
		&& mtx[1][3] != mat.mtx[1][3]
		&& mtx[2][0] != mat.mtx[2][0]
		&& mtx[2][1] != mat.mtx[2][1]
		&& mtx[2][2] != mat.mtx[2][2]
		&& mtx[2][3] != mat.mtx[2][3]
		&& mtx[3][0] != mat.mtx[3][0]
		&& mtx[3][1] != mat.mtx[3][1]
		&& mtx[3][2] != mat.mtx[3][2]
		&& mtx[3][3] != mat.mtx[3][3];
}
template <typename mat_t>
void Matrix4x4<mat_t>::zero()
{
	memset(mtx, 0, sizeof(mtx));
}
template <typename mat_t>
inline void Matrix4x4<mat_t>::setIdentity()
{
	*this = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}
template <typename mat_t>
inline void Matrix4x4<mat_t>::printMat()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << mtx[i][j] << "\t";
		}
		cout << endl;
	}
}
template <typename mat_t>
inline mat_t Matrix4x4<mat_t>::determinant() const
{
	mat_t det = 0;
	for (int i = 0; i < 4; i++)
	{
		det += mtx[0][i] * mtx[1][(i + 1) % 4] * mtx[2][(i + 2) % 4] * mtx[3][(i + 3) % 4];
		det -= mtx[3][i] * mtx[2][(i + 1) % 4] * mtx[1][(i + 2) % 4] * mtx[0][(i + 3) % 4];
	}
	return det;
}
template <typename mat_t>
inline mat_t Matrix4x4<mat_t>::getMinor(int x, int y) const
{
	Matrix3D tmpM;
	for (int i = 1; i < 4; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			tmpM.mtx[i][j] = mtx[(x + i) % 4][(y + j) % 4];
		}
	}
	//tmpM.determinant();
	return tmpM.determinant();
}
template <typename mat_t>
inline mat_t Matrix4x4::getCofactor(int x, int y) const
{
	if ((x + y) % 2 == 0)
	{
		return getMinor(x, y);
	}
	else
	{
		return -getMinor(x, y);
	}
}
template <typename mat_t>
Matrix4x4<mat_t> identityMat4()
{
	return Matrix4x4<mat_t>(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}
template <typename mat_t>
inline Matrix4x4<mat_t> Matrix4x4<mat_t>::transposeMat() const
{
	return Matrix4x4<mat_t>(mtx[0][0], mtx[1][0], mtx[2][0], mtx[3][0],
		mtx[0][1], mtx[1][1], mtx[2][1], mtx[3][1],
		mtx[0][2], mtx[1][2], mtx[2][2], mtx[3][2],
		mtx[0][3], mtx[1][3], mtx[2][3], mtx[3][3]);
	/*Matrix4x4<mat_t> buffer;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			buffer.mtx[i][j] = mtx[j][i];
		}
	}
	return buffer;*/
}
template <typename mat_t>
inline Matrix4x4<mat_t> Matrix4x4<mat_t>::adjointMat() const
{
	Matrix4x4<mat_t> ret;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret.mtx[i][j] = getCofactor(j, i);
		}
	}
	//buffer.det = 1.0 / det;
	return ret;
}
template <typename mat_t>
inline Matrix4x4 Matrix4x4::inverseMat() const
{
	Matrix4x4 ret, adjM;
	mat_t det = this->determinant();
	if (det == 0)
	{
		cout << "The matrix is non-inversable!" << endl;
		return Matrix4x4();
	}
	adjM = adjointMat();

	mat_t invDet = 1.0 / det;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret.mtx[i][j] = adjM.mtx[i][j] * invDet;
		}
	}

	return ret;
}

template <typename mat_t>
inline Matrix4x4 setTranslation(Vector3D& vec)
{
	return setTranslation(vec.x, vec.y, vec.z);
}
template <typename mat_t>
inline Matrix4x4 setTranslation(mat_t tx, mat_t ty, mat_t tz)
{
	return Matrix4x4(
		1, 0, 0, tx,
		0, 1, 0, ty,
		0, 0, 1, tz,
		0, 0, 0, 1
		);
}
template <typename mat_t>
inline Matrix4x4 setRoationX(mat_t theta)
{
	Matrix4x4 ret;

	theta = DegreeToRadian(theta);
	mat_t costh = cos(theta);
	mat_t sinth = sin(theta);

	ret.mtx[0][0] = ret.mtx[3][3] = 1.0;
	ret.mtx[1][1] = ret.mtx[2][2] = costh;
	ret.mtx[1][2] = -sinth;
	ret.mtx[2][1] = sinth;

	return ret;
}
template <typename mat_t>
inline Matrix4x4 setRoationY(mat_t theta)
{
	Matrix4x4 ret;

	theta = DegreeToRadian(theta);
	mat_t costh = cos(theta);
	mat_t sinth = sin(theta);

	ret.mtx[1][1] = ret.mtx[3][3] = 1.0;
	ret.mtx[0][0] = ret.mtx[2][2] = costh;
	ret.mtx[2][0] = -sinth;
	ret.mtx[0][2] = sinth;

	return ret;
}
template <typename mat_t>
inline Matrix4x4 setRoationZ(mat_t theta)
{
	Matrix4x4 ret;

	theta = DegreeToRadian(theta);
	mat_t costh = cos(theta);
	mat_t sinth = sin(theta);

	ret.mtx[2][2] = ret.mtx[3][3] = 1.0;
	ret.mtx[0][0] = ret.mtx[1][1] = costh;
	ret.mtx[0][1] = -sinth;
	ret.mtx[1][0] = sinth;

	return ret;
}
template <typename mat_t>
inline Matrix4x4 setRotation(mat_t alpha, mat_t beta, mat_t gamma)
{
	Matrix4x4 ret;
	alpha = DegreeToRadian(alpha);
	beta = DegreeToRadian(beta);
	gamma = DegreeToRadian(gamma);
	mat_t cosA = cos(alpha), sinA = sin(alpha);
	mat_t cosB = cos(beta), sinB = sin(beta);
	mat_t cosG = cos(gamma), sinG = sin(gamma);

	ret.mtx[0][0] = cosA * cosG - cosB * sinA * sinG;
	ret.mtx[0][1] = -cosB * cosG * sinA - cosA * sinG;
	ret.mtx[0][2] = sinA * sinB;
	ret.mtx[1][0] = cosG * sinA + cosA * cosB * sinG;
	ret.mtx[1][1] = cosA * cosB * cosG - sinA * sinG;
	ret.mtx[1][2] = -cosA * sinB;
	ret.mtx[2][0] = sinB * sinG;
	ret.mtx[2][1] = sinB * cosG;
	ret.mtx[2][2] = cosB;
	ret.mtx[3][3] = 1.0;

	return ret;
}
template <typename mat_t>
inline Matrix4x4 setScale(mat_t sx, mat_t sy, mat_t sz)
{
	return Matrix4x4(
		sx, 0.0, 0.0, 0.0,
		0.0, sy, 0.0, 0.0,
		0.0, 0.0, sz, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}
template <typename mat_t>
inline Matrix4x4 setScale(mat_t scale)
{
	return Matrix4x4(
		scale, 0.0, 0.0, 0.0,
		0.0, scale, 0.0, 0.0,
		0.0, 0.0, scale, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}
// inline void Matrix4x4::setShear(Vector3D& vec)
// {
// 	setIdentify();
// 	mtx[0][1] = vec.x;
// 	mtx[1][0] = vec.y;
// 	determinant();
// }
// inline void Matrix4x4::setReflection(Vector3D& vec)
// {
// 	// vec is a vector in the direction of the line
// 	mtx[0][0] = vec.x * vec.x - vec.y * vec.y;	mtx[0][1] = 2 * vec.x * vec.y;
// 	mtx[1][0] = 2 * vec.x * vec.y;	mtx[1][1] = vec.y * vec.y - vec.x * vec.x;
// 	mtx[2][2] = 1;
// 	determinant();
// }
// inline void Matrix4x4::setPerspective(Vector3D& vPnt)
// {
// 	setIdentify();
// 	mtx[2][0] = 1.0 / vPnt.x; mtx[2][1] = 1.0 / vPnt.y;
// }

typedef Matrix4x4<int> Matrix4I;
typedef Matrix4x4<float> Matrix4F;
typedef Matrix4x4<double> Matrix4D;
#endif
