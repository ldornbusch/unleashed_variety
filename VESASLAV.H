#ifndef __VESASLAVE__
#define __VESASLAVE__

#include "vesaerr.h"
#include "d:\code\watcom\hermes\hermes.h\hermes.h"
#include "vesavbe.h"
#include "mytype.h"
/////////////////////////////////////////////////////////////////////////////
// Vesa Slave V1.0
// using HERMES 1.2.4 and Submissives Vesa 2.0 API
//
// Version 1.01b
//
// Latest Changes:
//
// Messages can be forbidden exept the Errors !!!
// SwapPages works now perfect
// ClearScreen added
// Need of a No Supported videomodes found ...print routine ...!!!!
// Start to Implement Palette Mode handling
// Have some troubles with da LFB occured...IfLinear has been declared for
// ModeSetting-Routines !
/////////////////////////////////////////////////////////////////////////////
class VesaSlave
{
protected:
    int ModeNummer;
public:
    int Mode_X_Res;
    int Mode_Y_Res;
    int Xlenght;      // <-Copy of Bytes Per Scanline !!!
    int Mode_Bpp;
protected:

    int VirtualModeSize;    /* Diese Mode Spezificationen haben wir gefunden */
    int VirtualModeWidth;
    int VirtualModeHeigh;
    int VirtualWidth;       /* Breite des Screensx4 da ja 32 Bpp mode intern */
public:
    int RenderXlenght;      /* Hilft beim Rechen der X&Y Cords im VirtualScreen */
                             /* ist die taschliche Bytegr”sse eine Zeile */
protected:
    /*Hermes Handle ist vom Type Int */
    HermesHandle Converter;

    /*Hermes Converter Formate anlegen */
    HermesFormat FormatSource;
    HermesFormat FormatDest;

    BOOL TextModeFlag;      /* Wenn TRUE dann ist ein Textmode Aktiv */
    int TextModeNummer;

    VBE_VbeInfoBlock  VideoCardInfo;
    VBE_ModeInfoBlock ActualScreenMode;

public:
    int ModeXlength;
protected:
    int RedBitMask;
    int GreenBitMask;
    int BlueBitMask;
    int AlphaBitMask;

    int RedMaskPos;
    int RedMaskSize;
    int GreenMaskPos;
    int GreenMaskSize;
    int BlueMaskPos;
    int BlueMaskSize;
    int AlphaMaskPos;
    int AlphaMaskSize;

    /*Used to Calculate the ColorMasks for Hermes Format Conversion*/

    unsigned char BitMask[8];

    int DirectColorModeInfo;

    int PictureStartAdress1;
    int PictureStartAdress2;

    unsigned char * pPicturePtr1;
    unsigned char * pPicturePtr2;
    unsigned char * pScreenPointer;
    unsigned char * pBackgroundPointer;
protected:
    int AktivPage;
    int IfLinear;
public:
    /*!! Nur hiermit Arbeiten !! */
    unsigned char * pVirtualScreen;

protected:
    unsigned char * pVirtualModeScreen;
    unsigned char * pDummyPtr;

    unsigned char * pPalette;

    char *Types; // unbenutzt !!!

    int PictureSize;
protected:
    unsigned char VersionString[5];
    BOOL PrintMessages;
public:
    VesaSlave(BOOL ShowMessage);

    virtual ~VesaSlave();

public:
    void PrintError(const char * error);
    void PrintCardType();
    BOOL CheckMode (int Xres,int Yres,int Bpp);
    void PrintUsedMode();
    void SwapPages();
protected:
    BOOL CreateVirtualScreens();
public:
    void ErrorSetMode(int Mode);
    void BlitScreen();
    void SetMode();
    void PrintVersionNumber();
protected:
    void CalcGreyPal();
    void SetPal();
    void ConvertToPalMode();
public:
    void NoModeFound();
    unsigned int * ReturnVirtualScreenPointer();
protected:
    inline CopyFormatedScreen(unsigned int *source, unsigned int *dest,int PictureSize);
public:
    void ClearScreen();
};
#endif
