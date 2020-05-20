#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>

HINSTANCE hInst;
LPCTSTR szWindowClass = "WindowClass";
LPCTSTR szTitle = "Title";

void MyRegisterClass(HINSTANCE hInstance);
void InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND inputTextBox;
HWND positiveCountTextBox;
std::string positiveCountString;
HWND negativeCountTextBox;
std::string negativeCountString;
HWND sumTextBox;
std::string sumString;
HWND averageTextBox;
std::string averageString;
HWND aboveAverageTextBox;
std::string aboveAverageString;

int GetPositiveCount(int source[], int count)
{
    __asm
    {
        mov ebx, [source]
        mov eax, 0
        mov ecx, 0
        start_loop:
        cmp ecx, [count]
        jge end
        cmp DWORD PTR [ebx + ecx * 4], 0
        jle skipIncrement
        inc eax
        skipIncrement:
        inc ecx
        jmp start_loop
        end:
    }
}

int GetNegativeCount(int source[], int count)
{
    __asm
    {
        mov ebx, [source]
        mov eax, 0
        mov ecx, 0
        start_loop:
        cmp ecx, [count]
        jge end
        cmp DWORD PTR [ebx + ecx * 4], 0
        jge skipIncrement
        inc eax
        skipIncrement:
        inc ecx
        jmp start_loop
        end:
    }
}

int GetSum(int source[], int count)
{
    __asm
    {
        mov ebx, [source]
        mov eax, 0
        mov ecx, 0
        start_loop:
        cmp ecx, [count]
        jge end
        add eax, DWORD PTR [ebx + ecx * 4]
        inc ecx
        jmp start_loop
        end:
    }
}

int GetAverage(int source[], int count)
{
    __asm
    {
        mov eax, [count]
        push eax
        mov eax, [source]
        push eax
        call GetSum
        pop ebx
        pop ebx
        cmp [count], 0
        je returnZero
        cdq
        idiv DWORD PTR [count]
        jmp end
        returnZero:
        mov eax, 0
        end:
    }
}

int GetAboveAverageCount(int source[], int count)
{
    __asm
    {
        mov eax, [count]
        push eax
        mov eax, [source]
        push eax
        call GetAverage
        pop ebx
        pop ebx
        mov edx, eax
        mov eax, 0

        mov ebx, [source]
        mov ecx, 0
        start_loop:
        cmp ecx, [count]
        jge end
        cmp DWORD PTR [ebx + ecx * 4], edx
        jle skipIncrement
        inc eax
        skipIncrement:
        inc ecx
        jmp start_loop
        end:
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

std::vector<std::string> SplitStringBySpace(std::string source)
{
    std::vector<std::string> words;
    std::string currentWord;
    for (size_t i = 0; i < source.size(); i++)
    {
        if (source[i] != ' ')
        {
            currentWord += source[i];
        }
        else
        {
            if (currentWord.length() != 0)
            {
                words.push_back(currentWord);
                currentWord = "";
            }
        }
    }
    if (currentWord.length() != 0)
    {
        words.push_back(currentWord);
        currentWord = "";
    }
    return words;
}

bool ValidateWordsAreNumbers(std::vector<std::string> source)
{
    for (size_t wordIndex = 0; wordIndex < source.size(); wordIndex++)
    {
        for (size_t charIndex = 0; charIndex < source[wordIndex].length(); charIndex++)
        {
            char c = source[wordIndex][charIndex];
            if (!(c >= '0' && c <= '9') && !(c == '-' && charIndex == 0 && source[wordIndex].length() > 1))
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<int> ParseWordsToInts(std::vector<std::string> source)
{
    std::vector<int> ints;
    for (size_t i = 0; i < source.size(); i++)
    {
        ints.push_back(std::atoi(source[i].c_str()));
    }
    return ints;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            inputTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
                90, 10, 200, 25, hWnd, NULL, hInst, NULL);
            SetWindowText(inputTextBox, "1 2 3 4 5 -6 -7 -8 -9 -10");
            positiveCountTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_DISABLED,
                90, 40, 200, 25, hWnd, NULL, hInst, NULL);
            negativeCountTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_DISABLED,
                90, 70, 200, 25, hWnd, NULL, hInst, NULL);
            sumTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_DISABLED,
                90, 100, 200, 25, hWnd, NULL, hInst, NULL);
            averageTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_DISABLED,
                90, 130, 200, 25, hWnd, NULL, hInst, NULL);
            aboveAverageTextBox = CreateWindowEx(NULL, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_DISABLED,
                90, 160, 200, 25, hWnd, NULL, hInst, NULL);

            break;
        }
        case WM_COMMAND:
        {
            HWND control = (HWND)lParam;
            int message = HIWORD(wParam);
            if (control == inputTextBox && message == WM_USER)
            {
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paintStruct;
            HDC hdc = BeginPaint(hWnd, &paintStruct);

            char inputTitle[] = "Input:";
            TextOut(hdc, 5, 15, inputTitle, sizeof(inputTitle));
            char positiveCountText[] = "Positive:";
            TextOut(hdc, 5, 45, positiveCountText, sizeof(positiveCountText));
            char negativeCountText[] = "Negative:";
            TextOut(hdc, 5, 75, negativeCountText, sizeof(negativeCountText));
            char sumText[] = "Sum:";
            TextOut(hdc, 5, 105, sumText, sizeof(sumText));
            char averageText[] = "Average:";
            TextOut(hdc, 5, 135, averageText, sizeof(averageText));
            char aboveAverageText[] = "Above avg.:";
            TextOut(hdc, 5, 165, aboveAverageText, sizeof(aboveAverageText));

            char inputRawText[256];
            auto arrayString = GetWindowText(inputTextBox, inputRawText, 256);

            std::string inputText(inputRawText);

            auto words = SplitStringBySpace(inputText);
            if (ValidateWordsAreNumbers(words))
            {
                auto ints = ParseWordsToInts(words);

                auto positiveCount = GetPositiveCount(ints.data(), ints.size());
                positiveCountString = std::to_string(positiveCount);
                SetWindowText(positiveCountTextBox, positiveCountString.c_str());

                auto negativeCount = GetNegativeCount(ints.data(), ints.size());
                negativeCountString = std::to_string(negativeCount);
                SetWindowText(negativeCountTextBox, negativeCountString.c_str());

                auto sum = GetSum(ints.data(), ints.size());
                sumString = std::to_string(sum);
                SetWindowText(sumTextBox, sumString.c_str());

                auto average = GetAverage(ints.data(), ints.size());
                averageString = std::to_string(average);
                SetWindowText(averageTextBox, averageString.c_str());

                auto aboveAverage = GetAboveAverageCount(ints.data(), ints.size());
                aboveAverageString = std::to_string(aboveAverage);
                SetWindowText(aboveAverageTextBox, aboveAverageString.c_str());
            }
            else
            {
                char warning[] = "This is not a valid array of integers";
                SetTextColor(hdc, RGB(255, 0, 0));
                TextOut(hdc, 275, 15, warning, sizeof(warning));
            }

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
