#include "Titlebar.h"

LRESULT CALLBACK TitleBar::wndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{
	case WM_CREATE:
	{
		RECT winRect = { 0,0,0,0 };
		GetWindowRect(hwnd, &winRect);
		CreateWindowEx(0, "BUTTON", "close", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW, winRect.right - winRect.left - 62, 0, 52, 20, hwnd, reinterpret_cast<HMENU>(IDC_CLOSE), nullptr, nullptr);

	}
		break;
	case WM_LBUTTONDOWN:
		SetWindowPos(GetParent(hwnd), nullptr, 10, 10, 0, 0, SWP_NOSIZE);
		break;
	case WM_PAINT:
	{
		DefWindowProc(hwnd, umsg, wParam, lParam);
		HDC hdc = GetDC(hwnd);
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, static_cast<PVOID>(&ncm), 0);
		HFONT titleFont = CreateFontIndirect(&ncm.lfCaptionFont);
		SelectObject(hdc, titleFont);
		SetBkMode(hdc, TRANSPARENT);
		RECT areaRect = { 0,0,0,0 };
		GetWindowRect(hwnd, &areaRect);
		int width = areaRect.right - areaRect.left;
		int height = areaRect.bottom - areaRect.top;
		areaRect.left = 0;
		areaRect.top = 0;
		areaRect.right = width;
		areaRect.bottom = height;
		FillRect(hdc, &areaRect, CreateSolidBrush(RGB(180, 220, 255)));
		TRIVERTEX tri[2];
		tri[0].x = areaRect.left;
		tri[0].y = areaRect.top;
		tri[0].Red = 0x6060;
		tri[0].Green = 0x9090;
		tri[0].Blue = 0xf0f0;
		tri[0].Alpha = 0xffff;
		tri[1].x = areaRect.right;
		tri[1].y = areaRect.bottom;
		tri[1].Red = 0xc0c0;
		tri[1].Green = 0xe0e0;
		tri[1].Blue = 0xffff;
		tri[1].Alpha = 0xffff;
		GRADIENT_RECT r;
		r.LowerRight = 1;
		r.UpperLeft = 0;
		GradientFill(hdc, tri, 2, &r, 1, GRADIENT_FILL_RECT_H);
		char titleText[512];
		SetTextColor(hdc, RGB(255, 255, 255));
		GetWindowText(hwnd, titleText, 511);
		DrawText(hdc, titleText, -1, &areaRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
		return 0;
		break;
	case WM_DRAWITEM:
		switch (wParam)
		{
		case IDC_CLOSE:
			NONCLIENTMETRICS ncm;
			ncm.cbSize = sizeof(NONCLIENTMETRICS);
			HFONT font;
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, static_cast<PVOID>(&ncm), 0);
			LPDRAWITEMSTRUCT lpDIS = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
			SetBkMode(lpDIS->hDC, TRANSPARENT);
			font = CreateFontIndirect(&ncm.lfCaptionFont);
			SelectObject(lpDIS->hDC, font);
			SetTextColor(lpDIS->hDC, RGB(255, 255, 255));
			TRIVERTEX tri[2];
			tri[0].x = lpDIS->rcItem.left;
			tri[0].y = lpDIS->rcItem.top;
			tri[0].Red = 0xffff;
			tri[0].Green = 0xa0a0;
			tri[0].Blue = 0xa0a0;
			tri[0].Alpha = 0xffff;
			tri[1].x = lpDIS->rcItem.right;
			tri[1].y = lpDIS->rcItem.bottom;
			tri[1].Red = 0xa0a0;
			tri[1].Green = 0x0000;
			tri[1].Blue = 0x0000;
			tri[1].Alpha = 0xffff;
			GRADIENT_RECT r;
			r.UpperLeft = 0;
			r.LowerRight = 1;
			GradientFill(lpDIS->hDC, tri, 2, &r, 1, GRADIENT_FILL_RECT_V);
			char btnText[32];
			GetWindowTextA(lpDIS->hwndItem, btnText, 31);
			DrawText(lpDIS->hDC, btnText, -1, &lpDIS->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CLOSE:
			SendMessage(GetParent(hwnd), WM_CLOSE, 0,0);
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, umsg, wParam, lParam);
}

int TitleBar::InitTitleBar()
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(wc);
	wc.lpszClassName = className.c_str();
	wc.hInstance = GetModuleHandle(0);
	wc.lpfnWndProc = wndProc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = 0;
	wc.hbrBackground = static_cast<HBRUSH>(GetSysColorBrush(COLOR_WINDOWFRAME));
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIconSm = 0;

	RegisterClassEx(&wc);

	return 0;
}

TitleBar::TitleBar()
{
	InitTitleBar();
}

std::string TitleBar::getClassName()
{
	return className;
}