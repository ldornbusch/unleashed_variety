/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*  PCX.H 32 Bit Version V 1.0 */
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifndef _PCX_H_
#define _PCX_H_
/////////////////////////////////////////////////////////////////////////////
#include <string.h> // gemacht da im h-file inline functionen stehen...
#include "mytype.h"
/////////////////////////////////////////////////////////////////////////////
struct PcxRGB
{
    char R;
    char G;
    char B;
};
/////////////////////////////////////////////////////////////////////////////
struct Pcx
{
public:
    char Filename[80];
public:
    unsigned char * pPcxBuffer;
    PcxRGB * pPcxRGB; // MACHE Pointer auf struct

    unsigned int BytesInPcx; // grîsse in bytes eines bildes

    char manufacturer;      // puplic: da spÑter mal abfragbar
    char bitsperpixel;      // Color Depth

    //protected: // rausgenommen wegen Rawpic.h Access

    char version;
	char encoding;
    short xmin; // int's nach short !!!
    short ymin;
    short xmax;
    short ymax;
    short hres;
    short vres;
    char colormap[48];
	char reserved;
    char nplanes;
    short bytesperline;
    short palettetype;
    char filler[58];


protected:
    int readpcxblock(int *pblock,int *pcount,FILE * pcxfile);

public:

    Pcx(const char *pFilename=NULL); // Konstructor in Pcx.cpp

    virtual ~Pcx();

    BOOL Load(const char *pFilename);
    BOOL LoadPcxPalette(const char *pFilename);
    BOOL SetPal();
    void CopyPalette(struct RGB * Dest);
    void SubAddValue(unsigned char Value);

    void ToMem (void *pMem,unsigned lenght)
    {
        if (pPcxBuffer)
            memcpy(pMem,(void*) pPcxBuffer, lenght);
    }

    unsigned char * GetMemAdress()  // liefert Adresse von PcxBuffer zurÅck
    {
        return pPcxBuffer;
    }
};
/////////////////////////////////////////////////////////////////////////////
#endif
