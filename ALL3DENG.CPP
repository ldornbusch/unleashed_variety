// All3DEngine.cpp: Implementierung der Klasse CAll3DEngine.
//
//////////////////////////////////////////////////////////////////////

#include "All3DEng.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DEngine::CAll3DEngine()							//Engine withoutown Rendersurface (has to Set)
{
	RenderSurface=NULL;
	bOwnSurface=false;
}

CAll3DEngine::CAll3DEngine(int x, int y)	// Engine with own Rendersurface
{
	RenderSurface=new CBitMap(x,y);
	bOwnSurface=true;
}

CAll3DEngine::~CAll3DEngine()
{
	if (bOwnSurface)
	{
		delete RenderSurface;
		RenderSurface=NULL;
	}
}

void CAll3DEngine::ShowObject(CAll3DObject *VecObj)
{
	CAll3DLight* Light;
	CAll3DMaterial* Materials;
	CAll3DVector* vertex;
	CAll3DVector Normal;
	CAll3DVector* normals;
	CAll3DMatrix CMatrix, Result,XMatrix;
	CAll3DCamera* Cam;

	int numVertices, numFaces, numDot, face, vert, parse=0;
	int* Faces;

	Point* arrPoints,* PolyDot;
	
	Cam = new CAll3DCamera;
	Light=new CAll3DLight;

	Normal.SetVector(0,0,0);
	CMatrix=Cam->GetCameraMatrix() ;
	
	Light->SetDirection (MouseX,MouseY,-1);
	Light->Normalize();
	
	Normal.SetVector(-15,10,45);
//	VecObj->SetPlace(Normal);
	numVertices=VecObj->GetMesh()->GetNumVertices();
	numFaces=VecObj->GetMesh()->GetNumFaces();
	Materials=VecObj->GetMesh()->GetMaterial();
	normals=VecObj->GetMesh()->GetNormals();
	arrPoints=new Point[numVertices];
	vertex=VecObj->GetMesh()->GetVertices();
	Faces =VecObj->GetMesh()->GetFaces();
	XMatrix=VecObj->GetMatrix(); 
	Result = CMatrix*XMatrix;

	for (vert=0;vert<numVertices;vert++)		// Erstmal kopieren...später projezieren <:-)
	{
		arrPoints[vert] =All3DProjection(vertex[vert], Result);
	}
	for (face=0; face<numFaces; face++)
	{
		numDot=Faces[parse] ;
		Normal= Result.RotationOnly() * normals[face] ;
		if (FaceVisible(vertex[Faces[parse+1]], Normal, *Cam, Result))
		{
			PolyDot= new Point[numDot*2];
			for (vert=0; vert<numDot; vert++)
			{
				parse++;
				PolyDot[vert].x = arrPoints[Faces[parse]].x;
				PolyDot[vert].y = arrPoints[Faces[parse]].y;
			}
			parse++;
			int rgb=Materials[face].LightVector(&Normal, Light);
			RenderSurface->PolyFiller(PolyDot,numDot,rgb,3);//rgb>>16 & 0x000000ff,rgb>>8 & 0x000000ff, rgb & 0x000000ff));
			delete[] PolyDot;
		} 
		else
		{	
			parse+=numDot+1;
		}
	}
	delete[] arrPoints;
	delete Cam;
	delete Light;
}

boolean CAll3DEngine::FaceVisible(CAll3DVector PlaceVec, CAll3DVector DirVec, CAll3DCamera Cam, CAll3DMatrix Mat)
{
	CAll3DVector	SichtStrahl,dummy;

	dummy.SetVector(0,0,-128);
	PlaceVec= Mat * PlaceVec;					//Face wird in Camera Space transformiert
	SichtStrahl= PlaceVec - dummy;		// Cam.GetPlace()  ;			//Vector von der Camera zum Face
	//SichtStrahl.SetVector (0,0,1) ;
	float visible=SichtStrahl*DirVec;

	if (visible>0)
		return true;		//Face nicht sichtbar
	else
	return false;		//Face sichtbar
}

Point CAll3DEngine::All3DProjection(CAll3DVector vec, CAll3DMatrix mat)
{
	Point back;
	CAll3DMatrix proj;

	proj.SetProjection(128);
	
	proj=proj*mat;
	vec=proj*vec;


	vec.XVec =vec.XVec/vec.WVec;		//Normalize in 3-Space
	vec.YVec =vec.YVec/vec.WVec;
	vec.ZVec =vec.ZVec/vec.WVec;
	vec.WVec=1;

	back.x=(long)(vec.XVec +320);		// 3-Scaled 
	back.y=(long)(vec.YVec +240);		//Centered on Surface
	return back;
}

CBitMap* CAll3DEngine::GetRenderSurface()
{
	return RenderSurface;
}

void CAll3DEngine::SetRenderSurface(CBitMap *rSur)
{
	if (bOwnSurface) delete RenderSurface;
	RenderSurface = rSur;
	bOwnSurface=false;
}
