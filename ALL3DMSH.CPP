// All3DMesh.cpp: Implementierung der Klasse CAll3DMesh.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdio.h>
#include "All3DMsh.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DMesh::CAll3DMesh()
{
	All3DNormals = NULL;
	All3DVertexNormals =NULL;
	All3DVertices = NULL;
	Faces = NULL;
	Materials = NULL;
	NumFaces =0;
	NumVertices=0;
	ViewMode =0;
}

CAll3DMesh::CAll3DMesh(int numVertices, int NumFaces)
{

}

CAll3DMesh::~CAll3DMesh()
{
	if (All3DNormals != NULL)
	{
		delete [] All3DNormals;
		All3DNormals=NULL;
	}
	if (All3DVertexNormals != NULL) 
	{
		delete [] All3DVertexNormals;
		All3DVertexNormals=NULL;
	}
	if (Faces != NULL)			
	{
		delete [] Faces;
		Faces=NULL;
	}
	if (All3DVertices != NULL)			
	{
		delete [] All3DVertices;
		All3DVertices=NULL;
	}
	if (Materials != NULL)
	{
		delete [] Materials;
		Materials=NULL;
	}
}

void CAll3DMesh::SetVertices(CAll3DVector* ptrVertices)
{	All3DVertices=ptrVertices;}
CAll3DVector* CAll3DMesh::GetVertices()
{	return All3DVertices;}

int* CAll3DMesh::GetFaces()
{	return Faces;}
void CAll3DMesh::SetFaces(int* ptrFace)
{	Faces=ptrFace;}

void CAll3DMesh::SetNumFaces(int intNumber)
{	NumFaces=intNumber;}
int CAll3DMesh::GetNumFaces()
{	return NumFaces;}

void CAll3DMesh::SetNumVertices(int intNumber)
{	NumVertices=intNumber; }
int CAll3DMesh::GetNumVertices()
{	return NumVertices;}

int CAll3DMesh::GetViewMode()
{	return ViewMode;}
void CAll3DMesh::SetViewMode(int VMode)
{	ViewMode=VMode;}


void CAll3DMesh::SetMaterial(CAll3DMaterial *Mat)
{
	Materials=Mat;
}

CAll3DMaterial* CAll3DMesh::GetMaterial()
{
	return Materials;
}

void CAll3DMesh::CreateVertexNormals()
{

}

void CAll3DMesh::CreateFaceNormals()
{
	int num, ilt,x;
	CAll3DVector vec1,vec2,vec3, delta1, delta2;
	ilt=0;
	for (x=0; x<NumFaces; x++)
	{
		num=Faces[ilt];
if (x==120)
{x+=5;
x-=5;}
		vec1=All3DVertices[Faces[ilt+1]];
		vec2=All3DVertices[Faces[ilt+2]];
		vec3=All3DVertices[Faces[ilt+3]];
		delta1=vec2 - vec1;
		delta2=vec2 - vec3;
		delta1=delta1 / delta2;	//Vectoren kann mann nicht teilen.."/" ist das Kreuzprodukt ;-)
		delta1.Normalize() ;
		All3DNormals[x]=delta1 ;
		ilt+=num+1;				//Auf den Anfang der nächsten Zeile der Face-Description setzen..
	}
}

CAll3DVector* CAll3DMesh::GetNormals()
{	return All3DNormals; }

void CAll3DMesh::SetNormals(CAll3DVector *norms)
{ All3DNormals=norms; }
