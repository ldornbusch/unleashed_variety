/////////////////////////////////////////////////////////////////////////////
// All3DDos Anbindung an VesaSlave,eqvalent zur All3DWin Klasse / DirectX
//
// Die Member-Function PeekMessage wird nun nachgebildet !
//
/////////////////////////////////////////////////////////////////////////////
#include <conio.h> // printf
#include <stdio.h> // printf
#include <stdlib.h> // exit
#include <time.h> // exit

#include "vesaslav.h"
#include "all3ddos.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CAll3DDos::~CAll3DDos()
{
    printf("------>Destructor Von Call3DDos aufgrufen<-------\n\a\a\a\n");

    if(RenderSlave)
        delete RenderSlave;
}
/////////////////////////////////////////////////////////////////////////////
// This is the Specific Constructor for Mode Searching in a selected
// Color Depth
/////////////////////////////////////////////////////////////////////////////
CAll3DDos::CAll3DDos(int ScreenX,int ScreenY,int Bpp)
{
    printf("Specific CAll3DDos Constructor Aufgerufen\n");

    RenderSlave = NULL;
    RenderSlave = new VesaSlave(TRUE);
    if(!RenderSlave)
    {
        printf("[DEMO_ERROR] Could Not Init VesaMaschine\n");
        exit(1);
    }

    if (!RenderSlave->CheckMode(ScreenX,ScreenY,Bpp)==TRUE)
        RenderSlave->PrintError(No_Videomodes_found);

    RenderSlave->PrintUsedMode();

    BackBuffer = NULL;
    BackBuffer = RenderSlave->ReturnVirtualScreenPointer();
}
/////////////////////////////////////////////////////////////////////////////
CAll3DDos::CAll3DDos()
{
    printf("----> Generic Constuctor Aufgerufen <-----\n");

    RenderSlave = NULL;
    RenderSlave = new VesaSlave(TRUE);
    if (!RenderSlave)
    {
        printf("Could Not Init VesaMaschine\n");
        exit(1);
    }

    if (!RenderSlave->CheckMode(640,480,32)==TRUE)
       if(!RenderSlave->CheckMode(640,480,24)==TRUE)
            if(!RenderSlave->CheckMode(640,480,16)==TRUE)
                if(!RenderSlave->CheckMode(640,480,8)==TRUE)
                    RenderSlave->NoModeFound();

    RenderSlave->PrintUsedMode();

    BackBuffer = NULL;
    BackBuffer = RenderSlave->ReturnVirtualScreenPointer();

    InitSurfaces();
}
/////////////////////////////////////////////////////////////////////////////
void CAll3DDos::MessageLoop(CDemo * objDemo)
{
    int startsec = clock();

    if (!DemoScreen)
    {
        printf("MessageLoop DemoScreen Konstuctor not VALID!\n");
        exit(1);
    }

    if (!BackBuffer)
        printf("BackBuffer is not Valid\n");

   objDemo->UpdateFrame(DemoScreen);

    RenderSlave->BlitScreen();       // Zeichne neue Seite !
    RenderSlave->SwapPages();        // Die Seite Auf der VIDEO-KARTE wechseln

    int endsec = clock();

//    printf("FPS: %d\r",(CLOCKS_PER_SEC/(endsec-startsec)));

}
/////////////////////////////////////////////////////////////////////////////
void CAll3DDos::InitSurfaces()
{
    RenderSlave->SetMode();
    BackBuffer = (unsigned int*)RenderSlave->pVirtualScreen;

    DemoScreen = NULL;
    DemoScreen = new CBitMap();
    if (!DemoScreen)
    {
        printf("Konstructor from CBitMap not Valid!\n");
        exit(1);
    }
    DemoScreen->Init((int*)BackBuffer,640,480,32,0);
//    printf("DemosScreen Address 0x%x\n",DemoScreen);
}
/////////////////////////////////////////////////////////////////////////////
