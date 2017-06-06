/////////////////////////////////////////////////////////////////////////////
//  Midas Wrapper V1.0
//  by Mac Systems / [:NICE:98:DEZIGNS:]
/////////////////////////////////////////////////////////////////////////////
#ifndef __MIDASWRAPPER__
#define __MIDASWRAPPER__
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include "midasdll.h"
//#include "pattern.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
struct MusicPlayer
{
    int NewFrame;

    MIDASmodule module;                     /* Der Module */
    MIDASmodulePlayHandle playHandle;       /* Das Playing Handle */

    /////////////////////////////////////////////////////////////////////////////
    // Midas Sync Variables !
    /////////////////////////////////////////////////////////////////////////////
    unsigned        Position;               /* Current position */
    unsigned        Pattern;                /* Current pattern number */
    unsigned        Row;                    /* Current row number */
    int             SyncInfo;               /* Music synchronization info */

    static MIDASplayStatus status;

    char *moduleName;                        /*Module Name */
    unsigned refreshRate;                   /* Display refresh rate */
//    setbuf(stdout, NULL);

    /////////////////////////////////////////////////////////////////////////////
    // Functions Declarations !
    /////////////////////////////////////////////////////////////////////////////

    void Detect(BOOL ManualConfig);
    bool FindRefresh();
    void Init(char *moduleName);
    void PlayMod();
    bool SetVol(unsigned Volume);

	void StopMod();
	void FreeMod();
	void RemoveSync();
	void RemoveTimer();
	void SetSync(unsigned refreshrate,
                     BOOL ScreenSync,
                     void (MIDAS_CALL * VorVR)(void),
                     void (MIDAS_CALL * waerendVR)(void),
                     void (MIDAS_CALL * nachVR)(void)
                );

    static void MIDAS_CALL SyncCallback(unsigned syncNum,
                      unsigned position,
                      unsigned row);


    MIDAS_CALL UpdateInfo();
    void SetSyncToMusic(void (MIDAS_CALL callback)(unsigned int,unsigned int,unsigned int));
    void MIDASerror();
    void CheckMusicPos(int * newtimer,int * oldtimer);

    ~MusicPlayer();

    MusicPlayer()                           // Konstructor
    {
        MIDASstartup();

        moduleName = NULL;

        refreshRate = 0;
        SyncInfo    = 0;
        NewFrame    = 0;
    }
};
#endif
