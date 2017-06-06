/////////////////////////////////////////////////////////////////////////////
// Temp Picture Container for All3D,Converts 24BPP TO 32 BPP !
// RE->Coded at 15.11.99
// MAC SYSTEMS [16:30]  Hatte noch probleme mit der Anbindung an die CBitMap
//                      Klasse da die Var.SizeOfPicture in bytes gez„hlt hat
//                      da ich aber eine inline_asm copyroutine habe die
//                      in dwords copiert hat's mal wieder geklallt...
//                      deswegen vor dem copieren erstmal durch vier geteilt.
//                      SizeofPicture >> 2
//
//
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream.h>
/////////////////////////////////////////////////////////////////////////////
/*Own Libs*/
/////////////////////////////////////////////////////////////////////////////
#include "rawpic.h"
#include "mytype.h"
#include "pcx.h"
/////////////////////////////////////////////////////////////////////////////
// Picture Container !
// ONLY for 32 Bit RGBA-Pictures
/////////////////////////////////////////////////////////////////////////////
void PictureContainer::ErrorPrint(char * errormsg)
{
    printf("[PictureContainer] %s\n",errormsg);
}
/////////////////////////////////////////////////////////////////////////////
// Diese Function wird aufgerufen mit šbergabe eines Pcx-Pointers
// Function liest dan selbst„ndig die Parameter Breite und H”he des Bildes
// und Allociert dann dementsprechend Speicher !!
/////////////////////////////////////////////////////////////////////////////
void PictureContainer::ImportPcxPicture(Pcx * pPcxSource)
{
    int Xres = pPcxSource->xmax+1;
    int Yres = pPcxSource->ymax+1;

    PictureHeight = Yres;
    PictureWidth =  Xres;
    SizeOfPicture = Xres*Yres*4;

    pPicture = new unsigned char [SizeOfPicture];

    if (pPicture == NULL)
    {
        ErrorPrint("No Mem Could Allocated to Store Picture");
        exit(1);
    }

    int x,y=0;

    unsigned char * pSource = pPcxSource->pPcxBuffer;
    unsigned char * pDest   = pPicture;

    for (y=0;y<Yres;y++)
    {

        for (x=0;x<Xres;x++)
        {
            *pDest = *pSource++;
            pDest += 4;
        }
        pSource +=Xres*2;
    }

    pSource = pPcxSource->pPcxBuffer+Xres;
    pDest   = pPicture+1;

    for (y=0;y<Yres;y++)
    {

        for ( x = 0 ; x < Xres ; x++)
        {
            *pDest = *pSource++;
            pDest += 4;
        }
        pSource +=Xres*2;
    }

    pSource = pPcxSource->pPcxBuffer+2*Xres;
    pDest   = pPicture+2;

    for (y=0;y<Yres;y++)
    {

        for ( x = 0 ; x < Xres ; x++)
        {
            *pDest = *pSource++;
            pDest += 4;
        }
        pSource +=Xres*2;
    }

    pSource = pPcxSource->pPcxBuffer+3*Xres;
    pDest   = pPicture+3;

    for (y=0;y<Yres;y++)
    {

        for ( x = 0 ; x < Xres ; x++)
        {
            *pDest = 0;
            pDest += 4;
        }
        pSource += Xres*2;
    }
}
/////////////////////////////////////////////////////////////////////////////
PictureContainer::PictureContainer()
{
    int PictureHeight = FALSE;
    int PictureWidth  = FALSE;
    pPicture = NULL;
}
////////////////////////////////////////////////////////////////////////////
PictureContainer:: ~PictureContainer()
{
    if (pPicture)
        delete pPicture;
}
/////////////////////////////////////////////////////////////////////////////
void PictureContainer::PrintPictureInfos()
{
    cout << "PictureHeight            :" << PictureHeight <<"\n";
    cout << "PictureWidth             :" << PictureWidth  <<"\n";
    cout << "Size of Picture in Bytes :" << SizeOfPicture <<"\n";
}
/////////////////////////////////////////////////////////////////////////////
void PictureContainer::CopyPicture(unsigned int *pDest)
{
   /*casten da vorher char-pointer */
    unsigned int * pSource = (unsigned int *) pPicture;
    unsigned int PicSize = SizeOfPicture >> 2;
    __asm
    {
        mov esi, pSource
        mov edi, pDest
        mov ecx, PicSize
        cld
        rep movsd
    }
}
