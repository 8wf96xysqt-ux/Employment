#include "framework.h"
#include "ProjectBotLauncher.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <shellapi.h>
#include <fstream>
#include <string>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ULONG_PTR gdiToken;
Image* launcherImage = nullptr;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiToken, &gdiplusStartupInput, nullptr);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECTBOTLAUNCHER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTBOTLAUNCHER));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    delete launcherImage;
    GdiplusShutdown(gdiToken);

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));


    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW
    (
        szWindowClass,
        L"Project Bot Launcher",
        WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        700,
        500,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );


    if (!hWnd)
        return FALSE;

    launcherImage = new Image(L"E:\\Employment\\3DGAME\\x64\\Release\\Data\\Launcher\\launcher.png");

    if (launcherImage->GetLastStatus() != Ok)
    {
        MessageBoxW(hWnd, L"画像を読み込めません", L"エラー", MB_OK);
    }

    CreateWindowW
    (
        L"STATIC",
        L"ウィンドウサイズ",
        WS_CHILD | WS_VISIBLE,
        380, 60, 200, 30,
        hWnd, nullptr, hInstance, nullptr
    );

    HWND hResolution = CreateWindowW
    (
        L"COMBOBOX",
        nullptr,
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        380, 90, 250, 100,
        hWnd, (HMENU)1001, hInstance, nullptr
    );

    SendMessageW(hResolution, CB_ADDSTRING, 0, (LPARAM)L"1280 x 720");
    SendMessageW(hResolution, CB_ADDSTRING, 0, (LPARAM)L"1600 x 900");
    SendMessageW(hResolution, CB_ADDSTRING, 0, (LPARAM)L"1920 x 1080");
    SendMessageW(hResolution, CB_SETCURSEL, 1, 0);

    CreateWindowW
    (
        L"STATIC",
        L"画面モード",
        WS_CHILD | WS_VISIBLE,
        380, 170, 200, 30,
        hWnd, nullptr, hInstance, nullptr
    );

    HWND hDisplayMode = CreateWindowW
    (
        L"COMBOBOX",
        nullptr,
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        380, 200, 250, 100,
        hWnd, (HMENU)1003, hInstance, nullptr
    );

    SendMessageW(hDisplayMode, CB_ADDSTRING, 0, (LPARAM)L"ウィンドウ");
    SendMessageW(hDisplayMode, CB_ADDSTRING, 0, (LPARAM)L"フルスクリーン");
    SendMessageW(hDisplayMode, CB_SETCURSEL, 0, 0);

    CreateWindowW
    (
        L"BUTTON",
        L"ゲームスタート",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        380, 280, 160, 50,
        hWnd, (HMENU)1002, hInstance, nullptr
    );

    CreateWindowW
    (
        L"BUTTON",
        L"終了",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        550, 280, 80, 50,
        hWnd, (HMENU)1004, hInstance, nullptr
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1002:
        {
            HWND hResolution = GetDlgItem(hWnd, 1001);
            HWND hDisplayMode = GetDlgItem(hWnd, 1003);

            int resolution = (int)SendMessageW(hResolution, CB_GETCURSEL, 0, 0);
            int displayMode = (int)SendMessageW(hDisplayMode, CB_GETCURSEL, 0, 0);

            int width = 1280;
            int height = 720;

            if (resolution == 1)
            {
                width = 1600;
                height = 900;
            }
            else if (resolution == 2)
            {
                width = 1920;
                height = 1080;
            }

            int fullscreen = (displayMode == 1) ? 1 : 0;

            std::wofstream ini(L"E:\\Employment\\3DGAME\\x64\\Release\\Data\\Launcher\\config.ini");


            if (ini)
            {
                ini << L"[Display]\n";
                ini << L"Width=" << width << L"\n";
                ini << L"Height=" << height << L"\n";
                ini << L"Fullscreen=" << fullscreen << L"\n";
                ini.close();
            }

            ShellExecuteW
            (
                nullptr,
                L"open",
                L"E:\\Employment\\3DGAME\\x64\\Release\\3DGAME.exe",
                nullptr,
                L"E:\\Employment\\3DGAME\\x64\\Release",
                SW_SHOWNORMAL
            );


            DestroyWindow(hWnd);
            break;
        }

        case 1004:
            DestroyWindow(hWnd);
            break;

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        if (launcherImage)
        {
            Graphics graphics(hdc);
            graphics.DrawImage(launcherImage, 30, 30, 300, 400);
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

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
