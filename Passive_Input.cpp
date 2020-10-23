// Passive_Input.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Passive_Input.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PASSIVEINPUT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PASSIVEINPUT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PASSIVEINPUT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PASSIVEINPUT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
char keyboard_state[256];  //키보드의 상태(키보드 256개 안넘어감)
int mouseX, mouseY;  //마우스 좌표
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
          
            TCHAR buffer[100];
            wsprintf(buffer, _T("mouse %d %d"), mouseX, mouseY); //마우스의 x,y좌표 출력
            TextOut(hdc, 0, 130, buffer, lstrlen(buffer));
            //TextOut : 문자열 출력 buffer -> 문자열 , lstrlen(buffer) -> 문자열의 크기,,?
            if (keyboard_state[VK_LEFT] == 1) //왼쪽화살표의 상태가 1 ==> 왼쪽 화살표가 눌러짐
            {
                TextOut(hdc, 0, 45, _T("왼쪽 화살표"), lstrlen(_T("화살표")));//출력 "왼쪽화살표"
            }
            if (keyboard_state[VK_UP] == 1)//위쪽화살표의 상태가 1 ==> 위쪽 화살표가 눌러짐
            {
                TextOut(hdc, 100, 0, _T("위쪽 화살표"), lstrlen(_T("화살표")));
            }
            if (keyboard_state[VK_RIGHT] == 1)//오른쪽화살표의 상태가 1 ==> 오른쪽 화살표가 눌러짐
            {
                TextOut(hdc,200, 45, _T("오른쪽 화살표"), lstrlen(_T("화살표")));
            }
            if (keyboard_state[VK_DOWN] == 1)//아래쪽화살표의 상태가 1 ==> 아래쪽 화살표가 눌러짐
            {
                TextOut(hdc, 100, 90, _T("아래 화살표"), lstrlen(_T("화살표")));
            }

            EndPaint(hWnd, &ps);
        }
        break;
    //wParam : 키값이 눌러졌는지 상태를 확인
    case WM_KEYDOWN ://키가 눌러졌을 때
    {
        keyboard_state[wParam] = 1;  //키보드 상태 값을 1으로 주기
        InvalidateRect(hWnd, nullptr, true);
        break;
    }
    case WM_KEYUP://키가 떼졌을 때
    {
        keyboard_state[wParam] = 0; //키보드 상태 값을 0으로 주기
        InvalidateRect(hWnd, nullptr, true);
        break;
    }
    case WM_MOUSEMOVE://마우스가 움직일때
    {
        mouseX = (short)LOWORD(lParam); //LOWORD : X의 값
        //mouseX에 마우스 X값 저장
        mouseY = (short)HIWORD(lParam);//HIWORD : Y의 값
        //mouseX에 마우스 Y값 저장
        InvalidateRect(hWnd, nullptr, true);

        break;
    }
    
    case WM_LBUTTONDOWN: //좌클릭 시
    {
        MessageBox(hWnd, _T("좌클릭!!"), _T("클릭"), MB_OK); //메세지 박스로 "좌클릭!!" 출력 (확인)
        break;
    }
    case WM_RBUTTONDOWN: //우클릭시
    {
        MessageBox(hWnd, _T("우클릭!!"), _T("클릭"), MB_OKCANCEL); //메세지 박스로 "좌클릭!!" 출력 (확인,취소)
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

// 정보 대화 상자의 메시지 처리기입니다.
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
