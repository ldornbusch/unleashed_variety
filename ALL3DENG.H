// All3DEngine.h: Schnittstelle für die Klasse CAll3DEngine.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALL3DENGINE_H__054FA904_734F_11D3_B895_0000E8699C86__INCLUDED_)
#define AFX_ALL3DENGINE_H__054FA904_734F_11D3_B895_0000E8699C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "All3DObj.h"
#include "All3DMtx.h"    // Hinzugefügt von der Klassenansicht
#include "All3DLig.h"
#include "All3DCam.h"
#include "BitMap.h"
#include "Point.h"
#define boolean unsigned char
class CAll3DEngine  
{
public:
	CAll3DEngine(int x, int y);
	void SetRenderSurface(CBitMap* rSur);
	CBitMap* GetRenderSurface();
	float MouseY;
	float MouseX;
	void ShowObject(CAll3DObject* VecObj);
	CAll3DEngine();
	virtual ~CAll3DEngine();

private:
	bool bOwnSurface;
	CBitMap* RenderSurface;
	Point All3DProjection(CAll3DVector vec, CAll3DMatrix mat);
	boolean FaceVisible(CAll3DVector PlaceVec, CAll3DVector DirVec, CAll3DCamera Cam, CAll3DMatrix Mat);
};

#endif // !defined(AFX_ALL3DENGINE_H__054FA904_734F_11D3_B895_0000E8699C86__INCLUDED_)
