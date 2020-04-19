#include <windows.h>
#include <stdio.h>

HINSTANCE hInst;
LPCTSTR szWindowClass = "WindowClass";
LPCTSTR szTitle = "Title";

void MyRegisterClass(HINSTANCE hInstance);
void InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND inputTextBox;
HWND positiveCountTextBox;

int GetPositiveCount(int source[], int count)
{
    __asm
    {
        mov eax, 10
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MyRegisterClass(hInstance);
    InitInstance(hInstance, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
} 

void MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_HAND);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;

    RegisterClassEx(&wcex);
}

void InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            inputTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
                45, 10, 200, 25, hWnd, NULL, hInst, NULL);
            SetWindowText(inputTextBox, "1 2 3 4 5 -6 -7 -8 -9 -10");
            positiveCountTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_DISABLED,
                45, 40, 200, 25, hWnd, NULL, hInst, NULL);

            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paintStruct;
            HDC hdc = BeginPaint(hWnd, &paintStruct);

            char mousePositionTitle[] = "Input:";
            TextOut(hdc, 5, 15, mousePositionTitle, sizeof(mousePositionTitle));
            char positiveCountText[] = "Input:";
            TextOut(hdc, 5, 15, mousePositionTitle, sizeof(mousePositionTitle));

            EndPaint(hWnd, &paintStruct);

            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
