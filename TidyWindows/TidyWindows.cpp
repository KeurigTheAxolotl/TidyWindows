#include <iostream>
#include <string>
#include <Windows.h>
#include "WindowCenter.h"
#include "Resource.h"
#include "Titlebar.h"
#include <dwmapi.h>

LRESULT CALLBACK wndProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_CREATE:
		// Create decorative controls (labels etc)
	{

		// Enable blur behind

		/* DWM_BLURBEHIND bb = { 0 };

		bb.dwFlags = DWM_BB_ENABLE;
		bb.fEnable = true;
		bb.hRgnBlur = nullptr;

		DwmEnableBlurBehindWindow(hwnd, &bb); */

		TitleBar tb{};
		
		CreateWindowEx(0, tb.getClassName().c_str(), "TidyWindows", WS_CHILD | WS_VISIBLE, 2, 2, 246, 26, hwnd, nullptr, nullptr, nullptr);
		CreateWindowEx(0, "STATIC", "Shortcut: Ctrl + Shift + ", WS_CHILD | WS_VISIBLE, 10, 42, 170, 16, hwnd, nullptr, nullptr, nullptr);
		CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_VISIBLE , 180, 42, 50, 16, hwnd, nullptr, nullptr, nullptr);
		CreateWindowEx(0, "BUTTON", "Set Shortcut", WS_CHILD | WS_VISIBLE | WS_DISABLED, 10, 64, 230, 32, hwnd, reinterpret_cast<HMENU>(IDC_SET_SHORTCUT), nullptr, nullptr);
		CreateWindowEx(0, "BUTTON", "Hide", WS_CHILD | WS_VISIBLE, 10, 108, 230, 32, hwnd, reinterpret_cast<HMENU>(IDC_HIDE), nullptr, nullptr);

		

		if (!RegisterHotKey(hwnd, HK_SHOW, MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, 'X') || !RegisterHotKey(hwnd, HK_CENTER, MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, 'C'))
		{
			MessageBox(0, "Failed to register hot keys.", "Unable to start", MB_OK);
		}

		/*NOTIFYICONDATA nid;
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = hwnd;
		nid.uID = 1;
		nid.uFlags = NIF_ICON;                                                                                                                                                                                                                                                                                                                                                                                      
		nid.hIcon = LoadIcon(0, IDI_ERROR);
		nid.uCallbackMessage = 0;
		nid.dwState = 0;
		nid.dwStateMask = 0;

		Shell_NotifyIcon(NIM_ADD, &nid);*/
		
	}

		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_HIDE:
			ShowWindow(hwnd, SW_HIDE);
			return(0);
		case IDC_SET_SHORTCUT:
			ShowWindow(hwnd, SW_HIDE);
			ShowWindow(hwnd, SW_HIDE);
			MessageBox(0, "This feature is not available yet.", "Not Implemented", MB_OK | MB_ICONINFORMATION);
			ShowWindow(hwnd, SW_SHOW);
			ShowWindow(hwnd, SW_SHOW);
			return 0;
		case IDC_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		}
		return 0;
	case WM_HOTKEY:
		switch (wParam)
		{
		case HK_SHOW:
			ShowWindow(hwnd, SW_SHOW);
			return 0;
		case HK_CENTER:
			WindowCenter wc;
			wc.centerForegroundWindow();
			return 0;
		}
		return 0;
	case WM_DESTROY:
		UnregisterHotKey(hwnd, HK_CENTER);
		UnregisterHotKey(hwnd, HK_SHOW);
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}


}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	const char* clsName = "tidywin";

	HWND hwnd;
	WNDCLASSEX wndClass;

	wndClass.style = 0;
	wndClass.lpfnWndProc = wndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	wndClass.lpszMenuName = 0;
	wndClass.lpszClassName = clsName;
	wndClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wndClass.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wndClass))
	{
		DWORD error = GetLastError();
		std::string eMsg = "Error code " + std::to_string(error); // TODO: Get the error text.
		MessageBox(0, eMsg.c_str(), "Window Creation Failure", MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_TOPMOST,
		clsName,
		"TidyWin",
		WS_POPUP | WS_SYSMENU,
		10,
		10,
		250,
		150,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (hwnd == nullptr)
	{
		DWORD error = GetLastError();
		std::string eMsg = "Error code " + std::to_string(error); // TODO: Get the error text.
		MessageBox(0, eMsg.c_str(), "Window Creation Failure", MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);



	MSG msg = {0};

	while (GetMessage(&msg, nullptr, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}