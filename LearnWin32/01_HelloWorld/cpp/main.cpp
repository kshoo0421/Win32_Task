/*
- [������Ʈ ����] �� â ����
*/

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

/* [����] ������ ���ν���(�ݹ� �Լ�) : ������� ���޵Ǵ� �޽����� ó���ϴ� �Լ� */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* [����/����] Win32 ���α׷��� ������ */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    /* === |01| ������ Ŭ���� ���� === */
    const wchar_t CLASS_NAME[] = L"Sample Window Class";   

    WNDCLASS wc = { };              // ����ü �ʱ�ȭ
    wc.lpfnWndProc = WindowProc;    // �޽����� ó���� �ݹ� �Լ� ���
    wc.hInstance = hInstance;       // ���� ���� ���� ���α׷� �ν��Ͻ� �ڵ�
    wc.lpszClassName = CLASS_NAME;  // ������ Ŭ���� �̸�

    RegisterClass(&wc); // ������ Ŭ���� ��� (OS�� �˸�)

    /* === |02| ������ ���� === */
    HWND hwnd = CreateWindowEx(
        0,                              // Ȯ�� ������ ��Ÿ��
        CLASS_NAME,                     // ����� ������ Ŭ���� �̸�
        L"Learn to Program Windows",    // ������ ���� (Ÿ��Ʋ�ٿ� ǥ��)
        WS_OVERLAPPEDWINDOW,            // �⺻���� ������ ��Ÿ��(Ÿ��Ʋ��, �ּ�/�ִ�ȭ ��ư ��)

        // ��ġ�� ũ�� (�ڵ����� ����)
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // �θ� ������ (����)
        NULL,       // �޴� �ڵ� (����)
        hInstance,  // ���α׷� �ν��Ͻ� �ڵ�
        NULL        // �߰� ���ø����̼� ������ (����)
    );

    if (hwnd == NULL) // ������ ���� ���� ��
    {
        return 0;
    }

    // �����츦 ȭ�鿡 ǥ��
    ShowWindow(hwnd, nCmdShow);


    /* === |03| �޽��� ���� : �ü���� �߻���Ű�� �޽����� ��� �޾Ƽ� ó�� === */
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) // �޽��� ���� ��
    {
        TranslateMessage(&msg);  // Ű �Է� �޽��� ���� | DispatchMessage �� ���� �ʿ�
        DispatchMessage(&msg);   // �ش� �޽����� WindowProc���� ����
    }

    return 0; // ���α׷� ����
}

/* [����] ������ ���ν���(�ݹ� �Լ�) : ������� ���޵Ǵ� �޽����� ó���ϴ� �Լ� */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) // � �޽����� �Դ��� Ȯ��
    {
    case WM_DESTROY: // �����찡 ���� ��
        PostQuitMessage(0); // �޽��� ���� ���� ��û
        return 0;

    case WM_PAINT: // ȭ�� �ٽ� �׸��� ��û (�����찡 ���ŵ� ��)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps); // �׸��� ����, DC(����̽� ���ؽ�Ʈ) ȹ��

        // ���� �׸��� ����: Ŭ���̾�Ʈ ������ ������� ĥ��
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps); // �׸��� ����
    }
    return 0;
    }

    // ó������ ���� �޽����� �⺻ ������ ó����(DefWindowProc)�� ����
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
