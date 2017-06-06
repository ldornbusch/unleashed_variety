// All3DMaterial.cpp: Implementierung der Klasse CAll3DMaterial.
//
//////////////////////////////////////////////////////////////////////

#include "All3DMat.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DMaterial::CAll3DMaterial()
{
	Helligkeit = new char[256];		//Für 180 Grad ~ 256 Einheiten  Helligkeit abspeichern
	for (int x=20; x<256; x++)
	{
		Helligkeit[x]=x;
	}
	for (int x=0; x<20; x++)
	{
		Helligkeit[x]=20;
	}
}

CAll3DMaterial::~CAll3DMaterial()
{
	delete[] Helligkeit;
}

int CAll3DMaterial::GetColor()
{
	return Color;
}

void CAll3DMaterial::SetColor(int Col)
{
	Color = Col;
}

void CAll3DMaterial::SetColor(int R, int G, int B)
{
	Color=( (R<<16) | (G<<8) | B);
}

int CAll3DMaterial::LightVector(CAll3DVector *Vec, CAll3DLight *Light)
{
	int back, temp;		//	Die Farbe des Materials wird mit der Farbe des Lichtes verknüpft
	unsigned char dummy, r, g, b;
	back=0;
	float Produkt= *Vec * Light->GetDirection() ;
	int intensity=-(int)(Produkt*256);
	if (intensity<0) intensity=0;
	dummy=(unsigned char)Helligkeit[intensity];
	if (dummy==255)
			back=0xffffff;		// wird ja spaeter nur geORt, deswegen bleibt weiss
	r= (unsigned char)(Color>>16)& 0xff   ;
	g= (unsigned char)(Color>>8) & 0xff  ;
	b= (unsigned char) Color     & 0xff  ;
	temp=(dummy*r)>>8& 0xff;
	back=back | (temp<<16);
	temp=(dummy*g)>>8& 0xff;
	back=back | (temp<<8);
	temp=(dummy*b)>>8& 0xff;
	back=back | temp;
	return back;
}
