/////////////////////////////////////////////////////////////////////////////
//                         MEA CULPA BY NICE DEZIGNS
//
// BUILD 0.1 (13.10.99) Erstmals dos version versucht zu machen !
// All 3d Engine includes werden von "engincl.h" definiert !!
//
// BUILD 0.2 (13.11.99) Die KLasse All3DDos Komuniziert mit VesaSlave
//
// BUILD 0.3 (15.11.99) Alte Rawpic Klasse etwas umgestrickt so das
//                      sie jetzt mit CBitMap arbeitet da der Pcx-Loader
//                      vom Lutz ja eh nur 256 Colors Pictures lesen konnte.
//                      Ist ja bei 32Bpp farbtiefe eher doof.
//                      In der Routine LoadPcx knallts dann aber auch wie Sau!
//                          
//                      Denke mal es sollte klappen was ich da mache  !
//                      Der Solid-Poly-Filler muss nun in CBitMapRein !
//                          
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//INCLUDES
////////////////

#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <iostream.h>
#include <dos.h>
#include <time.h>

#include "errors.h"

/////////////////////////////////////////////////////////////////////////////
//own includes
///////////////
#include "mytype.h"
#include "engincl.h"
#include "vesavbe.h"

#include "vesaslav.h"
#include "pcx.h"
#include "rawpic.h"
#include "bitmap.h"
/////////////////////////////////////////////////////////////////////////////
// Functions Declaration
/////////////////////////////////////////////////////////////////////////////
// Pictures
/////////////////////////////////////////////////////////////////////////////
PictureContainer * Background;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void main ()
{
    cout << "--------> MEA CULPA TEST <---------\n";

    // Erschaffe Renderscreen !

    while(kbhit())
        getch();



    CAll3DDos * myWin = NULL;
    myWin = new CAll3DDos();
    if (!myWin)
    {
        cout << "Konnte my win nicht init\n";
        exit(1);
    }
    myWin->RenderSlave->SetMode();



    // Erschaffe Demo Objekt !

    CDemo * intro = NULL;
    intro = new CDemo();


    if (!intro)
    {
        cout << "Konnte Demo Klasse nicht erschaffen\n";
        exit(1);
    }

    while (!kbhit())
    {

        myWin->MessageLoop(intro);
    }

    myWin->RenderSlave->ErrorSetMode(ModeText);
    cout << "                       Mea Culpa By [:NICE:98:DEZIGNS:]\n";
}
/////////////////////////////////////////////////////////////////////////////
