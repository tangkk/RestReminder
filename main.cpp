// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include <windows.h>

#define RESTPERIOD 2400000 //40 minutes
//#define RESTPERIOD 10000 //10 seconds

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                                  // Optional window styles.
        CLASS_NAME,                         // Window class
        L"Learn to Program for Windows",    // Window text
        WS_OVERLAPPEDWINDOW,                // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    //ShowWindow(hwnd, nCmdShow);
	ShowWindow(hwnd, SW_HIDE);

	const wchar_t *tip = L"Reminder";
	NOTIFYICONDATA nid;
	nid.hWnd = hwnd;
	nid.uCallbackMessage=WM_MOUSEMOVE;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.hIcon=LoadIconW(NULL, MAKEINTRESOURCE(IDI_WARNING));  
	lstrcpy(nid.szTip, tip);


	Shell_NotifyIcon(NIM_ADD, &nid);

	SetTimer(hwnd,
		0,
		RESTPERIOD,
		(TIMERPROC) NULL);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
		KillTimer(hwnd, 0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

	case WM_TIMER:
		{
			KillTimer(hwnd, 0);
			MessageBox(hwnd, L"40 minitues already! Time to take a rest!", L"RestReminder", MB_ICONWARNING|MB_SYSTEMMODAL);
			SetTimer(hwnd,
				0,
				RESTPERIOD,
				(TIMERPROC) NULL);
		}
		return 0;

	case WM_MOUSEMOVE:
		{
			switch(lParam)
			{
				case WM_LBUTTONDOWN:
					{
						int MsgID = MessageBox(hwnd, L"Rest the Timer?", L"RestReminder",
							MB_OKCANCEL|MB_TOPMOST);
						switch(MsgID) 
							{
								case IDOK:
									KillTimer(hwnd, 0);
									SetTimer(hwnd,
										0,
										RESTPERIOD,
										(TIMERPROC) NULL);
									break;
								case IDCANCEL:
									break;
								default:
									break;
							}
						break;
					}
				case WM_RBUTTONDOWN:
					{
						PostQuitMessage(0);
						KillTimer(hwnd, 0);
						break;
					}
			}
		}
		return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

 