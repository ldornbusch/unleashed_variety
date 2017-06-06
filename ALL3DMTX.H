// All3DMatrix.h: Schnittstelle für die Klasse CAll3DMatrix.
//
//////////////////////////////////////////////////////////////////////
#include "All3DVec.h"

#if !defined(AFX_ALL3DMATRIX_H__030C0DC1_6953_11D3_A6F8_0080AD40DBC7__INCLUDED_)
#define AFX_ALL3DMATRIX_H__030C0DC1_6953_11D3_A6F8_0080AD40DBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAll3DMatrix  
{
public:
	CAll3DMatrix TransformationOnly();
	CAll3DMatrix RotationOnly();
	void SetProjection(float AUGPUNKT);
	void SetTranslation(CAll3DVector place);
	void SetRotation(float w, int axis);
	void ResetMatrix();
	CAll3DMatrix operator *(CAll3DMatrix M2)
	{
		CAll3DMatrix temp;
		for (int y=0;y<4;y++)
			for (int x=0;x<4; x++)
			{
				temp.Elm[y][x]=0;
				for (int a=0;a<4;a++)
					temp.Elm[y][x]+= Elm[y][a] * M2.Elm[a][x];
			}
	return temp;
	}
	CAll3DVector operator *(CAll3DVector vec)
	{
		CAll3DVector back;
		back.XVec= vec.XVec * Elm[0][0] + vec.YVec * Elm[0][1] + vec.ZVec *Elm[0][2] +vec.WVec * Elm[0][3] ;
		back.YVec= vec.XVec * Elm[1][0] + vec.YVec * Elm[1][1] + vec.ZVec *Elm[1][2] +vec.WVec * Elm[1][3] ;
		back.ZVec= vec.XVec * Elm[2][0] + vec.YVec * Elm[2][1] + vec.ZVec *Elm[2][2] +vec.WVec * Elm[2][3] ;
		back.WVec= vec.XVec * Elm[3][0] + vec.YVec * Elm[3][1] + vec.ZVec *Elm[3][2] +vec.WVec * Elm[3][3] ;
		return back;
	}
	float Elm[4][4];
	CAll3DMatrix();
	virtual ~CAll3DMatrix();

};

#endif // !defined(AFX_ALL3DMATRIX_H__030C0DC1_6953_11D3_A6F8_0080AD40DBC7__INCLUDED_)
