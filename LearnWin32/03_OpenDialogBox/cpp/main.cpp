// 매니페스트 설정 : Windows XP 이후 버전에서 새로운 스타일의 공용 컨트롤(버튼, 다이얼로그 등)을 사용하도록 지정
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <shobjidl.h> // IFileOpenDialog, IShellItem 인터페이스 포함

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // COM 라이브러리 초기화 (단일 스레드 아파트먼트, OLE1 DDE 비활성화)
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr)) // COM 초기화 성공 시
    {
        IFileOpenDialog* pFileOpen; // 파일 열기 다이얼로그 COM 객체

        // FileOpenDialog COM 객체 생성
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // 다이얼로그 표시 (사용자가 파일 선택)
            hr = pFileOpen->Show(NULL);

            // 사용자가 정상적으로 파일을 선택했을 때
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem; // 선택된 항목(파일)
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath; // 파일 경로 (유니코드 문자열)
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // 파일 경로 가져오기 성공 시
                    if (SUCCEEDED(hr))
                    {
                        // 선택된 파일 경로를 메시지 박스로 출력
                        MessageBox(NULL, pszFilePath, L"File Path", MB_OK);

                        // COM 메모리 해제 (CoTaskMemAlloc/Free 사용)
                        CoTaskMemFree(pszFilePath);
                    }
                    // COM 객체 해제
                    pItem->Release();
                }
            }
            // 다이얼로그 객체 해제
            pFileOpen->Release();
        }
        // COM 라이브러리 해제
        CoUninitialize();
    }
    return 0;
}
