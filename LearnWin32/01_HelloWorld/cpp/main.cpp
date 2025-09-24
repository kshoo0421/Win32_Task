/*
- [프로젝트 목적] 빈 창 생성
*/

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

/* [선언] 윈도우 프로시저(콜백 함수) : 윈도우로 전달되는 메시지를 처리하는 함수 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* [선언/구현] Win32 프로그램의 진입점 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    /* === |01| 윈도우 클래스 정의 === */
    const wchar_t CLASS_NAME[] = L"Sample Window Class";   

    WNDCLASS wc = { };              // 구조체 초기화
    wc.lpfnWndProc = WindowProc;    // 메시지를 처리할 콜백 함수 등록
    wc.hInstance = hInstance;       // 현재 실행 중인 프로그램 인스턴스 핸들
    wc.lpszClassName = CLASS_NAME;  // 윈도우 클래스 이름

    RegisterClass(&wc); // 윈도우 클래스 등록 (OS에 알림)

    /* === |02| 윈도우 생성 === */
    HWND hwnd = CreateWindowEx(
        0,                              // 확장 윈도우 스타일
        CLASS_NAME,                     // 등록한 윈도우 클래스 이름
        L"Learn to Program Windows",    // 윈도우 제목 (타이틀바에 표시)
        WS_OVERLAPPEDWINDOW,            // 기본적인 윈도우 스타일(타이틀바, 최소/최대화 버튼 등)

        // 위치와 크기 (자동으로 지정)
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // 부모 윈도우 (없음)
        NULL,       // 메뉴 핸들 (없음)
        hInstance,  // 프로그램 인스턴스 핸들
        NULL        // 추가 애플리케이션 데이터 (없음)
    );

    if (hwnd == NULL) // 윈도우 생성 실패 시
    {
        return 0;
    }

    // 윈도우를 화면에 표시
    ShowWindow(hwnd, nCmdShow);


    /* === |03| 메시지 루프 : 운영체제가 발생시키는 메시지를 계속 받아서 처리 === */
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) // 메시지 수신 시
    {
        TranslateMessage(&msg);  // 키 입력 메시지 번역 | DispatchMessage 전 실행 필요
        DispatchMessage(&msg);   // 해당 메시지를 WindowProc으로 전달
    }

    return 0; // 프로그램 종료
}

/* [구현] 윈도우 프로시저(콜백 함수) : 윈도우로 전달되는 메시지를 처리하는 함수 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) // 어떤 메시지가 왔는지 확인
    {
    case WM_DESTROY: // 윈도우가 닫힐 때
        PostQuitMessage(0); // 메시지 루프 종료 요청
        return 0;

    case WM_PAINT: // 화면 다시 그리기 요청 (윈도우가 갱신될 때)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps); // 그리기 시작, DC(디바이스 컨텍스트) 획득

        // 실제 그리기 동작: 클라이언트 영역을 흰색으로 칠함
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps); // 그리기 종료
    }
    return 0;
    }

    // 처리하지 않은 메시지는 기본 윈도우 처리기(DefWindowProc)에 전달
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
