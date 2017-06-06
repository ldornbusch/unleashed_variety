// All3DVector.h: Schnittstelle für die Klasse CAll3DVector.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALL3DVECTOR_H__82CA5162_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_)
#define AFX_ALL3DVECTOR_H__82CA5162_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++                                                ++++++++
++++       CAll3DVector-Klasse kapselt die Koordinaten     ++++
++                                                           ++ 
++           einfache Vector-Klasse, mit float Membern       ++ 
++                                                           ++ 
++           Vectoren können addiert, subtrahiert, multi-    ++ 
++                                                           ++ 
++           pliziert, skaliert usw werden.                  ++ 
++++                                                       ++++
+++++++       code: LouyThorn 990826                   ++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

*/

class CAll3DVector  
{
public:
	void Normalize();
	float CalcLenght();
	void SetVector(float x, float y, float z){
		XVec=x;
		YVec=y;
		ZVec=z;
		WVec=1;
	};

	CAll3DVector operator+ (CAll3DVector b)
	{
		CAll3DVector back;
		back.XVec = XVec + b.XVec;
		back.YVec = YVec + b.YVec;
		back.ZVec = ZVec + b.ZVec;
		return back;
	}
	CAll3DVector operator- (CAll3DVector b)
	{
		CAll3DVector back;
		back.XVec = XVec - b.XVec;
		back.YVec = YVec - b.YVec;
		back.ZVec = ZVec - b.ZVec;
		return back;
	}
	CAll3DVector operator* (float s)		//Skalar-Multiplikation
	{
		CAll3DVector back;
		back.XVec = XVec*s;
		back.YVec = YVec*s;
		back.ZVec = ZVec*s;
		return back;
	}
	float operator* (CAll3DVector V2)		//Vektorprodukt
	{
		float back;
		back = XVec*V2.XVec + YVec*V2.YVec + ZVec*V2.ZVec;
		return back;
	}
	CAll3DVector operator/ (CAll3DVector V2)	//Kreuz-Multiplikation
	{
		CAll3DVector temp;
		temp.XVec = YVec*V2.ZVec - ZVec*V2.YVec;
		temp.YVec = ZVec*V2.XVec - XVec*V2.ZVec;
		temp.ZVec = XVec*V2.YVec - YVec*V2.XVec;
		XVec=temp.XVec;
		YVec=temp.YVec;
		ZVec=temp.ZVec;
		WVec=1;
		return temp;
	}
	float Lenght;

	float XVec;
	float YVec;
	float ZVec;
	float WVec;
	
	CAll3DVector();
	virtual ~CAll3DVector();

};

#endif // !defined(AFX_ALL3DVECTOR_H__82CA5162_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_)
