#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("hideteamviewertoolbar");

void ShowTeamViewerToolBar(int nCmdShow)
{
	HWND hTeamViewer = FindWindow(L"ClientWindowSciter", 0);
	if (hTeamViewer) {
		HWND hChild = GetWindow(hTeamViewer, GW_CHILD);
		if (hChild) {
			HWND hTeamViewerToolBar = FindWindowEx(hChild, 0, 0, L"TV_CClientToolBar");
			if (hTeamViewerToolBar) {
				ShowWindow(hTeamViewerToolBar, nCmdShow);
			}
		}
	}
}

BOOL IsTeamViewerToolBarVisible()
{
	HWND hTeamViewer = FindWindow(L"ClientWindowSciter", 0);
	if (hTeamViewer) {
		HWND hChild = GetWindow(hTeamViewer, GW_CHILD);
		if (hChild) {
			HWND hTeamViewerToolBar = FindWindowEx(hChild, 0, 0, L"TV_CClientToolBar");
			if (hTeamViewerToolBar) {
				return IsWindowVisible(hTeamViewerToolBar);
			}
		}
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCheck;
	switch (msg)
	{
	case WM_CREATE:
		hCheck = CreateWindow(L"BUTTON", L"Show TeamVewer ToolBar", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_CHECKBOX, 10, 10, 256, 32, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		if (IsTeamViewerToolBarVisible()) {
			SendMessage(hCheck, BM_SETCHECK, BST_CHECKED, 0);
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			int nCheck = SendMessage(hCheck, BM_GETCHECK, 0, 0);
			if (nCheck == BST_CHECKED) {
				ShowTeamViewerToolBar(SW_HIDE);
				SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, 0);
			}
			else if (nCheck == BST_UNCHECKED) {
				ShowTeamViewerToolBar(SW_SHOW);
				SendMessage(hCheck, BM_SETCHECK, BST_CHECKED, 0);

			}
			PostMessage(hCheck, BM_SETCHECK, IsTeamViewerToolBarVisible() ? BST_CHECKED : BST_UNCHECKED, 0);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		0,
		WndProc,
		0,
		DLGWINDOWEXTRA,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		0,
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	RECT rect = {0,0, 276, 52};
	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN, FALSE);
	HWND hWnd = CreateWindow(
		szClassName,
		L"TeamViewer ToolBar Settings",
		WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
