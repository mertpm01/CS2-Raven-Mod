#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

#include "classes/utils.h"
#include "memory/memory.hpp"
#include "classes/vector.hpp"
#include "hacks/reader.hpp"
#include "hacks/hack.hpp"
#include "classes/globals.hpp"
#include "classes/render.hpp"
#include "classes/auto_updater.hpp"
#include "classes/menu.hpp"

bool finish = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		g::hdcBuffer = CreateCompatibleDC(NULL);
		g::hbmBuffer = CreateCompatibleBitmap(GetDC(hWnd), g::gameBounds.right, g::gameBounds.bottom);
		SelectObject(g::hdcBuffer, g::hbmBuffer);

		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (menu::is_open) {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			
			menu::handle_mouse_down(x, y);
			
			return 0;
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		if (menu::is_open) {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			
			menu::handle_mouse_up(x, y);
			menu::handle_mouse_click(x, y);
			
			return 0;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (menu::is_open) {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			
			menu::handle_mouse_move(x, y);
			
			return 0;
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (menu::is_open) {
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			menu::handle_mouse_wheel(delta);
			return 0;
		}
		break;
	}
	case WM_ERASEBKGND: // We handle this message to avoid flickering
		return TRUE;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//DOUBLE BUFFERING
		// Always fill with white (for transparent)
		FillRect(g::hdcBuffer, &ps.rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// Render ESP (even if menu is open)
		if (GetForegroundWindow() == g_game.process->hwnd_) {
			hack::loop();
		}
		
		// Render menu (on top, over ESP)
		menu::render(g::hdcBuffer);

		BitBlt(hdc, 0, 0, g::gameBounds.right, g::gameBounds.bottom, g::hdcBuffer, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		DeleteDC(g::hdcBuffer);
		DeleteObject(g::hbmBuffer);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void read_thread() {
	while (!finish) {
		g_game.loop();
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Konsolu tamamen kapat
	FreeConsole();
	
	utils.update_console_title();
	if (config::read()) {
		// Config loaded
	}
	else {
		// Config reset
	}

#ifndef _UC
	try {
		updater::check_and_update(config::automatic_update);
	}
	catch (std::exception& e) {
		// Error
	}
#endif

	if (updater::read()) {
		// Offsets loaded
	}
	else {
		// Offsets reset
	}

	g_game.init();

	if (g_game.buildNumber != updater::build_number) {
		// Build mismatch - wait for user
		MessageBoxA(NULL, "Build number mismatch! ESP may not work.\nPress OK to continue.", "CS2 ESP Warning", MB_OK | MB_ICONWARNING);
	}

	// Wait for game window
	while (GetForegroundWindow() != g_game.process->hwnd_) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		g_game.process->UpdateHWND();
		ShowWindow(g_game.process->hwnd_, TRUE);
	}

	WNDCLASSEXA wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = WHITE_BRUSH;
	wc.hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongA(g_game.process->hwnd_, (-6))); // GWL_HINSTANCE));
	wc.lpszMenuName = " ";
	wc.lpszClassName = " ";

	RegisterClassExA(&wc);

	GetClientRect(g_game.process->hwnd_, &g::gameBounds);

	// Create the window - use hInstance from WinMain
	HWND hWnd = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, " ", "cs2-external-esp", WS_POPUP,
		g::gameBounds.left, g::gameBounds.top, g::gameBounds.right - g::gameBounds.left, g::gameBounds.bottom + g::gameBounds.left, NULL, NULL, hInstance, NULL); // NULL, NULL);
	
	g::overlayHwnd = hWnd;

	if (hWnd == NULL)
		return 0;

	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(hWnd, TRUE);
	
	// Initialize menu
	menu::init();

	// Launch game memory reading thread
	std::thread read(read_thread);

	// Message loop
	MSG msg;
	while (!finish)
	{
		// PeekMessage kullan (non-blocking)
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) {
				finish = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (GetAsyncKeyState(VK_END) & 0x8000) finish = true;
		
		// DELETE key to open/close menu
		static DWORD last_menu_press = 0;
		static bool menu_was_pressed = false;
		
		bool menu_is_pressed = (GetAsyncKeyState(VK_DELETE) & 0x8000) != 0;
		
		if (menu_is_pressed && !menu_was_pressed) {
			DWORD now = GetTickCount();
			if (now - last_menu_press > 200) {
				menu::toggle();
				last_menu_press = now;
			}
		}
		menu_was_pressed = menu_is_pressed;

		// Manual render trigger - IMPORTANT!
		InvalidateRect(hWnd, NULL, FALSE);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	read.detach();

	DeleteDC(g::hdcBuffer);
	DeleteObject(g::hbmBuffer);

	DestroyWindow(hWnd);

	g_game.close();

	return 1;
}
