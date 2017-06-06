/////////////////////////////////////////////////////////////////////////////
// Dos-Basierter Screen Manager
// [10.11.99]  13:00h Die Klasse Sucht falls verlangt selber nach Screen Modi
// MAC SYSTEMS        oder aber man sagt nur das man folgende Auflîsung haben
//                    mîchte,VesaSlave wird als RenderSlave instanziert.
//                    Muss mir nochmal denn destructor von VesaSlave&Call3DDos
//                    etwas nÑher angucken,weiss nicht ober der VirtualScreen
//                    etc..freigegeben wird..virtueller destuctor nîtig !?
//                    Ich schÑtze das das doch schon ein gutes grundgerÅst
//                    geworden ist !
//
//  [11.11.99]  15.00 Verschiedene Konstructoren gemacht,damit die klasse
//  MAC SYSTEMS       flexibl auf configuationen ist,suche nach modes mit
//                    bestimmter farbtiefe sind mîglich
//
//              16.00 Hurra,Endlich denn scheiss Bug gefunden!
//                    Dieser Bug nÑhmlich hatte denn seiten Effect das
//                    alle Variablen etc.nach dem Aufruf des Konstructors
//                    nicht mehr gÅltig waren,mit anderen worten es stand
//                    nur noch bockmist da drin!
//                    Habe nun nur einen pointer vom typ VesaSlave in der
//                    Klasse Call3ddos dieser wird auf null gesetzt vor
//                    dem allocieren zum testen ob auch alles i.o !
//                    Mann sollte nie VesaSlave * RenderSlave = NULL
//                    im konstructor schreiben,RenderSlave = NULL reicht da
//                    aus,aber das das solche wirkung hat konnte ich nicht
//                    wissen ...auf.. auf weiter gehts's !
//
//  [12.11.99]  16.?? Die Anbindung der Engine an denn ScreenPointer liegt
//  MAC SYSTEMS       jetzt sehr nah !
//
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __DOS_SCREEN_MANAGER__
#define __DOS_SCREEN_MANAGER__
#include "vesaslav.h"
#include "bitmap.h"
#include "demo.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class CAll3DDos
{
public:
    /* Suche nach Auflîsung und finde passende Farbtiefe */
    CAll3DDos();

    /* Suche Explizit nach dieser Auflîsung mit der Farbtiefe */
    CAll3DDos(int ScreenX,int ScreenY,int Bpp);

    virtual ~CAll3DDos();

    void MessageLoop(CDemo * objDemo);
    void InitSurfaces();

    unsigned int * BackBuffer; //Dx compatible !
    VesaSlave * RenderSlave;
    CBitMap * DemoScreen;
};
/////////////////////////////////////////////////////////////////////////////
#endif
