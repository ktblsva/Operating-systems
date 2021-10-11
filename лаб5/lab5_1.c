//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <windows.h>

LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);
HDC hdc;
RECT rt;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd; 
    MSG msg; 
    WNDCLASS wc; 
    BOOL ret;
    LPCSTR lpszAppName = "CTemplate1";
    wc.lpszClassName = lpszAppName;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = (WNDPROC)MyWndProc; 
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(NULL, IDC_HELP);
    wc.lpszMenuName = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    if(!RegisterClass(&wc))
    {
        return 0;
    }
    
    hWnd = CreateWindow(lpszAppName, lpszAppName,
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, 100, 100, 400, 200, NULL, NULL, hInstance, NULL);
    ret = RegisterHotKey(hWnd, 0xB001, MOD_CONTROL|MOD_ALT, 'W');
    ShowWindow(hWnd, nCmdShow); 
    UpdateWindow(hWnd);          
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
    HBRUSH hBrush;
    PAINTSTRUCT ps;
    switch (Message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_HOTKEY:
        ShowWindow(hWnd, SW_SHOWNORMAL);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rt);
        DrawText(hdc, "From Paint", strlen("From Paint"), &rt, DT_CENTER);
        EndPaint(hWnd, &ps);
        break;
    case WM_COPYDATA:
        GetClientRect(hWnd, &rt);
        hdc = GetDC(hWnd);
        DrawText(hdc, (char*)((COPYDATASTRUCT*)lparam)->lpData, ((COPYDATASTRUCT*)lparam)->cbData, &rt, DT_LEFT);
        ReleaseDC(hWnd, hdc);
        break;

    default:
        return DefWindowProc(hWnd, Message, wparam, lparam);
    }
    return 0;
}