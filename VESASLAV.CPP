#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream.h>

/////////////////////////////////////////////////////////////////////////////
// Own Libs
/////////////////////////////////////////////////////////////////////////////
#include "mytype.h"
#include "d:\code\watcom\hermes\hermes.h\hermes.h"
#include "vesavbe.h"
#include "vesaerr.h"
#include "vesaslav.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
VesaSlave::VesaSlave(BOOL ShowMessage)
{
    PrintMessages = ShowMessage;

//    PrintVersionNumber();
    if (PrintMessages)
        printf("[VESA_SLAVE] V1.01b BY MAC SYSTEMS/[:NICE:98:DEZIGNS:]\n");

    VBE_Init();

    if (VBE_Test()==FALSE)
        PrintError("No VBE 2.0 or greater found.");

    VBE_Controller_Information (&VideoCardInfo);

    if ( Hermes_Init() == FALSE)
        PrintError("Could not Init Hermes");

    PrintCardType();

    PictureSize = 0;

    Mode_X_Res = 0; /* nach diesen sachen haben wir gesucht */
    Mode_Y_Res = 0;
    Mode_Bpp   = 0;
    ModeNummer = 0;
    Xlenght    = 0;

    DirectColorModeInfo = 0; /*Mode Info */

    VirtualModeSize   = 0; /* Diese Mode Spezificationen haben wir gefunden */
    VirtualModeWidth  = 0;
    VirtualModeHeigh  = 0;
    VirtualWidth      = 0; /* Breite des Screensx4 da ja 32 Bpp mode intern */

    RenderXlenght     = 0; /* Hilft beim Rechen der X&Y Cords im VirtualScreen */
                           /* ist die taschliche Bytegrîsse eine Zeile */

    /* Color Format Infos for the Output format */

    RedBitMask   = 0; /* The Shifted Mask */
    BlueBitMask  = 0;
    GreenBitMask = 0;
    AlphaBitMask = 0;

    RedMaskSize  = 0; /* The Size of a Mask */
    RedMaskPos   = 0; /* The Position in a DoubleWord */
    BlueMaskSize = 0;
    BlueMaskPos  = 0;
    GreenMaskSize= 0;
    GreenMaskPos = 0;
    AlphaMaskSize= 0;
    AlphaMaskPos = 0;

    /*Used to Calculate the ColorMasks for Hermes Format Conversion*/
    BitMask[0] = 1;
    BitMask[1] = 2;
    BitMask[2] = 4;
    BitMask[3] = 8;
    BitMask[4] = 16;
    BitMask[5] = 32;
    BitMask[6] = 64;
    BitMask[7] = 128;

    /*Hermes Converter Index*/
    Converter    = 0;

    TextModeFlag  = TRUE;  /* Wenn TRUE befinden wir uns im Textmode */
    TextModeNummer= 0;     /* Noch nicht benutzt wird alten text mode erthalten */

    ModeXlength   =0;

    /* Seiten zeiger fÅr Double Buffering */
    PictureStartAdress1 = 0;
    PictureStartAdress2 = 0;

    AktivPage = 1;

    /* POINTER */
    pPicturePtr1       = NULL;
    pPicturePtr2       = NULL;

    pVirtualScreen     = NULL;
    pVirtualModeScreen = NULL;

    pPalette = NULL;

    pScreenPointer     = NULL;

    pBackgroundPointer = NULL;

    VersionString[0] = '1';
    VersionString[1] = '.';
    VersionString[2] = '0';
    VersionString[3] = '1';
    VersionString[4] = 'B';

}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
VesaSlave::~VesaSlave()
{
    printf("Destructor von vesaslave aufgerufen\n");

    if (pVirtualScreen)
        delete pVirtualScreen;
    if (pVirtualModeScreen)
        delete pVirtualModeScreen;
    if (pPalette)
        delete pPalette;

    /*
        Falls noch Speicher freizugeben ist,mache dies !
    */

    Hermes_ConverterReturn(Converter);

    if (Hermes_Done()==FALSE)
        PrintError("Deinitializing Hermes went wrong");

    /* Destroy VBE-Handler !*/
    VBE_Done();
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::PrintCardType()
{
    VBE_Controller_Information(&VideoCardInfo);

    if (PrintMessages)
    {
        printf("[VESA_SLAVE] %s ",VideoCardInfo.OemProductNamePtr);
        printf("%4s",VideoCardInfo.vbeSignature);
        printf("%hi.%hi ",VideoCardInfo.vbeVersion.hi,VideoCardInfo.vbeVersion.lo);
        printf("%hiKb\n",VideoCardInfo.TotalMemory*64);
    }
}
////////////////////////////////////////////////////////////////////////////
void VesaSlave::PrintUsedMode()
{
    if (PrintMessages)
    {
        printf("[VESA_SLAVE] %dx%dx%d for Display found\n",Mode_X_Res,Mode_Y_Res,Mode_Bpp);
        delay(1000);
    }
}
/////////////////////////////////////////////////////////////////////////////
// Function VBE_FindMode is Buggy on my system ..so i written a own ....
//
// Function speichert die Infoz eines Mode ab insofern es einen gefunden hat.
//
// Der Hermes Handle wird erst hier Init,vorher wird nur Hermes_Init aufgerufen
/////////////////////////////////////////////////////////////////////////////
BOOL VesaSlave::CheckMode(int Xres,int Yres,int Bpp)
{
    for (unsigned short mode=0;mode<8192;mode++)
    {
        VBE_Mode_Information ( mode , &ActualScreenMode);

        if ((ActualScreenMode.XResolution == Xres) &&
            (ActualScreenMode.YResolution == Yres) &&
            (ActualScreenMode.BitsPerPixel== Bpp))
        {
                /*
                   Hier mÅsen jetzt all mode sachen erledigt werden..
                   VirtalScreens etc ..SwapPages Init
                   Pointer auf Screen(ScreenPointer),

                   !BitMasken mÅssen noch Initialisiert werden!
                */

                if (!VBE_IsModeLinear(mode))
                    IfLinear = 0 ; // Disable Linear Frame Buffer
                else
                    IfLinear = 1 ; // Set Linear Frame Buffer

                Mode_X_Res = Xres;
                Mode_Y_Res = Yres;
                Mode_Bpp   = Bpp;
                ModeNummer = mode;
                Xlenght    = ActualScreenMode.BytesPerScanline;

                VirtualModeSize = Mode_Bpp/8;

                RenderXlenght = Mode_X_Res << 2;

                RedMaskSize         = ActualScreenMode.RedMaskSize;
                RedMaskPos          = ActualScreenMode.RedFieldPosition;
                GreenMaskSize       = ActualScreenMode.GreenMaskSize;
                GreenMaskPos        = ActualScreenMode.GreenFieldPosition;
                BlueMaskSize        = ActualScreenMode.BlueMaskSize;
                BlueMaskPos         = ActualScreenMode.BlueFieldPosition;
                AlphaMaskSize       = ActualScreenMode.RsvdMaskSize;
                AlphaMaskPos        = ActualScreenMode.RsvdFieldPosition;
                DirectColorModeInfo = ActualScreenMode.DirectColorModeInfo;

                if (CreateVirtualScreens()==FALSE)
                    PrintError("Can't Allocate Virtual Screens");

                if (Mode_Bpp==8)
                {
                    pPalette = new unsigned char [256];
                    if (pPalette == NULL)
                        PrintError("Can't Allocate Palette Array");
                }

                /* Init Seiten-Zeiger */
                PictureStartAdress1 = 0;
                PictureStartAdress2 = Mode_Y_Res;

                /* Wichtige Variable einfach durch 4 dann hat man double-words*/

//                PictureSize = (Xlenght * Mode_Y_Res)*(Bpp/8);
                PictureSize = (Xlenght * Mode_Y_Res)>>2;
//                cout << "Picture Size is "<< PictureSize << "\n";

                /* Set Source Color Format */

                FormatSource.indexed = 0;       // Kein Paletten Mode
                FormatSource.bits   = 32;       // Bpp
                FormatSource.r      = 0xff0000; // Red Field Pos
                FormatSource.g      = 0x00ff00; // Green Field Pos
                FormatSource.b      = 0x0000ff; // Blue Field
                FormatSource.a      = 0;        // Alpha Field Pos

                /* Calc Destination Color Format */

                FormatDest.indexed=0; // 0 wenn dircet color format am start
                                      // sonst 1 bei palette
                                      // obwohl Hermes convertierung nach
                                      // Paletten modes noch nicht kann !!!

                FormatDest.bits = Mode_Bpp;   // Bits Per Pixel

                /*Calc Red Mask */

                for (int r=0;r<RedMaskSize;r++)
                    RedBitMask += BitMask[r];

                RedBitMask = RedBitMask << RedMaskPos;

                /*Calc Green Mask*/
                for (int g=0;g<GreenMaskSize;g++)
                    GreenBitMask += BitMask[g];

                GreenBitMask = GreenBitMask << GreenMaskPos;

                /*Calc Blue Mask*/
                for (int b=0;b<BlueMaskSize;b++)
                    BlueBitMask += BitMask[b];

                BlueBitMask= BlueBitMask << BlueMaskPos;

                /*Calc Alpha Mask*/
                for (int a=0;g<AlphaMaskSize;a++)
                    AlphaBitMask += BitMask[a];

                AlphaBitMask = AlphaBitMask << AlphaMaskPos;

                /* Set Destination Color Format */
                FormatDest.indexed=0; // 0 wenn dircet color format am start
                                      // sonst 1 bei palette
                                      // obwohl Hermes convertierung nach
                                      // Paletten modes noch nicht kann !!!

                FormatDest.bits = Mode_Bpp;   // Bits Per Pixel

                FormatDest.r = RedBitMask;    //  Red   Component Pos.
                FormatDest.g = GreenBitMask;  //  Green Component Pos.
                FormatDest.b = BlueBitMask;   //  Blue  Component Pos.
                FormatDest.a = AlphaBitMask;  //  Alpha Channel   Pos

                /*
                cout << "AlphaBit Mask :" << AlphaBitMask<< "\n";
                cout << "RedBit Mask   :" << RedBitMask  << "\n";
                cout << "BlueBit Mask  :" << BlueBitMask << "\n";
                cout << "GreenBit Mask :" << GreenBitMask<< "\n";


                cout << "Size A:"<< AlphaMaskSize << "\n";
                cout << "Size R:"<< RedMaskSize   << "\n";
                cout << "Size G:"<< GreenMaskSize << "\n";
                cout << "Size B:"<< BlueMaskSize  << "\n";

                cout <<"Any Key";
                getch();
                */

                /* Suche Passenden Converter zwischen zwei BildFormaten/Grîssen */
                /* Kein Dither benutzten sondern,einfach nur runterrechnen wenn  */
                /* erforderlich*/

                Converter = Hermes_ConverterInstance(HERMES_CONVERT_NORMAL);

                if (!Converter)
                    PrintError("Could not Convert between Internal/External Image Formats");

                mode=0xffff;
                return TRUE;
        }
    }
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// Function Allociert VirtualScreen immer ! in 32 bit Farbiefe !!!
// Also Rechnen wir alles in auf die gewÅnschte farbtiefe runter !!!
/////////////////////////////////////////////////////////////////////////////
BOOL VesaSlave::CreateVirtualScreens()
{
    /*Dieser Screen ist ! INTERN ! */
    pVirtualScreen = new unsigned char [(Mode_X_Res*Mode_Y_Res)<<2]; // *4
    if (!pVirtualScreen)
        return FALSE;
    memset(pVirtualScreen,0,((Mode_Y_Res*Mode_Y_Res)<<2));

    /*Dieser Screen ist ! EXTERN !*/
    pVirtualModeScreen = new unsigned char [Xlenght*Mode_Y_Res];
    if (!pVirtualModeScreen)
        return FALSE;

    memset(pVirtualModeScreen,0,Xlenght*Mode_Y_Res);

    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::PrintError(const char * error)
{
    if (!TextModeFlag)
        ErrorSetMode(ModeText);

    printf("[VESA_SLAVE] %s\n",error);
    exit(1);
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::ErrorSetMode (int Mode)
{
    union REGS r;

    r.h.ah = 0;
    r.h.al =(char) Mode;
    int386 (0x10 , &r , &r);

    if (Mode == 3)
        TextModeFlag = TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// !!! SetMode prÅft auch ob mode zulÑssig !!!
// !!! Jetzt werden auch erst PictureStartAdress Initialisiert !!!
// !!! Hier kînnte man doch direkt den Screen lîschen,dann hat man !!!
// !!! direkt alle 2 Seiten schîn sauber ????? !!!
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::SetMode()
{
    if (!ModeNummer == FALSE)
    {

        pScreenPointer = (unsigned char *) VBE_GetVideoPtr(ModeNummer);

//        printf("Linear Frame Buffer Adress 0x%x\n",pScreenPointer);
//        printf("IfLinear %d\n",IfLinear);

        if (pScreenPointer == NULL)
            PrintError("Screen Pointer not Valid");


        VBE_SetMode(ModeNummer,IfLinear,1);
//        ErrorSetMode(3);
//        exit(1);


        TextModeFlag = FALSE;

        pPicturePtr1 = pScreenPointer;

            /*Mode_X_Res ersetzt mit Xlenght ???? RICHTIG ???*/
            /*1280 / 4 = 320 , 320x240 */

            pPicturePtr2 = pScreenPointer + (Xlenght* Mode_Y_Res);

        AktivPage = 1;

        if (Mode_Bpp==8)
            SetPal();
    }
    else
        PrintError("Mode not Valid");

}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::BlitScreen()
{
    if (Mode_Bpp==8)
    {
        ConvertToPalMode();
        return;
    }
    if (Hermes_ConverterRequest(Converter,&FormatSource,&FormatDest)==FALSE)
        PrintError("Couldn't Find Converter\n");

    /*
      Complete function declaration: Hermes_ConverterCopy(HermesHandle
      handle, void *s_pixels,int s_x,int s_y,int s_width,int s_height,int
      s_pitch, void *d_pixels,int d_x,int d_y,int d_width,int d_height,int
      d_pitch)

      Also sollte die Destinaion Breite nicht gleich der Source Breite sein
      ist dies Åberhaupt nicht schlimm,denn die SourceBreite Kann einfach
      als dest Breite benutzt werden nur die genauen BytesPerScanline/Xlenght
      muss halt stimmen.
    */

    Hermes_ConverterCopy(Converter,      // Converter Handle
                         pVirtualScreen, // Adresse Source Screen !
                         0,              // Anfang Pos->X des Source Screen
                         0,              // Anfang Pos->Y des Source Screen
                         Mode_X_Res,     // Breite Source Screen in Pixels
                         Mode_Y_Res,     // Hîhe   Source Screen in Pixels
                         Mode_X_Res*4,   // Breite in Bytes des Source Screens
                         pVirtualModeScreen, // Dest. Screen Adresse
                         0,              // Anfang Pos->X des Dest Screens
                         0,              // Anfang Pos->Y des Dest Screens
                         Mode_X_Res,     // Breite des Dest Screens in Pixels
                         Mode_Y_Res,     // Hîhe   des Dest Screens
                         Xlenght);       // Bytes per Scanline Dest Screen
                                         // Somit des "RealScreens" !!

    CopyFormatedScreen((unsigned int*)pVirtualModeScreen,(unsigned int*)pScreenPointer,PictureSize);
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::SwapPages()
{
//    static int AktivPage = 1;

    if (AktivPage==1)
    {
        AktivPage=2;
        VBE_SetDisplayStart(0,PictureStartAdress2);
        pScreenPointer = pPicturePtr1;
    }
    else
    {
        AktivPage=1;
        VBE_SetDisplayStart(0,PictureStartAdress1);
        pScreenPointer = pPicturePtr2;
    }
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::PrintVersionNumber()
{
//    cout << "[VesaSlave] Version" << VersionString ;
//    cout << " Coded by MAC SYSTEMS /[:NICE:98:DEZIGNS:]\n";

    if (PrintMessages)
    {
        printf("[VesaSlave] Version %s",VersionString);
        printf(" Coded by MAC SYSTEMS /[:NICE:98:DEZIGNS:]\n");
    }
}
/////////////////////////////////////////////////////////////////////////////
inline VesaSlave::CopyFormatedScreen(unsigned int *source, unsigned int *dest,int PictureSize)
{
//    memcpy(dest,source,PictureSize);
    __asm
    {
        mov esi, source
        mov edi, dest
        mov ecx, PictureSize /// muss hier aus screensize achten !!!!!
        cld
        rep movsd
    }
}
/////////////////////////////////////////////////////////////////////////////
// Lîsche VirtualScreen..
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::ClearScreen()
{
    /*gemacht da ich keine ahnung habe wie ich aus klasse vesaslave zugreifen
      kann !  */

    unsigned int * pTrashPointer = (unsigned int*)pVirtualScreen;

    __asm
    {
        mov edi, pTrashPointer
        mov ecx, 76800        // 320*240
        xor eax, eax
        cld
        rep stosd
    }
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::CalcGreyPal()
{
    for (unsigned int c=0;c<256;c++)
        pPalette[c] = (char)(c/64)&63;
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::SetPal()
{
    char a;
    outp (0x03c8,0);
    for (unsigned int c = 0; c < 256 ; c++)
    {
        a = pPalette[c];
        outp(0x03c9,a);
        outp(0x03c9,a);
        outp(0x03c9,a);
    }
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::ConvertToPalMode()
{
    char r;
    char g;
    char b;

    unsigned char newcolor;

    unsigned char * pConverter = pVirtualScreen;
    unsigned char * pDestination = pVirtualModeScreen;

    for (int y = 0 ; y < Mode_Y_Res ; y++)
    {
        for (int x = 0 ; x < Mode_X_Res ; x++)
        {
            r = *(pConverter++);
            g = *(pConverter++);
            b = *(pConverter++);
            pConverter++;


//              newcolor =(char) ((r + g+g + b) >>2);
//            newcolor = (r+r + g +g+g + b+b) >>4;
//            newcolor = (r+r+r + g +g+g+g + b+b+b) >>6;
//            newcolor = (r+r+r+r + g+g+g+g+g + b+b+b+b) >>6; // !!
//            newcolor = (r2 + g<<3 +b<<2 ) >>; //!!
            newcolor =(char)((float) (r*0.32) + (g * 0.57) + (b *0.11));

            *(pDestination++) = newcolor;
        }
    }
    CopyFormatedScreen((unsigned int*)pVirtualModeScreen,(unsigned int*)pScreenPointer,PictureSize);
}
/////////////////////////////////////////////////////////////////////////////
void VesaSlave::NoModeFound()
{
    ErrorSetMode(ModeText);
    cout << No_Videomodes_found;
}
/////////////////////////////////////////////////////////////////////////////
unsigned int * VesaSlave::ReturnVirtualScreenPointer()
{
    return (unsigned int *) pVirtualScreen;
}
