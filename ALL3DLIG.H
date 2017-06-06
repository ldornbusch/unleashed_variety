// All3DLight.h: Schnittstelle für die Klasse CAll3DLight.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALL3DLIGHT_H__ED029061_6E9F_11D3_A6F8_0080AD40DBC7__INCLUDED_)
#define AFX_ALL3DLIGHT_H__ED029061_6E9F_11D3_A6F8_0080AD40DBC7__INCLUDED_

#include "All3DVec.h"    // Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ALL3DLT_AMBIENT			2
#define ALL3DLT_DIRECTIONAL		4
#define ALL3DLT_POINT			8
#define ALL3DLT_SPOT			16

class CAll3DLight  
{
public:
	void Normalize();
	void SetPosition(float x, float y, float z){	Position.SetVector (x,y,z);};
	void SetPosition(CAll3DVector LightPosition){	Position=LightPosition; };
	CAll3DVector GetPosition(){	return Position; };
	void SetDirection(CAll3DVector LightDirection){	Direction=LightDirection; };
	void SetDirection( float x, float y, float z){	Direction.SetVector(x,y,z); };
	CAll3DVector GetDirection(){	return Direction; };
	void SetLightColor(int LightColor){	Color=LightColor; };
	int GetLightColor(){	return Color; };
	void SetType(int LightType){	Type=LightType; };
	int GetType(){	return Type; };
	CAll3DLight();
	virtual ~CAll3DLight();

private:
	int Type;
	int Color;
	CAll3DVector Direction;
	CAll3DVector Position;
};

#endif // !defined(AFX_ALL3DLIGHT_H__ED029061_6E9F_11D3_A6F8_0080AD40DBC7__INCLUDED_)
