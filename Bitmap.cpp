// BitMap.cpp: Implementierung der Klasse CBitMap.
//
//////////////////////////////////////////////////////////////////////
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "BitMap.h"
#include "jpeglib.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CBitMap::CBitMap()	//übergibt ein CBitmap-Object, das noch initialisiert werden muss.
{
	BMap=NULL;
	Height=0;
	Width=0;
	Depht=0;
	Pitch=0;
	CalcPixPerRow();
	bClone=true;
}

CBitMap::~CBitMap()
{
  if ((BMap)&&(!bClone))		//Nur Freigeben, wenn die Bitmap kein Clone ist
	{
		delete BMap;
		BMap=NULL;
	}
}

CBitMap::CBitMap(char *PCXFile)
{
  BMap=NULL;
  Height=0;
  Width=0;
	Depht=0;
	Pitch=0;
	LoadPCX(PCXFile);
	CalcPixPerRow();
  bClone=false;
}
/////////////////////////////////////////////////////////////////////////////
void CBitMap::LoadJpeg(const char * Name)
{
//	LoadPCX((char*)Name);
/*		unsigned int width;
    unsigned int height;

    printf("NAME OF JPEG %s\n",Name);

    unsigned char * pJpegBuffer = JpegFile::JpegFileToRGB(Name,&width,&height);

*/
    unsigned char a,r,g,b;

		int width, height;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  FILE * infile;		/* source file */
  JSAMPARRAY pJpegBuffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

	if ((infile = fopen(Name, "rb")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", Name);
//		return 0;
	}

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);


	(void) jpeg_start_decompress(&cinfo);

	width = cinfo.output_width;
	height = cinfo.output_height;

	unsigned char * pDummy = new unsigned char [width*height*4];
  unsigned char * pTest=pDummy;
	if (!pDummy)
      printf("NO MEM FOR JPEG CONVERT!\n");

  row_stride = width * cinfo.output_components ;

  pJpegBuffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  while (cinfo.output_scanline < cinfo.output_height) {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
//    put_scanline_someplace(buffer[0], row_stride);
    for (int x=0;x<width;x++)
    {
       a = 0;
       r = pJpegBuffer[0][cinfo.output_components*x];
			 if (cinfo.output_components>2)
			 {
					g = pJpegBuffer[0][cinfo.output_components*x+1];
					b = pJpegBuffer[0][cinfo.output_components*x+2];
			 } else {
					g = r;
					b = r;
			 }
        *(pDummy++) = b;
        *(pDummy++) = g;
        *(pDummy++) = r;
        *(pDummy++) = a;
    }
	}

  fclose(infile);

  (void) jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

    BMap = (int*)pTest; // Pointer sind equalent
    Height = height;
    Width = width;
    Depht = 32;
    CalcPixPerRow();
	  bClone=false;

}
/////////////////////////////////////////////////////////////////////////////
CBitMap::CBitMap(int W, int H)
{
	BMap=NULL;
	Width=W;
	Height=H;
	Depht=32;
	Pitch=0;
	CalcPixPerRow();
	BMap=new int[Width * Height];
	bClone=false;
}

//  ******************************* Methoden Aufrufe ********************************************


void CBitMap::Clear(int Col)
{
	int  SrcMod;
	int *ptrBMap;
	ptrBMap=BMap ;
	SrcMod=Width+Pitch;

	for (int y=Height;y>0;y--)
	{
		memset(ptrBMap,Col, Width<<2);
		ptrBMap+=SrcMod;
	}
//	memset((void*)BMap, Col,GetBytesPerRow()*Height);
/*	int Offset=0;
	for (int y=0 ; y <Height ;y++)
	for (int x=0 ; x <Width ;x ++)
		BMap[Offset++]=0;
*/
}

int inline CBitMap::ABS(int arg)
{
	if (arg<0 ) arg=-arg;
	return arg;
}

int* CBitMap::LoadPCX(char *FileName)		//Load PCX-File and Converts to 32Bit Image
{
	int Format, Version, Kompression, BitsPerPixel, Planes, BytesPerRow, Pal;
	char* PicMem, *MemPtr;
	BYTE	readByte;
	int packCount, PixelCount, LineCount ,  RGB, i, wert;
	FILE *PicFile;
	if (BMap)			// Falls schon eine Bitmap reserviert war, löschen...
	{
		delete BMap;
		BMap=NULL;
	}
	PicMem=NULL;
	MemPtr=NULL;
	PixelCount=0;
	LineCount=0;
	RGB=2;
	PicFile=fopen(FileName,"rb");
	if (PicFile)		//Falls das File geöffnet werden konnte....
	{
		Format=GetC(PicFile);			// Header Information lesen.....
		Version=GetC(PicFile);
		Kompression=GetC(PicFile);
		BitsPerPixel=GetC(PicFile);
		i=GetC(PicFile);			//0
		i=GetC(PicFile);		//0
		i=GetC(PicFile);			//0
		i=GetC(PicFile);		//0
		Width=GetC(PicFile);			//Breite
		Width+=GetC(PicFile)*256;
		Height=GetC(PicFile);		//Hoehe
		Height+=GetC(PicFile)*256;
		for (i=13; (i<66) && (feof(PicFile)==0);i++)
			readByte=GetC(PicFile);						//Palette Überspringen...
		Planes=GetC(PicFile);
		BytesPerRow=GetC(PicFile);
		BytesPerRow+=GetC(PicFile)*256;
		Pal=GetC(PicFile);
		Pal+=GetC(PicFile)*256;
		for (i=70; (i<128)&& (feof(PicFile)==0);i++)
			readByte=GetC(PicFile);						//Rest des Header Überspringen...
		RGB=Planes-1;
		Width++;
		Height++;
		PicMem = new char[(Width)*(Height)*4+256];	//Offscreensurface 1 (640x480x32)
		Depht=32;
		CalcPixPerRow();
		MemPtr=PicMem;

		while ((feof(PicFile)==0)&&(LineCount<Height))
		{
			readByte=GetC(PicFile);
			if ((readByte&0xc0)==0xc0)
			{													// Byte ist gepackt
				readByte-=192;
				packCount=readByte;
				PixelCount+=packCount;
				readByte=GetC(PicFile);
				for (i=packCount;i>0;i--)
				{
					*MemPtr=readByte;
					MemPtr+=4;
				}
			}
			else											//Byte ist nicht gepackt!
			{
					*MemPtr=readByte;
					MemPtr+=4;
				PixelCount++;
			}
			if (PixelCount>=Width)
			{
				PixelCount=0;//-=(Width);
				RGB--;
				if (RGB<0)
				{
					LineCount++;
					RGB=Planes-1;
				}
				MemPtr=PicMem+(LineCount*Width*4)+RGB;
			}
		}
		wert=GetC(PicFile);
		if ((BitsPerPixel==8)&&(Planes==1))			//Falls Palette, in 24-Bit übersetzen...
		{
			i=0;
			while ((feof(PicFile)==0))
			{
				readByte=GetC(PicFile);
				Palette[i++]=readByte;
			}
			MemPtr=PicMem;
			for (i=0; i<(Width*Height+1);i++) //Übersetzen der Palette in RGBA
			{
				wert=(BYTE)*MemPtr;
				*MemPtr++=Palette[wert*3+2];
				*MemPtr++=Palette[wert*3+1];
				*MemPtr++=Palette[wert*3+0];
				*MemPtr++=0;
			}
		}
	fclose(PicFile);
	}
	BMap=(int*)PicMem;
	return BMap;
}

BYTE CBitMap::GetC(FILE* stream)
{
	static int Offset;
	Offset++;
	return getc(stream);
}

int* CBitMap::GetBitmap()
{
	return BMap;
}


void CBitMap::Init(int *ptrBitMap, int W, int H, int D, int Mod)	//Initialisiert ein bestehendes Bitmap zu einer CBitmap-Klasse (DirectX BackBuffer!)
{
	BMap=ptrBitMap;
	Height=H;
	Width=W;
	Depht=D;
	Pitch=Mod;
	bClone=true;		//Bitmap hat keinen eigenen BMap-Memory
	CalcPixPerRow();
}

void CBitMap::Done()	//Deinitialisiert ein CBitmapObject, ohne die Ressourcen freizugeben (zb. beim Backbuffer)
{											// So kann eine bestehende (system)-Bitmap in ein CBitmap-Object umgewandelt werden und später wieder von diesem gelöst werden
	BMap=NULL;
	Height=0;
	Width=0;
	Depht=0;
	Pitch=0;
	bClone=false;
}
// Erstellt einen neuen Bitmap-Clone, mit derselben Bitmap des Ursprungs-Objects
CBitMap* CBitMap::Clone(int x, int y, int Wid, int Hei)
{
	CBitMap*	Back;
	int NewPitch, Offset;
	Back=new CBitMap();

	if (x<0) {Wid+=x;x=0;}
	if (y<0) {Hei+=y;y=0;}
	if (x+Wid>Width) Wid=Width-x;
	if (y+Hei>Height) Hei=Height-y;

	NewPitch=Pitch+(Width-Wid);
	Offset=y*(Width+Pitch)+x;

	Back->Init(BMap+Offset,Wid,Hei,Depht,NewPitch);
	return Back;
}

int CBitMap::GetPixel(int x, int y)		//VerySlow only for initialisations!!
{
	int Offset;
	while(x<0) x+=Width ;
	while(y<0) y+=Height;
	while(x>=Width) x-=Width ;
	while(y>=Height) y-=Height;
	Offset=y*Width+x;
	return BMap[Offset];
}

void CBitMap::Dot(int x, int y, int Col)										// A simple Dot
{
	int Offset;
	if ((x<Width) && (x>0) && (y<Height) && (y>0))
	{
		Offset=y*Width+x;
		BMap[Offset]=Col;
	}

}

void CBitMap::Poly (Point *vertex, int n, int Col)			// A simple Polygon
{
//    Line(vertex[0].x ,vertex[0].y ,vertex[1].x ,vertex[1].y ,Col);
//    Line(vertex[1].x ,vertex[1].y  ,vertex[2].x ,vertex[2].y ,Col);
//    Line(vertex[2].x ,vertex[2].y  ,vertex[0].x ,vertex[0].y ,Col);

    PolyFiller(vertex,n,Col);

}


int CBitMap::CalcPixPerRow()
{
	switch (Depht)
		{
		case 4:
			BytesPerPixel = 0	;
			break;
		case 8:
			BytesPerPixel = 1;
			break;
		case 15:
			BytesPerPixel = 2;
			break;
		case 16:
			BytesPerPixel = 2;
			break;
		case 24:
			BytesPerPixel = 3;
			break;
		case 32:
			BytesPerPixel = 4;
			break;
		}
	return BytesPerPixel*(Width+Pitch);
}

inline int CBitMap::GetPitch()
{
	return Pitch ;
}

void CBitMap::InitDeltaMap()		//the Differences in X and Y for Fluid and BumpMapping WORKS only in 32 Bit
{
	int x,y,dx,dy,left,right;
	int Offset=0;
	int Offset2=0;
	int*	Dest;
	if (Depht==32)
	{
		CBitMap*	temp;
		temp=new CBitMap(Width ,Height); //TempoaryMap as big as this one
		if (temp)
		{
			Dest=temp->GetBitmap();
			for(x=0;x<Width;x++)
				for(y=0;y<Height; y++)
				{
					Dest[Offset] = BMap[Offset];
					Offset++;
				}
			Offset =0;
			for ( y=0;y<Height; y++)
 				for( x=0;x<Width;x++)
				{
					left	= (short)((temp->GetPixel(x-1,y))&0xff);
					right	= (short)((temp->GetPixel(x+1,y))&0xff);
					dx=(short)(((left-right)))&0xffff;
					left	= (short)((temp->GetPixel(x,y-1))&0xff);
					right	= (short)((temp->GetPixel(x,y+1))&0xff);
					dy=(short)(((left-right)))&0xffff;

					BMap[Offset++]=(dy<<16)+dx;
				}
		delete temp;
		}
	}
}

void CBitMap::InitPhong(int Phong, int Col, int Size )
{
	int* ColorMap;
	float intensity;
	int ColorWert;
	int r=(Col&0x00ff0000)>>16;
	int g=(Col&0x0000ff00)>>8;
	int b=(Col&0x000000ff);
	float fr,fg,fb;

	if (BMap) delete BMap;
	Pitch=0;
	BytesPerPixel=4;
	Depht=32;
	Height=Size;
	Width=Size;
	BMap=new int[Width*Height];

	ColorMap= new int[256];
	for (int i=0;i<Phong;i++)
	{
		fr=r*i/(Phong-1);
		fg=g*i/(Phong-1);
		fb=b*i/(Phong-1);
		ColorMap[i]=((int)(fr)<<16)+((int)(fg)<<8)+((int)(fb));
	}
	for (int i=Phong;i<256;i++)
	{
		ColorMap[i]=Col;
	}

	for (int y=0;y<Height;y++)
		for (int x=0;x<Width;x++)
		{
			intensity=1-(((float)x/(Size-1))*((float)x/(Size-1))+((float)y/(Size-1))*((float)y/(Size-1)));
			if (intensity<0) intensity=0;
			ColorWert=(int)(intensity*255);
			BMap[y*Width+x]=ColorMap[ColorWert];
		}
		delete ColorMap;
}

void CBitMap::Fluid(CBitMap *Source, CBitMap *FluidMap)
{
	int Offset1=0;
	static int scroll;
	int Offset2=0;
	short dx, dy;
	int srcPixelW, srcW, srcH, FluSize;
	int dummy;
	int *Flu, *Src, *Dest;
	short *WortBuffer;
	Flu = FluidMap->GetBitmap();
	WortBuffer=(short*) Flu;
	FluSize = FluidMap->GetHeight() * (FluidMap->GetWidth()+FluidMap->GetPitch());
	Src =Source->GetBitmap();
	Dest=BMap;
	srcPixelW=Source->GetWidth()+Source->GetPitch();
	srcW=Source->GetWidth();
	srcH=Source->GetHeight();
	for (int y=0;y<Height	; y++)
	{
		for(int x=0;x<Width;x++)
		{
				dummy=(Offset1+scroll);
				if(dummy>=FluSize)
					dummy-=FluSize;
				dummy=dummy<<1;
				dx=ABS(WortBuffer[dummy]) +x;
				dy=ABS(WortBuffer[dummy+1])+y;
				if ((dx)>=srcW) dx-=srcW;
				if ((dy)>=srcH) dy-=srcH;
				Offset2=dy*srcPixelW+dx;			//PerformanceKiller!!
				BMap[Offset1++] = Src[Offset2];
		}
		Offset1+=Pitch;
		Offset2+=Source->GetPitch();
	}
	scroll+=FluidMap->GetBytesPerRow()-1;
	if (scroll>FluSize) scroll-=FluSize;	//Sonst wird es immer langsamer...
}

void CBitMap::Bump(CBitMap *Bump, CBitMap * Phong, int LX, int LY)
{
	int Offset1, Offset2;		//1=Source...2=Bump...3=Phong
	int dx,dy, Size, PixPerRow;
	int  *ptrDest, *ptrPhong;
	short *ptrBump;
	ptrPhong	= Phong->GetBitmap();
	ptrBump		= (short*)Bump->GetBitmap();
	Size=Phong->GetWidth()-1;
	ptrDest=BMap;
	Offset1=0;
	PixPerRow=Bump->GetWidth()+Bump->GetPitch();
	for (int y=0;y<Height; y++)
	{
		for(int x=0;x<Width;x++)
		{
			dx=ABS(ptrBump[(x+y*PixPerRow)*2]  -x+LX);
			dy=ABS(ptrBump[(x+y*PixPerRow)*2+1]-y+LY);
			if (dx>Size) dx=Size;
			if (dy>Size) dy=Size;
			Offset2=(dy*(Size+1))+dx;
				ptrDest[Offset1++] = ptrPhong[Offset2];
		}
		Offset1+=Pitch;
		Offset2+=Bump->GetPitch();
	}
}

void CBitMap::BltFast(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest)
{
	int SrcOffset, DestOffset, DstMod, SrcMod;
	int *ptrSrc, *ptrDst;
	SrcOffset=(Width+Pitch)*Yco+Xco;
	ptrSrc=BMap + SrcOffset;
	DestOffset=(Dest->GetWidth()+Dest->GetPitch())*YDest+XDest;
	ptrDst=Dest->GetBitmap() + DestOffset;
	SrcMod=Width+Pitch;
	DstMod=Dest->GetWidth() + Dest->GetPitch();

	for (int y=Hei;y>0;y--)
	{
		memcpy(ptrDst,ptrSrc, Wid<<2);
		ptrDst+=DstMod;
		ptrSrc+=SrcMod;
	}
}
void CBitMap::BillBoard(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int ZDest, int Shift)
{
	if (ZDest>-64)
	{
		int xpro1, ypro1, xpro2,ypro2;	// Die projezierten Coords des BillBoards
		int CenterX, CenterY, Xcoord,Ycoord;
		Xcoord=XDest-(Wid>>1);
		Ycoord=YDest-(Hei>>1);
		CenterX=Dest->GetWidth()>>1;
		CenterY=Dest->GetHeight()>>1;
		xpro1= CenterX + ((Xcoord*128)/(ZDest+128));
		ypro1= CenterY + ((Ycoord*128)/(ZDest+128));
		xpro2= CenterX + (((Xcoord+Wid)*128)/(ZDest+128));
		ypro2= CenterY + (((Ycoord+Hei)*128)/(ZDest+128));
		if (Shift>-1)
			BltAdd(Xco,Yco,Wid,Hei,Dest,xpro1,ypro1,xpro2-xpro1,ypro2-ypro1,Shift);
		else
			Blt(Xco,Yco,Wid,Hei,Dest,xpro1,ypro1,xpro2-xpro1,ypro2-ypro1);
	}
}
void CBitMap::Blt(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int transp)
{
	int SrcOffset, DestOffset, DstMod, SrcMod;
	int *ptrSrc, *ptrDst;
	int actPix;
	// Zuerst die Seiten Clippen...
    if (XDest+Wid>Dest->GetWidth()) Wid=Dest->GetWidth()-XDest;     //RECHTS
    if ((XDest<0) &&(XDest+Wid)>0) { Xco-=XDest;Wid+=XDest;XDest=0; }              //LINKS

    if (YDest+Hei>Dest->GetHeight()) Hei=Dest->GetHeight()-YDest;   //UNTEN
    if ((YDest<0) &&(YDest+Hei)>0) { Yco-=YDest;Hei+=YDest;YDest=0; }              //OBEN

	//Clipping done
	SrcOffset=(Width+Pitch)*Yco+Xco;
	ptrSrc=BMap;
	SrcMod=Width+Pitch-Wid;
	DestOffset=(Dest->GetWidth()+Dest->GetPitch())*YDest+XDest;
	ptrDst=Dest->GetBitmap();
	DstMod=Dest->GetWidth() + Dest->GetPitch() - Wid;

	for (int y=0;y<Hei;y++)
	{
		for (int x=0;x<Wid;x++)
		{
			actPix=ptrSrc[SrcOffset++];
			if ((actPix&0x00ffffff)==transp)
				DestOffset++;
			else
				ptrDst[DestOffset++]=actPix;
		}
		DestOffset+=DstMod;
		SrcOffset +=SrcMod;
	}
}
void CBitMap::Blt(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int DestWid, int DestHei, int transp)
// Very Quick and VERY Dirty converted from Pascal......
{
	int xdir,ydir,xp,yp,xpo,xi,yi;
	int xl,yl,xco,yco;
	int xsc,ysc;
	int SrcWid=Width+Pitch;
	int DstWid=Dest->GetWidth() + Dest->GetPitch();
	int* SrcBMap=BMap;
	int* DstBMap=Dest->GetBitmap();
	int SrcPointer, DstPointer;
	int actPix,DestPix,over,Pixel;
  if ((XDest+ABS(DestWid)>0) && (XDest<Dest->GetWidth() ) &&
      (YDest+ABS(DestHei)>0) && (YDest<Dest->GetHeight()))
	{
		if (DestWid==0) DestWid=1;
		if (DestHei==0) DestHei=1;
		if ((XDest<0) && (XDest+DestWid>0))               //CLIPPING-AREA.......
		{
			xsc=(-XDest*Wid) / DestWid ;
			Xco=xsc;
			Wid-=Xco;
			DestWid+=XDest;
			XDest=0;
		}
		if ((XDest<Dest->GetWidth()) && (XDest+DestWid>=Dest->GetWidth()))
		{
			xsc=Dest->GetWidth()-XDest-1;
			Wid=(xsc*Wid) / DestWid;
			DestWid=xsc;
		}
		if ((YDest<0) && (YDest+DestHei>0))
		{
			ysc=(-YDest*Hei) / DestHei ;
			Yco=ysc;
			Hei-=Yco;
			DestHei+=YDest;
			YDest=0;
		}
		if ((YDest<Dest->GetHeight()) && (YDest+DestHei>=Dest->GetHeight()))
		{
			ysc=Dest->GetWidth()-YDest-1;
			Hei=(ysc*Hei) / DestHei ;
			DestHei=ysc;
		}
		if (DestWid<0)	{ xdir=-1;xp=Wid;DestWid=-DestWid;xpo=Wid;}					//Here is the Flipping Part....
		else						{ xdir= 1;xp=0  ;						      xpo=-Wid;}
		if (DestHei<0)	{ ydir=-SrcWid;yp=Hei;DestHei=-DestHei;}
		else						{ ydir= SrcWid;yp=0  ;       }
		xi=DestWid;yi=DestHei;

		SrcPointer=(Yco+yp)*SrcWid+Xco+xp;
		DstPointer=YDest*DstWid+XDest;
		for(yl=DestHei;yl>0;yl--)
		{
		  for (xl=DestWid;xl>0;xl--)
			{
				if ((SrcBMap[SrcPointer]&0xffffff)!=transp)
				{
          DstBMap[DstPointer]=SrcBMap[SrcPointer];
				}
				DstPointer++;
				xi-=Wid;
				while (xi<=0)
				{		xi+=DestWid;	SrcPointer+=xdir;		}
			}
			SrcPointer+=xpo;
			DstPointer+=DstWid-DestWid;
			xi=DestWid;
			yi-=Hei;
			while (yi<=0)
			{	yi+=DestHei;			SrcPointer+=ydir;			}
		}
	}
}

void CBitMap::BltTrans(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int transp)
{
	int SrcOffset, DestOffset, DstMod, SrcMod;
	int *ptrSrc, *ptrDst;
	int actPix, DestPix;
	// Zuerst die Seiten Clippen...
    if (XDest+Wid>Dest->GetWidth()) Wid=Dest->GetWidth()-XDest;     //RECHTS
    if ((XDest<0) &&(XDest+Wid)>0) { Xco-=XDest;Wid+=XDest;XDest=0; }              //LINKS

    if (YDest+Hei>Dest->GetHeight()) Hei=Dest->GetHeight()-YDest;   //UNTEN
    if ((YDest<0) &&(YDest+Hei)>0) { Yco-=YDest;Hei+=YDest;YDest=0; }              //OBEN


	//Clipping done
	SrcOffset=(Width+Pitch)*Yco+Xco;
	ptrSrc=BMap;
	SrcMod=Width+Pitch-Wid;
	DestOffset=(Dest->GetWidth()+Dest->GetPitch())*YDest+XDest;
	ptrDst=Dest->GetBitmap();
	DstMod=Dest->GetWidth() + Dest->GetPitch() - Wid;

	for (int y=Hei;y>0;y--)
	{
		for (int x=Wid;x>0;x--)
		{
			actPix=ptrSrc[SrcOffset++];
			DestPix=ptrDst[DestOffset];
			actPix=((actPix>>1)&0x7f7f7f7f) + ((DestPix>>1)&0x7f7f7f7f);
			ptrDst[DestOffset++]=actPix;
		}
		DestOffset+=DstMod;
		SrcOffset +=SrcMod;
	}
}

void CBitMap::BltAdd(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int Shift)
{
	int SrcOffset, DestOffset, DstMod, SrcMod;
	int *ptrSrc, *ptrDst;
	int actPix, DestPix;
	int over,Pixel;
	// Zuerst die Seiten Clippen...
	if (XDest+Wid>Dest->GetWidth()) Wid=Dest->GetWidth()-XDest;		//RECHTS
    if ((XDest<0) &&(XDest+Wid)>=0) { Xco-=XDest;Wid+=XDest;XDest=0; }              //LINKS

    if (YDest+Hei>Dest->GetHeight()) Hei=Dest->GetHeight()-YDest;   //UNTEN
    if ((YDest<0) &&(YDest+Hei)>=0) { Yco-=YDest;Hei+=YDest;YDest=0; }              //OBEN

    //Clipping done
	SrcOffset=(Width+Pitch)*Yco+Xco;
	ptrSrc=BMap;
	SrcMod=Width+Pitch-Wid;
	DestOffset=(Dest->GetWidth()+Dest->GetPitch())*YDest+XDest;
	ptrDst=Dest->GetBitmap();
	DstMod=Dest->GetWidth() + Dest->GetPitch() - Wid;

	for (int y=0;y<Hei;y++)
	{
		for (int x=0;x<Wid;x++)
		{

			actPix=ptrSrc[SrcOffset++];
			DestPix=ptrDst[DestOffset];
			if ((actPix & 0x00ffffff)>0)
			{
				Pixel= ((actPix)&0x00fefeff) + ((DestPix)&0x00fefeff);
				over=Pixel&0x01010100;
				over=over-(over>>8);
				actPix=Pixel|over;
				ptrDst[DestOffset++]=actPix;
			}
			else
				DestOffset++;
		}
		DestOffset+=DstMod;
		SrcOffset +=SrcMod;
	}
}
void CBitMap::BltAdd(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest, int DestWid, int DestHei, int Shift)
// Very Quick and VERY Dirty converted from Pascal......
{
	int xdir,ydir,xp,yp,xpo,xi,yi;
	int xl,yl,xco,yco;
	int xsc,ysc;
	int SrcWid=Width+Pitch;
	int DstWid=Dest->GetWidth() + Dest->GetPitch();
	int* SrcBMap=BMap;
	int* DstBMap=Dest->GetBitmap();
	int SrcPointer, DstPointer;
	int actPix,DestPix,over,Pixel;
	int ShiftMask;
	if ((Shift>0)&&(Shift<8))
	{
		ShiftMask=(0x00ff>>Shift)&0x00ff;
		ShiftMask=(ShiftMask<<16) | (ShiftMask<<8) | ShiftMask;
	} else ShiftMask=0x00ffffff;
  if ((XDest+ABS(DestWid)>0) && (XDest<Dest->GetWidth() ) &&
      (YDest+ABS(DestHei)>0) && (YDest<Dest->GetHeight()))
	{
		if (DestWid==0) DestWid=1;
		if (DestHei==0) DestHei=1;
		if ((XDest<0) && (XDest+DestWid>0))               //CLIPPING-AREA.......
		{
			xsc=(-XDest*Wid) / DestWid ;
			Xco=xsc;
			Wid-=Xco;
			DestWid+=XDest;
			XDest=0;
		}
		if ((XDest<Dest->GetWidth()) && (XDest+DestWid>=Dest->GetWidth()))
		{
			xsc=Dest->GetWidth()-XDest-1;
			Wid=(xsc*Wid) / DestWid;
			DestWid=xsc;
		}
		if ((YDest<0) && (YDest+DestHei>0))
		{
			ysc=(-YDest*Hei) / DestHei ;
			Yco=ysc;
			Hei-=Yco;
			DestHei+=YDest;
			YDest=0;
		}
		if ((YDest<Dest->GetHeight()) && (YDest+DestHei>=Dest->GetHeight()))
		{
			ysc=Dest->GetWidth()-YDest-1;
			Hei=(ysc*Hei) / DestHei ;
			DestHei=ysc;
		}
		if (DestWid<0)	{ xdir=-1;xp=Wid;DestWid=-DestWid;xpo=Wid;}					//Here is the Flipping Part....
		else						{ xdir= 1;xp=0  ;						      xpo=-Wid;}
		if (DestHei<0)	{ ydir=-SrcWid;yp=Hei;DestHei=-DestHei;}
		else						{ ydir= SrcWid;yp=0  ;       }
		xi=DestWid;yi=DestHei;

		SrcPointer=(Yco+yp)*SrcWid+Xco+xp;
		DstPointer=YDest*DstWid+XDest;
		for(yl=DestHei;yl>0;yl--)
		{
		  for (xl=DestWid;xl>0;xl--)
			{
				actPix=SrcBMap[SrcPointer]&0xffffff;
				if (actPix>0)
				{
					actPix=(actPix>>Shift)& ShiftMask;
					DestPix=DstBMap[DstPointer];
					Pixel= ((actPix)&0x00fefeff) + ((DestPix)&0x00fefeff);
					over=Pixel&0x01010100;
					over=over-(over>>8);
					actPix=Pixel|over;
          DstBMap[DstPointer]=actPix;
				}
				DstPointer++;
				xi-=Wid;
				while (xi<=0)
				{		xi+=DestWid;	SrcPointer+=xdir;		}
			}
			SrcPointer+=xpo;
			DstPointer+=DstWid-DestWid;
			xi=DestWid;
			yi-=Hei;
			while (yi<=0)
			{	yi+=DestHei;			SrcPointer+=ydir;			}
		}
	}
}
void CBitMap::BltTV(int Xco, int Yco, int Wid, int Hei, CBitMap *Dest, int XDest, int YDest,int verzerr, int transp)
{
	int SrcOffset, DestOffset, DstMod, SrcMod;
	int *ptrSrc, *ptrDst;
	int actPix;
	int distort;
	if (verzerr<1) verzerr=1;
	// Zuerst die Seiten Clippen...
    if (XDest+Wid>Dest->GetWidth()) Wid=Dest->GetWidth()-XDest;     //RECHTS
    if ((XDest<0) &&(XDest+Wid)>0) { Xco-=XDest;Wid+=XDest;XDest=0; }              //LINKS

    if (YDest+Hei>Dest->GetHeight()) Hei=Dest->GetHeight()-YDest;   //UNTEN
    if ((YDest<0) &&(YDest+Hei)>0) { Yco-=YDest;Hei+=YDest;YDest=0; }              //OBEN

	//Clipping done
	SrcOffset=(Width+Pitch)*Yco+Xco;
	ptrSrc=BMap;
	SrcMod=Width+Pitch-Wid;
	DestOffset=(Dest->GetWidth()+Dest->GetPitch())*YDest+XDest;
	ptrDst=Dest->GetBitmap();
	DstMod=Dest->GetWidth() + Dest->GetPitch() - Wid;

	for (int y=0;y<Hei;y+=2)
	{
		distort=rand()%verzerr-(verzerr>>1);		//Verzerrung
		SrcOffset+=distort;

		for (int x=0;x<Wid-distort;x++)
		{
			actPix=ptrSrc[SrcOffset++];
			if ((actPix&0x00ffffff)==transp)
				DestOffset++;
			else
				ptrDst[DestOffset++]=actPix;
		}
		DestOffset+=distort;
		DestOffset+=DstMod;
		SrcOffset +=SrcMod;

		distort=rand()%verzerr-(verzerr>>1);		//Verzerrung
		SrcOffset+=distort;
		for (int x=0;x<Wid-distort;x++)
		{
			actPix=ptrSrc[SrcOffset++];
			if ((actPix&0x00ffffff)==transp)
				DestOffset++;
			else{
				actPix=((actPix>>1)&0x7f7f7f7f) ;
				ptrDst[DestOffset++]=actPix;
			}
		}
		DestOffset+=distort;
		DestOffset+=DstMod;
		SrcOffset +=SrcMod;
	}
}




// Dies ist die PolyFill Routine
void CBitMap::PolyFiller(Point * vertex,int n,int RGB, int RenderMode)
{
    n=n;  // keine warnings !
    Point * V1 = &vertex[0];
    Point * V2 = &vertex[1];
    Point * V3 = &vertex[2];
    if (V1->y > V2->y)
    {
        Point * MTemp;        //swap (&V1,&V2);
        MTemp = V1;
        V1 = V2;
        V2 = MTemp;
    }
    if (V1->y > V3->y)
    {
        Point * MTemp;        //swap(&V1,&V3);
        MTemp = V1;
        V1 = V3;
        V3 = MTemp;
   }
    if (V2->y > V3->y)
    {
        Point * MTemp;        //swap (&V2,&V3);
        MTemp = V2;
        V2 = V3;
        V3 = MTemp;
    }
    /*  SubPixel Accuarcy der Polygon Ecken */
    int y1i = ceil(V1->y);
    int y2i = ceil(V2->y);
    int y3i = ceil(V3->y);
    if (y1i==y3i)
        return;
    pPolyDest = (unsigned int*)GetBitmap() + (y1i*GetBytesPerRow()>>2);
    /* Konstanten errechnen ...hier kann noch optimiert werden !!! */
     float V1V3 = V3->y - V1->y;
     float V2V3 = V3->y - V2->y;
     float V1V2 = V2->y - V1->y;

     if (!V1V3)        V1V3 =1;
     if (!V2V3)        V2V3 =1;
     if (!V1V2)        V1V2 =1;

     float dXdY_V1V3=(V3->x - V1->x) / V1V3;
     float dXdY_V2V3=(V3->x - V2->x) / V2V3;
     float dXdY_V1V2=(V2->x - V1->x) / V1V2;
    // Stelle fest ob V2 auf der rechten seite oder links
    bool mid = dXdY_V1V3 < dXdY_V1V2;
    if (!mid)
    {
        // V2 Links
        if (y1i==y2i)
        {
            Left_dXdY  = dXdY_V2V3;
            Right_dXdY = dXdY_V1V3;
            LeftX  = V2->x;
            RightX = V1->x;
            DrawRenderMode(y1i, y3i,RGB, RenderMode);
            return;
        }
        Right_dXdY = dXdY_V1V3;
        if (y1i<y2i)
        {
            Left_dXdY = dXdY_V1V2;
            LeftX  = V1->x;
            RightX = V1->x;
            DrawRenderMode(y1i, y2i,RGB, RenderMode);
        }
        if (y2i<y3i)
        {
            Left_dXdY = dXdY_V2V3;
            LeftX = V2->x;
            DrawRenderMode(y2i, y3i,RGB, RenderMode);
		}
    }
    else
    if (mid) // Positve
    {
        if (y1i==y2i)
        {
					Left_dXdY = dXdY_V1V3;
					Right_dXdY = dXdY_V2V3;
					LeftX  = V1->x;
					RightX = V2->x;
          DrawRenderMode(y1i, y3i,RGB, RenderMode);
					return;
        }
        /* Brauchen wir ja nur einmal Init */
        Left_dXdY = dXdY_V1V3;
        if (y1i<y2i)
        {
            Right_dXdY = dXdY_V1V2;
            LeftX = V1->x; // gleiche Start X cor !!!
            RightX= V1->x;
            DrawRenderMode(y1i, y2i,RGB, RenderMode);
				}
				if (y2i<y3i)
				{
            Right_dXdY = dXdY_V2V3;
            RightX     = V2->x;
            DrawRenderMode(y2i, y3i,RGB, RenderMode);
        }
    }
}
// Verzweigt zur richtigen Zeichenroutine
inline void CBitMap::DrawRenderMode(int y1, int y2,int RGB,int RM)
{
	switch (RM)
	{
		case 0: case 1:
				DrawSolidSegment(y1,y2,RGB);
			break;
		case 2:
				RGB=(RGB>>1) & 0x007f7f7f;
				DrawAlphaSegment(y1,y2,RGB);
			break;
		case 3:
				RGB=RGB & 0x00fefeff;
				DrawAdditiveSegment(y1,y2,RGB);
			break;
	}
}

// Zeichnet eine Solid Linie .....
inline void CBitMap::DrawSolidSegment(int y1, int y2,int RGB)
{
    // WORKING VERSION In FIXED POINT
    signed int x1, x2, y;

    for (y=y1;y<y2;y++)
    {

        /*eigene inline function schreiben !*/
        x1 = ceil (LeftX);   // Subpixel Accuracy fr Poly-Aussenseiten links  !
        x2 = ceil (RightX);  // Subpixel Accuarcy fr Poly-Aussenseiten rechts !

        unsigned int * pTriangle = pPolyDest + x1;

        int width;

        if (x1<x2)
        {
            width = x2-x1;

             //ist cool, weil vc++ das uebernehmen hat
            asm("mov %edi, $pTriangle"
                "mov %ecx, $width"
                "mov %eax, $RGB"
                "cld"
                "rep stosd");
        }

        LeftX+=Left_dXdY;
        RightX+=Right_dXdY;
        pPolyDest+=Width + Pitch;  // Eine Zeile tiefer addieren !
    }
}


// Zeichnet eine AlphaHalf Linie .....(RGB vorher anpassen!)
inline void CBitMap::DrawAlphaSegment(int y1, int y2,int RGB)
{
    // WORKING VERSION In FIXED POINT
    signed int x1, x2, y;


    for (y=y1;y<y2;y++)
    {

        /*eigene inline function schreiben !*/
        x1 = ceil (LeftX);   // Subpixel Accuracy fr Poly-Aussenseiten links  !
        x2 = ceil (RightX);  // Subpixel Accuarcy fr Poly-Aussenseiten rechts !

        unsigned int * pTriangle = pPolyDest + x1;

        for (int z=x1;z<x2;z++)
        {
            int Alpha1 = *pTriangle >> 1;
            Alpha1 = Alpha1 & 0x007f7f7f;
            *pTriangle = Alpha1 + RGB;
            pTriangle++;
        }

        LeftX+=Left_dXdY;
        RightX+=Right_dXdY;
        pPolyDest+=Width + Pitch;  // Eine Zeile tiefer addieren !
    }
}
// Zeichnet eine AlphaAdd Linie .....
inline void CBitMap::DrawAdditiveSegment(int y1, int y2,int RGB)
{
    // WORKING VERSION In FIXED POINT
    signed int x1, x2, y;
		int Pix, Overflow;

    for (y=y1;y<y2;y++)
    {

        /*eigene inline function schreiben !*/
        x1 = ceil (LeftX);   // Subpixel Accuracy fr Poly-Aussenseiten links  !
        x2 = ceil (RightX);  // Subpixel Accuarcy fr Poly-Aussenseiten rechts !

        int* pTriangle = (int*)pPolyDest + (int)(x1);

        for (int z=x1;z<x2;z++)
        {
            Pix=(*pTriangle&(0x00fefeff)) + RGB;

						Overflow=Pix &(0x01010100);
						Overflow=Overflow-(Overflow>>8);
            *pTriangle++=Pix | Overflow;
        }

        LeftX+=Left_dXdY;
        RightX+=Right_dXdY;
        pPolyDest+=Width+Pitch;  // Eine Zeile tiefer addieren !
    }
}

//Funct nur bei gleich grossen BitMaps
void CBitMap::Blend(CBitMap *Source, CBitMap *Dest, int Alpha)	//Alpha 0..127 Source->Dest
{
	int SrcFact, DestFact, ySrcPix, yDestPix, tempPix, tempSrc, tempDst;
	unsigned int *pSrc, *pDest, *result;
	if (Alpha<0) Alpha=0;
	if (Alpha>127) Alpha=127;
	SrcFact=Alpha<<1;
	DestFact=(127-Alpha)<<1;
	pSrc=(unsigned int*)Source->GetBitmap();
	pDest=(unsigned int*)Dest->GetBitmap();
	result=(unsigned int*) BMap;
	for (int y=0;y<Source->GetHeight();y++)
	{
		for (int x=0;x<Source->GetWidth();x++)
		{
			tempPix=0;
			tempSrc=*pSrc;
			tempDst=*pDest;

			ySrcPix =(tempSrc & 0x00ff00ff) * SrcFact;
			yDestPix=(tempDst & 0x00ff00ff)* DestFact;
			tempPix=(((ySrcPix + yDestPix)>>8) & 0x00ff00ff);

			ySrcPix =((tempSrc & 0x0000ff00)) * SrcFact;
			yDestPix=((tempDst & 0x0000ff00))* DestFact;
			tempPix=tempPix | (((ySrcPix + yDestPix)>>8) & 0x0000ff00);

			pSrc++;
			pDest++;
			*result=tempPix;
			result++;
		}
		pSrc+=Source->GetPitch();
		pDest+=Dest->GetPitch();
		result+=Pitch;
	}
}
/////////////////////////////////////////////////////////////////////////////
void CBitMap::BlendPixel(CBitMap *Source, CBitMap *Dest,CBitMap* Mask, int Alpha)
{
	int *pSrc, *pDest, *pMask,*pResult;
	pSrc=Source->GetBitmap();
	pDest=Dest->GetBitmap();
	pMask=Mask->GetBitmap();
	pResult= BMap;
	for (int y=0;y<Source->GetHeight();y++)
	{
		for (int x=0;x<Source->GetWidth();x++)
		{
			if ((*pMask&0xff)<Alpha)
				*pResult = *pSrc;
			else
				*pResult = *pDest;
			pResult++;
			pMask++;
			pSrc++;
			pDest++;
		}
		pSrc+=Source->GetPitch();
		pDest+=Dest->GetPitch();
		pMask+=Mask->GetPitch();
		pResult+=Pitch;
	}
}
/////////////////////////////////////////////////////////////////////////////
void CBitMap::LensFlare(CBitMap *Src, int LX, int LY, int sizeX, int sizeY)
{
	int CenterX = Width/2;
	int CenterY = Height/2;

	int distX = LX - CenterX;
	int distY = LY - CenterY;

	float xco = (float)(distX)/4;
	float yco = (float)(distY)/4;

	LX-=sizeX/2;
	LY-=sizeY/2;

	Src->BltAdd(0*sizeX,0,sizeX,sizeY,this,LX,LY);
	Src->BltAdd(1*sizeX,0,sizeX,sizeY,this,(int)LX-(xco*8),(int)LY-(yco*8));
	Src->BltAdd(2*sizeX,0,sizeX,sizeY,this,(int)LX-(xco*2),(int)LY-(yco*2));
	Src->BltAdd(3*sizeX,0,sizeX,sizeY,this,(int)LX-(xco*6),(int)LY-(yco*6));
	Src->BltAdd(4*sizeX,0,sizeX,sizeY,this,(int)LX-(xco*3),(int)LY-(yco*3));
	Src->BltAdd(5*sizeX,0,sizeX,sizeY,this,(int)LX-(xco*7),(int)LY-(yco*7));
	Src->BltAdd(6*sizeX,0,sizeX,sizeY,this,(int)LX-(xco*5),(int)LY-(yco*5));
	Src->BltAdd(7*sizeX,0,sizeX,sizeY,this,(int)LX-(xco*1),(int)LY-(yco*1));
}

void CBitMap::TextureMapper(Point * vertex,CBitMap * pTexture)
{

	// Hier ist der GROSSE TextureMapper !!!

	//void Morphing::TextureTriangles (unsigned int * Source1,unsigned int* Source2,
    //                       unsigned int TriangleToMorph,unsigned int* pDest,struct AlphaMul * AlphaDummy,int Frame,int MaxFrame)

    //static MPoint * V1;
    //static MPoint * V2;
    //static MPoint * V3;

    Point * V1 = new Point;
    Point * V2 = new Point;
    Point * V3 = new Point;

    V1 = & vertex[0];
    V2 = & vertex[1];
    V3 = & vertex[2];

    /*Schneller wenn man MTemp Static declariert ?*/
    /* oder ASM XCHG benutzen */
    if (V1->y > V2->y)
    {
        Point * MTemp;
        MTemp = V1;
        V1 = V2;
        V2 = MTemp;
    }
    if (V1->y > V3->y)
    {
        Point * MTemp;
        MTemp = V1;
        V1 = V3;
        V3 = MTemp;
    }

    if (V2->y > V3->y)
    {
        Point * MTemp;
        MTemp = V2;
        V2 = V3;
        V3 = MTemp;
    }

    /* TEST VERSION */
/*
    V1->u = 11;
    V2->u = 100;
    V3->u = 200;

    V1->v = 11;
    V2->v = 70;
    V3->v = 220;
*/
    /* TEST VERSION END */

    /*  SubPixel Accuarcy der Polygon Ecken */
    /* DIESE STELLE MUSS NOCHMAL ANGEGUCKT WERDEN */
	int y1i = ceil(V1->y);
    int y2i = ceil(V2->y);
    int y3i = ceil(V3->y);

    if (y1i==y3i) return;

    float prestep;

	pPolyDest = (unsigned int*)GetBitmap() + (y1i*GetBytesPerRow()>>2);

    //pMorphingDest = pDest + ((y1i<<9) + (y1i<<7));

//    pMorphingDest = &pDest[y1i*320];

    /* Konstanten errechnen ...hier kann noch optimiert werden !!! */
    float dXdY_V1V3=(V3->x - V1->x) / (V3->y - V1->y);
    float dXdY_V2V3=(V3->x - V2->x) / (V3->y - V2->y);
    float dXdY_V1V2=(V2->x - V1->x) / (V2->y - V1->y);

    float dUdY_V1V3=(V3->u - V1->u) / (V3->y - V1->y);
    float dUdY_V2V3=(V3->u - V2->u) / (V3->y - V2->y);
    float dUdY_V1V2=(V2->u - V1->u) / (V2->y - V1->y);

    float dVdY_V1V3=(V3->v - V1->v) / (V3->y - V1->y);
    float dVdY_V2V3=(V3->v - V2->v) / (V3->y - V2->y);
    float dVdY_V1V2=(V2->v - V1->v) / (V2->y - V1->y);

    // Stelle fest ob V2 auf der rechten seite oder links
    bool mid = dXdY_V1V3 < dXdY_V1V2;


//          (At-Ct)*(By-Cy) - (Bt-Ct)*(Ay-Cy)
//  dt/dx = ---------------------------------
//          (Ax-Cx)*(By-Cy) - (Bx-Cx)*(Ay-Cy)


// Optimieren denn divisor ist gleich !

    float dUdX_O = ((V1->u - V3->u) * (V2->y - V3->y)) -
                   ((V2->u - V3->u) * (V1->y - V3->y));
    float dUdX_U = ((V1->x - V3->x) * (V2->y - V3->y) -
                   ( V2->x - V3->x) * (V1->y - V3->y));

    float dVdX_O = ((V1->v - V3->v) * (V2->y - V3->y)) -
                   ((V2->v - V3->v) * (V1->y - V3->y));
    float dVdX_U = ((V1->x - V3->x) * (V2->y - V3->y) -
                   ( V2->x - V3->x) * (V1->y - V3->y));

    dUdX = dUdX_O / dUdX_U;
    dVdX = dVdX_O / dVdX_U;

	unsigned int * pTextureMap = (unsigned int*)pTexture->BMap;

	if (!mid)
    {
        // V2 Links
        prestep = SubPixel(V1->y);

        if (y1i==y2i)
        {

            Left_dUdY  = dUdY_V2V3;
            Left_dVdY  = dVdY_V2V3;
            Left_dXdY  = dXdY_V2V3;
			Right_dXdY = dXdY_V1V3;

            LeftU  = V2->u  + SubPixel(V2->y)*Left_dUdY;
            LeftV  = V2->v  + SubPixel(V2->y)*Left_dVdY;
            LeftX  = V2->x  + SubPixel(V2->y)*Left_dXdY;
            RightX = V1->x  + prestep*Right_dXdY;

            DrawTextureSegment(y1i, y3i,pTextureMap);


            return;
        }

        Right_dXdY = dXdY_V1V3;

        if (y1i<y2i)
        {
//            printf("ficken\n");

            Left_dUdY = dUdY_V1V2;
			Left_dVdY = dVdY_V1V2;
			Left_dXdY = dXdY_V1V2;


            LeftU = V1->u   + prestep * Left_dUdY;
            LeftV = V1->v   + prestep * Left_dVdY;
            LeftX = V1->x  + prestep * Left_dXdY;
            RightX = V1->x + prestep * Right_dXdY;

            DrawTextureSegment(y1i, y2i,pTextureMap);
		}
        if (y2i<y3i)
        {
            Left_dXdY = dXdY_V2V3;
			Left_dUdY = dUdY_V2V3;
			Left_dVdY = dVdY_V2V3;

            LeftU = V2->u + SubPixel(V2->y)*Left_dUdY;
            LeftV = V2->v + SubPixel(V2->y)*Left_dVdY;
            LeftX = V2->x + SubPixel(V2->y) * Left_dXdY;

            DrawTextureSegment(y2i, y3i,pTextureMap);
		}
    }
    else
    if (mid) // Positve
    {
        prestep =0;// SubPixel(V1->YP);

        if (y1i==y2i)
        {
            Left_dUdY = dUdY_V1V3;
			Left_dVdY = dVdY_V1V3;
			Left_dXdY = dXdY_V1V3;
			Right_dXdY = dXdY_V2V3;

            LeftU  = V1->u + prestep * Left_dUdY;
            LeftV  = V1->v + prestep * Left_dVdY;
            LeftX  = V1->x + prestep * Left_dXdY;
            RightX = V2->x + SubPixel(V2->y)*Right_dXdY;

            DrawTextureSegment(y1i, y3i,pTextureMap);
			return;
        }

        /* Brauchen wir ja nur einmal Init */
        Left_dXdY = dXdY_V1V3;
		Left_dUdY = dUdY_V1V3;
		Left_dVdY = dVdY_V1V3;

        if (y1i<y2i)
        {
            Right_dXdY = dXdY_V1V2;

            LeftU = V1->u  + prestep * Left_dUdY;
            LeftV = V1->v  + prestep * Left_dVdY;
            LeftX = V1->x  + prestep * Left_dXdY;
            RightX= V1->x  + prestep * Right_dXdY;

            DrawTextureSegment(y1i, y2i,pTextureMap);
		}
        if (y2i<y3i)
		{
            Right_dXdY = dXdY_V2V3;
            RightX = V2->x + SubPixel(V2->y)*Right_dXdY; // XP ge„nfdert
            DrawTextureSegment(y2i, y3i,pTextureMap);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////
// Ceil wird im INNER-LOOP fr die subtexel Accuarcy benutzt !
// Functions Aufruf (y1,y2,Texture = CBitMap->BMap Pointer);
//
/////////////////////////////////////////////////////////////////////////////
void CBitMap::DrawTextureSegment(int y1, int y2,unsigned int * pSource)
//void CBitMap::InitPhong(int Phong, int Col, int Size )
{
	// WORKING VERSION
	// ui, vi können raus !
    unsigned int x1, x2, y, vi=0,ui=0;
    float u,v;
//    printf("Pointer %x \n",pDest);
//    printf("y1 %d \n",y1);
//    printf("y2 %d \n",y2);
    for (y=y1;y<y2;y++)
    {
        x1 = ceil(LeftX);   // Subpixel Accuracy fr Poly-Aussenseiten links  !
        x2 = ceil(RightX);  // Subpixel Accuarcy fr Poly-Aussenseiten rechts !

        float u = LeftU + SubPixel(LeftU);//*dUdX; // vorher left_X
        float v = LeftV + SubPixel(LeftV);//*dVdX;

//        unsigned int * dest = (unsigned int *)pMorphingDest + x1;
        /*war darber falsch !*/

        unsigned int * pTriangle = (unsigned int *)pPolyDest + x1;
		unsigned int * pDest;

        while (x1++<x2)
        {
            // HOHL MURCKS
			*pDest++ = pTriangle[ui + ((vi<<9)+(vi<<7))];
//            *dest++ = *(pSource +  ui+ (vi<<8) + (vi<<6));

            u+=dUdX;
            ui =(int)u ; // _asm FIST benutzen !!! (6 clock ticks)
            v+=dVdX;
            vi=(int)v;
        }
        LeftX+=Left_dXdY;
        RightX+=Right_dXdY;
//        pMorphingDest+=320;
        pDest+=640;					// PITCH BENUTZTEN
        LeftU+=Left_dUdY;
        LeftV+=Left_dVdY;
    }
}
/////////////////////////////////////////////////////////////////////////////
inline float CBitMap::SubPixel(float x)
{
    x = ceil (x)-x;
    return x;
}
///////////////////////////////////////////////////////////////////////////
// Diese Methode Negiert einfach den Bildinhalt ......
///////////////////////////////////////////////////////////////////////////
void CBitMap::XorPattern()
{

	int Lenght    = Width;
	int Hoehe     = Height;
	int * pBuffer = BMap;
	int Pitch     = GetPitch();


	for (int y=0;y<Hoehe;y++)
	{
		for (int x = 0 ; x < Lenght ; x ++)
		{
			*pBuffer = *pBuffer ^ 0x00ffffff ;
			pBuffer++;
		}
	pBuffer+=Pitch;
	}



}
// *************** Line Algorithms *************************
void CBitMap::Line(int x1, int y1, int x2, int y2, int Col)	// A simple Line in a static Color.. non transparent
{
	int dx, dy,i,y,x, dir, temp, totalWidth, offset;
	dx = x2-x1;
	dy = y2-y1;
	totalWidth=Pitch+Width;
	if (ABS(dx)>ABS(dy))
	{
		if (dx<0)				// Über die X-Achse laufen
		{
			temp=x2;		x2=x1;		x1=temp;	//SWAP them
			temp=y2;		y2=y1;		y1=temp;	//SWAP them
			dx = x2-x1;
			dy = y2-y1;
		}
		if (dy>0)
		{
			dir=totalWidth;
		}
		else
		{
			dir=-totalWidth;
			dy=-dy;
		}
		i=dx;
		y=y1;
		dx=dx<<1;dy=dy<<1;
		offset=x1+y1*totalWidth;
		for (x=x1;x<x2+1;x++)
		{
			BMap[offset]=Col;
			offset++;
			i=i-dy;
			if (i<0)
			{	i+=dx;	offset+=dir;	}
		}
	}
	else
	{
		if (dy<0)				// Über die Y-Achse laufen
		{
			temp=x2;		x2=x1;		x1=temp;	//SWAP them
			temp=y2;		y2=y1;		y1=temp;	//SWAP them
			dx = x2-x1;
			dy = y2-y1;
		}
		if (dx>0)
		{
			dir=1;
		}
		else
		{
			dir=-1;
			dx=-dx;
		}
		i=dy;
		x=x1;
		dx=dx<<1;dy=dy<<1;
		offset=x1+y1*totalWidth;
		for (y=y1;y<y2+1;y++)
		{
			BMap[offset]=Col;
			offset+=totalWidth;
			i=i-dx;
			if (i<0)
			{	i+=dy;	offset+=dir;		}
		}
	}
}
void CBitMap::Line(int x1, int y1, int x2, int y2, int col1, int col2)
{
	int dx, dy,i,y,x, dir, temp, col, offset, totalWidth;
	int ra,ga,ba, dr,dg,db,r,g,b;
	dx = x2-x1;
	dy = y2-y1;
	totalWidth =(Pitch + Width);
	if ((dx==0)&&(dy==0))
	{
		BMap[x1+y1*totalWidth]=col1;
		return;
	}
	if (ABS(dx)>ABS(dy))
	{
		if (dx<0)				// Über die X-Achse laufen
		{
			temp=x2;		x2=x1;		x1=temp;	//SWAP them
			temp=y2;		y2=y1;		y1=temp;	//SWAP them
			temp=col1;	col1=col2;col2=temp;
			dx = -dx;//x2-x1;
			dy = -dy;// y2-y1;
		}
		dr=(col2&0xff0000)-(col1&0xff0000);
		dg=(col2&0x00ff00)-(col1&0x00ff00);
		db=(col2&0x0000ff)-(col1&0x0000ff);
		if (dr<0) ra=(-1)<<16;else ra=1<<16;
		if (dg<0) ga=(-1)<<8 ;else ga=1<<8;
		if (db<0) ba=-1;else ba=1;
		dr=ABS(dr)>>15;
		dg=ABS(dg)>>7;
		db=ABS(db)<<1;
		if ((db>dg)&&(db>dr))		//Blau steigt am meisten
		{
			temp=db;	db=dr;	dr=temp;
			temp=b;		b=r;		r=temp;
			temp=ba;	ba=ra;	ra=temp;
		}else
		{	if (dg>dr) //gruen steigt am meisten
			{
				temp=dg;	dg=dr;	dr=temp;
				temp=g;		g=r;		r=temp;
				temp=ga;	ga=ra;	ra=temp;
			}
		}
		col=col1;
		if (dy>0)
		{			dir=totalWidth;		}
		else
		{		dir=-totalWidth; 	dy=-dy;	}
		i=dx;
		r=dx;		g=dr;		b=dr;
		dx=dx<<1;dy=dy<<1;				//Ausserhalb der schleife mit 2 Multiplizieren
		offset=x1+y1*(totalWidth);// Offset berechnen
		for (x=x2-x1;!(x<0);x--)
		{
			BMap[offset]=col;
			offset++;
			i=i-dy;				//	ySteigung
			if (i<0)	 {	i+=dx;	offset+=dir;	}
			r-=dr;				//	R-Wert
			while (r<0)
			{
				r+=dx;	col+=ra;
				g-=dg;				//	G-wert
				b-=db;				//	B-Wert
				while (g<0){	g+=dr;	col+=ga;		}
				while (b<0){	b+=dr;	col+=ba;		}
			}
		}
	}
	else
	{
		if (dy<0)				// Über die Y-Achse laufen
		{
			temp=x2;		x2=x1;		x1=temp;	//SWAP them
			temp=y2;		y2=y1;		y1=temp;	//SWAP them
			temp=col1;	col1=col2;col2=temp;
			dx = -dx ;//x2-x1;
			dy = -dy; //y2-y1;
		}
		dr=(col2&0xff0000)-(col1&0xff0000);
		dg=(col2&0x00ff00)-(col1&0x00ff00);
		db=(col2&0x0000ff)-(col1&0x0000ff);
		if (dr<0) ra=(-1)<<16;else ra=1<<16;
		if (dg<0) ga=(-1)<<8 ;else ga=1<<8;
		if (db<0) ba=-1;else ba=1;
		dr=ABS(dr)>>15;
		dg=ABS(dg)>>7;
		db=ABS(db)<<1;
		if ((db>dg)&&(db>dr))		//Blau steigt am meisten
		{
			temp=db;	db=dr;	dr=temp;
			temp=b;		b=r;		r=temp;
			temp=ba;	ba=ra;	ra=temp;
		}else
		{	if (dg>dr) //gruen steigt am meisten
			{
				temp=dg;	dg=dr;	dr=temp;
				temp=g;		g=r;		r=temp;
				temp=ga;	ga=ra;	ra=temp;
			}
		}
		col=col1;
		if (dx>0)
		{		dir=1;	}
		else
		{		dir=-1; 	dx=-dx;		}
		i=dy;
		r=dy;		g=dr;		b=dr;
		dx=dx<<1;dy=dy<<1;
		offset=x1+y1*(totalWidth);
		for (y=y2-y1;!(y<0);y--)
		{
			BMap[offset]=col;
			offset+=totalWidth;
			i=i-dx;
			if (i<0)		{	i+=dy;	offset+=dir;	}
			r-=dr;				//	R-Wert
			while (r<0)
			{
				r+=dy;	col+=ra;
				g-=dg;				//	G-wert
				b-=db;				//	B-Wert
				while (g<0)	{	g+=dr;	col+=ga;}
				while (b<0)	{	b+=dr;	col+=ba;}
			}
		}
	}
}
void CBitMap::LineAdd(int x1, int y1, int x2, int y2, int col1, int col2)
{
	int dx, dy,i,y,x, dir, temp, col, offset, totalWidth;
	int ra,ga,ba, dr,dg,db,r,g,b;
	dx = x2-x1;
	dy = y2-y1;
	totalWidth =(Pitch + Width);
	if ((dx==0)&&(dy==0))
	{
		BMap[x1+y1*totalWidth]=col1;
		return;
	}
	if (ABS(dx)>ABS(dy))
	{
		if (dx<0)				// Über die X-Achse laufen
		{
			temp=x2;		x2=x1;		x1=temp;	//SWAP them
			temp=y2;		y2=y1;		y1=temp;	//SWAP them
			temp=col1;	col1=col2;col2=temp;
			dx = -dx;//x2-x1;
			dy = -dy;// y2-y1;
		}
		dr=(col2&0xff0000)-(col1&0xff0000);
		dg=(col2&0x00ff00)-(col1&0x00ff00);
		db=(col2&0x0000ff)-(col1&0x0000ff);
		if (dr<0) ra=(-1)<<16;else ra=1<<16;
		if (dg<0) ga=(-1)<<8 ;else ga=1<<8;
		if (db<0) ba=-1;else ba=1;
		dr=ABS(dr)>>15;
		dg=ABS(dg)>>7;
		db=ABS(db)<<1;
		if ((db>dg)&&(db>dr))		//Blau steigt am meisten
		{
			temp=db;	db=dr;	dr=temp;
			temp=b;		b=r;		r=temp;
			temp=ba;	ba=ra;	ra=temp;
		}else
		{	if (dg>dr) //gruen steigt am meisten
			{
				temp=dg;	dg=dr;	dr=temp;
				temp=g;		g=r;		r=temp;
				temp=ga;	ga=ra;	ra=temp;
			}
		}
		col=col1;
		if (dy>0)
		{			dir=totalWidth;		}
		else
		{		dir=-totalWidth; 	dy=-dy;	}
		i=dx;
		r=dx;		g=dr;		b=dr;
		dx=dx<<1;dy=dy<<1;				//Ausserhalb der schleife mit 2 Multiplizieren
		offset=x1+y1*(totalWidth);// Offset berechnen
		for (x=x2-x1;!(x<0);x--)
		{
      int Pix=(BMap[offset]&(0x00fefeff)) + (col & 0x00fefeff);
			int	Overflow=Pix &(0x01010100);
			Overflow=Overflow-(Overflow>>8);
      BMap[offset]=Pix | Overflow;

			offset++;
			i=i-dy;				//	ySteigung
			if (i<0)	 {	i+=dx;	offset+=dir;	}
			r-=dr;				//	R-Wert
			while (r<0)
			{
				r+=dx;	col+=ra;
				g-=dg;				//	G-wert
				b-=db;				//	B-Wert
				while (g<0){	g+=dr;	col+=ga;		}
				while (b<0){	b+=dr;	col+=ba;		}
			}
		}
	}
	else
	{
		if (dy<0)				// Über die Y-Achse laufen
		{
			temp=x2;		x2=x1;		x1=temp;	//SWAP them
			temp=y2;		y2=y1;		y1=temp;	//SWAP them
			temp=col1;	col1=col2;col2=temp;
			dx = -dx ;//x2-x1;
			dy = -dy; //y2-y1;
		}
		dr=(col2&0xff0000)-(col1&0xff0000);
		dg=(col2&0x00ff00)-(col1&0x00ff00);
		db=(col2&0x0000ff)-(col1&0x0000ff);
		if (dr<0) ra=(-1)<<16;else ra=1<<16;
		if (dg<0) ga=(-1)<<8 ;else ga=1<<8;
		if (db<0) ba=-1;else ba=1;
		dr=ABS(dr)>>15;
		dg=ABS(dg)>>7;
		db=ABS(db)<<1;
		if ((db>dg)&&(db>dr))		//Blau steigt am meisten
		{
			temp=db;	db=dr;	dr=temp;
			temp=b;		b=r;		r=temp;
			temp=ba;	ba=ra;	ra=temp;
		}else
		{	if (dg>dr) //gruen steigt am meisten
			{
				temp=dg;	dg=dr;	dr=temp;
				temp=g;		g=r;		r=temp;
				temp=ga;	ga=ra;	ra=temp;
			}
		}
		col=col1;
		if (dx>0)
		{		dir=1;	}
		else
		{		dir=-1; 	dx=-dx;		}
		i=dy;
		r=dy;		g=dr;		b=dr;
		dx=dx<<1;dy=dy<<1;
		offset=x1+y1*(totalWidth);
		for (y=y2-y1;!(y<0);y--)
		{
      int Pix=(BMap[offset]&(0x00fefeff)) + (col&0x00fefeff);
			int	Overflow=Pix &(0x01010100);
			Overflow=Overflow-(Overflow>>8);
      BMap[offset]=Pix | Overflow;

			offset+=totalWidth;
			i=i-dx;
			if (i<0)		{	i+=dy;	offset+=dir;	}
			r-=dr;				//	R-Wert
			while (r<0)
			{
				r+=dy;	col+=ra;
				g-=dg;				//	G-wert
				b-=db;				//	B-Wert
				while (g<0)	{	g+=dr;	col+=ga;}
				while (b<0)	{	b+=dr;	col+=ba;}
			}
		}
	}
}

