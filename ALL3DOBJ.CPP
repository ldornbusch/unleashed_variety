// All3DObject.cpp: Implementierung der Klasse CAll3DObject.
//
//////////////////////////////////////////////////////////////////////

#include "All3DObj.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CAll3DObject::CAll3DObject()
{
	CAll3DObject(1);
}
CAll3DObject::CAll3DObject(int obj)
{
	CAll3DMaterial* ptrColor;
	int* ptrFaces;
	CAll3DVector* vertices, *normals;
	int Points=0,Faces=0,FaceArray=0;
	switch (obj)
	{
	case 0:		// The Ball...
		Points=42;
		Faces=80;
		FaceArray=320;
		break;
	case 1:		//the Star...
		Points=80;
		Faces=156;
		FaceArray=624;
		break;
	case 2:		//The Logo..
		Points=92;
		Faces=180;
		FaceArray=720;
		break;
	case 3:		// The Sputnik....
		Points=74;
		Faces=120;
		FaceArray=480;
		break;
	}
	vertices =new CAll3DVector[Points];		//42 Vertices
	normals  =new CAll3DVector[Faces];		// 80 Faces
	ptrFaces =new int[FaceArray];								//FacesDescript..
	ptrColor =new CAll3DMaterial[Faces];
	mptr_Mesh=new CAll3DMesh;



	m_Place.SetVector(0,0,0);
	xrot=yrot=zrot=0;

	InitMesh(vertices, ptrFaces, ptrColor, obj);
	mptr_Mesh->SetMaterial(ptrColor);
	mptr_Mesh->SetFaces (ptrFaces);
	mptr_Mesh->SetNumFaces (Faces);
	mptr_Mesh->SetNumVertices (Points);
	mptr_Mesh->SetVertices(vertices);
	mptr_Mesh->SetNormals(normals);
	mptr_Mesh->CreateFaceNormals();
}

CAll3DObject::~CAll3DObject()
{
	delete mptr_Mesh;
}

void CAll3DObject::InitMesh(CAll3DVector* vertex, int* Face , CAll3DMaterial* ptrColor, int Object)
{
	int x;
	switch (Object)
	{
	case 0:
		#include "Ball.3DD"
		break;
	case 1:
		#include "Star.3DD"
		break;
	case 2:
		#include "ND.3DD"
		break;
	case 3:
		#include "Sputnik.3DD"
		break;
	case 4:
		vertex[0].SetVector (-10,-10,-10);
		vertex[1].SetVector (0,0,10);
		vertex[2].SetVector (0,10,0);
		vertex[3].SetVector (10,0,0);
		vertex[4].SetVector (10,10,10);

		Face[ 0]=3;Face[ 1]=0;Face[ 2]=1;Face[ 3]=2;
		Face[ 4]=3;Face[ 5]=0;Face[ 6]=3;Face[ 7]=1;
		Face[ 8]=3;Face[ 9]=2;Face[10]=3;Face[11]=0;
		Face[12]=3;Face[13]=4;Face[14]=3;Face[15]=2;
		Face[16]=3;Face[17]=4;Face[18]=2;Face[19]=1;
		Face[20]=3;Face[21]=4;Face[22]=1;Face[23]=3;

		ptrColor[0].SetColor(163,20,10)	;
		ptrColor[1].SetColor(163,20,10)	;
		ptrColor[2].SetColor(163,20,10)	;
		ptrColor[3].SetColor(163,20,10)	;
		ptrColor[4].SetColor( 83,240,210)	;
		ptrColor[5].SetColor(223,60,200)	;
		break;
	}

}


CAll3DMesh* CAll3DObject::GetMesh()
{ return mptr_Mesh;}
void CAll3DObject::SetMesh(CAll3DMesh *meshobj)
{ mptr_Mesh=meshobj; }

CAll3DVector CAll3DObject::GetPlace()
{ return m_Place ;}
void CAll3DObject::SetPlace(CAll3DVector Place)
{ m_Place = Place;
}

CAll3DMatrix CAll3DObject::GetMatrix()
{
	CAll3DMatrix back, dummy;
	dummy.SetTranslation(m_Place);
	back=back*dummy;
	dummy.SetRotation(zrot,2);
	back=back*dummy;
	dummy.SetRotation(yrot,1);
	back=back*dummy;
	dummy.SetRotation(xrot,0);
	back=back*dummy;
	return back;
}
