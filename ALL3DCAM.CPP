// All3DCamera.cpp: Implementierung der Klasse CAll3DCamera.
//
//////////////////////////////////////////////////////////////////////

#include "All3DCam.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DCamera::CAll3DCamera()
{
	m_ScreenX.SetVector(1,0,0);
	m_ScreenY.SetVector(0,1,0);
	m_ScreenZ.SetVector(0,0,1);
	m_Place.SetVector(0,0,0);
	UpdateCamera();
}

CAll3DCamera::~CAll3DCamera()
{

}

void CAll3DCamera::SetX(CAll3DVector Vec)
{	m_ScreenX = Vec;
	UpdateCamera();
}

void CAll3DCamera::SetY(CAll3DVector Vec)
{	m_ScreenY = Vec;
	UpdateCamera();
}

void CAll3DCamera::SetZ(CAll3DVector Vec)
{	m_ScreenZ = Vec;
	UpdateCamera();
}

void CAll3DCamera::SetPlace(CAll3DVector Vec)
{	m_Place = Vec;
	UpdateCamera();
}

CAll3DVector CAll3DCamera::GetX(){ return m_ScreenX ;}
CAll3DVector CAll3DCamera::GetY(){ return m_ScreenY ;}
CAll3DVector CAll3DCamera::GetZ(){ return m_ScreenZ ;}
CAll3DVector CAll3DCamera::GetPlace(){	return m_Place;}
CAll3DMatrix CAll3DCamera::GetCamera(){	return m_Camera;}
void CAll3DCamera::SetCamera(CAll3DMatrix mat)
{	m_Camera=mat;
	UpdateVectors();
}



void CAll3DCamera::UpdateCamera()
{
	m_Camera.Elm[0][0]=m_ScreenX.XVec;m_Camera.Elm[0][1]=m_ScreenY.XVec;m_Camera.Elm[0][2]=m_ScreenZ.XVec;m_Camera.Elm[0][3]=m_Place.XVec ;
	m_Camera.Elm[1][0]=m_ScreenX.YVec;m_Camera.Elm[1][1]=m_ScreenY.YVec;m_Camera.Elm[1][2]=m_ScreenZ.YVec;m_Camera.Elm[1][3]=m_Place.YVec ;
	m_Camera.Elm[2][0]=m_ScreenX.ZVec;m_Camera.Elm[2][1]=m_ScreenY.ZVec;m_Camera.Elm[2][2]=m_ScreenZ.ZVec;m_Camera.Elm[2][3]=m_Place.ZVec ;
	m_Camera.Elm[3][0]=0             ;m_Camera.Elm[3][1]=0             ;m_Camera.Elm[3][2]=0             ;m_Camera.Elm[3][3]=1 ;
}

void CAll3DCamera::UpdateVectors()
{
	m_ScreenX.SetVector(m_Camera.Elm[0][0],
						m_Camera.Elm[1][0],
						m_Camera.Elm[2][0]);
	
	m_ScreenY.SetVector(m_Camera.Elm[0][1],
						m_Camera.Elm[1][1],
						m_Camera.Elm[2][1]);
	
	m_ScreenZ.SetVector(m_Camera.Elm[0][2],
						m_Camera.Elm[1][2],
						m_Camera.Elm[2][2]);

	m_Place.SetVector  (m_Camera.Elm[0][3],
						m_Camera.Elm[1][3],
						m_Camera.Elm[2][3]);
}

void CAll3DCamera::Normalize()
{
	m_ScreenX.Normalize();
	m_ScreenY.Normalize();
	m_ScreenZ.Normalize();
}

CAll3DMatrix CAll3DCamera::GetCameraMatrix()
{
	CAll3DMatrix back;
	back=m_Camera;
	back.Elm [3][0]=0 ;
	back.Elm [3][1]=0 ;
	back.Elm [3][2]=0 ;
	back.Elm [3][3]=1 ;
	return back;
}

void CAll3DCamera::MoveCamera(float Amount, int Axis)
{
	CAll3DVector Displacement;
	switch (Axis)
	{
	case 0:
	{
		Displacement = m_ScreenX * Amount ;
		break;
	}
	case 1:
	{
		Displacement = m_ScreenY * Amount ;
		break;
	}
	case 2:
		Displacement = m_ScreenZ * Amount ;
		break;
	}
	m_Place =m_Place + Displacement ;
	UpdateCamera();
}

void CAll3DCamera::RotateCamera(float Angle, int Axis)
{
	CAll3DMatrix dummy;
	CAll3DVector	u,v;		//Dummy CoordinatenAchsen, die Rotiert werden und dann als Neue Achsen in die Camera-Matrix eingesetz werden.
	dummy.SetRotation (Angle,Axis);
	switch (Axis)
	{
	case 0:		// Um die X-Achse rotieren: Y-Achse und Z-Achse ändern sich
	{
		u.SetVector(dummy.Elm[0][1],dummy.Elm[1][1],dummy.Elm[2][1]);	//Y-Vector
		v.SetVector(dummy.Elm[0][2],dummy.Elm[1][2],dummy.Elm[2][2]);	//Z-Vector
		u=m_Camera * u;
		v=m_Camera *v;			//Neue Vectoren in das Camera-System transformiert
		m_ScreenY = u;
		m_ScreenZ = v;
		break;
	}
	case 1:		// Um die Y-Achse rotieren: X-Achse und Z-Achse ändern sich
	{
		u.SetVector(dummy.Elm[0][0],dummy.Elm[1][0],dummy.Elm[2][0]);	//Y-Vector
		v.SetVector(dummy.Elm[0][2],dummy.Elm[1][2],dummy.Elm[2][2]);	//Z-Vector
		u=m_Camera * u;
		v=m_Camera *v;			//Neue Vectoren in das Camera-System transformiert
		m_ScreenX = u;
		m_ScreenZ = v;
		break;
	}
	case 2:		// Um die Z-Achse rotieren: X-Achse und Y-Achse ändern sich
		u.SetVector(dummy.Elm[0][0],dummy.Elm[1][0],dummy.Elm[2][0]);	//Y-Vector
		v.SetVector(dummy.Elm[0][1],dummy.Elm[1][1],dummy.Elm[2][1]);	//Z-Vector
		u=m_Camera * u;
		v=m_Camera *v;			//Neue Vectoren in das Camera-System transformiert
		m_ScreenX = u;
		m_ScreenY = v;
		break;
	}
	UpdateCamera();
}
