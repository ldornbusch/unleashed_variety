/////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

#include "player.h"
/////////////////////////////////////////////////////////////////////////////
//      Detects Soundcard automaticly even Manual is TRUE,                 //
//      then running Midas Configuation Screen...                          //
//      Es fehlen noch Screen Sync Rountinen z.b SetTimerCallbacks()       //
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::Detect(BOOL ManualConfig)
{
    if (ManualConfig == TRUE)
    {
        MIDASconfig();
        return;
    }

    if ( !MIDASdetectSoundCard() ) // Diese Function sucht nach einer Soundcard!
    {
        if ( !MIDASconfig() )      // Diese Function fragt nach der configuation
        {
            if ( MIDASgetLastError() )
            {
                MIDASerror();
            }
         }
    }
}
/////////////////////////////////////////////////////////////////////////////
bool MusicPlayer::FindRefresh()
{
//    refreshRate = MIDASgetDisplayRefreshRate();

    if ( refreshRate > 0 )
    {
        refreshRate = 60000; /* default 70Hz */

        if ( getch() == 27 ) // wenn hier ESC gedrÅckt wird,dann Dos-Shell !!!
            return FALSE;

    }
    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::Init(char *moduleName)
{
    /* Now initialize MIDAS Digital Audio System: */
    if ( !MIDASinit() )
        MIDASerror();
		MIDASstartBackgroundPlay(0);

    if ( (module = MIDASloadModule(moduleName)) == NULL )
        MIDASerror();
}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::PlayMod()
{
    /* Start playing the module: */

    if ( (playHandle = MIDASplayModule(module, TRUE)) == 0 )
      MIDASerror();

}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::StopMod()
{
    /* Stop playing module: */
    if ( !MIDASstopModule(playHandle) )
        MIDASerror();
}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::FreeMod()
{
   MIDASstopBackgroundPlay();
	/* Deallocate the module: */
    if ( !MIDASfreeModule(module) )
        MIDASerror();
}
/////////////////////////////////////////////////////////////////////////////
// Setzt immer wenn ein command gelesen wurde,diese function aufrufen
// wo der Command gespeichert wird !
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::SetSyncToMusic(void ( callback)(unsigned int,unsigned int,unsigned int))
{
	if (!MIDASsetMusicSyncCallback(playHandle, callback))
                                    //SyncInfo,MusicPlayer.Position,
                                    //MusicPlayer.Row))
        MIDASerror();

}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::RemoveSync()
{
    /* Remove music sync callback: */
    if ( !MIDASsetMusicSyncCallback(playHandle, NULL) )
        MIDASerror();
}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::RemoveTimer()
{
    /* Remove timer callback: */
//    if ( !MIDASremoveTimerCallbacks() )
//        MIDASerror();
}
/////////////////////////////////////////////////////////////////////////////
MusicPlayer::~MusicPlayer()
{
    /* And close MIDAS: */
    if ( !MIDASclose() )
        MIDASerror();

}
/////////////////////////////////////////////////////////////////////////////
bool MusicPlayer::SetVol(unsigned Volume)
{

    if (!MIDASsetMusicVolume(playHandle,Volume))
        return FALSE;
    else
        return TRUE;

}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::SetSync(unsigned refreshrate,    // refresrate des VideoModus
                     BOOL ScreenSync,              // True wenn Sync mit Screen!!
                     void (MIDAS_CALL * VorVR)(void),     // Pointer Kurz VorVR !
                     void (MIDAS_CALL * waerendVR)(void), // Pointer wÑrend VR
                     void (MIDAS_CALL * nachVR)(void)     // Pointer nach VR !
                    )
{
//    MIDASsetTimerCallbacks(refreshrate,ScreenSync,VorVR,waerendVR,nachVR);
}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::SyncCallback(unsigned syncNum, unsigned position, unsigned row)
{


}
/////////////////////////////////////////////////////////////////////////////
MIDAS_CALL MusicPlayer::UpdateInfo()
{
    /* MIDAS_CALL is cdecl for Watcom, empty for DJGPP. Helps calling this
       from assembler, otherwise unnecessary */

    static MIDASplayStatus status;

    /* Get playback status: */
    if ( !MIDASgetPlayStatus(playHandle, &status) )
        MIDASerror();

    /* Store interesting information in easy-to-access variables: */
    Position = status.position;
    Pattern = status.pattern;
    int Row = status.row;
    SyncInfo = status.syncInfo;
}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::MIDASerror()
{
    std::cout << "[PLAYER] " << MIDASgetErrorMessage(MIDASgetLastError()) << "\n";
    std::cout.flush();
}
/////////////////////////////////////////////////////////////////////////////
void MusicPlayer::CheckMusicPos(int * newtimer,int * oldtimer)
{
/*  static int Pos=0;

    static MIDASplayStatus status;

    if ( !MIDASgetPlayStatus(playHandle, &status))
        MIDASerror();

    unsigned Pattern = status.pattern;
    unsigned Row = status.row;

    printf("row %d\n",Row);
    printf("pat %d\n",Pattern);
    printf("PPos %d\t",PatternTimingList[Pos][0]);
    printf("PRow %d\n",PatternTimingList[Pos][1]);
    Pos++; // wech

    if ((PatternTimingList[Pos][0] == Pattern)&& PatternTimingList[Pos][1] == Row)
    {
        *newtimer     = PatternTimingList[Pos][2];
        Pos++;
        printf("FICKEN\n");
        getch();
    }
    //!!eventuelle Timing probleme machen wir hiermit zunichte !!!
    else if ((PatternTimingList[Pos][0] == Pattern)&& PatternTimingList[Pos][1] <= Row+1)
    {
       *newtimer     = PatternTimingList[Pos][2];
       Pos++;
       printf("FICKEN\n");
       getch();
    }
	*/
}
