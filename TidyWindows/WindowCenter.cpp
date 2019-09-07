#include "WindowCenter.h"
#include <iostream>
#include <string>

void WindowCenter::centerWindow(HWND hwnd)
{
	// Rect Structures

	RECT screenSize;
	RECT winRect;
	RECT newWinRect = { 0,0,0,0 };

	// Try to get the specified window's bounds.

	if (!GetWindowRect(hwnd, &winRect))
	{
		MessageBox(0, "Unable to detect foreground window.", "Error Centering Window", MB_OK);
		return;
	}

	// Try to get the work area (this excludes the taskbar) 

	if (!SystemParametersInfo(SPI_GETWORKAREA, NULL, &screenSize, 0))
	{
		MessageBox(0, "Unable to detect the work area using SPI_GETWORKAREA.", "Error Centering Window", MB_OK);
		return;
	}

	// Calculate the rectangle necessary.

	newWinRect.left = screenSize.right / 2 - (winRect.right - winRect.left) / 2;
	newWinRect.top = screenSize.bottom / 2 - (winRect.bottom - winRect.top) / 2;
	newWinRect.right = 0;		// SWP_NOSIZE makes these two parameters unnecessary.
	newWinRect.bottom = 0;

	// Attempt to set the window's new position.

	if (!SetWindowPos(hwnd, HWND_TOP, newWinRect.left, newWinRect.top, 0, 0, SWP_NOSIZE))
	{
		// Cry
		MessageBox(0, "Failed to set the position of the window.", "Error Centering Window", MB_OK);
		return;
	}
}

void WindowCenter::centerForegroundWindow()
{
	HWND win = GetForegroundWindow();
	centerWindow(win);
}

WindowCenter::WindowCenter()
{
	activatorKey = 'c';
}