//
//  Matrix3D.h
//
//  Created by Shenyao Ke on 1/21/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __Matrix3D__
#define __Matrix3D__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT
#include "Math/CGVector.h"
#include "Core/MemoryControl.h"

class Matrix3D
{
public:
	// Column Major
	Float mtx[3][3];
	//Float det = 0;

	Matrix3D() : mtx{}
	{
		//Determinant();
	}
	Matrix3D(const Float mat[3][3])
	{
		memcpy(mtx, mat, sizeof(mtx));
	}
	Matrix3D(const Matrix3D &mat)
	{
		memcpy(mtx, mat.mtx, sizeof(mtx));
	}
	Matrix3D(Float t00, Float t01, Float t02,
		Float t10, Float t11, Float t12,
		Float t20, Float t21, Float t22)
	{
		mtx[0][0] = t00; mtx[0][1] = t01; mtx[0][2] = t02;
		mtx[1][0] = t10; mtx[1][1] = t11; mtx[1][2] = t12;
		mtx[2][0] = t20; mtx[2][1] = t21; mtx[2][2] = t22;
	}
	Matrix3D(Vector3D &col0, Vector3D &col1, Vector3D &col2)
	{
		mtx[0][0] = col0.x;	mtx[0][1] = col0.y;	mtx[0][2] = col0.z;
		mtx[1][0] = col1.x;	mtx[1][1] = col1.y;	mtx[1][2] = col1.z;
		mtx[2][0] = col2.x;	mtx[2][1] = col2.y;	mtx[2][2] = col2.z;
		//Determinant();
	}
	Matrix3D(Float val)// : mtx{ { val } }
	{
		memset(mtx, val, sizeof(mtx));
		//Determinant();
	}
	~Matrix3D()
	{
		//delete_2DArray(mtx, 3, 3);
	}
	Float* operator [] (int i);
	const Float* operator [] (int i) const;
	Matrix3D operator + (const Matrix3D &mat) const;
	Matrix3D operator - (const Matrix3D &mat) const;
	Matrix3D operator * (const Matrix3D &mat) const;
	Vector3D operator * (Vector3D &p) const;
	Matrix3D operator = (const Matrix3D &mat) const;
	Matrix3D operator == (const Matrix3D &mat) const;
	Matrix3D operator != (const Matrix3D &mat) const;
	//Vector2D operator * (Vector2D& p) const;
	//Matrix3D operator / (const Matrix3D &) const;

	void setIdentity() { mtx[0][0] = mtx[1][1] = mtx[2][2] = 1.0; }
	void printMat();
	Float determinant() const;
	Matrix3D transposeMat() const;
	Matrix3D adjointMat() const;
	Matrix3D inverseMat() const;

	//Set transformation matrix
	void setTranslation(const Vector2D& vec);
	void setRotation(Float theta);
	void setRotation(Float sinth, Float costh);
	void setScale(Float sx, Float sy);
	void setScale(Float scale);
	void setShear(const Vector2D& vec);
	void setReflection(const Vector2D& vec);
	void setPerspective(const Vector2D& vPnt);
};
inline Float* Matrix3D::operator[](int i)
{
	return mtx[i];
}
inline const Float* Matrix3D::operator[](int i)const
{
	return mtx[i];
}
inline Matrix3D Matrix3D::operator + (const Matrix3D &mat) const
{
	Matrix3D buffer;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			buffer.mtx[i][j] = mtx[i][j] + mat.mtx[i][j];
		}
	}

	return buffer;
}
inline Matrix3D Matrix3D::operator - (const Matrix3D &mat) const
{
	Matrix3D buffer;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			buffer.mtx[i][j] = mtx[i][j] - mat.mtx[i][j];
		}
	}

	return buffer;
}
inline Matrix3D Matrix3D::operator * (const Matrix3D &mat) const
{
	Matrix3D buffer;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				buffer.mtx[i][j] += mtx[i][k] * mat.mtx[k][j];
			}
		}
	}
	//buffer.Determinant();
	return buffer;
}
inline Vector3D Matrix3D::operator * (Vector3D &p) const
{
	return Vector3D(p.x * mtx[0][0] + p.y * mtx[0][1] + p.z * mtx[0][2],
		p.x * mtx[1][0] + p.y * mtx[1][1] + p.z * mtx[1][2],
		p.x * mtx[2][0] + p.y * mtx[2][1] + p.z * mtx[2][2]);
}
/*
inline Vector2D Matrix3D::operator * (Vector2D& p) const
{
	return Vector2D(p.x * mtx[0][0] + p.y * mtx[0][1] + mtx[0][2],
		p.x * mtx[1][0] + p.y * mtx[1][1] + mtx[1][2]);
}*/
inline void Matrix3D::printMat() {
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << mtx[i][j] << "\t";
		}
		cout << endl;
	}
}
inline Float Matrix3D::determinant() const
{
	Float det = 0;
	for (int i = 0; i < 3; i++)
	{
		det += mtx[0][i] * mtx[1][(i + 1) % 3] * mtx[2][(i + 2) % 3];
		det -= mtx[2][i] * mtx[1][(i + 1) % 3] * mtx[0][(i + 2) % 3];
	}
	return det;
}
inline Matrix3D Matrix3D::transposeMat() const
{
	return Matrix3D(mtx[0][0], mtx[1][0], mtx[2][0],
		mtx[0][1], mtx[1][1], mtx[2][1],
		mtx[0][2], mtx[1][2], mtx[2][2]);
/*
	Matrix3D buffer;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			buffer.mtx[i][j] = mtx[j][i];
		}
	}
	return buffer;
*/
}
inline Matrix3D Matrix3D::adjointMat() const
{
	Matrix3D buffer, trpM;// trpM is transposed matrix of origin matrix.

	trpM = transposeMat();
	int coeff = -1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			coeff *= -1;
			buffer.mtx[i][j] = coeff * (trpM.mtx[(i + 1) % 3][(j + 1) % 3] * trpM.mtx[(i + 2) % 3][(j + 2) % 3]
									- trpM.mtx[(i + 1) % 3][(j + 2) % 3] * trpM.mtx[(i + 2) % 3][(j + 1) % 3]);
		}
	}
	//buffer.det = 1.0 / det;
	return buffer;
}
inline Matrix3D Matrix3D::inverseMat() const
{
	Matrix3D buffer, adjM;
	Float det = this->determinant();
	if (det == 0)
	{
		cout << "The matrix is non-inversable!" << endl;
		return Matrix3D();
	}
	adjM = adjointMat();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			buffer.mtx[i][j] = adjM.mtx[i][j] / det;
		}
	}
	//buffer.Determinant();
	return buffer;
}
inline void Matrix3D::setTranslation(const Vector2D& vec)
{
	setIdentity();
	mtx[0][2] = vec.x;
	mtx[1][2] = vec.y;
	//Determinant();
}
inline void Matrix3D::setRotation(Float theta)
{
	if (cos(theta) ==  1)
	{
		setIdentity();
		return;
	}
	setRotation(sin(DegreeToRadian(theta)), cos(DegreeToRadian(theta)));
}
inline void Matrix3D::setRotation(Float sinth, Float costh)
{
	mtx[0][0] = costh;	mtx[0][1] = -sinth;
	mtx[1][0] = sinth;	mtx[1][1] = costh;
	mtx[2][2] = 1;
	//Determinant();
}
inline void Matrix3D::setScale(Float sx, Float sy)
{
	mtx[0][0] = sx;	mtx[1][1] = sy; mtx[2][2] = 1;
	//Determinant();
}
inline void Matrix3D::setScale(Float scale)
{
	mtx[0][0] = scale;	mtx[1][1] = scale; mtx[2][2] = 1;
	//Determinant();
}
inline void Matrix3D::setShear(const Vector2D& vec)
{
	setIdentity();
	mtx[0][1] = vec.x;
	mtx[1][0] = vec.y;
	//Determinant();
}
inline void Matrix3D::setReflection(const Vector2D& vec)
{
	// vec is a vector in the direction of the line
	mtx[0][0] = vec.x * vec.x - vec.y * vec.y;	mtx[0][1] = 2 * vec.x * vec.y;
	mtx[1][0] = 2 * vec.x * vec.y;	mtx[1][1] = vec.y * vec.y - vec.x * vec.x;
	mtx[2][2] = 1;
	//Determinant();
}
inline void Matrix3D::setPerspective(const Vector2D& vPnt)
{
	setIdentity();
	mtx[2][0] = 1.0 / vPnt.x; mtx[2][1] = 1.0 / vPnt.y;
}
#endif
