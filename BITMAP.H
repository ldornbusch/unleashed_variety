// BitMap.h: Schnittstelle für die Klasse CBitMap.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "All3DObj.h"
#include "All3DMtx.h"    // Hinzugefügt von der Klassenansicht
#include "All3DLig.h"
#include "All3DCam.h"
#include "Point.h"


#if !defined(AFX_BITMAP_H__FCDA7C53_685F_11D3_B889_0000E8699C86__INCLUDED_)
#define AFX_BITMAP_H__FCDA7C53_685F_11D3_B889_0000E8699C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BYTE unsigned char

class CBitMap  
{
public:
	void LineAdd(int x1, int y1, int x2, int y2, int col1, int col2);
	void Line(int x1, int y1, int x2, int y2, int col1, int col2);
    void LensFlare(CBitMap* Src,int LX, int LY, int sizeX=128, int sizeY=128);
    void BlendPixel(CBitMap* Src, CBitMap* Dest, CBitMap* Mask, int Counter);
	CBitMap* Clone(int x=0, int y=0, int Wid=320, int Hei=320);
	void Blend(CBitMap* Source, CBitMap* Dest, int Alpha);
	void Blt(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int transp=0);
	void Blt(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int DestWid, int DestHei, int transp=0); // BltStretch
	void BltAdd(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int Shift=0);
	void BltAdd(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int DestWid, int DestHei, int Shift=0); // BltStretch
	void BltTV(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest,int verzehr, int transp=0);
	void BltTrans(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int transp=0);
	void BltFast(int Xco, int Yco,int Wid, int Hei, CBitMap*Dest, int XDest, int YDest);
	void BillBoard(int Xco, int Yco,int Wid, int Hei, CBitMap*Dest, int XDest, int YDest, int ZDest,int Shift=-1);

	void InitDeltaMap();
	void InitPhong(int Phong =254, int Col=0xffffff, int Size=256);
	void Bump(CBitMap* Bump, CBitMap* Phong, int LX, int LY);
	void Fluid(CBitMap* Source , CBitMap* FluidMap);

	inline int GetPixel(int x, int y);
	inline int GetPitch();
	int CalcPixPerRow();		//Calculates the Bytes per Row and sets BytesPerPixel 
	inline int GetBytesPerRow(){return ((Pitch+Width)*BytesPerPixel) ;}
	int GetHeight(){	return Height; }
	int GetWidth() {	return Width ; }
	int* GetBitmap();
	
	inline float SubPixel(float x);
	void Clear(int Col=0);
	void Dot(int x, int y, int Col);
	void Line(int x1, int y1, int x2, int y2, int Col);
	void Poly(Point* vertex, int n, int Col=0x00FFFFFF);

	void PolyFiller(Point * vertex,int n,int Col, int RenderMode=0);

	void TextureMapper(Point * vertex,CBitMap * pTexture);

  void Done();
	void Init(int* ptrBitMap,int W, int H, int D, int Mod);
	
	int* LoadPCX(char* FileName);
  void LoadJpeg(const char * Name);
	void XorPattern();

	CBitMap(int Width, int Height);
	CBitMap(char* PCXFile);
	CBitMap();
	virtual ~CBitMap();

private:
	bool bClone;
	inline void DrawRenderMode(int y1, int y2,int RGB, int RM);
	inline void DrawSolidSegment(int y1, int y2,int RGB);
	inline void DrawAlphaSegment(int y1, int y2,int RGB);
	inline void DrawAdditiveSegment(int y1, int y2,int RGB);

	inline void DrawTextureSegment (int y1, int y2,unsigned int * pSource);

	int ABS(int arg);
	BYTE GetC(FILE *stream);

	int BytesPerPixel;
	int* BMap;
	int Pitch;		//Breite der Bitmap = Pitch + Width
	int Width;
	int Height;
	int Depht;
	char	Palette[1024];	//aarrggbb for Hermes compatibility

protected:
    float Left_dXdY;
    float Right_dXdY;
    float LeftX;
    float RightX;

		float LeftU;
		float LeftV;
		float Left_dUdY;
		float Left_dVdY;
		float dUdY;
		float dVdY;
		float dVdX;
		float dUdX;

    unsigned int * pPolyDest;
};

#endif // !defined(AFX_BITMAP_H__FCDA7C53_685F_11D3_B889_0000E8699C86__INCLUDED_)
