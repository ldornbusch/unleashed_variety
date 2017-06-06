//-----------------------------------------------------------------------------
// File: switcher.cpp
//
// Desc: Dies ist die BasisAnwendung für alle All3D-Demos 
//       Full-Screen oder Windowed Modes are available.
// Code : LouyThorn 17.10.1999
//-----------------------------------------------------------------------------
#define INITGUID
#define NAME    "All3DInterface"
#define TITLE   "DDraw to All3D Window/Screen"

#ifndef WIN32_FUCK_AND_SUCK
#define WIN32_FUCK_AND_SUCK
#endif
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <ddraw.h>
#include "resource.h"
#include "All3DWin.h"

bool	fullscreen = false;		// App is in windowed mode
CAll3DWin *myWin;				//Die Interface-Klasse für DirectDraw Zugriff
//-----------------------------------------------------------------------------
// Name: MainWndProc()
// Desc: The Main Window Procedure - Habe ich leider nicht in die Klasse packen können...
LRESULT CALLBACK
MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (myWin)	//Bei der Initialisierung wird die Routine schon aufgerufen ohne gültiges Object..
		myWin->All3DWndProc(hWnd, msg, wParam, lParam);
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
void SetHighPriority()
{
	HANDLE	prc,thr;
	prc=GetCurrentProcess(); 
	thr=GetCurrentThread();
	if (!SetPriorityClass(prc,HIGH_PRIORITY_CLASS))
		MessageBox(NULL,"Konnte Process_priority nicht setzten!","All3D InitError", MB_OK);
	if (!SetThreadPriority(thr,THREAD_PRIORITY_NORMAL))
		MessageBox(NULL,"Konnte Thread_priority nicht setzten!","All3D InitError", MB_OK);
}

int PASCAL
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpszCmdLine,
        int         nCmdShow)
{
	int a=0;
	SetHighPriority();
	CDemo *intro;
	myWin = new CAll3DWin(hInstance, hPrevInstance, MainWndProc, fullscreen);
	intro=new CDemo();
	intro->Init();
	myWin->Frames=0;
	a=myWin->MessageLoop(*intro);
	delete myWin;

	return a;
}