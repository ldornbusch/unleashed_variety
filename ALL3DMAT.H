// All3DMaterial.h: Schnittstelle für die Klasse CAll3DMaterial.
//
//////////////////////////////////////////////////////////////////////
#include "All3DVec.h"
#include "All3DLig.h"

#if !defined(AFX_ALL3DMATERIAL_H__82CA5163_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_)
#define AFX_ALL3DMATERIAL_H__82CA5163_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++                                                ++++++++
++++       CAllMaterial-Klasse kapselt die Farben und      ++++
++                                                           ++ 
++         Materialien verwaltet TextureMaps, Farben und     ++ 
++                                                           ++ 
++         Material eigenschaften wird von All3dMesh         ++ 
++                                                           ++ 
++           instanziert.                                    ++ 
++++                                                       ++++
+++++++       code: LouyThorn 990826                   ++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

class CAll3DMaterial  
{
public:
	int LightVector(CAll3DVector* Vec, CAll3DLight* Light);
	void SetColor(int R, int G, int B);
	void SetColor(int Col);
	int GetColor();
	CAll3DMaterial();
	virtual ~CAll3DMaterial();

private:
	char * Helligkeit;
	int Color;
};

#endif // !defined(AFX_ALL3DMATERIAL_H__82CA5163_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_)
