#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const int columnNumber = 6;
const int rowNumber = 4;

const int MAX_STRINGS = 14;

const int offset = 5;

const wchar_t text[MAX_STRINGS][255] = {
	L"Мой дзядзька правіл бездаганы, калі няжарта занямог, усіх прымусіў дапашаны, ды лепей выдумаць не мог.",
	L"Другіх той прыклад навучае: ды Божа мой, нуда якая - пры хворым дзень і ноч сядзець.",
	L"Не адыходзіцца, нудзець!",
	L"Прытворства нізкае якое напоўжывога забаўляць!.",
	L"Ды лекі падаваць с тугою і моўчкі думаць сам сабе: калі ўжо возьме чорт цябе.",
	L"Тут стане горад з бегам дзён.",
	L"Назло пышліваму суседу.",
	L"Прыродай суджана даўно,",
	L"У Еўропу высяч тут акно.",
	L"Сюды праз новыя шляхі, ўсе у госці прыплывуць сцягі.",
	L"І забалюем на прасторы!",
	L"Лагодна Украінская ноч,",
	L"Празрыста неба, ззоры ззяюць",
	L"Паветра веяк, сооных ноч, раскрыць ня хочуць, чуць хістаюць лістём таполі ў цішыні!"
};

HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
HFONT hFont = CreateFont(20, 6.7 , 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
	                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_ROMAN, (LPCWSTR)"Arial");

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawTable(HDC hDC, int windowWidth, int windowHeight);
void InitDC(HWND hWnd, int windowWidth, int windowHeight);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"SecondLab";
	wcex.hIconSm = wcex.hIcon;

	if (!RegisterClassEx(&wcex))
	{
		return 0;
	}

	hWnd = CreateWindowEx(0, L"SecondLab", L"Second lab", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int width, height;

	switch (uMsg)
	{
	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		break;
	case WM_PAINT:
		InitDC(hWnd, width, height);
		break;
	case WM_DESTROY:
		DeleteObject(hPen);
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DrawTable(HDC hDC, int windowWidth, int windowHeight)
{
	int columnWidth = windowWidth / columnNumber;

	int i = 0;
	int textHeight = 0;
	while (i < MAX_STRINGS)
	{
		RECT rect = { 0, 0, 0, 0 };
		rect.top += textHeight;

		int maxTextHeight = 0;
		for (int j = 0; (j < columnNumber) && (i < MAX_STRINGS); j++)
		{
			const wchar_t* str = text[i];
			i++;
			
			rect.left = j * columnWidth + offset;
			rect.right = (j + 1) * columnWidth - offset;
			
			int currentTextHeight = DrawText(hDC, str, wcslen(str), &rect, DT_VCENTER | DT_EDITCONTROL | DT_WORDBREAK | DT_NOCLIP);
			if (maxTextHeight < currentTextHeight)
				maxTextHeight = currentTextHeight;
		}
		textHeight += maxTextHeight;
	}
}

void InitDC(HWND hWnd, int windowWidth, int windowHeight)
{
	RECT rect;
	HDC memDC;
	HBITMAP hBmp, hOldBmp;
	PAINTSTRUCT ps;

	HDC currDC = BeginPaint(hWnd, &ps);

	GetClientRect(hWnd, &rect);
	memDC = CreateCompatibleDC(currDC);
	hBmp = CreateCompatibleBitmap(currDC, rect.right - rect.left, rect.bottom - rect.top);
	hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);

	HBRUSH hBkgrndBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(memDC, &rect, hBkgrndBrush);
	DeleteObject(hBkgrndBrush);

	SetBkMode(memDC, TRANSPARENT);

	SelectObject(memDC, hPen);
	SetTextColor(memDC, RGB(0, 0, 0));

	SelectObject(memDC, hFont);

	DrawTable(memDC, rect.right - rect.left, rect.bottom - rect.top);
	BitBlt(currDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, hOldBmp);

	DeleteDC(memDC);

	EndPaint(hWnd, &ps);
}