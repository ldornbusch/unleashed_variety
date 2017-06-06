////////////////////////////////////////////////////////////////////////////
//   Pcx Loader Klasse !
/////////////////////////////////////////////////////////////////////////////


//#include "mytype.h"


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include "rawpic.h"
#include "pcx.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
inline short GetShort(FILE * fp)
{
    int ch1 = getc(fp);
	if (feof(fp))
        return ch1;

	int ch2 = getc(fp);
	if (feof(fp))
		return ch1;

    return (short) ch1 + ch2 * 256;

}
/////////////////////////////////////////////////////////////////////////////
Pcx::~Pcx()
{
    if (pPcxBuffer)
        delete (pPcxBuffer);

    if (pPcxRGB)
        delete (pPcxRGB);
}
/////////////////////////////////////////////////////////////////////////////
Pcx::Pcx(const char *pFilename)
{
    pPcxBuffer  = NULL;
    pPcxRGB = new PcxRGB [256];
    Filename[0] = 0;


    if (pFilename)
    {
        Load(pFilename);
//        LoadPcxPalette(pFilename);

    }
    else
    {
        manufacturer=0;
        version=0;
        encoding=0;
        bitsperpixel=0;
        xmin=0;
        ymin=0;
        xmax=0;
        ymax=0;
        hres=0;
        vres=0;
        memset(colormap, 0,48);
        reserved=0;
        nplanes=0;
        bytesperline=0;
        palettetype=0;
        memset(filler,0,58);
        BytesInPcx = 0;
    }

}
/////////////////////////////////////////////////////////////////////////////
BOOL Pcx::Load(const char *filename)
{
    int i;
    long l, linesize;
    int block, count;
    FILE *pcxfile = fopen(filename, "rb");

    if (!pcxfile)
        return FALSE;

    strcpy(Filename, filename); // rette filename fÅr palette !!!

    manufacturer = getc(pcxfile);

    /*                     Keine Abfrage momentan !!!!
    if (manufacturer != 10)
    {
        fclose(pcxfile);
        return FALSE;
    }
    */


    version = getc(pcxfile);
    encoding = getc(pcxfile);
    bitsperpixel = fgetc(pcxfile);

    xmin = GetShort(pcxfile);
    ymin = GetShort(pcxfile);
    xmax = GetShort(pcxfile);
    ymax = GetShort(pcxfile);
    hres = GetShort(pcxfile);
    vres = GetShort(pcxfile);

    fread(colormap, 48, sizeof(char), pcxfile);

    reserved = getc(pcxfile);
    nplanes = getc(pcxfile);
    bytesperline = GetShort(pcxfile);
    palettetype = GetShort(pcxfile);
    fread(filler, 58, sizeof(char), pcxfile);

    BytesInPcx = (xmax-xmin+1)*(ymax-ymin+1); // ERRECHNE Byte Grîsse !!!
    BytesInPcx = BytesInPcx * nplanes;        // Falls Truecolor anzahl Planes nehmen

    if (pPcxBuffer == NULL)
        pPcxBuffer = new unsigned char [BytesInPcx];

    if (!pPcxBuffer)
    {
        printf("Can't Allocate enough memory in function BOOL Pcx::Load(const char *filename)");
        exit(1);
    }

    linesize = (long) bytesperline * nplanes *
           (1 + ymax - ymin);

    unsigned char *pWork = pPcxBuffer;
    for (l = 0;
         l < linesize && pWork <= pPcxBuffer+BytesInPcx; )             /* increment by count below */
    {

        if (readpcxblock(&block,
           &count,
           pcxfile) == EOF)
            break;

        for (i = 0; i < count; i++)
        {
            *pWork++ = block;
        }
        l += count;
    }

    fclose (pcxfile);
    return TRUE;

}
/////////////////////////////////////////////////////////////////////////////
int Pcx::readpcxblock(int *pblock, int *pcount, FILE *pcxfile)
{
    int i;
    *pcount = 1;
    if ((i = getc(pcxfile)) == EOF)
        return(EOF);

    if (0xc0 == (0xc0 & i))      /* is it greater than 192? */
    {
        *pcount = 0x3f&i;      /* subtract 192 to get count */
        if((i = getc(pcxfile)) == EOF)
            return(EOF);
    }
    *pblock = i;
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Pcx::LoadPcxPalette(const char *pFilename)
{
    FILE * pcxfile;
    if ((pcxfile = fopen (pFilename,"rb")) == NULL)
    {
        printf("Can't open file %s\n in Function int Pcx::readpcxblock(int *pblock, int *pcount, FILE *pcxfile)\n",*pFilename);
        getch();
        return FALSE;
    }

    long PalPos=0;
    PalPos= filelength (pcxfile->_handle); // Hole Dateigrîsse und ziehe 768 ab
    PalPos = (PalPos - 256*3);

    if (fseek(pcxfile,PalPos,SEEK_SET))  //768 Bytes vor datei ende!!
    {
        printf("Error in fseek \n int Pcx::readpcxblock(int *pblock, int *pcount, FILE *pcxfile)\n");
        getch();
        return FALSE;
    }

    fread (pPcxRGB,256*3,1,pcxfile);

    if (fclose (pcxfile) == EOF)
    {
        printf("Can`t close %s \n int Pcx::readpcxblock(int *pblock, int *pcount, FILE *pcxfile)\n",pcxfile);
        getch ();
        return FALSE;
    }

    for (int i=0; i < 256; i++)
    {
            pPcxRGB[i].R = pPcxRGB[i].R >> 2;
            pPcxRGB[i].G = pPcxRGB[i].G >> 2;
            pPcxRGB[i].B = pPcxRGB[i].B >> 2;
    }
    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL Pcx::SetPal()
{
    if (!Filename[0])
        return FALSE;

    return LoadPcxPalette (Filename);
}
/////////////////////////////////////////////////////////////////////////////
// Copiere Palette nach dest !!!
/////////////////////////////////////////////////////////////////////////////
void Pcx::CopyPalette(struct RGB * Dest)
{
    memcpy (Dest,pPcxRGB,256*3);
}
/////////////////////////////////////////////////////////////////////////////
void Pcx::SubAddValue(unsigned char Value)
{
	 unsigned char *pDummyBuffer = (unsigned char *)pPcxBuffer;

     for (unsigned int va=0 ; va < BytesInPcx; va++)
	 {
          if (*pDummyBuffer)  // Teste ob null
				*pDummyBuffer += Value;

		  pDummyBuffer++;
	 }
}
/////////////////////////////////////////////////////////////////////////////
