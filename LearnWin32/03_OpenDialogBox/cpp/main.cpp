// �Ŵ��佺Ʈ ���� : Windows XP ���� �������� ���ο� ��Ÿ���� ���� ��Ʈ��(��ư, ���̾�α� ��)�� ����ϵ��� ����
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <shobjidl.h> // IFileOpenDialog, IShellItem �������̽� ����

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // COM ���̺귯�� �ʱ�ȭ (���� ������ ����Ʈ��Ʈ, OLE1 DDE ��Ȱ��ȭ)
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr)) // COM �ʱ�ȭ ���� ��
    {
        IFileOpenDialog* pFileOpen; // ���� ���� ���̾�α� COM ��ü

        // FileOpenDialog COM ��ü ����
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // ���̾�α� ǥ�� (����ڰ� ���� ����)
            hr = pFileOpen->Show(NULL);

            // ����ڰ� ���������� ������ �������� ��
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem; // ���õ� �׸�(����)
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath; // ���� ��� (�����ڵ� ���ڿ�)
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // ���� ��� �������� ���� ��
                    if (SUCCEEDED(hr))
                    {
                        // ���õ� ���� ��θ� �޽��� �ڽ��� ���
                        MessageBox(NULL, pszFilePath, L"File Path", MB_OK);

                        // COM �޸� ���� (CoTaskMemAlloc/Free ���)
                        CoTaskMemFree(pszFilePath);
                    }
                    // COM ��ü ����
                    pItem->Release();
                }
            }
            // ���̾�α� ��ü ����
            pFileOpen->Release();
        }
        // COM ���̺귯�� ����
        CoUninitialize();
    }
    return 0;
}
