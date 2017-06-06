// All3DMatrix.cpp: Implementierung der Klasse CAll3DMatrix.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "All3DMtx.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DMatrix::CAll3DMatrix()
{
	ResetMatrix();
}

CAll3DMatrix::~CAll3DMatrix()
{

}

void CAll3DMatrix::ResetMatrix()	// Einheitsmatrix setzen
{
	Elm[0][0]= 1;	Elm[0][1]= 0;	Elm[0][2]= 0;	Elm[0][3]= 0;	
	Elm[1][0]= 0;	Elm[1][1]= 1;	Elm[1][2]= 0;	Elm[1][3]= 0;	
	Elm[2][0]= 0;	Elm[2][1]= 0;	Elm[2][2]= 1;	Elm[2][3]= 0;	
	Elm[3][0]= 0;	Elm[3][1]= 0;	Elm[3][2]= 0;	Elm[3][3]= 1;	
}

void CAll3DMatrix::SetRotation(float w, int axis)
{
	ResetMatrix();
	double a=cos((double)w);
	double b=sin((double)w);
	switch (axis)
	{
	case 0:
	{
		Elm[1][1]=(float) a;	Elm[1][2]=(float)b;	
		Elm[2][1]=(float)-b;	Elm[2][2]=(float)a;	
		break;
	}
	case 1:
	{
		Elm[0][0]=(float) a;	Elm[0][2]=(float)b;	
		Elm[2][0]=(float)-b;	Elm[2][2]=(float)a;	
		break;
	}
	case 2:
		Elm[0][0]=(float) a;	Elm[0][1]=(float)b;	
		Elm[1][0]=(float)-b;	Elm[1][1]=(float)a;	
		break;
	}
}

void CAll3DMatrix::SetTranslation(CAll3DVector place)
{
	Elm[0][3]=place.XVec;
	Elm[1][3]=place.YVec;
	Elm[2][3]=place.ZVec;
}

void CAll3DMatrix::SetProjection(float AUG)
{
	Elm[0][0]= 1;	Elm[0][1]= 0;	Elm[0][2]= 0;			Elm[0][3]= 0;	
	Elm[1][0]= 0;	Elm[1][1]= 1;	Elm[1][2]= 0;			Elm[1][3]= 0;	
	Elm[2][0]= 0;	Elm[2][1]= 0;	Elm[2][2]= 1;			Elm[2][3]= 0;	
	Elm[3][0]= 0;	Elm[3][1]= 0;	Elm[3][2]= 1/AUG;	Elm[3][3]= 1;	
}


CAll3DMatrix CAll3DMatrix::RotationOnly()
{
	CAll3DMatrix back;
	back=*this;
	back.Elm [0][3]=0;
	back.Elm [1][3]=0;
	back.Elm [2][3]=0;
	back.Elm [3][3]=1;
	return back;
}

CAll3DMatrix CAll3DMatrix::TransformationOnly()
{
	CAll3DMatrix back;
	back.Elm[0][3]=this->Elm[0][3];
	back.Elm[1][3]=this->Elm[1][3];
	back.Elm[2][3]=this->Elm[2][3];
	back.Elm[3][3]=this->Elm[3][3];
	return back;
}
