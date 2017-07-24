// Tetris.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Tetris.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
HDC hdc, mdc, bufdc;
HBITMAP hTetri_i, hTetri_j, hTetri_l, hTetri_s, hTetri_o, hTetri_z, hTetri_t, hmap, hEmpty;
Tetris map[10][20];
BOOL move, over;
Tetri tetri;
DWORD tPre, tNow;

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID				Print(HDC);
VOID				InitGame();
VOID				DisplayTetri(Tetri&, Tetris);
BOOL				Moveable(Tetri&);
VOID				Move(Tetri&, bool);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

	MSG msg;

	// ����Ϣѭ��: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//else
		//{
		//	tNow = GetTickCount();
		//	if (tNow - tPre > 15)
		//	{
		//		Print(hdc);
		//	}
		//}
	}

	return (int)msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRIS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd, 400, 30, 510, 800, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	hmap = CreateCompatibleBitmap(hdc, 500, 800);

	hTetri_i = (HBITMAP)LoadImage(NULL, _T("bmp\\tetris_I.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hTetri_j = (HBITMAP)LoadImage(NULL, _T("bmp\\tetris_J.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hTetri_l = (HBITMAP)LoadImage(NULL, _T("bmp\\tetris_L.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hTetri_s = (HBITMAP)LoadImage(NULL, _T("bmp\\tetris_S.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hTetri_o = (HBITMAP)LoadImage(NULL, _T("bmp\\tetris_O.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hTetri_z = (HBITMAP)LoadImage(NULL, _T("bmp\\tetris_Z.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hTetri_t = (HBITMAP)LoadImage(NULL, _T("bmp\\tetris_T.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hEmpty = (HBITMAP)LoadImage(NULL, _T("bmp\\empty.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	SelectObject(mdc, hmap);
	InitGame();
	SetTimer(hWnd, 1, 350, NULL);
	Print(hdc);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case 'W': case VK_UP:
			if (!Moveable(tetri))
				break;
			tetri.Rotate();
			break;
		case 'A': case VK_LEFT:
			if (!Moveable(tetri))
				break;
			DisplayTetri(tetri, empty);
			Move(tetri, TRUE);
			break;
		case 'D': case VK_RIGHT:
			if (!Moveable(tetri))
				break;
			DisplayTetri(tetri, empty);
			Move(tetri, FALSE);
			break;
		default:
			break;
		}
	}
	break;
	case WM_TIMER:
	{
		//tNow = GetTickCount();
		if (!over)
		{
			if (!Moveable(tetri))
			{
				DWORD t = GetTickCount();
				srand(t);
				POINT pos;
				pos.x = 2;
				pos.y = 4;
//				tetri = Tetri(pos, static_cast<Tetris>(rand() % 7 + 1));
				tetri = Tetri(pos, tetri_i);
			}
			DisplayTetri(tetri, empty);
			tetri.Down();
			DisplayTetri(tetri, tetri.t);
			Print(hdc);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		EndPaint(hWnd, &ps);
		Print(hdc);
	}
	break;
	case WM_DESTROY:
	{
		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteObject(hTetri_i);
		DeleteObject(hTetri_j);
		DeleteObject(hTetri_l);
		DeleteObject(hTetri_s);
		DeleteObject(hTetri_o);
		DeleteObject(hTetri_z);
		DeleteObject(hTetri_t);
		DeleteObject(hmap);
		DeleteObject(hEmpty);
		ReleaseDC(hWnd, hdc);
		KillTimer(hWnd, 1);

		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID Print(HDC hdc)
{
	for (int i = 0; i < MAP_WID; ++i)
	{
		for (int j = 0; j < MAP_HIG; ++j)
		{
			switch (map[i][j])
			{
			case empty:
			{
				SelectObject(bufdc, hEmpty);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			case tetri_i:
			{
				SelectObject(bufdc, hTetri_i);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			case tetri_j:
			{
				SelectObject(bufdc, hTetri_j);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			case tetri_l:
			{
				SelectObject(bufdc, hTetri_l);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			case tetri_s:
			{
				SelectObject(bufdc, hTetri_s);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			case tetri_o:
			{
				SelectObject(bufdc, hTetri_o);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			case tetri_z:
			{
				SelectObject(bufdc, hTetri_z);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			case tetri_t:
			{
				SelectObject(bufdc, hTetri_t);
				BitBlt(mdc, i * 50, j * 50, 50, 50, bufdc, 0, 0, SRCCOPY);
			}
			break;
			default:
				break;
			}
		}

	}

	BitBlt(hdc, 0, 0, 500, 1000, mdc, 0, 0, SRCCOPY);

	//	if (!over)
	//	{
	////		SetBkMode(hdc, TRANSPARENT);
	//		TCHAR str[100];
	//		_stprintf(str, _T("center: %d, p[1]: %d, p[2]: %d, p[3]: %d"), tetri.center.y, tetri.p[1].y, tetri.p[2].y, tetri.p[3].y);
	//		TextOut(hdc, 200, 20, str, _tclen(str));
	//	}
	//
	tPre = GetTickCount();
}

VOID InitGame()
{
	for (int i = 0; i < MAP_WID; ++i)
		for (int j = 0; j < MAP_HIG; ++j)
			map[i][j] = empty;

	DWORD t = GetTickCount();
	srand(t);
	POINT pos;
	pos.x = 2;
	pos.y = 4;
	//	tetri = Tetri(pos, static_cast<Tetris>(rand() % 8));
	tetri = Tetri(pos, tetri_i);
	DisplayTetri(tetri, tetri.t);

	move = true;
	over = false;
}

VOID DisplayTetri(Tetri& diamon, Tetris style)
{
	if (INEDGE(diamon.center))
		map[diamon.center.x][diamon.center.y] = style;
	for (int i = 0; i < 3; ++i)
		if (INEDGE(diamon.p[i]))
			map[diamon.p[i].x][diamon.p[i].y] = style;
}

BOOL Moveable(Tetri& diamon)
{
	if (diamon.GetBottom() >= 14)
		return false;
	bool flag[4] = { FALSE, FALSE, FALSE, FALSE };
	for (int i = 0; i < 3; ++i)
	{
		if (tetri.center.y + 1 == tetri.p[i].y)
			flag[3] = TRUE;
	}
	if (!flag[3] && map[diamon.center.x][diamon.center.y + 1] != empty)
		return false;

	for (int i = 0; i < 3; ++i)
	{
		if ((tetri.p[i].y + 1) == tetri.center.y)
		{
			flag[i] = TRUE;
			continue;
		}
		for (int j = 0; j < 3; ++j)
		{
			if (j != i && (tetri.p[i].y + 1) == (tetri.p[j].y))
				flag[i] = TRUE;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if (!flag[i] && map[diamon.p[i].x][diamon.p[i].y + 1] != empty)
			return false;
	}

	return true;
}

VOID Move(Tetri& diamon,bool left)
{
	//�����ƶ�
	int LeftEdge = 0;
	int RightEdge = 9;
	if (left)
	{

		if (diamon.center.x == 0)
			return;
		--diamon.center.x;
		for (int i = 0; i < 3; ++i)
		{
			if (diamon.p[i].x == 0)
				return;
			--diamon.p[i].x;
		}
	}
	else
	{
		if (diamon.center.x == 9)
			return;
		++diamon.center.x;
		for (int i = 0; i < 3; ++i)
		{
			if (diamon.p[i].x == 9)
				return;
			++diamon.p[i].x;
		}
	}
}