// All3DLight.cpp: Implementierung der Klasse CAll3DLight.
//
//////////////////////////////////////////////////////////////////////

#include "All3DLig.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DLight::CAll3DLight()
{
	Direction.SetVector(0,0,0);		// Standardwerte : keinen Ort;
	Position.SetVector(0,0,0);		// Keine Richtung :
	Color=0;						// Keine Farbe;
	Type=ALL3DLT_AMBIENT;			// Ambient shading
}

CAll3DLight::~CAll3DLight()
{

}
void CAll3DLight::Normalize()
{
	Direction.Normalize() ;
	Position.Normalize() ;
}
