#pragma once
#include <string>
#include <Windows.h>
#include "Resource.h"
#include <memory>

class TitleBar
{
private:
	int lastMouseX = -1;
	int lastMouseY = -1;
	std::string title;
	const std::string className = "lotlTitleBar";
	int InitTitleBar();
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	TitleBar();
	std::string getClassName();
};