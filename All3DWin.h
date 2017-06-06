// All3DWin.h: Schnittstelle für die Klasse CAll3DWin.
//
//////////////////////////////////////////////////////////////////////
#define INITGUID
#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include "MyHermes.h"
#include "Bitmap.h"
#include "Demo.h"

#if !defined(AFX_ALL3DWIN_H__07C97742_8027_11D3_A6F8_0080AD40DBC7__INCLUDED_)
#define AFX_ALL3DWIN_H__07C97742_8027_11D3_A6F8_0080AD40DBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HERMES_INITFAIL 10
#define HERMES_DONEFAIL 11
#define HERMESBUFFER_FAILED 12
#define HERMES_CONV_NOTFOUNT 13
#define HERMES_NO_DXPIXFORM 14

class CAll3DWin  
{
public:
	LRESULT All3DWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int MessageLoop(CDemo objDemo);

	static void CALLBACK TimerProc (UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
	{
		Frames++;
	}
	static int Frames;
	void SetPrevInstance(HINSTANCE Prev);
	void SetInstance(HINSTANCE hInstance);
	void Flip();
	void ClearBackBuffer(int Color=0);
	
	HRESULT ChangeCoopLevel();
	HRESULT ReleaseAllObjects(bool EndDemo=false);
	HRESULT InitSurfaces();
	HWND InitWindow(WNDPROC MainProc);

	CAll3DWin(HINSTANCE hInstance, HINSTANCE hPrevInstance, WNDPROC MainWndProc, bool Fullscr=true);
	CAll3DWin();
	virtual ~CAll3DWin();

private:
	UINT TimerID;
	int BitsPixel;
	HermesFormat* DirectXBufferFormat;
	HermesFormat* HermesBufferFormat;
	void CopyHermesToBackBuffer();
	CBitMap* MakeBitMap();
	void ReleaseHermesBuffer();
	void CreateHermesBuffer();
	int WindowX;
	int WindowY;
	int DisplayDepht;
	bool bFullScreen;
	bool bReady;
	bool bActive;
	bool bHermes;
	void ShowError(HRESULT);
	char* ErrorToString(HRESULT);
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	HWND hWnd;
	HACCEL hAccel;
	RECT rcWindow;
	RECT rcScreen;
	RECT rcViewPort;
	CBitMap* HermesBuffer;							// Hermes-Buffer (if no 640x480x32 is available)
	HermesHandle HermesConverter;
	DDSURFACEDESC sdesc;
	LPDIRECTDRAWSURFACE BackBuffer;			// DirectX-Backbuffer
	LPDIRECTDRAWSURFACE Primary;				// DirectX-Frontbuffer
	LPDIRECTDRAW2 dd;										// Direct Draw Object
};

#endif // !defined(AFX_ALL3DWIN_H__07C97742_8027_11D3_A6F8_0080AD40DBC7__INCLUDED_)
