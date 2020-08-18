// NVFC_AZ.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "NVFC_AZ.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
void				DisplayTotals(HWND);
void				DisplayVLT(HWND);
void				DisplayTRN(HWND);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NVFC_AZ, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_NVFC_AZ);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)DlgProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NVFC_AZ);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_NVFC_AZ;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = (HWND)DialogBox(hInst,MAKEINTRESOURCE(IDD_FEE),NULL,(DLGPROC)DlgProc);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Message handler for about box.
LRESULT CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(hInst,MAKEINTRESOURCE(IDI_NVFC_AZ)));
			SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)LoadIcon(hInst,MAKEINTRESOURCE(IDI_NVFC_AZ)));

			SetDlgItemText(hWnd,IDC_EDITREG,"8.25");
			SetDlgItemText(hWnd,IDC_EDITAQF,"1.50");
			SetDlgItemText(hWnd,IDC_EDITTTL,"4.00");

			DisplayVLT(hWnd);
			DisplayTRN(hWnd);
			DisplayTotals(hWnd);

			return TRUE;
		}
		break;

		case WM_COMMAND:
		{
			int CtrlID = LOWORD(wParam);
			HWND hWndFrom = (HWND)(lParam);
			int CtrlCode = HIWORD(wParam);

			if (CtrlCode == EN_CHANGE &&
				CtrlID == IDC_EDITFLP)
			/* Update the VLT fee */
			{
				DisplayVLT(hWnd);
				DisplayTotals(hWnd);

				return TRUE;
			}
			else if (CtrlCode == EN_CHANGE &&
				CtrlID == IDC_EDITPCR)
			/* Update the TRN */
			{
				DisplayTRN(hWnd);
				DisplayTotals(hWnd);

				return TRUE;
			}

			return FALSE;
		}
		break;

		case WM_CLOSE:
			DestroyWindow(hWnd);
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}

	return FALSE;
}

void DisplayTotals(HWND hMain)
/*
** DisplayTotals()
** Displays the calculated amounts of the total and grand total boxes
*/
{
	char szVLT[256], szREG[256], szAQF[256], szTTL[256],
		szTRN[256], szPCR[256];

	char szTotal[256], szGrandTotal[256];

	float fVLT = 0, fREG = 0, fAQF = 0, fTTL = 0, fTRN = 0, fPCR = 0;
	float fTotal = 0.00, fGrandTotal = 0.00;

	/* Get amounts entered */
	GetDlgItemText(hMain,IDC_EDITVLT,szVLT,sizeof(szVLT));
	GetDlgItemText(hMain,IDC_EDITREG,szREG,sizeof(szREG));
	GetDlgItemText(hMain,IDC_EDITAQF,szAQF,sizeof(szAQF));
	GetDlgItemText(hMain,IDC_EDITTTL,szTTL,sizeof(szTTL));
	GetDlgItemText(hMain,IDC_EDITTRN,szTRN,sizeof(szTRN));
	GetDlgItemText(hMain,IDC_EDITPCR,szPCR,sizeof(szPCR));

	/* Calculate amounts into totals */
	fVLT = atof(szVLT);
	fREG = atof(szREG);
	fAQF = atof(szAQF);
	fTTL = atof(szTTL);
	fTRN = atof(szTRN);
	fPCR = atof(szPCR);

	fTotal = fVLT + fREG + fAQF + fTTL;

	fGrandTotal = fTotal + (fTRN - fPCR);

	/* Display amounts in edit boxes */
	sprintf(szTotal,"%2.2f",fTotal);
	sprintf(szGrandTotal,"%2.2f",fGrandTotal);

	SetDlgItemText(hMain,IDC_EDITTOTAL,szTotal);
	SetDlgItemText(hMain,IDC_EDITGRANDTOTAL,szGrandTotal);

	return;
}

void DisplayVLT(HWND hMain)
/*
** DisplayVLT()
** Displays the VLT based on the list price
*/
{
	char szFLP[256], szVLT[256];
	float fFLP = 0, fVLT = 0;

	GetDlgItemText(hMain,IDC_EDITFLP,szFLP,sizeof(szFLP));

	fFLP = atof(szFLP);

	/* VLT Fee is .0168 times the list price */
	fVLT = fFLP * .0168;

	sprintf(szVLT,"%2.2f",fVLT);

	SetDlgItemText(hMain,IDC_EDITVLT,szVLT);

}

void DisplayTRN(HWND hMain)
/*
** DisplayTRN()
** Displays the TRN fee(if applicable)
*/
{
	char szPCR[256];
	float fPCR = 0;

	GetDlgItemText(hMain,IDC_EDITPCR,szPCR,sizeof(szPCR));

	fPCR = atof(szPCR);

	if (fPCR != 0.00)
		SetDlgItemText(hMain,IDC_EDITTRN,"12.00");
	else
		SetDlgItemText(hMain,IDC_EDITTRN,"0.00");

}