// All3DWin.cpp: Implementierung der Klasse CAll3DWin.
//
//////////////////////////////////////////////////////////////////////
#include <ddraw.h>
#include "All3DWin.h"
#include "resource.h"

#define WINX	640
#define WINY	480
#define WINPLANES 32
int CAll3DWin::Frames=0;
// -------------------------------------------------------------------------
// Dialog CallBack Function keine Member klasse, aber sie muss hier stehen
LRESULT CALLBACK
AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch (wParam)
            {
                case IDOK:
                    EndDialog(hDlg, TRUE);
                    return TRUE;
            }
            break;
    }
    return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CAll3DWin::CAll3DWin()		// Simple Constructor
{
	bHermes=false;
//	isExit=true;
	WindowX = WINX;
	WindowY = WINY;
	DisplayDepht = WINPLANES;
	HermesBuffer=NULL;
	BackBuffer=NULL;
	Primary=NULL;
	dd=NULL;
	bReady=false;
	bActive=false;
	bFullScreen=false;
	if (Hermes_Init()==0) ShowError (HERMES_INITFAIL);
	HermesConverter = Hermes_ConverterInstance(0) ;
}

CAll3DWin::CAll3DWin(HINSTANCE hInst, HINSTANCE hPrevInst, WNDPROC MainWndProc, bool Fullscr)
{
	bHermes=false;
	WindowX = WINX;
	WindowY = WINY;
	DisplayDepht = WINPLANES;
	HermesBuffer=NULL;
	BackBuffer=NULL;
	Primary=NULL;
	dd=NULL;
	bFullScreen=Fullscr;
	hInstance=hInst;
	hPrevInstance=hPrevInst;
	InitWindow(MainWndProc);
	if (InitSurfaces()!=DD_OK) 
	{
		MessageBox(hWnd,"Fehler bei Surface Initialisierung!!","All3D-DirectX-Error", MB_OK);
		exit(0);
	}
	bReady=true;
	bActive=true;
	if (Hermes_Init()==0) ShowError (HERMES_INITFAIL);
	HermesConverter = Hermes_ConverterInstance(0) ;
}

CAll3DWin::~CAll3DWin()
{
	timeEndPeriod(10);
	if (TimerID) 	timeKillEvent(TimerID);
	bHermes=false;
	WindowX = 0;
	WindowY = 0;
	DisplayDepht = 0;
	Hermes_ConverterReturn(HermesConverter);
	if (Hermes_Done()<0) ShowError (HERMES_DONEFAIL);
}

void CAll3DWin::SetInstance(HINSTANCE hInst){hInstance=hInst;}
void CAll3DWin::SetPrevInstance(HINSTANCE Prev){hPrevInstance=Prev;}

HWND CAll3DWin::InitWindow(WNDPROC MainWndProc)	//Registers the Window and other Windows STUFF
{
  WNDCLASS	 wc;
	LPDIRECTDRAW ddraw1;
	HRESULT		 result;
  int          cx,cy;

  if (!hPrevInstance)
  {
      // Register the Window Class
      wc.lpszClassName = "All3D";
      wc.lpfnWndProc = MainWndProc;
      wc.style = 0;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
      wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1) );
      wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+2);
      wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;
      RegisterClass(&wc);
  }

  // Calculate the proper size for the window given a client of 640x480
  cx = WindowX+GetSystemMetrics(SM_CXSIZEFRAME)*2;
  cy = WindowY+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);
  // Create and Show the Main Window
  hWnd = CreateWindowEx(0,
                        "All3D",
                        "UNLEASHED VARIETY / [:N:98:D:]",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
  	                    cx,
                        cy,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

  if (hWnd == NULL)
    return false;
  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  // Save the window size/pos for switching modes
  GetWindowRect(hWnd, &rcWindow);
  hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN_ACCEL));

	result=DirectDrawCreate(NULL, &ddraw1,NULL);
	if (result!=DD_OK) 
	{
		ShowError(result);
		return NULL;
	}
	result = ddraw1->QueryInterface(IID_IDirectDraw2,(LPVOID *) &dd);
	if (result!=DD_OK) 
	{
		ShowError(result);
		return NULL;
	}
	ddraw1->Release();
  GetWindowRect(hWnd, &rcWindow);
	
	timeBeginPeriod(10);
	TimerID=timeSetEvent(10,10, (LPTIMECALLBACK) &TimerProc,NULL,TIME_PERIODIC);
	
	return hWnd;
}

void CAll3DWin::ShowError(HRESULT hRet)			//Returns a messageBox with Translated ErrorMessages
{
	char* Message;
	dd->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
	Message=ErrorToString(hRet);
	MessageBox(NULL, Message,"All3D Engine ERROR", MB_OK);
}

HRESULT CAll3DWin::InitSurfaces()						// Init the Hermes/DirectX-Surfaces
{
    HRESULT				hRet;
    DDSURFACEDESC		ddsd;
    DDSCAPS				ddscaps;
    LPDIRECTDRAWCLIPPER pClipper;
    if (!bFullScreen)
    {
        // Get normal windowed mode
        hRet = dd->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
        if (hRet != DD_OK)
				{
					ShowError(hRet);
					return hRet;
				}

    	// Get the dimensions of the viewport and screen bounds
    	GetClientRect(hWnd, &rcViewPort);
    	GetClientRect(hWnd, &rcScreen);
    	ClientToScreen(hWnd, (POINT*)&rcScreen.left);
    	ClientToScreen(hWnd, (POINT*)&rcScreen.right);

        // Create the primary surface
        ZeroMemory(&ddsd,sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        hRet = dd->CreateSurface(&ddsd, &Primary, NULL);
        if (hRet != DD_OK)
            return hRet;

        // Create a clipper object since this is for a Windowed render
        hRet = dd->CreateClipper(0, &pClipper, NULL);
        if (hRet != DD_OK)
            return hRet;

        // Associate the clipper with the window
        pClipper->SetHWnd(0, hWnd);
        Primary->SetClipper(pClipper);
        pClipper->Release();
        pClipper = NULL;

        // Get the backbuffer. For fullscreen mode, the backbuffer was created
        // along with the primary, but windowed mode still needs to create one.
        ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS ;// | DDSD_PIXELFORMAT;
        ddsd.dwWidth        = WindowX;
        ddsd.dwHeight       = WindowY;

				HDC	dc=GetDC(hWnd);							// Check BitDepht if !=32Bit HERMES BUFFER WILL BE CREATED...
				BitsPixel=GetDeviceCaps (dc,BITSPIXEL);
				ReleaseDC(hWnd,dc);
				bHermes=(BitsPixel!=DisplayDepht);

/*        ddsd.ddpfPixelFormat.dwSize=sizeof(DDPIXELFORMAT);		// We have either 32 Bit and do not need Pixelformat declaration 
				ddsd.ddpfPixelFormat.dwFlags=DDPF_RGB;				// RGB		// or we use Hermes, to convert to Backbufferformat
				ddsd.ddpfPixelFormat.dwFourCC=0;							// I dont know
				ddsd.ddpfPixelFormat.dwRGBBitCount=DisPlayDepht;// 32 Bit Yeahh!
				ddsd.ddpfPixelFormat.dwRBitMask=0x00ff0000;		// Rot   Anteil...
				ddsd.ddpfPixelFormat.dwGBitMask=0x0000ff00;		// Gruen Anteil..
				ddsd.ddpfPixelFormat.dwBBitMask=0x000000ff;		// Blau  Anteil.
				ddsd.ddpfPixelFormat.dwRGBAlphaBitMask =0x00000000; //Alpha anteil....
*/			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        hRet = dd->CreateSurface(&ddsd, &BackBuffer, NULL);
        if (hRet != DD_OK)
				{
					ShowError(hRet); 
					return hRet;
				}
    }
    else
    {
        // Get exclusive mode
        hRet = dd->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE |
                                                DDSCL_FULLSCREEN);
        if (hRet != DD_OK)
				{
						ShowError(hRet);
            return hRet;
				}

        // Set the video mode to 640x480x32
				BitsPixel=32;
        hRet = dd->SetDisplayMode(WindowX,WindowY,DisplayDepht, 0, 0);
        if (hRet != DD_OK)
				{
					bHermes=true;
					BitsPixel=24;
					hRet = dd->SetDisplayMode(WindowX,WindowY,24, 0, 0);

					if (hRet != DD_OK)
					{
						BitsPixel=16;
						hRet = dd->SetDisplayMode(WindowX,WindowY,16, 0, 0);
						if (hRet != DD_OK)
						{				
							ShowError(hRet);
							exit(1);
						}
					}
				}
    	// Get the dimensions of the viewport and screen bounds
    	// Store the rectangle which contains the renderer
    	SetRect(&rcViewPort, 0, 0, WindowX, WindowY );
    	memcpy(&rcScreen, &rcViewPort, sizeof(RECT) );

			// Create the primary surface with 1 back buffer
			ZeroMemory(&ddsd,sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			ddsd.dwFlags = DDSD_CAPS |
										 DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                			DDSCAPS_FLIP |
                			DDSCAPS_COMPLEX;
			ddsd.dwBackBufferCount = 1;
			hRet = dd->CreateSurface( &ddsd, &Primary, NULL);
			if (hRet != DD_OK)
			{
				ShowError(hRet); 
				return hRet;
			}
			ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
			hRet = Primary->GetAttachedSurface(&ddscaps, &BackBuffer);
			if (hRet != DD_OK)
			{
				ShowError(hRet); 
				return hRet;
			}
		}

		switch (BitsPixel)
		{
		case 8:
			BitsPixel=1;
			break;
		case 15:
			BitsPixel=2;
			break;
		case 16:
			BitsPixel=2;
			break;
		case 24:
			BitsPixel=3;
			break;
		case 32:
			BitsPixel=4;
			break;
		}

		CreateHermesBuffer();		// If not 32Bit->CreateHermes
    return DD_OK;

}

HRESULT CAll3DWin::ReleaseAllObjects(bool EndDemo)
{
    if (dd != NULL)
    {
        dd->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
        if (BackBuffer != NULL)
        {
            BackBuffer->Release();
            BackBuffer = NULL;
        }
        if (Primary != NULL)
        {
            Primary->Release();
            Primary = NULL;
        }
		if (EndDemo)		// WICHTIG: dd-Obj MUSS Releast werden BEVOR das Fenster destroyed wird.. deswegen darf dd NICHT im Destruktor releast werden!! (Absturz bei FullScreen)
		{
			dd->Release();
			dd=NULL;
		}
	}
	ReleaseHermesBuffer();
    return DD_OK;
}


//#############################  HERMES STUFF #####################################
void CAll3DWin::CreateHermesBuffer()
{
	if (bHermes)
	{
		DDPIXELFORMAT DDPixelFormat;
		DDPixelFormat.dwSize=sizeof(DDPIXELFORMAT);
		HRESULT hRes;
		hRes=Primary->GetPixelFormat(&DDPixelFormat);
		if ( hRes!= DD_OK) 
		{
			ShowError(hRes);//HERMES_NO_DXPIXFORM);  
			exit(1);
		}
		else 
		{
			HermesBuffer=new CBitMap(WindowX, WindowY);		//CBitmap erstellen von 640x480x32
			HermesBufferFormat = Hermes_FormatNew(32 ,0x00ff0000,0x0000ff00, 0x000000ff ,0xff000000 ,0);
			DirectXBufferFormat= Hermes_FormatNew(DDPixelFormat.dwRGBBitCount ,DDPixelFormat.dwRBitMask,DDPixelFormat.dwGBitMask, DDPixelFormat.dwBBitMask ,DDPixelFormat.dwAlphaBitDepth, 0);	
		}
	}
	else
	{
		HermesBuffer=new CBitMap();										//Ansonsten Leere Bitmap erstellen(wird an BackBuffer gelinkt)
	}
}

void CAll3DWin::ReleaseHermesBuffer()
{
	if ((HermesBuffer)&&(bHermes)) delete HermesBuffer;		//DoubleCheck, da ja auch die Addresse des BackBuffers da stehen könnte
	HermesBuffer = NULL;
	bHermes=false;
}

CBitMap* CAll3DWin::MakeBitMap()
{
	HRESULT result;
	sdesc.dwSize =sizeof(sdesc);	// sdesc is global..
	result=BackBuffer->Lock(NULL, &sdesc, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR ,NULL);
	if (result==DDERR_SURFACELOST)
	{
		if (BackBuffer->Restore() == DD_OK) 
			result=BackBuffer->Lock(NULL,&sdesc, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR ,NULL);
		else 
		{
			ShowError (result);
			return NULL;
		}	
	}
	if (result!=DD_OK)	
	{
		ShowError (result);//HERMESBUFFER_FAILED);
		return NULL;
	}
	if (bHermes)		// if Hermes Buffer used, Give this CBitMap back
	{
		return HermesBuffer;
	}
	else
	{
	//										BackBuffer wird in CBitmap gewandelt 
			HermesBuffer->Init((int*)sdesc.lpSurface, sdesc.dwWidth, sdesc.dwHeight, 32, sdesc.lPitch-(sdesc.dwWidth*BitsPixel) );
			return HermesBuffer;
	}
}

void CAll3DWin::CopyHermesToBackBuffer()
{
	if (bHermes)		//Only if Hermes Buffer used, copy the Hermesbuffer to BackBuffer with HERMES LIB
	{
		if (Hermes_ConverterRequest(HermesConverter, HermesBufferFormat, DirectXBufferFormat)==0) ShowError(HERMES_CONV_NOTFOUNT);
		else 
			Hermes_ConverterCopy(HermesConverter, 
														(void*) HermesBuffer->GetBitmap(), 0,0 , 
														HermesBuffer->GetWidth() ,HermesBuffer->GetHeight(),HermesBuffer->GetBytesPerRow(),
														(void*) sdesc.lpSurface ,0,0,
														sdesc.dwWidth,sdesc.dwHeight,(int)sdesc.lPitch) ;
	}
	BackBuffer->Unlock(sdesc.lpSurface);
}

HRESULT CAll3DWin::ChangeCoopLevel()
{
    HRESULT hRet;

    if (!bFullScreen)	GetWindowRect(hWnd, &rcWindow);
    // Release all objects that need to be re-created for the new device
	hRet = ReleaseAllObjects();
    if (hRet != DD_OK)
	{
		ShowError(hRet);
		return hRet;
    }
	// In case we're coming from a fullscreen mode, restore the window size
	if (bFullScreen)
    {
        SetWindowPos(hWnd, HWND_NOTOPMOST, rcWindow.left, rcWindow.top,
                     (rcWindow.right - rcWindow.left), 
                     (rcWindow.bottom - rcWindow.top), SWP_SHOWWINDOW );
    }
	bFullScreen=!bFullScreen;
    // Re-create the surfaces
    hRet = InitSurfaces();
    return hRet;
}

void CAll3DWin::ClearBackBuffer(int Color)
{
    DDBLTFX     ddbltfx;

    ZeroMemory(&ddbltfx, sizeof(ddbltfx));
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = Color;
    BackBuffer->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

void CAll3DWin::Flip()
{
	HRESULT hRet;
	while (TRUE)
  {
      // If we are in windowed mode, perform a blt.
      if (!bFullScreen)
      {
          hRet = Primary->Blt(&rcScreen, BackBuffer,
                                    &rcViewPort, DDBLT_WAIT,
                                    NULL);
      }
      else
      {
          // Else we are in fullscreen mode, so perform a flip.
          hRet = Primary->Flip( NULL, 0L );
      }
      if (hRet == DD_OK )
          break;
      if (hRet == DDERR_SURFACELOST )
      {
          hRet = Primary->Restore();
          if (hRet != DD_OK )
					{
						ShowError(hRet);
						break;
					}
      }
      if (hRet != DDERR_WASSTILLDRAWING )
			{
				ShowError(hRet);
				break;
			}
  }
  if (bActive && bReady )
  {	  SetCursor(NULL);}

}

int CAll3DWin::MessageLoop(CDemo objDemo)	//CDemo.UpdateFrame () wird von Hier aufgerufen
{
	MSG		msg;
	HDC		dc ;
	static bool isExit=true;
	int newtime, lasttime;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0 ))
			  break;
			// Translate and dispatch the message
			if (0 == TranslateAccelerator(hWnd,hAccel, &msg))
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg);
			}
		}
		else
			if (bActive && bReady)
			{ //                    Idle processing
				lasttime =GetTickCount();
				isExit=objDemo.UpdateFrame(MakeBitMap(), Frames); 	//Creates a CBitmap of the Backbuffer, or the HermesBuffer
				CopyHermesToBackBuffer();										//Actualize the BackBuffer (if needed)
				Flip();
				newtime =GetTickCount();
				newtime=newtime-lasttime;
				float FPS=1000/newtime;
				char	dummy[60];
				//Frames ++;
				wsprintf(dummy,"Max Time: %d Frames per Second, %d ms" ,(int)(FPS), Frames);
				dc = GetDC(hWnd);
					TextOut(dc,10,5,dummy, strlen(dummy));
				ReleaseDC(hWnd,dc);
				if (!isExit) 
					PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
			else
			{   WaitMessage();   }
		}
	return msg.wParam;
}

LRESULT CAll3DWin::All3DWndProc(HWND Handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HRESULT         hRet;
	MINMAXINFO      *pMinMax;
	switch (msg)
	{
		case WM_ACTIVATEAPP:
			// Pause if minimized or not the top window
			// bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
		return 0L;
		case WM_COMMAND:
			// Handle all menu and accelerator commands 
			switch (LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInstance, MAKEINTRESOURCE(DLG_ABOUTDLG), Handle, (DLGPROC)AboutDlgProc);
					return 0L;
				case IDM_TOGGLEFULLSCREEN:
					// Toggle the fullscreen/window mode
					hRet=ChangeCoopLevel();
					return 0L;
				case IDM_EXIT:
					// Received key/menu command to exit app
					PostMessage(Handle, WM_CLOSE, 0, 0);
					return 0L;
			}
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_CLOSE:
			// Clean up and close the app
			ReleaseAllObjects(true);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0L;
		case WM_GETMINMAXINFO:
			// Fix the size of the window to 640x480 (client size)
			pMinMax = (MINMAXINFO *)lParam;
			pMinMax->ptMinTrackSize.x = WindowX+GetSystemMetrics(SM_CXSIZEFRAME)*2;
			pMinMax->ptMinTrackSize.y = WindowY+GetSystemMetrics(SM_CYSIZEFRAME)*2
										 +GetSystemMetrics(SM_CYMENU);
			pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
			pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
			break;
		case WM_KEYDOWN:
			// Handle any non-accelerated key commands
			switch (wParam)
			{
				case VK_ESCAPE:
				case VK_F12:
					PostMessage(Handle, WM_CLOSE, 0, 0);
				return 0L;
			}
			break;
		case WM_MOVE:
			// Retrieve the window position after a move
			if (bActive && bReady && !bFullScreen)
			{
			GetWindowRect(Handle, &rcWindow);
			GetClientRect(Handle, &rcViewPort);
			GetClientRect(Handle, &rcScreen);
			ClientToScreen(Handle, (POINT*)&rcScreen.left);
			ClientToScreen(Handle, (POINT*)&rcScreen.right);
			}
			break;
		case WM_PAINT:
			// Update the screen if we need to refresh
			if (!bFullScreen && bReady)
			{
						Flip();
			}
			break;
		case WM_SETCURSOR:
		  // Display the cursor in the window if windowed
		  if (bActive && bReady )//&& !bFullScreen)
		  {
			  SetCursor(NULL);
			  //return TRUE;
		  }
		break;
		case WM_SIZE:
		  // Check to see if we are losing our window...
		  if (SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam)
			  bActive = FALSE;
		  else
			  bActive = TRUE;
		break;
	}
	return (LRESULT)0;
}

char* CAll3DWin::ErrorToString(HRESULT error)
{
    switch(error) {
        case DD_OK:
            return "No error.\0";
        case DDERR_ALREADYINITIALIZED:
            return "This object is already initialized.\0";
        case DDERR_BLTFASTCANTCLIP:
            return "Return if a clipper object is attached to the source surface passed into a BltFast call.\0";
        case DDERR_CANNOTATTACHSURFACE:
            return "This surface can not be attached to the requested surface.\0";
        case DDERR_CANNOTDETACHSURFACE:
            return "This surface can not be detached from the requested surface.\0";
        case DDERR_CANTCREATEDC:
            return "Windows can not create any more DCs.\0";
        case DDERR_CANTDUPLICATE:
            return "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0";
        case DDERR_CLIPPERISUSINGHWND:
            return "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0";
        case DDERR_COLORKEYNOTSET:
            return "No src color key specified for this operation.\0";
        case DDERR_CURRENTLYNOTAVAIL:
            return "Support is currently not available.\0";
        case DDERR_DIRECTDRAWALREADYCREATED:
            return "A DirectDraw object representing this driver has already been created for this process.\0";
        case DDERR_EXCEPTION:
            return "An exception was encountered while performing the requested operation.\0";
        case DDERR_EXCLUSIVEMODEALREADYSET:
            return "An attempt was made to set the cooperative level when it was already set to exclusive.\0";
        case DDERR_GENERIC:
            return "Generic failure.\0";
        case DDERR_HEIGHTALIGN:
            return "Height of rectangle provided is not a multiple of reqd alignment.\0";
        case DDERR_HWNDALREADYSET:
            return "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0";
        case DDERR_HWNDSUBCLASSED:
            return "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0";
        case DDERR_IMPLICITLYCREATED:
            return "This surface can not be restored because it is an implicitly created surface.\0";
        case DDERR_INCOMPATIBLEPRIMARY:
            return "Unable to match primary surface creation request with existing primary surface.\0";
        case DDERR_INVALIDCAPS:
            return "One or more of the caps bits passed to the callback are incorrect.\0";
        case DDERR_INVALIDCLIPLIST:
            return "DirectDraw does not support the provided cliplist.\0";
        case DDERR_INVALIDDIRECTDRAWGUID:
            return "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0";
        case DDERR_INVALIDMODE:
            return "DirectDraw does not support the requested mode.\0";
        case DDERR_INVALIDOBJECT:
            return "DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0";
        case DDERR_INVALIDPARAMS:
            return "One or more of the parameters passed to the function are incorrect.\0";
        case DDERR_INVALIDPIXELFORMAT:
            return "The pixel format was invalid as specified.\0";
        case DDERR_INVALIDPOSITION:
            return "Returned when the position of the overlay on the destination is no longer legal for that destination.\0";
        case DDERR_INVALIDRECT:
            return "Rectangle provided was invalid.\0";
        case DDERR_LOCKEDSURFACES:
            return "Operation could not be carried out because one or more surfaces are locked.\0";
        case DDERR_NO3D:
            return "There is no 3D present.\0";
        case DDERR_NOALPHAHW:
            return "Operation could not be carried out because there is no alpha accleration hardware present or available.\0";
        case DDERR_NOBLTHW:
            return "No blitter hardware present.\0";
        case DDERR_NOCLIPLIST:
            return "No cliplist available.\0";
        case DDERR_NOCLIPPERATTACHED:
            return "No clipper object attached to surface object.\0";
        case DDERR_NOCOLORCONVHW:
            return "Operation could not be carried out because there is no color conversion hardware present or available.\0";
        case DDERR_NOCOLORKEY:
            return "Surface doesn't currently have a color key\0";
        case DDERR_NOCOLORKEYHW:
            return "Operation could not be carried out because there is no hardware support of the destination color key.\0";
        case DDERR_NOCOOPERATIVELEVELSET:
            return "Create function called without DirectDraw object method SetCooperativeLevel being called.\0";
        case DDERR_NODC:
            return "No DC was ever created for this surface.\0";
        case DDERR_NODDROPSHW:
            return "No DirectDraw ROP hardware.\0";
        case DDERR_NODIRECTDRAWHW:
            return "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0";
        case DDERR_NOEMULATION:
            return "Software emulation not available.\0";
        case DDERR_NOEXCLUSIVEMODE:
            return "Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0";
        case DDERR_NOFLIPHW:
            return "Flipping visible surfaces is not supported.\0";
        case DDERR_NOGDI:
            return "There is no GDI present.\0";
        case DDERR_NOHWND:
            return "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0";
        case DDERR_NOMIRRORHW:
            return "Operation could not be carried out because there is no hardware present or available.\0";
        case DDERR_NOOVERLAYDEST:
            return "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0";
        case DDERR_NOOVERLAYHW:
            return "Operation could not be carried out because there is no overlay hardware present or available.\0";
        case DDERR_NOPALETTEATTACHED:
            return "No palette object attached to this surface.\0";
        case DDERR_NOPALETTEHW:
            return "No hardware support for 16 or 256 color palettes.\0";
        case DDERR_NORASTEROPHW:
            return "Operation could not be carried out because there is no appropriate raster op hardware present or available.\0";
        case DDERR_NOROTATIONHW:
            return "Operation could not be carried out because there is no rotation hardware present or available.\0";
        case DDERR_NOSTRETCHHW:
            return "Operation could not be carried out because there is no hardware support for stretching.\0";
        case DDERR_NOT4BITCOLOR:
            return "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0";
        case DDERR_NOT4BITCOLORINDEX:
            return "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0";
        case DDERR_NOT8BITCOLOR:
            return "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0";
        case DDERR_NOTAOVERLAYSURFACE:
            return "Returned when an overlay member is called for a non-overlay surface.\0";
        case DDERR_NOTEXTUREHW:
            return "Operation could not be carried out because there is no texture mapping hardware present or available.\0";
        case DDERR_NOTFLIPPABLE:
            return "An attempt has been made to flip a surface that is not flippable.\0";
        case DDERR_NOTFOUND:
            return "Requested item was not found.\0";
        case DDERR_NOTLOCKED:
            return "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0";
        case DDERR_NOTPALETTIZED:
            return "The surface being used is not a palette-based surface.\0";
        case DDERR_NOVSYNCHW:
            return "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0";
        case DDERR_NOZBUFFERHW:
            return "Operation could not be carried out because there is no hardware support for zbuffer blitting.\0";
        case DDERR_NOZOVERLAYHW:
            return "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0";
        case DDERR_OUTOFCAPS:
            return "The hardware needed for the requested operation has already been allocated.\0";
        case DDERR_OUTOFMEMORY:
            return "DirectDraw does not have enough memory to perform the operation.\0";
        case DDERR_OUTOFVIDEOMEMORY:
            return "DirectDraw does not have enough memory to perform the operation.\0";
        case DDERR_OVERLAYCANTCLIP:
            return "The hardware does not support clipped overlays.\0";
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
            return "Can only have ony color key active at one time for overlays.\0";
        case DDERR_OVERLAYNOTVISIBLE:
            return "Returned when GetOverlayPosition is called on a hidden overlay.\0";
        case DDERR_PALETTEBUSY:
            return "Access to this palette is being refused because the palette is already locked by another thread.\0";
        case DDERR_PRIMARYSURFACEALREADYEXISTS:
            return "This process already has created a primary surface.\0";
        case DDERR_REGIONTOOSMALL:
            return "Region passed to Clipper::GetClipList is too small.\0";
        case DDERR_SURFACEALREADYATTACHED:
            return "This surface is already attached to the surface it is being attached to.\0";
        case DDERR_SURFACEALREADYDEPENDENT:
            return "This surface is already a dependency of the surface it is being made a dependency of.\0";
        case DDERR_SURFACEBUSY:
            return "Access to this surface is being refused because the surface is already locked by another thread.\0";
        case DDERR_SURFACEISOBSCURED:
            return "Access to surface refused because the surface is obscured.\0";
        case DDERR_SURFACELOST:
            return "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0";
        case DDERR_SURFACENOTATTACHED:
            return "The requested surface is not attached.\0";
        case DDERR_TOOBIGHEIGHT:
            return "Height requested by DirectDraw is too large.\0";
        case DDERR_TOOBIGSIZE:
            return "Size requested by DirectDraw is too large, but the individual height and width are OK.\0";
        case DDERR_TOOBIGWIDTH:
            return "Width requested by DirectDraw is too large.\0";
        case DDERR_UNSUPPORTED:
            return "Action not supported.\0";
        case DDERR_UNSUPPORTEDFORMAT:
            return "FOURCC format requested is unsupported by DirectDraw.\0";
        case DDERR_UNSUPPORTEDMASK:
            return "Bitmask in the pixel format requested is unsupported by DirectDraw.\0";
        case DDERR_VERTICALBLANKINPROGRESS:
            return "Vertical blank is in progress.\0";
        case DDERR_WASSTILLDRAWING:
            return "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0";
        case DDERR_WRONGMODE:
            return "This surface can not be restored because it was created in a different mode.\0";
        case DDERR_XALIGN:
            return "Rectangle provided was not horizontally aligned on required boundary.\0";
				case HERMES_INITFAIL:
            return "HERMES initialisation failed.\0";
				case HERMES_DONEFAIL:
            return "HERMES end Routine failed.\0";
				case HERMESBUFFER_FAILED:
            return "HERMES Buffer Init-Routine failed.\0";
				case HERMES_CONV_NOTFOUNT:
            return "HERMES Converter not Found.\0";
				case 	HERMES_NO_DXPIXFORM:
            return "DirectX-PixelFormat not Found.\0";

				default:
            return "Unrecognized error value.\0";
    }
}

