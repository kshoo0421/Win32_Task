// [������Ʈ ����] ��ü ��� â ����
#include <windows.h>

// [BaseWindow ���ø� Ŭ����] ������ Ŭ������ ���������� �����ϴ� ���̽� Ŭ����
// DERIVED_TYPE : ������ �� Ŭ������ ��ӹ޾� �����ϴ� �Ļ� Ŭ���� Ÿ��
template <class DERIVED_TYPE>
class BaseWindow
{
public:
    // ����(Static) WindowProc : �ü������ ȣ���ϴ� �ݹ��� �ݵ�� C ��Ÿ�� �Լ����� �ϹǷ� static���� ����
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        // �����찡 ó�� ������ ��
        if (uMsg == WM_NCCREATE)
        {
            // CreateWindowEx�� ������ �Ű�����(this ������)�� ������
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;

            // ������ �ڵ鿡 ����� ������ ������ �Ҵ��� this ������ ����
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            // this ��ü�� ���� m_hwnd�� hwnd ����
            pThis->m_hwnd = hwnd;
        }
        else
        {
            // �����ص� this �����͸� ������
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (pThis) // �����Ͱ� �ִٸ�
        {
            // �Ļ� Ŭ�������� ������ HandleMessage ȣ��
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            // �ش� �����Ͱ� ������ �⺻ ������ ó����� �ѱ�
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hwnd(NULL) {}

    // ���� ������ ���� �Լ�
    BOOL Create(
        PCWSTR lpWindowName,     // ������ ����
        DWORD dwStyle,           // ������ ��Ÿ��
        DWORD dwExStyle = 0,     // Ȯ�� ��Ÿ��
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };

        // �� �������� �޽��� ó���⸦ ���ø� ��� WindowProc���� ����
        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc); // ������ Ŭ���� ���

        // ���� ������ ����
        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
            // ������ �Ű�����(this)�� WM_NCCREATE �ܰ迡�� ���޵�
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    // ���� ������ �ڵ� ��ȯ
    HWND Window() const { return m_hwnd; }

protected:
    // �Ļ� Ŭ������ �ݵ�� �����ؾ� �ϴ� �Լ��� (���� ���� �Լ�)
    virtual PCWSTR  ClassName() const = 0;                  // Ŭ���� �̸�
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0; // �޽��� ó����

    HWND m_hwnd; // ������ �ڵ�
};


// MainWindow Ŭ���� : BaseWindow�� ��ӹ޾� ���� ������ ����
class MainWindow : public BaseWindow<MainWindow>
{
public:
    // ������ Ŭ���� �̸�
    PCWSTR  ClassName() const { return L"Sample Window Class"; }

    // �޽��� ó�� �Լ� (WM_PAINT, WM_DESTROY ��)
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


// ���α׷� ������ (�����ڵ�)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    MainWindow win;

    // ������ ����
    if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    // ������ ������ ȭ�鿡 ǥ��
    ShowWindow(win.Window(), nCmdShow);

    // �޽��� ����
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


// MainWindow �޽��� ó�� ����
LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY: // ������ ���� ��
        PostQuitMessage(0); // �޽��� ���� ����
        return 0;

    case WM_PAINT:   // ȭ�� ���� ��û
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        // Ŭ���̾�Ʈ ���� ������� ĥ�ϱ�
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    default:
        // ó������ ���� �޽����� �⺻ ó����� �ѱ�
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}
