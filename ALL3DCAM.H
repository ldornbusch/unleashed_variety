// All3DCamera.h: Schnittstelle für die Klasse CAll3DCamera.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALL3DCAMERA_H__054FA903_734F_11D3_B895_0000E8699C86__INCLUDED_)
#define AFX_ALL3DCAMERA_H__054FA903_734F_11D3_B895_0000E8699C86__INCLUDED_

#include "All3DVec.h"    // Hinzugefügt von der Klassenansicht
#include "All3DMtx.h"    // Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAll3DCamera  
{
public:
	void RotateCamera(float Angle, int Axis);
	void MoveCamera(float Amount, int Axis);
	CAll3DMatrix GetCameraMatrix();
	void SetPlace(CAll3DVector Vec);
	CAll3DVector GetPlace();
	void Normalize();
	void SetZ(CAll3DVector Vec);
	void SetY(CAll3DVector Vec);
	void SetX(CAll3DVector Vec);
	CAll3DVector GetX();
	CAll3DVector GetY();
	CAll3DVector GetZ();
	void SetCamera(CAll3DMatrix mat);
	CAll3DMatrix GetCamera();

	virtual ~CAll3DCamera();
	CAll3DCamera();

private:
	CAll3DMatrix m_Camera;
	CAll3DVector m_ScreenX;
	CAll3DVector m_ScreenY;
	CAll3DVector m_ScreenZ;
	CAll3DVector m_Place;
	void UpdateVectors();
	void UpdateCamera();
};

#endif // !defined(AFX_ALL3DCAMERA_H__054FA903_734F_11D3_B895_0000E8699C86__INCLUDED_)
