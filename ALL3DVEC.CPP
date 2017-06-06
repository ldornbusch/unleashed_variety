// All3DVector.cpp: Implementierung der Klasse CAll3DVector.
//
//////////////////////////////////////////////////////////////////////

#include "All3DVec.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DVector::CAll3DVector()
{

}

CAll3DVector::~CAll3DVector()
{

}

//void CAll3DVector::SetVector(float x, float y, float z)	// sets the Values of the Vector

float CAll3DVector::CalcLenght()	//Calc the Lenght of the Vector and returns it
{
	float back;
	back=(float)sqrt((double)(XVec*XVec + YVec*YVec + ZVec*ZVec ));
	Lenght=back;
	return back;
}

void CAll3DVector::Normalize()
{
	float factor=0;
	CalcLenght();
	factor=(XVec*XVec + YVec*YVec + ZVec*ZVec)/Lenght ;
	XVec/=factor;
	YVec/=factor;
	ZVec/=factor;
	WVec=1;
	Lenght=1;
}
