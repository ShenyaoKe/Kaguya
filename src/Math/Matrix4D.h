//
//  Matrix4D.h
//
//  Created by Shenyao Ke on 1/21/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Matrix4D__
#define __Matrix4D__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Core/Kaguya.h"
#include "Math/CGVector.h"
#include "Math/Matrix3D.h"

class Matrix4D
{
public:
	// Column Major
	Float mtx[4][4];
	//Float det = 0;

	Matrix4D() : mtx{}
	{
		//determinant();
	}
	Matrix4D(const Float mat[4][4])
	{
		memcpy(mtx, mat, sizeof(mtx));
	}
	Matrix4D(const Matrix4D &mat)
	{
		memcpy(mtx, mat.mtx, sizeof(mtx));
	}
	Matrix4D(Float t00, Float t01, Float t02, Float t03,
		Float t10, Float t11, Float t12, Float t13,
		Float t20, Float t21, Float t22, Float t23,
		Float t30, Float t31, Float t32, Float t33)
	{
		mtx[0][0] = t00; mtx[0][1] = t01; mtx[0][2] = t02; mtx[0][3] = t03;
		mtx[1][0] = t10; mtx[1][1] = t11; mtx[1][2] = t12; mtx[1][3] = t13;
		mtx[2][0] = t20; mtx[2][1] = t21; mtx[2][2] = t22; mtx[2][3] = t23;
		mtx[3][0] = t30; mtx[3][1] = t31; mtx[3][2] = t32; mtx[3][3] = t33;
	}

	Matrix4D(Vector4D& col0, Vector4D& col1, Vector4D& col2, Vector4D& col3)
	{
		// Column Major
		mtx[0][0] = col0.x;	mtx[0][1] = col0.y;	mtx[0][2] = col0.z;	mtx[0][3] = col0.w;
		mtx[1][0] = col1.x;	mtx[1][1] = col1.y;	mtx[1][2] = col1.z;	mtx[1][3] = col1.w;
		mtx[2][0] = col2.x;	mtx[2][1] = col2.y;	mtx[2][2] = col2.z;	mtx[2][3] = col2.w;
		mtx[3][0] = col3.x;	mtx[3][1] = col3.y;	mtx[3][2] = col3.z;	mtx[3][3] = col3.w;
		//determinant();
	}
	Matrix4D(Float val)
	{
		memset(mtx, val, sizeof(mtx));
		//determinant();
	}
	~Matrix4D()
	{
	}
	Float* operator [] (int i);
	const Float* operator [] (int i) const;
	Matrix4D operator + (const Matrix4D &mat) const;
	Matrix4D operator - (const Matrix4D &mat) const;
	Matrix4D operator * (const Matrix4D &mat) const;
	Vector4D operator * (const Vector4D& p) const;
	const Matrix4D &operator = (const Matrix4D &mat);
	bool operator == (const Matrix4D &mat) const;
	bool operator != (const Matrix4D &mat) const;
	//Vector3D operator * (Vector3D& p) const;
	//Matrix4D operator / (const Matrix4D &) const;

	void zero();
	void setIdentity();
	void printMat();
	Float determinant() const;
	Float getMinor(int x, int y) const;
	Float getCofactor(int x, int y) const;
	Matrix4D transposeMat() const;
	Matrix4D adjointMat() const;
	Matrix4D inverseMat() const;

	//Set transformation matrix
	friend Matrix4D identityMat4();
	friend Matrix4D setTranslation(Float tx, Float ty, Float tz);
	friend Matrix4D setTranslation(Vector3D& vec);
	friend Matrix4D setRoationX(Float theta);
	friend Matrix4D setRoationY(Float theta);
	friend Matrix4D setRoationZ(Float theta);
	friend Matrix4D setRotation(Float alpha, Float beta, Float gamma);// in degree
	friend Matrix4D setScale(Float sx, Float sy, Float sz);
	friend Matrix4D setScale(Float scale);
	friend Matrix4D setShear(Vector3D& vec);
	friend Matrix4D setReflection(Vector3D& vec);
	friend Matrix4D setPerspective(Vector3D& vPnt);

	template <typename vbo_t>
	friend void exportVBO(const Matrix4D &mat, vbo_t *vtx_array);
};
inline Float* Matrix4D::operator[](int i)
{
	return mtx[i];
}
inline const Float* Matrix4D::operator[](int i)const
{
	return mtx[i];
}
inline Matrix4D Matrix4D::operator + (const Matrix4D &mat) const
{
	Matrix4D ret;

	for (int i = 0; i < 16; i++)
	{
		ret.mtx[0][i] = mtx[0][i] + mat.mtx[0][i];
	}

	return ret;
}
inline Matrix4D Matrix4D::operator - (const Matrix4D &mat) const
{
	Matrix4D ret;

	for (int i = 0; i < 16; i++)
	{
		ret.mtx[0][i] = mtx[0][i] - mat.mtx[0][i];
	}

	return ret;
}
inline Matrix4D Matrix4D::operator * (const Matrix4D &mat) const
{
	Matrix4D buffer;

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
inline Vector4D Matrix4D::operator * (const Vector4D& p) const
{
	return Vector4D(
		p.x * mtx[0][0] + p.y * mtx[0][1] + p.z * mtx[0][2] + p.w * mtx[0][3],
		p.x * mtx[1][0] + p.y * mtx[1][1] + p.z * mtx[1][2] + p.w * mtx[1][3],
		p.x * mtx[2][0] + p.y * mtx[2][1] + p.z * mtx[2][2] + p.w * mtx[2][3],
		p.x * mtx[3][0] + p.y * mtx[3][1] + p.z * mtx[3][2] + p.w * mtx[3][3]
		);
}

inline const Matrix4D& Matrix4D::operator = (const Matrix4D &mat)
{
	memcpy(mtx, mat.mtx, sizeof(mtx));
	return *this;
}


inline bool Matrix4D::operator == (const Matrix4D &mat) const
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


inline bool Matrix4D::operator != (const Matrix4D &mat) const
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

inline void Matrix4D::zero()
{
	memset(mtx, 0, sizeof(mtx));
}

inline void Matrix4D::setIdentity()
{
	*this = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
}

inline void Matrix4D::printMat()
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
inline Float Matrix4D::determinant() const
{
	Float det = 0;
	for (int i = 0; i < 4; i++)
	{
		det += mtx[0][i] * mtx[1][(i + 1) % 4] * mtx[2][(i + 2) % 4] * mtx[3][(i + 3) % 4];
		det -= mtx[3][i] * mtx[2][(i + 1) % 4] * mtx[1][(i + 2) % 4] * mtx[0][(i + 3) % 4];
	}
	return det;
}
inline Float Matrix4D::getMinor(int x, int y) const
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
inline Float Matrix4D::getCofactor(int x, int y) const
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


inline Matrix4D identityMat4()
{
	return Matrix4D(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

inline Matrix4D Matrix4D::transposeMat() const
{
	return Matrix4D(mtx[0][0], mtx[1][0], mtx[2][0], mtx[3][0],
		mtx[0][1], mtx[1][1], mtx[2][1], mtx[3][1],
		mtx[0][2], mtx[1][2], mtx[2][2], mtx[3][2],
		mtx[0][3], mtx[1][3], mtx[2][3], mtx[3][3]);
	Matrix4D buffer;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			buffer.mtx[i][j] = mtx[j][i];
		}
	}
	return buffer;
}
inline Matrix4D Matrix4D::adjointMat() const
{
	Matrix4D ret;

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
inline Matrix4D Matrix4D::inverseMat() const
{
	Matrix4D ret, adjM;
	Float det = this->determinant();
	if (det == 0)
	{
		cout << "The matrix is non-inversable!" << endl;
		return Matrix4D();
	}
	adjM = adjointMat();

	Float invDet = 1.0 / det;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret.mtx[i][j] = adjM.mtx[i][j] * invDet;
		}
	}

	return ret;
}

template <typename vbo_t>
void exportVBO(const Matrix4D &mat, vbo_t *vtx_array)
{
	if (sizeof(Float) == sizeof(vbo_t))
	{
		memcpy(vtx_array, mat.mtx[0], sizeof(mat.mtx));
	}
	else
	{
		for (int i = 0; i < 16; i++)
		{
			vtx_array[i] = static_cast<vbo_t>(mat.mtx[0][i]);
		}
	}
}

inline Matrix4D setTranslation(Vector3D& vec)
{
	return setTranslation(vec.x, vec.y, vec.z);
}
inline Matrix4D setTranslation(Float tx, Float ty, Float tz)
{
	return Matrix4D(
		1.0, 0.0, 0.0, tx,
		0.0, 1.0, 0.0, ty,
		0.0, 0.0, 1.0, tz,
		0.0, 0.0, 0.0, 1.0
		);
}
inline Matrix4D setRoationX(Float theta)
{
	Matrix4D ret;

	theta = DegreeToRadian(theta);
	Float costh = cos(theta);
	Float sinth = sin(theta);

	ret.mtx[0][0] = ret.mtx[3][3] = 1.0;
	ret.mtx[1][1] = ret.mtx[2][2] = costh;
	ret.mtx[1][2] = -sinth;
	ret.mtx[2][1] = sinth;

	return ret;
}

inline Matrix4D setRoationY(Float theta)
{
	Matrix4D ret;

	theta = DegreeToRadian(theta);
	Float costh = cos(theta);
	Float sinth = sin(theta);

	ret.mtx[1][1] = ret.mtx[3][3] = 1.0;
	ret.mtx[0][0] = ret.mtx[2][2] = costh;
	ret.mtx[2][0] = -sinth;
	ret.mtx[0][2] = sinth;

	return ret;
}

inline Matrix4D setRoationZ(Float theta)
{
	Matrix4D ret;

	theta = DegreeToRadian(theta);
	Float costh = cos(theta);
	Float sinth = sin(theta);

	ret.mtx[2][2] = ret.mtx[3][3] = 1.0;
	ret.mtx[0][0] = ret.mtx[1][1] = costh;
	ret.mtx[0][1] = -sinth;
	ret.mtx[1][0] = sinth;

	return ret;
}

inline Matrix4D setRotation(Float alpha, Float beta, Float gamma)
{
	Matrix4D ret;
	alpha = DegreeToRadian(alpha);
	beta = DegreeToRadian(beta);
	gamma = DegreeToRadian(gamma);
	Float cosA = cos(alpha), sinA = sin(alpha);
	Float cosB = cos(beta), sinB = sin(beta);
	Float cosG = cos(gamma), sinG = sin(gamma);

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
inline Matrix4D setScale(Float sx, Float sy, Float sz)
{
	return Matrix4D(
		sx, 0.0, 0.0, 0.0,
		0.0, sy, 0.0, 0.0,
		0.0, 0.0, sz, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}
inline Matrix4D setScale(Float scale)
{
	return Matrix4D(
		scale, 0.0, 0.0, 0.0,
		0.0, scale, 0.0, 0.0,
		0.0, 0.0, scale, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}
// inline void Matrix4D::setShear(Vector3D& vec)
// {
// 	setIdentify();
// 	mtx[0][1] = vec.x;
// 	mtx[1][0] = vec.y;
// 	determinant();
// }
// inline void Matrix4D::setReflection(Vector3D& vec)
// {
// 	// vec is a vector in the direction of the line
// 	mtx[0][0] = vec.x * vec.x - vec.y * vec.y;	mtx[0][1] = 2 * vec.x * vec.y;
// 	mtx[1][0] = 2 * vec.x * vec.y;	mtx[1][1] = vec.y * vec.y - vec.x * vec.x;
// 	mtx[2][2] = 1;
// 	determinant();
// }
// inline void Matrix4D::setPerspective(Vector3D& vPnt)
// {
// 	setIdentify();
// 	mtx[2][0] = 1.0 / vPnt.x; mtx[2][1] = 1.0 / vPnt.y;
// }
template void exportVBO<float>(const Matrix4D &, float *);
//template void exportVBO(const Matrix4D &, double *);
#endif
