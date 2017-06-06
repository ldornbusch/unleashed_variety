// All3DObject.h: Schnittstelle für die Klasse CAll3DObject.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ALL3DOBJECT_H__FCDA7C59_685F_11D3_B889_0000E8699C86__INCLUDED_)
#define AFX_ALL3DOBJECT_H__FCDA7C59_685F_11D3_B889_0000E8699C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "All3DMsh.h"
//#include "All3DMat.h"
#include "All3DMtx.h"
class CAll3DObject  
{
public:
	float zrot;
	float xrot;
	float yrot;
	void SetMesh(CAll3DMesh* meshobj);
	void SetPlace(CAll3DVector Place);
	CAll3DVector GetPlace();
	CAll3DMatrix GetMatrix();
	CAll3DMesh* GetMesh();
	CAll3DObject();
	CAll3DObject(int obj);
	virtual ~CAll3DObject();

private:
	void InitMesh(CAll3DVector* vertex, int* Face, CAll3DMaterial* ptrColor, int Object);
	CAll3DVector m_Place;
	CAll3DMesh* mptr_Mesh;
};

#endif // !defined(AFX_ALL3DOBJECT_H__FCDA7C59_685F_11D3_B889_0000E8699C86__INCLUDED_)
