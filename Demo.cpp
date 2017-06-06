// Demo.cpp: Implementierung der Klasse CDemo.//
// Demo Klasse als abstrakte Basisklasse für alle Demos
//
//////////////////////////////////////////////////////////////////////

#include "pattern.h"
#include "midasdll.h"
#include "Demo.h"

#include <stdlib.h>
#include <conio.h>
#include <conio.h>
#include "player.h"
#include "uptext.h"
#include "sideasm.h"
/////////////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////////////////////////////////////////////////////////

	unsigned CDemo::Pos=0;
	unsigned CDemo::Row=0;
	unsigned CDemo::Param=0;

CDemo::CDemo()
{
    Player = new MusicPlayer();
	Player->Detect(FALSE);
    Player->Init("\\data\\nice8b.xm");
    engine=new CAll3DEngine();

    StarObj=new CAll3DObject(1);
    NDObj=new CAll3DObject(2);
    Star2 = new CAll3DObject(1);

    LinsenReflex = new CBitMap();
    LinsenReflex->LoadJpeg("\\data\\flares2.jpg");

    LensFader = new CBitMap();
    LensFader->LoadJpeg("\\data\\flares2.jpg");	//Schwarzer Dummy, damit er dieselbe groesse hat wie LinsenReflex(zum Blenden!)
    LensFader->Clear();

    Raser0=new CBitMap();
		Raser0->LoadJpeg("\\data\\00.jpg");
    Raser1=new CBitMap();
		Raser1->LoadJpeg("\\data\\01.jpg");
    Raser2=new CBitMap();
		Raser2->LoadJpeg("\\data\\02.jpg");
    Raser3=new CBitMap();
		Raser3->LoadJpeg("\\data\\03.jpg");
    Raser4=new CBitMap();
		Raser4->LoadJpeg("\\data\\04.jpg");

    PicRaser=new CBitMap* [20];

    PicRaser[0]=Raser0->Clone(0,0,320,240);
    PicRaser[1]=Raser0->Clone(320,0,320,240);
    PicRaser[2]=Raser0->Clone(0,240,320,240);
    PicRaser[3]=Raser0->Clone(320,240,320,240);
    PicRaser[4]=Raser1->Clone(0,0,320,240);
    PicRaser[5]=Raser1->Clone(320,0,320,240);
    PicRaser[6]=Raser1->Clone(0,240,320,240);
    PicRaser[7]=Raser1->Clone(320,240,320,240);
    PicRaser[8]=Raser2->Clone(0,0,320,240);
    PicRaser[9]=Raser2->Clone(320,0,320,240);
    PicRaser[10]=Raser2->Clone(0,240,320,240);
    PicRaser[11]=Raser2->Clone(320,240,320,240);
    PicRaser[12]=Raser3->Clone(0,0,320,240);
    PicRaser[13]=Raser3->Clone(320,0,320,240);
    PicRaser[14]=Raser3->Clone(0,240,320,240);
    PicRaser[15]=Raser3->Clone(320,240,320,240);
    PicRaser[16]=Raser4->Clone(0,0,320,240);
    PicRaser[17]=Raser4->Clone(320,0,320,240);
    PicRaser[18]=Raser4->Clone(0,240,320,240);
    PicRaser[19]=Raser4->Clone(320,240,320,240);

    DemoScreen          = new CBitMap(640,480);
    DemoScreen->Clear();

    Strudel             = new CBitMap();
    Strudel->LoadJpeg("\\data\\blend.jpg");

    BlackScreen         = new CBitMap (640,480);
    BlackScreen->Clear();

    ThePromisedLandPic  = new CBitMap();
    ThePromisedLandPic->LoadJpeg("\\data\\tpl.jpg");

    EndPicture = new CBitMap();
    EndPicture->LoadJpeg("\\data\\endpic.jpg");

//    CreditzPic = new CBitMap();
//    CreditzPic->LoadJpeg("\data\\creditz.jpg");

    SputnikBackground = new CBitMap("\\data\\sputnik.pcx");
    Symbols = new CBitMap("\\data\\symbols.pcx");
	SymbolRaser = new CBitMap*[10];
	for (int i=0;i<5;i++)
	{
		SymbolRaser[i]=Symbols->Clone(0,i*128,128,128);
		SymbolRaser[i+5]=Symbols->Clone(128,i*128,128,128);
	}

	NDSign = new CBitMap();
	NDSign->LoadJpeg("\\data\\sign.jpg");

    FluidMap        = new CBitMap();
    FluidMap->LoadJpeg("\\data\\fluid.jpg");
	FluidMap->InitDeltaMap();

    LenzFlares      = new CBitMap();
    LenzFlares->LoadJpeg("\\data\\flares.jpg");

    Spot = new CBitMap();
    Spot->LoadJpeg("\\data\\Spot.jpg");

	AorPic = new CBitMap();
	AorPic->LoadJpeg("\\data\\aor.jpg");
	YScrollPos =0;

	TPLLogo = new CBitMap("\\data\\Letters.pcx");

    Font1 = new CBMText((unsigned char*)UpMessage,10);
    Font1->LoadJpeg("\\data\\char01.jpg");

    NasaFont = new CBMText();
    NasaFont->LoadJpeg("\\data\\Nasa.jpg");
    NasaFont->SetFontHeight(32);
    NasaFont->SetFontWidth(32);
    NasaFont->SetCharPerRow(47);
    NasaFont->SetTranslationTable("ABCDEFGHIJKLMNOPQRSTUVWYXZ1234567890?!.,[]: ");

    AsmFont = new CBMText();
    AsmFont->LoadJpeg("\\data\\char02.jpg");
    AsmFont->SetFontHeight(23);
    AsmFont->SetFontWidth(16);
    AsmFont->SetCharPerRow(40);
    AsmFont->SetTranslationTable("^^^^^^^^^^^^^^^^^^^^^^^^^ ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-+**.,;:<>[] $_#!%{}()@");

    Font1->SetFontHeight(24);
    Font1->SetFontWidth(24);
    Font1->SetCharPerRow(55);
    Font1->SetTranslationTable("abcdefghijklmnopqrstuvwxyz1234567890!~?.,;-()*#_<>&:'@ ");

	/* Mac Version Add */
	SmallChar = new CBMText();
	SmallChar->LoadJpeg("\\data\\schar.jpg");
    SmallChar->SetFontHeight(10);
    SmallChar->SetFontWidth(5);
    SmallChar->SetCharPerRow(84);
    SmallChar->SetTranslationTable("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890<>!\"*#-_:.+?()/\\$%#' ");

    Snow = new CBitMap("\\data\\disto2.pcx");

    UpScrollerDummy = new CBitMap(640,480);
    UpScrollerDummy->Clear();

    AsmPic = new CBitMap(640,480);
    AsmPic->Clear();
    GreetPic = new CBitMap(640,480);
    GreetPic->Clear();
    UpScrollPic = new CBitMap(640,480);
    UpScrollPic->Clear();

    SnowCoord=new int[10];
	for (int i=0;i<10;i++)
	    SnowCoord[i]=(rand())%640;

	Lenzflare_YPos = new int [10];
	Lenzflare_YAdd = new int [10];

	for (int i =0;i< 10 ; i++)
	{
		Lenzflare_YPos[i] = 10;
		Lenzflare_YAdd[i] = (rand()%15) +3;
	}

// ##############							Fading zwischen Marsmensch & Greetpart        ####################

#define FADEBLCKSSTEPS 12				// Wieviele Blocks auf einmal?
#define FADEBLCKS 96						// Wieviele Blocks?
#define FADEBLCKSWIDTH  40			//Breite jedes Blocks (Pixel)
#define FADEBLCKSHEIGHT 45			//Hoehe jedes Blocks
#define FADEBLCKSPERROW 16			// Wieviele pro Reihe
		FadeBlocks = new CBitMap*[FADEBLCKS];
		FadeBlockClones= new CBitMap*[FADEBLCKS];
		FadeBlockCounter = new int[FADEBLCKS];
		BlendOrderArray = new int[FADEBLCKS];
		InitBlendOrder();	//Fill with random Order
		for (int iindex=0; iindex<FADEBLCKS ; iindex++)
		{
			FadeBlocks[iindex]=new CBitMap(FADEBLCKSWIDTH,FADEBLCKSHEIGHT);
			FadeBlocks[iindex]->Clear(0xff);
			FadeBlockCounter[iindex]=-1;
			FadeBlockClones[iindex]=new CBitMap();
			FadeBlockClones[iindex]= GreetPic->Clone(
				(iindex%FADEBLCKSPERROW)*FADEBLCKSWIDTH,
				(iindex/FADEBLCKSPERROW)*FADEBLCKSHEIGHT+105,
				FADEBLCKSWIDTH,FADEBLCKSHEIGHT);
		}
}
/////////////////////////////////////////////////////////////////////////////
CDemo::~CDemo()
{
	Player->RemoveSync();
	Player->StopMod();
    if (Player)
        delete Player;

	if (engine)
    {
        delete engine;
        engine=NULL;
    }
    if (LenzFlares)
        delete LenzFlares;

    if (NDObj)
    {
        delete NDObj ;
        NDObj=NULL;
    }
    if (StarObj)
        delete StarObj;
    if (Star2)
        delete Star2;

    if (Strudel)
        delete Strudel;

    if (DemoScreen)
    {
        delete DemoScreen;
        DemoScreen=NULL;
    }
    if (ThePromisedLandPic)
    {
        delete ThePromisedLandPic;
        ThePromisedLandPic=NULL;
    }

    if (BlackScreen)
        delete BlackScreen;

    if (LinsenReflex)       delete LinsenReflex;
        LinsenReflex=NULL;

    if (Spot)       delete Spot;
        Spot=NULL;

    if (TPLLogo)       delete TPLLogo;
        TPLLogo=NULL;

 /*   if (CreditzPic)
    {
        delete CreditzPic;
        CreditzPic=NULL;
    }
*/
    if (FluidMap)
    {
       delete FluidMap;
       FluidMap=NULL;
    }

    if (SputnikBackground)
    {
        delete SputnikBackground;
        SputnikBackground = NULL;
    }
    if (Symbols)
    {
        delete Symbols;
        Symbols = NULL;
    }
    if (NDSign)
    {
        delete NDSign;
        NDSign = NULL;
    }

    if (EndPicture)
    {
        delete EndPicture;
        EndPicture = NULL;
    }

    if (UpScrollerDummy)
        delete UpScrollerDummy;

    if (AsmPic)
        delete AsmPic;

    if (GreetPic)
        delete GreetPic;

    if (UpScrollPic)
        delete UpScrollPic;

    if (Font1)
		delete Font1;
    Font1= NULL;

    if (AsmFont)
		delete AsmFont;
	AsmFont = NULL;

	if (SmallChar)
		delete SmallChar;
	SmallChar=NULL;

	if (NasaFont)
		delete NasaFont;
	NasaFont=NULL;

	for (int iindex=0; iindex<FADEBLCKS; iindex++)
	{
		delete FadeBlocks[iindex];
		FadeBlocks[iindex]=NULL;
		delete FadeBlockClones[iindex];
		FadeBlockClones[iindex]=NULL;
	}

    if (FadeBlockCounter)
    {
		delete FadeBlockCounter;
		delete BlendOrderArray;
		FadeBlockCounter = NULL;
		BlendOrderArray =NULL;
    }
    if (FadeBlocks)
    {
		delete FadeBlocks;
		FadeBlocks = NULL;
		delete FadeBlockClones;
		FadeBlockClones = NULL;
    }
    if (Snow)
        delete Snow;
    if (SnowCoord)
        delete SnowCoord;

	if (AorPic)
		delete AorPic;

}

/////////////////////////////////////////////////////////////////////////////
void CDemo::Init()
{
		Player->PlayMod();
		Player->SetSyncToMusic(&MidasCallBack);
		Player->SetSync(5000,false,NULL,&MidasTimer,NULL);
}
/////////////////////////////////////////////////////////////////////////////
void CDemo::Finish()
{
}
/////////////////////////////////////////////////////////////////////////////
bool CDemo::UpdateFrame(CBitMap * Buffer, int FrameCounter)
{
// 	FrameCounter+=1900;
    static float LensMoveZ =0.0;
    static float LensMoveX =0.0;
    static float LensMoveY =0.0;

	engine->SetRenderSurface(DemoScreen);

	if ((FrameCounter>-10)&&(FrameCounter<400))
	{
		PaintLines(FrameCounter-50);
		DemoScreen->BltFast(0,0,640,480,Buffer,0,0);
	} else PaintLines(330);

	// BackGround FADE IN
    if ((FrameCounter>400)&&(FrameCounter<600))
    {
        CBitMap * BackgroundAlphaBlend = DemoScreen->Clone(0,105,640,270);
        BackgroundAlphaBlend->Clear();
        BackgroundAlphaBlend->Blend(ThePromisedLandPic,BackgroundAlphaBlend,(FrameCounter-400));
        DemoScreen->BltFast(0,0,640,480,Buffer,0,0);
				delete BackgroundAlphaBlend;
    }

    // Hier werden nur die Lenzflares bewegt !!! *Logo Wobble
    if ((FrameCounter>600)&&(FrameCounter<1200))
    {
			CBitMap * LogoScreen	= BlackScreen->Clone(0,0,495,30);
			CBitMap * LogoScreen2	= BlackScreen->Clone(0,30,495,30);
			CBitMap * LogoScreen3	= BlackScreen->Clone(0,60,495,30);
      CBitMap * DummyPic		= DemoScreen->Clone(72,243,495,30);

      LensMoveZ =(FrameCounter-600)*0.012;
      float zrot = PI/4 + (FrameCounter-600)*0.004;
			int FadeWert;

      ThePromisedLandPic->BltFast (0,0,640,270,DemoScreen,0,105); //DemoScreen->Clone(0,105,640,270);
      for (int x=0;x<10;x++)
      {
				float a=(2*PI/15)*x;
				LensMoveX=cos((double)a+LensMoveZ)*290;
				LensMoveY=sin((double)a+LensMoveZ)*10;
				Spot->BltAdd(0,0,Spot->GetWidth(),Spot->GetHeight(),LogoScreen,(int)LensMoveX+245,(int)LensMoveY-15,4);
				LensMoveX=cos((double)2*a+LensMoveZ)*200;
				LensMoveY=sin((double)a*0.56+LensMoveZ)*20;
				int iFade=5-(FrameCounter-600)/20;
				if (iFade<0) iFade=0;
				LenzFlares->BltAdd(128,0,128,128,DemoScreen,(int)LensMoveX+275,(int)LensMoveY+205,128,96,2+iFade);
      }
			TPLLogo->Blt(0,1,495,30,LogoScreen,0,0,0xffffff);
			LogoScreen2->Fluid(LogoScreen,FluidMap);
			LogoScreen->Clear();
			LogoScreen3->Blend(LogoScreen,LogoScreen2,128-((FrameCounter-600)>>2));
			LogoScreen3->BltAdd(0,0,495,30,DummyPic,0,0);

			if ((FrameCounter-600)<472)
				FadeWert=(FrameCounter-600)*128 /400;
			else
				FadeWert=128-(FrameCounter-1000) ;

			LensFader->Clear();

      CBitMap*    DemoPic=DemoScreen->Clone(0,105,640,270);			//For clipping the Lensflares!!
			LensFader->Blend(LinsenReflex,LensFader,4*FadeWert);			//Fading the Lensflares
      int LensX;
      int LensY;
      LensX=320+cos(zrot)*305;
      LensY=135+sin(-zrot)*113;
      DemoPic->LensFlare(LensFader,LensX,LensY);
      DemoScreen->BltFast(0,0,640,480,Buffer,0,0);
			delete DemoPic;
			delete LogoScreen;
			delete LogoScreen2;
			delete LogoScreen3;
			delete DummyPic;
		}
    // Zweiter Spot Move
    if ((FrameCounter>1200)&&(FrameCounter<1800))
    {
			CBitMap * LogoScreen = BlackScreen->Clone(0,0,495,30);
			CBitMap * LogoScreen2 = BlackScreen->Clone(0,30,495,30);
			CBitMap * LogoScreen3 = BlackScreen->Clone(0,60,495,30);
      CBitMap * DummyPic = DemoScreen->Clone(72,243,495,30);
      LensMoveZ =(FrameCounter-600)*0.012;
      ThePromisedLandPic->BltFast(0,0,640,270,DemoScreen,0,105);
      for (int x=0;x<10;x++)
      {
          float a=(2*PI/15)*x;
          LensMoveX=cos((double)a+LensMoveZ)*290;
          LensMoveY=sin((double)a+LensMoveZ)*10;
          Spot->BltAdd(0,0,Spot->GetWidth(),Spot->GetHeight(),LogoScreen,(int)LensMoveX+245,(int)LensMoveY-15,4);
          LensMoveX=cos((double)2*a+LensMoveZ)*200;
          LensMoveY=sin((double)a*0.56+LensMoveZ)*20;
          int iFade=(FrameCounter-1700)/20;
					if (iFade<0)iFade=0;
					LenzFlares->BltAdd(128,0,128,128,DemoScreen,(int)LensMoveX+275,(int)LensMoveY+205,128,96,2+iFade);
      }
			TPLLogo->Blt(0,1,495,30,LogoScreen,0,0,0xffffff);

			LogoScreen2->Fluid(LogoScreen,FluidMap);
			LogoScreen3->Blend(LogoScreen,LogoScreen2,(FrameCounter-1200)>>2);
			LogoScreen3->BltAdd(0,0,495,30,DummyPic,0,0);
			LogoScreen->BltAdd(0,0,495,30,DummyPic,71,137,497,32,3);
			if (FrameCounter<1500) BlackScreen->Clear();
			DemoScreen->BltFast(0,0,640,480,Buffer,0,0);
			delete LogoScreen;
			delete LogoScreen2;
			delete LogoScreen3;
			delete DummyPic;
    }
    // Überblender von TPL nach MarsMensch
    if ((FrameCounter>1800)&&(FrameCounter<1900))
    {
			CBitMap * LogoScreen = BlackScreen->Clone(0,0,495,30);
			CBitMap * DummyPic = DemoScreen->Clone(0,105,640,270);
			int AlphaCount = FrameCounter-1800;

			LensMoveZ =(FrameCounter-600)*0.012;
			ThePromisedLandPic->BltFast(0,0,640,270,DemoScreen,0,105);
			for (int x=0;x<10;x++)
			{
					float a=(2*PI/15)*x;
					LensMoveX=cos((double)a+LensMoveZ)*290;
					LensMoveY=sin((double)a+LensMoveZ)*10;
					Spot->BltAdd(0,0,Spot->GetWidth(),Spot->GetHeight(),LogoScreen,(int)LensMoveX+245,(int)LensMoveY-5,4);
			}
			TPLLogo->Blt(0,1,495,30,LogoScreen,0,0,0xffffff);
			LogoScreen->BltAdd(0,0,495,30,DummyPic,72,138);
			LogoScreen->BltAdd(0,0,495,30,DummyPic,71,137,497,32,3);

			CBitMap * DestPic   = AsmPic->Clone(0,105,640,270);
			SputnikBackground->BltFast(0,0,412,270,AsmPic,0,105);
			SputnikBackground->BltTV(412,0,228,270,AsmPic,0,105,0);

			DummyPic->Blend(DestPic,DummyPic,AlphaCount<<1);

			DemoScreen->BltFast(0,0,640,480,Buffer,0,0);
			delete LogoScreen;
			delete DummyPic;
			delete DestPic;
		}
		// ##################  Mars Mensch ##############################
    if ((FrameCounter>1900)&&(FrameCounter<5500))
    {
        ShowMarsMensch(FrameCounter);
        AsmPic->BltFast(0,105,640,270, Buffer, 0,105);

        DemoScreen->BltFast(0,0,120,104,Buffer,0,0);
    }

	// #####################Blend to Greets Part #####################
    if ((FrameCounter > (5500)) && ( FrameCounter <6300))
    {
			static int BlendFac=-1;

			ShowGreets(FrameCounter);
			ShowMarsMensch(FrameCounter);
			if (GotSync(5))
			{
				BlendFac+=(FADEBLCKS/FADEBLCKSSTEPS);
				if (BlendFac<FADEBLCKS)
				{
					for (int a=0; a<(FADEBLCKS/FADEBLCKSSTEPS);a++)
						FadeBlockCounter[(BlendOrder(BlendFac-a))%FADEBLCKS]=127;
				}else  BlendFac=FADEBLCKS-1;
			}
			for (int x=0; x<FADEBLCKS;x++)
			{
				if 	((FadeBlockCounter[x])>-1)
				{
					CBitMap* Blendy=Buffer->Clone((x%FADEBLCKSPERROW)*FADEBLCKSWIDTH,
												 							  (x/FADEBLCKSPERROW)*FADEBLCKSHEIGHT+105,
																				 FADEBLCKSWIDTH,FADEBLCKSHEIGHT);
					Blendy->Blend(FadeBlocks[x],FadeBlockClones[x],FadeBlockCounter[x]);
					FadeBlockCounter[x]-=12;
					delete Blendy;
					if (FadeBlockCounter[x]<0) FadeBlockCounter[x]=0;
				}else{
					AsmPic->BltFast(	(x%FADEBLCKSPERROW)*FADEBLCKSWIDTH,
														(x/FADEBLCKSPERROW)*FADEBLCKSHEIGHT+105,
														FADEBLCKSWIDTH,FADEBLCKSHEIGHT,
														Buffer, (x%FADEBLCKSPERROW)*FADEBLCKSWIDTH,(x/FADEBLCKSPERROW)*FADEBLCKSHEIGHT+105);
				}
			}
			DemoScreen->BltFast(0,0,120,104,Buffer,0,0);
    }

		//############# Greet Part ##################
    if ((FrameCounter>6300)&&(FrameCounter<12000))
    {
        ShowGreets(FrameCounter);
        GreetPic->BltFast(0,105,640,270,Buffer,0,105);
        DemoScreen->BltFast(0,0,120,104,Buffer,0,0);
    }
    if ((FrameCounter > (12000)) && ( FrameCounter <13000))
    {
        ShowGreets(FrameCounter);
        ShowUpScroller(FrameCounter);
        CBitMap* Blendy=Buffer->Clone(0,105,640,270);
        CBitMap* Greet=GreetPic->Clone(0,105,640,270);
        CBitMap* Upper=UpScrollPic->Clone(0,105,640,270);

        Blendy->BlendPixel(Upper,Greet,Strudel,(FrameCounter-12000)>>2);
        DemoScreen->BltFast(0,0,120,104,Buffer,0,0);
    }
// +++++++++++++++Up-Scrooler part and Logo Moving...++++++++++++++++++++
    if ((FrameCounter > (13000)) && ( FrameCounter <16400))
    {
			ShowUpScroller(FrameCounter);
			MoveLogo(FrameCounter);

			UpScrollPic->BltFast(0,0,640,104,Buffer,0,0);
			UpScrollPic->BltFast(0,105,640,270,Buffer,0,105);

    }
		static int LinesOut= 0;
		if (GotSync(8))
    {
				LinesOut=FrameCounter;
		}
    if ((FrameCounter > 16400) &&(FrameCounter<17000))
    {
		ShowNDSign(FrameCounter);
		ShowUpScroller(FrameCounter);
		MoveLogo(FrameCounter);
		CBitMap* dummy=UpScrollPic->Clone(0,375,640,105);
		dummy->Clear();
		DemoScreen->Blend(GreetPic,UpScrollPic,(FrameCounter-16400)>>1);
		DemoScreen->BltFast(0,0,640,480,Buffer,0,0);
    }
    if (FrameCounter > 17000)
	{
		ShowNDSign(FrameCounter);
		DemoScreen->BltFast(0,0,640,480,Buffer,0,0);
	}
	// Mac Version Add
	static int LineCounter =0;
	if ((FrameCounter >= 4500) && (LineCounter <21)) // Max Zeilen
	{


		static int poslenght=1;

		switch (LineCounter)
		{
			case  0:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"unity needs cooperation - cooperation needs communication	        	",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;



				break;

			case 1:


				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"		              communication needs an open mind	             				",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;
				break;

			case 2:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"          							  CAN YOU MOVE AT DIGITAL SPEED							",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;


			case 3:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"          							  CAN YOU MOVE AT DIGITAL SPEED							",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 4:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"                      								                                     				",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 5:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"           			  WHAT'S THE SOUND OF A SINGLE CLAPPING HAND			",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;


			case 6:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"        			     	WHAT THE MEANING OF THIS SENSE ?				         ",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 7:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"           			     				                        			         							",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 8:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"             						      THE NEXT SENTENCE IS RIGHT!				 ",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 9:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"             						    THE LAST SENTENCE WAS WRONG !				 ",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;
			case 10:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"            						                     		     										 ",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;
			case 11:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"         							    ERST BRANDMELDER INSLAAN			  	    	",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;
			case 12:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"             							         EN DAN BELLEN				           			",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;
			case 13:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"         		   															            		",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 14:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"             		   						   AVE SANTANI				         			",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 15:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"            							    Where is the Coffee			          	",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;
			case 16:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"                 SOMETHIMES MY MIND PLAYS TRICKS ON ME            ",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;
			case 17:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"            						( : N I C E : 9 8 : D E Z I G N S : )	      			",poslenght)==true)

				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 18:
				SmallChar->WriteStringWithDelay(Buffer,150,410,"            						( : N I C E : 9 8 : D E Z I G N S : )		     		",poslenght);
				//SmallChar->WriteStringWithDelay(Buffer,150,410,		"                               ( : N I C E : 9 8 : D E Z I G N S : )	       		",256);
				if (SmallChar->WriteStringWithDelay(Buffer,150,420, "                         >   at Ambience 2K 	 <				       	",poslenght)==true)

				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

			case 19:

				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"                                                                                               ",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;
		case 20:
				if (SmallChar->WriteStringWithDelay(Buffer,150,410,"                                                                                           ",poslenght)==true)
				{;;}

				if (SmallChar->WriteStringWithDelay(Buffer,150,420,"           		   							    				          		          	",poslenght)==true)
				{
					poslenght=1;
					LineCounter++;
				}
				else
					poslenght++;

				break;

		}


		//SmallChar->WriteStringWithDelay(Buffer,150,410,"              communication needs an open mind           ",50);
	}


	if ((FrameCounter > 2500) && (FrameCounter < 12000))
	{
		YScrollPos++;
		if (YScrollPos >= 670)
			YScrollPos = 0;
	}
	else if ((FrameCounter >12000) && (YScrollPos <= 670))
	{
		YScrollPos++;
	}

	AorPic->BltFast(0,YScrollPos,31,103,Buffer,608,377);

	if (LinesOut>0)
	{		// Fehler oben !!!
			Buffer->Line(640,104,640-((FrameCounter-LinesOut)<<1),104,0);
			Buffer->Line(0,376,(FrameCounter-LinesOut)<<1,376,0);
	}
	if (FrameCounter<13000) RotateLogo();	//Ab 13000 wird das Logo wegfliegen!!

	if (GotSync(0xff))
	{
		return false;	//Demo beenden
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////////
void CDemo::ShowMarsMensch(int MyCounter)
{
	SputnikBackground->BltFast(0,0,412,270,AsmPic,0,105);
	BlackScreen->BltFast(412,105,228,270,AsmPic,412,105);

	static int verzerr=98;
	static int verzerrshrink=7;
	verzerr-=verzerrshrink;
	if (verzerr<0) verzerr=0;
	SputnikBackground->BltTV(412,0,228,270,AsmPic,0,105,verzerr);

	static int OldSymbol=-1;
	static int InX, InY,OutX,OutY;
	if (MyCounter<5000)
	{
		int Symb=((MyCounter-2000)/300)%10;
		if (Symb!=OldSymbol)
		{
			InX= rand()%60;
			InY= rand()%30;
			OutX=rand()%60;
			OutY=rand()%30;
			if ((rand()%100)>50){ InX=-InX;InY=-InY; } else {OutX=-OutX;OutY=-OutY;}
		}
		int Fly=(MyCounter-(2000+Symb*300))-100;
		BlackScreen->BltFast(0,430,640,35,BlackScreen,0,0);
		NasaFont->WriteStringAdd(BlackScreen,0,0,getSymbolName(Symb));
		if (Fly>-127)
		for (int za=0;za<12;za++)
		{
			BlackScreen->BillBoard(0,0,640,33,AsmPic,150,35,300-4*Fly+za,5);
			SymbolRaser[Symb]->BillBoard(0,0,128,128,AsmPic,InX,InY,Fly+za*4,4);
			if (Symb>0)
				SymbolRaser[Symb-1]->BillBoard(0,0,128,128,AsmPic,OutX,OutY,200-2*Fly+za*4,5);
		}
		OldSymbol=Symb;
	}
	int Zoomer;
	Zoomer = 4000-(MyCounter-2000)*30;
	if (Zoomer<200) Zoomer=200;

  static bool NegativeFlag = FALSE;

	static float Zadd = 0.0;
  static float Xadd = 0.0;
  static float Yadd = 0.0;

	static bool scrollready=false;
  static int AsmTextCount =0;
  CBitMap * UpScroller = UpScrollerDummy->Clone(0,0,640,300);
	if ((GotSync(3))||(scrollready))
	{
		scrollready=true;
		AsmFont->WriteStringAdd(AsmPic,384,105,pAsmText+AsmTextCount,16);
		AsmFont->WriteStringAdd(AsmPic,384,129,pAsmText+AsmTextCount+26,16);
		AsmFont->WriteStringAdd(AsmPic,384,153,pAsmText+AsmTextCount+52,16);
		AsmFont->WriteStringAdd(AsmPic,384,177,pAsmText+AsmTextCount+78,16);
		AsmFont->WriteStringAdd(AsmPic,384,201,pAsmText+AsmTextCount+104,16);
		AsmFont->WriteStringAdd(AsmPic,384,223,pAsmText+AsmTextCount+130,16);
		AsmFont->WriteStringAdd(AsmPic,384,247,pAsmText+AsmTextCount+156,16);
		AsmFont->WriteStringAdd(AsmPic,384,271,pAsmText+AsmTextCount+182,16);
		AsmFont->WriteStringAdd(AsmPic,384,295,pAsmText+AsmTextCount+208,16);
		AsmFont->WriteStringAdd(AsmPic,384,319,pAsmText+AsmTextCount+234,16);
		AsmFont->WriteStringAdd(AsmPic,384,343,pAsmText+AsmTextCount+260,16);

		static int TimeStep = 2;
		TimeStep--;
		if (!TimeStep)
		{
				TimeStep=2;
				AsmTextCount+=26;
				if (AsmTextCount == 26*180)
						AsmTextCount=0;
		}
	}
  engine->SetRenderSurface(AsmPic);
  CAll3DVector place;


  if (Zoomer < 1000)	//Object rotate1 ON
      Yadd = 0.139;

  if (GotSync(1))		//Object Rotate2 ON
      Zadd = 0.07;

  if (GotSync(2))		//Object Rotate2 ON
      Xadd =- 0.123;

	static int ZoomCounter=0;
	if (GotSync(4))	ZoomCounter=MyCounter+20;
	if (ZoomCounter!=0)
	if (ZoomCounter>MyCounter)
	{
		Zoomer = 120;
		Xadd =  0.083;
		Yadd =  0.19;
		Zadd =- 0.17;
		NegativeFlag = false;


	} else{
		Zoomer = 200;
		ZoomCounter=0;
		Xadd = -0.1;
		Yadd =  0.09;
		Zadd =  0.07;
		NegativeFlag = true;
		verzerr=9;
		verzerrshrink=1;
	}

	if (GotSync(9)){	verzerr=98; verzerrshrink=7;}

	float xco,yco;
  static float Xlght,Ylght;

  engine->MouseX=-PI;
  engine->MouseY=-PI;

  StarObj->xrot += Xadd;
  StarObj->yrot += Yadd;
  StarObj->zrot += Zadd;
  place.SetVector(-0,20,Zoomer);
  StarObj->SetPlace(place);
  engine->ShowObject(StarObj);

  CBitMap*    SnowCrash=AsmPic->Clone(0,105,640,270);
  for(int i=0;i<10;i++)
  {
		int x=SnowCoord[i];
		SnowCoord[i]+=rand()%11-5;
//                if (SnowCoord[i]>640) SnowCoord[i]=640;
//                if (SnowCoord[i]<0) SnowCoord[i]=0;

		for (int y=0;y<270;y+=9)
			Snow->BltAdd(0,0,1,8,SnowCrash,x,y);
  }
	delete SnowCrash;
	delete UpScroller;
}
/////////////////////////////////////////////////////////////////////////////
void CDemo::ShowGreets(int MyCounter)
{
//  ################  PICRASER Action###############
	static bool MyPicRaser = false;
	int prx=(rand()%8);
    int pry=(rand()%8);

    BlackScreen->BltTrans(0,105,640,270,GreetPic,0,105);

	static int Color1;
	static int Color2;
	static int Color3;
	int DestColor1=0x997733;
	int DestColor2=0x331166;
	int DestColor3=0x664411;
	if ((MyCounter>6400)&&(MyCounter<12000))
	{
	if (MyCounter<8500)
	{
		Color1+=0x010101;
		Color2+=0x010101;
		Color3+=0x010101;
		if ((Color1&0xff0000)>(DestColor1&0xff0000)) Color1=(Color1&0x00ffff)|(DestColor1&0xff0000);
		if ((Color1&0x00ff00)>(DestColor1&0x00ff00)) Color1=(Color1&0xff00ff)|(DestColor1&0x00ff00);
		if ((Color1&0x0000ff)>(DestColor1&0x0000ff)) Color1=(Color1&0xffff00)|(DestColor1&0x0000ff);
		if ((Color2&0xff0000)>(DestColor2&0xff0000)) Color2=(Color2&0x00ffff)|(DestColor2&0xff0000);
		if ((Color2&0x00ff00)>(DestColor2&0x00ff00)) Color2=(Color2&0xff00ff)|(DestColor2&0x00ff00);
		if ((Color2&0x0000ff)>(DestColor2&0x0000ff)) Color2=(Color2&0xffff00)|(DestColor2&0x0000ff);
		if ((Color3&0xff0000)>(DestColor3&0xff0000)) Color3=(Color3&0x00ffff)|(DestColor3&0xff0000);
		if ((Color3&0x00ff00)>(DestColor3&0x00ff00)) Color3=(Color3&0xff00ff)|(DestColor3&0x00ff00);
		if ((Color3&0x0000ff)>(DestColor3&0x0000ff)) Color3=(Color3&0xffff00)|(DestColor3&0x0000ff);
	}else{
		Color1-=0x010000;
		if ((Color1&0xff0000)==(0x000000)) Color1=(Color1&0x00ffff)|0x010000;
		Color1-=0x000100;
		if ((Color1&0x00ff00)==(0x000000)) Color1=(Color1&0xff00ff)|0x000100;
		Color1-=0x000001;
		if ((Color1&0x0000ff)==(0x000000)) Color1=(Color1&0xffff00)|0x000001;
		Color2-=0x010000;
		if ((Color2&0xff0000)==(0x000000)) Color2=(Color2&0x00ffff)|0x010000;
		Color2-=0x000100;
		if ((Color2&0x00ff00)==(0x000000)) Color2=(Color2&0xff00ff)|0x000100;
		Color2-=0x000001;
		if ((Color2&0x0000ff)==(0x000000)) Color2=(Color2&0xffff00)|0x000001;
		Color3-=0x010000;
		if ((Color3&0xff0000)==(0x000000)) Color3=(Color3&0x00ffff)|0x010000;
		Color3-=0x000100;
		if ((Color3&0x00ff00)==(0x000000)) Color3=(Color3&0xff00ff)|0x000100;
		Color3-=0x000001;
		if ((Color3&0x0000ff)==(0x000000)) Color3=(Color3&0xffff00)|0x000001;
	}
	int CenterX=cos((double)MyCounter/60)*60+320;
		int CenterY=cos((double)MyCounter/61)*30+240;
		CenterX=200;
		CenterY=240;
		for (double x=0;x<2*PI;x+=0.02)
		{
			int rad0=cos(x+(double)(MyCounter)/41)*20;
			int rad1=cos(x*7)*rad0;
			int xc=cos(x)*(rad1+50)+CenterX;
			int yc=sin(x)*(rad1+50)+CenterY;
			if (xc<0) xc=0;
			if (yc<0) yc=0;
			if (xc>639) xc=639;
			if (yc>479) yc=479;
			GreetPic->LineAdd(CenterX,CenterY,xc,yc,Color1, Color2);
			int rad2=cos(x+(double)(MyCounter)/31)*25;
			int rad3=cos(x*5)*rad2;
			int xc2=cos(x)*(rad3+100)+CenterX;
			int yc2=sin(x)*(rad3+100)+CenterY;
			if (xc2<0) xc2=0;
			if (yc2<0) yc2=0;
			if (xc2>639) xc2=639;
			if (yc2>479) yc2=479;
			GreetPic->LineAdd(xc,yc,xc2,yc2,Color2, Color3);
			int rad4=cos(x+(double)(MyCounter)/11)*20;
			int rad5=cos(x*4)*rad4;
			int xc3=cos(x)*(rad5+150)+CenterX;
			int yc3=sin(x)*(rad5+150)+CenterY;
			if (xc3<0) xc3=0;
			if (yc3<0) yc3=0;
			if (xc3>639) xc3=639;
			if (yc3>479) yc3=479;
			GreetPic->LineAdd(xc2,yc2,xc3,yc3,Color3, 0x000000);
		}
	}
	// Falling Lenzfares
	if ((MyCounter >8200 )&& (MyCounter <10500 ))
	{
		/*										Xpos in Screen , Y Pos  */
		LenzFlares->BltAdd(0,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,5,Lenzflare_YPos[0],64,64);
		LenzFlares->BltAdd(0,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,30,Lenzflare_YPos[1],84,84);
		LenzFlares->BltAdd(LenzFlares->GetWidth()/2,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,60,Lenzflare_YPos[2],54,54);
		LenzFlares->BltAdd(0,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,80,Lenzflare_YPos[3],44,44);
		LenzFlares->BltAdd(LenzFlares->GetWidth()/2,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,140,Lenzflare_YPos[4],64,64);
		LenzFlares->BltAdd(0,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,220,Lenzflare_YPos[5],100,100);
		LenzFlares->BltAdd(0,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,300,Lenzflare_YPos[6],25,25);
		LenzFlares->BltAdd(0,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,400,Lenzflare_YPos[7],84,84);
		LenzFlares->BltAdd(LenzFlares->GetWidth()/2,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,500,Lenzflare_YPos[8],16,16);
		LenzFlares->BltAdd(0,0,LenzFlares->GetWidth()/2,LenzFlares->GetHeight(),GreetPic,540,Lenzflare_YPos[9],32,32);

		for (int x=0;x<10;x++)
		{
			Lenzflare_YPos[x] += Lenzflare_YAdd[x];



			if (Lenzflare_YPos[x] > 370 )
			{
				Lenzflare_YPos[x] = 20;
				Lenzflare_YAdd[x] = (rand()%15)+3;
			}
		}
	}

	static int PicNumber;
	if (GotSync(6)) MyPicRaser=true;
	if (MyPicRaser)
	{
		PicNumber++;
		PicRaser[PicNumber%20]->BltTrans(prx,pry,320-prx,240-pry,GreetPic,0,120);
	}

// ###############  Double Star ###########################
      static float xrot;
      static float yrot;
      static float zrot;
      static float Xlght,Ylght;
      static int FaceBlend;
      CAll3DVector place;
      float xco,yco;
      if ((Xlght!=(float)0.1)&&(Xlght!=(float)(-0.1)))
              Xlght=.1;
      if ((Ylght!=(float)0.02)&&(Ylght!=(float)(-.02)))
              Ylght=.02;
      xrot+=Xlght;
      if ((xrot>2*PI)||(xrot<0))
      {
          Xlght=-Xlght;
      }

      yrot+=Ylght;
      if ((yrot>2*PI)||(yrot<0))
      {
          Ylght=-Ylght;
      }
      zrot+=.05;
      // ******************************* Vector Objecte gezeichnet ***************************

      engine->SetRenderSurface(GreetPic);

      engine->MouseX=xrot-PI;
      engine->MouseY=yrot-PI;
      Star2->xrot =xrot;
      Star2->yrot =yrot;

      FaceBlend+=(int)(Xlght*11);
      if (FaceBlend>156) FaceBlend=156;
      if (FaceBlend<0) FaceBlend=0;
      Star2->GetMesh()->SetNumFaces(FaceBlend);

      xco=cos((double)zrot)*12;
      yco=sin((double)zrot)*6;
      place.SetVector(xco+100,yco,120);
      Star2->SetPlace(place);
      //engine->ShowObject (Star2);

      Star2->GetMesh()->SetNumFaces(156-FaceBlend);
      place.SetVector(-xco+100,yco,120);
      Star2->xrot =-xrot;
      Star2->yrot =yrot;
      Star2->SetPlace(place);
      engine->ShowObject (Star2);

//############### Greetings ########################
    char *Greetings;
    static int grex;
    static int grey;
	static int GreetText=-1;
      if (GotSync(7))
      {
        grex=rand()%320;
        grey=rand()%230;
		GreetText++;
      }
	    switch (GreetText)
      {
      case 0:
          Greetings="Nomad";
          break;
      case 1:
          Greetings="The Black Lotus";
          break;
      case 2:
          Greetings="Pulse";
          break;
      case 3:
          Greetings="Cubic ";
          break;
      case 4:
          Greetings="Complex";
          break;
      case 5:
          Greetings="New Order";
          break;
      case 6:
          Greetings="Haensel";
          break;
      case 7:
          Greetings="Kolor";
          break;
      case 8:
          Greetings="TMB";
          break;
      case 9:
          Greetings="inSOMMIA";
          break;
      case 10:
          Greetings="POPSY TEAM";
          break;
      case 11:
          Greetings="CONDENSE";
          break;
      case 12:
          Greetings="RAZOR 1911";
          break;
      case 13:
          Greetings="SAVAGE";
          break;
      case 14:
          Greetings="O-JUICE";
          break;
      case 15:
          Greetings="ProCreation";
          break;
      case 16:
          Greetings="AArdbei";
          break;
      case 17:
          Greetings="Smash Designs";
          break;
      case 18:
          Greetings="Trinity";
          break;
      case 19:
          Greetings="Beyond";
          break;
      case 20:
          Greetings="AMBIENCE PARTY POSSE";
          break;
      case 21:
          Greetings="Kibosh";
          break;
      case 22:
          Greetings="Olaf";
          break;
      case 23:
          Greetings="Ewa und Max";
          break;
    default:
        Greetings=" ";
		GreetText=-1;
          break;

      }
      if (((MyCounter)%5)==0)
      {
          AsmFont->WriteScatter(GreetPic,320,240,"   WE RESPECT",10);
          AsmFont->WriteScatter(GreetPic,320,240,"   WE RESPECT",10);
          AsmFont->WriteScatter(GreetPic,320,240,"   WE RESPECT",10);
      }
      if (((MyCounter)%2)==0)
      {
          AsmFont->WriteScatter(GreetPic,320,240,"   we respect",7);
          AsmFont->WriteScatter(GreetPic,320,240,"   we respect",7);
          AsmFont->WriteScatter(GreetPic,320,240,"   we respect",7);
      }

      AsmFont->WriteScatter(GreetPic,grex, grey+105,Greetings,7);
      AsmFont->WriteScatter(GreetPic,grex, grey+105,Greetings,7);
      AsmFont->WriteScatter(GreetPic,grex, grey+105,Greetings,7);

}
/////////////////////////////////////////////////////////////////////////////
void CDemo::ShowUpScroller(int FrameCounter)
{
 	if (rand()%100<90)
	    EndPicture->BltTrans(0,0,640,270,UpScrollPic,0,105);
	else{
	    EndPicture->BltTV(0,0,640,270,UpScrollPic,0,105,12);
	}
    UpScroll(UpScrollPic,Font1, FrameCounter);
}

/////////////////////////////////////////////////////////////////////////////
void CDemo::PaintLines(int FrameCounter)
{
    // **************************** Vectors bewegt ***********************************
	int x1=FrameCounter<<1;
	if (x1<0) x1=0;
	if (x1>639) x1=639;
	DemoScreen->Line(0,104,x1,104,0xffffff);
	DemoScreen->Line(639-x1,376,639,376,0xffffff);
}


/////////////////////////////////////////////////////////////////////////////
void CDemo::UpScroll(CBitMap* Dest, CBMText* Font, int FrameCounter)      // Copies the Pic1 Bmap into Background....
{
  static int LinePos;
	static int PixelPos;
	int Height=300-(FrameCounter-13000)/7;
	int FontHeight=Font->GetFontHeight();
	for (int x=0;x<19;x++)
	{
		if ( (Height+x*FontHeight>0-FontHeight) && ( Height+x*FontHeight<405))
			Font->WriteStringAdd(Dest,0,Height+x*Font->GetFontHeight(), UpMessage+x*26,26);
	}
/*
	if ((PixelPos++)<Font->GetFontHeight()+1)
		{
					Dest->BltFast(0,1,Dest->GetWidth(),298,Dest,0,0);
		}
		else
		{
			PixelPos=0;
			Font->WriteString(Dest,0,Dest->GetHeight()-(Font->GetFontHeight()) - 4, UpMessage + LinePos,26);

			LinePos+=26;
			if (LinePos>(20*26))	//20 Reihen a 26 zeichen...
				LinePos=19*26;		// No Loop
			Dest->BltFast(0,1,Dest->GetWidth(),298,Dest,0,0);
		}
*/
}
/////////////////////////////////////////////////////////////////////////////
void CDemo::RotateLogo()
{
    engine->SetRenderSurface(UpScrollerDummy);
    CAll3DVector place;
    static float xrot;
    static float yrot;
    static float zrot;

    float xco,yco;
    static float Xlght,Ylght;
    NDObj->xrot = 3.1415;
    NDObj->yrot += 0.04;
    NDObj->zrot = 0;

    place.SetVector(-0,20,180);

    NDObj->SetPlace(place);
    engine->ShowObject(NDObj);
    CBitMap * LogoDest = DemoScreen->Clone(0,0,640,104);

    UpScrollerDummy->BltTrans(280,214,80,40,DemoScreen,0,64);
    UpScrollerDummy->Clear();
		delete  LogoDest;
}

void CDemo::MoveLogo(int FrameCounter)
{
    engine->SetRenderSurface(UpScrollerDummy);
    CAll3DVector place;
    static float xrot;
    static float yrot;
    static float zrot;

		int xoff,xrad,yrad,yoff, xpl,ypl,zpl;
		float	rot;
		xpl=0;ypl=20;zpl=180;
		xrad=440-(FrameCounter-13000);
		yrad=233-(FrameCounter-13000)/2;
		if (xrad<0) xrad=0;
		if (yrad<0) yrad=0;
		rot=(float)-(FrameCounter-13000);

		xoff=cos(rot/90+3*(PI/4))*xrad;
		yoff=sin(rot/90+3*(PI/4))*yrad;
//	xoff=cos(3*(PI/4))*440;
	//	yoff=sin(3*(PI/4))*233;

		if ((xrad<90)&&(yrad<90))
		{
			xpl=(FrameCounter-13350)/5;
			ypl=(FrameCounter-13350)/5;
			zpl=180-(FrameCounter-13350);
			if (xpl<0) xpl=0;
			if (ypl<20) ypl=20;
			if (zpl>180) zpl=180;
			if (xpl>50) xpl=50;
			if (ypl>45) ypl=45;
			if (zpl<20) zpl=20;
		}
    float xco,yco;
    static float Xlght,Ylght;
    NDObj->xrot = 3.1415;
    NDObj->yrot += 0.04;
    NDObj->zrot += 0.03;

    place.SetVector(xpl,ypl,zpl);

    NDObj->SetPlace(place);
    engine->ShowObject(NDObj);
    CBitMap * LogoDest = UpScrollPic->Clone(0,0,640,104);
    LogoDest->Clear();

    UpScrollerDummy->BltAdd(250,180,260,210,UpScrollPic,282+xoff,194-yoff);
    UpScrollerDummy->Clear();
		delete LogoDest;

}

/////////////////////////////////////////////////////////////////////////////
void CDemo::ShowCreditz(CBitMap * Buffer, int FrameCounter)
{
//    CreditzPic->BltFast(0,0,640,270,Buffer,0,105);
}


bool CDemo::GotSync(unsigned int ReqSync)
{
	if (ReqSync==Param)
	{
		Param=0;
		return true;
	}else return false;
}

void CDemo::ShowNDSign(int MyCounter)
{
	if (MyCounter<16700)
		NDSign->BltFast(0,0,640,480,GreetPic,0,0);
	else
	{
		NDSign->BltFast(0,0,640,480,DemoScreen,0,0);
		static int OldSymbol=-1;
		static int OutX,OutY;
		int Symb=((MyCounter-16700)/100)%11;
		if (Symb!=OldSymbol)
		{
			if (Symb!=10)
			{
				OutX=(rand()%220)+60;
				OutY=(rand()%50);
			}else{
				OutX=0;
				OutY=0;
			}
		}
		int Fly=(MyCounter-(16700+Symb*100))-30;
		BlackScreen->BltFast(0,430,640,35,BlackScreen,0,0);
		NasaFont->WriteStringAdd(BlackScreen,0,0,getSymbolName(Symb));
		for (int za=0;za<12;za++)
		{
			BlackScreen->BillBoard(0,0,640,33,DemoScreen,OutX,OutY,300-6*Fly+2*za,5);
		}
		OldSymbol=Symb;
	}
}

int CDemo::BlendOrder(int i)
{
	int back;
	back= BlendOrderArray[i];
	return (back);
}

void CDemo::InitBlendOrder()
{
	int wert;
	bool	found;
	for (int i=0;i<FADEBLCKS;i++) BlendOrderArray[i]=-1;
	for (int x=0;x<FADEBLCKS; x++)
	{
		do
		{
			found = false;
			wert=rand()%FADEBLCKS;
			for (int i=0;i<FADEBLCKS;i++)
				if (BlendOrderArray[i]==wert) found=true;
		} while (found);
		BlendOrderArray[x]=wert;
	}
}

char* CDemo::getSymbolName(int index)
{
	   switch (index)
      {
      case 0:
          return ("LOVE");
          break;
      case 1:
          return ("SPACE MIGRATION");
          break;
      case 2:
          return ("LAZER TECHNOLOGY");
          break;
      case 3:
          return ("DNA ENCODING");
          break;
      case 4:
          return ("PSYCHEDELICS");
          break;
      case 5:
          return ("UNITY");
          break;
      case 6:
          return ("SENSIBILITY");
          break;
      case 7:
          return ("DIGNITY");
          break;
      case 8:
          return ("PASSION");
          break;
      case 9:
          return ("BINARY DIGITS");
          break;
      case 10:
          return ("UNLEASED VARIETY");
          break;
      default:
          return ("");
          break;
		}
}
