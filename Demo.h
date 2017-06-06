// Demo.h: Schnittstelle für die Klasse CDemo.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>

#include "Bmtext.h"
#include "Bitmap.h"
#include "All3DObj.h"
#include "All3DEng.h"	// Hinzugefügt von der Klassenansicht
#include "player.h"
#include "midasdll.h" //TEST

#if !defined PI
#define PI 3.1428
#endif

#if !defined(AFX_DEMO_H__843D4131_84C1_11D3_B8B2_0000E8699C86__INCLUDED_)
#define AFX_DEMO_H__843D4131_84C1_11D3_B8B2_0000E8699C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDemo  
{
public:
	static void MIDAS_CALL MidasCallBack(unsigned syncInfo, unsigned position, unsigned row)
	{
		Param=syncInfo;
		Pos=position;
		Row=row;
	}
	static void MIDAS_CALL MidasTimer()
	{
	}
	void Finish();
	bool UpdateFrame(CBitMap* BackBuffer, int FrameCounter);
    void Init();
	CDemo();
	virtual ~CDemo();

private:
	char* getSymbolName(int index);
	void MoveLogo(int FrameCounter);
	void InitBlendOrder();
	int* BlendOrderArray;
	int BlendOrder(int i);
	CBitMap** FadeBlockClones;
	int* FadeBlockCounter;
	CBitMap** FadeBlocks;
	void ShowNDSign(int MyCount);
	static unsigned Pos;
	static unsigned Row;
	static unsigned Param;

	MIDASmodule module;				//TEST
	MIDASmodulePlayHandle playHandle;
	MusicPlayer * Player;

  
	int PlayHandle; // Timing ! ob wir das brauchen ????

	CBitMap * StartPic;
	CBitMap * AsmPic;
	CBitMap * GreetPic;
	CBitMap * UpScrollPic;

	CBitMap * DemoScreen;
	CBitMap* Symbols;
	CBitMap * LenzFlares;
	CBitMap * LensFader;
	CBitMap * Spot;
	CBitMap * TPLLogo;
	CBitMap * BlackScreen;
	CBitMap * ThePromisedLandPic;
	CBitMap * SputnikBackground;
	CBitMap * EndPicture;


	CBitMap * Raser0;
	CBitMap * Raser1;
	CBitMap * Raser2;
	CBitMap * Raser3;
	CBitMap * Raser4;
	CBitMap * Strudel;
	CBitMap * Strudel2;
	CBitMap * NDSign;

	CBitMap ** PicRaser;
	CBitMap ** SymbolRaser;

	CBitMap * FluidMap;
	CBitMap * UpScrollerDummy;
	CBitMap * Snow;
	CBitMap * LinsenReflex;

	/* Mac VErsion Add*/

	CBitMap * AorPic;

	CBMText * NasaFont;
	CBMText * Font1;
	CBMText * AsmFont;

	/* Mac Version Add */
	CBMText * SmallChar;

	CAll3DEngine* engine;

	bool GotSync(unsigned ReqSync);

	void PaintLines(int FrameCounter);
	void UpScroll(CBitMap* Dest, CBMText* Font, int FrameCounter);
	void RotateLogo();

	void ShowMarsMensch(int FrameCounter);
	void ShowGreets(int FrameCounter);
	void ShowUpScroller(int FrameCounter);
	void ShowCreditz(CBitMap * Buffer, int FrameCounter);

	CAll3DObject * NDObj;
	CAll3DObject * StarObj;
	CAll3DObject * Star2;

	int * SnowCoord;
	int * Lenzflare_YPos; // Aktuelle Pos
	int * Lenzflare_YAdd; // Speed Add 	 	
	int YScrollPos;
};


#endif // !defined(AFX_DEMO_H__843D4131_84C1_11D3_B8B2_0000E8699C86__INCLUDED_)
