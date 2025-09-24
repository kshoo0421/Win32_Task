// [프로젝트 목적] 객체 기반 창 생성
#include <windows.h>

// [BaseWindow 템플릿 클래스] 윈도우 클래스를 공통적으로 관리하는 베이스 클래스
// DERIVED_TYPE : 실제로 이 클래스를 상속받아 구현하는 파생 클래스 타입
template <class DERIVED_TYPE>
class BaseWindow
{
public:
    // 정적(Static) WindowProc : 운영체제에서 호출하는 콜백은 반드시 C 스타일 함수여야 하므로 static으로 선언
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        // 윈도우가 처음 생성될 때
        if (uMsg == WM_NCCREATE)
        {
            // CreateWindowEx의 마지막 매개변수(this 포인터)를 가져옴
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;

            // 윈도우 핸들에 사용자 데이터 영역을 할당해 this 포인터 저장
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            // this 객체가 가진 m_hwnd에 hwnd 저장
            pThis->m_hwnd = hwnd;
        }
        else
        {
            // 저장해둔 this 포인터를 꺼내옴
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (pThis) // 포인터가 있다면
        {
            // 파생 클래스에서 구현한 HandleMessage 호출
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            // 해당 포인터가 없으면 기본 윈도우 처리기로 넘김
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) {}

    // 실제 윈도우 생성 함수
    BOOL Create(
        PCWSTR lpWindowName,     // 윈도우 제목
        DWORD dwStyle,           // 윈도우 스타일
        DWORD dwExStyle = 0,     // 확장 스타일
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };

        // 이 윈도우의 메시지 처리기를 템플릿 기반 WindowProc으로 지정
        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc); // 윈도우 클래스 등록

        // 실제 윈도우 생성
        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
            // 마지막 매개변수(this)가 WM_NCCREATE 단계에서 전달됨
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    // 현재 윈도우 핸들 반환
    HWND Window() const { return m_hwnd; }

protected:
    // 파생 클래스가 반드시 구현해야 하는 함수들 (순수 가상 함수)
    virtual PCWSTR  ClassName() const = 0;                  // 클래스 이름
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0; // 메시지 처리기

    HWND m_hwnd; // 윈도우 핸들
};


// MainWindow 클래스 : BaseWindow를 상속받아 실제 동작을 구현
class MainWindow : public BaseWindow<MainWindow>
{
public:
    // 윈도우 클래스 이름
    PCWSTR  ClassName() const { return L"Sample Window Class"; }

    // 메시지 처리 함수 (WM_PAINT, WM_DESTROY 등)
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


// 프로그램 시작점 (유니코드)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    MainWindow win;

    // 윈도우 생성
    if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    // 생성된 윈도우 화면에 표시
    ShowWindow(win.Window(), nCmdShow);

    // 메시지 루프
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


// MainWindow 메시지 처리 구현
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY: // 윈도우 닫힐 때
        PostQuitMessage(0); // 메시지 루프 종료
        return 0;

    case WM_PAINT:   // 화면 갱신 요청
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        // 클라이언트 영역 흰색으로 칠하기
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    default:
        // 처리하지 않은 메시지는 기본 처리기로 넘김
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}
