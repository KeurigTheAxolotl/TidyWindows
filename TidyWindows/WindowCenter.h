#include <Windows.h>
#pragma once

class WindowCenter
{
private:
	char activatorKey;
public:
	void centerForegroundWindow();
	void centerWindow(HWND hwnd);
	WindowCenter();
};