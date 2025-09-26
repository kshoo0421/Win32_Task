#include "stdafx.h"
#include <commdlg.h> // GetOpenFileName, OPENFILENAME
#include "MainWnd.h"
#include "resource.h"

#define	APP_NAME				L"AtlWinApp"
#define APP_CLASS_NAME			L"AtlWinAppClass"
#define APP_TITLE				L"AtlWinApp"

CMainWnd::CMainWnd(void) noexcept
{
}

CMainWnd::~CMainWnd(void)
{
	// jpeg 담당
	if (m_hBmp)
	{
		DeleteObject(m_hBmp);
		m_hBmp = nullptr;
	}
}

BOOL CMainWnd::CreateMainWindow()
{
	// atl 윈도우 생성
	CWndClassInfo& winInfo = GetWndClassInfo();
	winInfo.m_wc.lpszClassName = APP_CLASS_NAME;
	winInfo.m_wc.style = CS_DBLCLKS;
	winInfo.m_wc.hbrBackground = nullptr;
	winInfo.m_wc.hIcon = ::LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_MAIN));
	CRect rWnd(0, 0, 640, 480);

	__super::Create(nullptr, &rWnd, APP_NAME, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	CenterWindow();

	// 추가 코드 : 메뉴 붙이기
	HMENU hMenu = LoadMenu(_Module.m_hInst, MAKEINTRESOURCE(IDR_MENU_FILE));
	SetMenu(hMenu);

	// 보이기
	ShowWindow(SW_SHOW);
	DragAcceptFiles(TRUE);

	return TRUE;
}

LRESULT CMainWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) noexcept
{
	PostQuitMessage(0);
	return 0;
}

BOOL CMainWnd::PreTranslateMessage(MSG* /*pMsg*/) noexcept
{
	return FALSE;
}

// 파일 → 열기
LRESULT CMainWnd::OnFileOpen(WORD, WORD, HWND, BOOL&)
{
	// 1) 파일 경로를 저장할 버퍼 준비
	WCHAR szFile[MAX_PATH] = { 0 };

	// 2) 파일 열기 다이얼로그 구조체 초기화
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	// 3) OPENFILENAME 구조체 채우기
	ofn.lStructSize = sizeof(ofn);     // 구조체 크기
	ofn.hwndOwner = m_hWnd;          // 다이얼로그 부모 윈도우 핸들
	ofn.lpstrFilter = L"JPEG Files (*.jpg;*.jpeg)\0*.jpg;*.jpeg\0All Files (*.*)\0*.*\0\0";
	// 확장자 필터 (JPEG 파일 우선 표시, 모든 파일도 가능)
	ofn.lpstrFile = szFile;          // 선택된 파일 경로 저장 버퍼
	ofn.nMaxFile = MAX_PATH;        // 버퍼 크기
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	// 존재하는 파일만 선택 가능, 유효한 경로만 허용

	// 4) 파일 열기 다이얼로그 표시
	if (GetOpenFileName(&ofn)) {
		// 사용자가 정상적으로 파일을 선택하면, JPEG 로드 함수 호출
		LoadAndDisplayJpeg(szFile);
	}

	return 0;
}

// 파일 닫기
LRESULT CMainWnd::OnFileClose(WORD, WORD, HWND, BOOL&)
{
	// 현재 로드된 비트맵 해제
	if (m_hBmp) {
		DeleteObject(m_hBmp);
		m_hBmp = nullptr;
	}

	// 화면 갱신 요청 (→ OnPaint에서 빈 창 그림)
	Invalidate(FALSE);
	return 0;
}

LRESULT CMainWnd::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
    // 1) WM_PAINT 메시지를 처리하기 위해 PAINTSTRUCT 구조체 준비
    PAINTSTRUCT ps;

    // 2) BeginPaint() : 그리기 준비 + DC(HDC) 획득
    //    이때 얻는 HDC는 반드시 EndPaint()로 짝을 맞춰야 함
    HDC hdc = BeginPaint(&ps);


	// 3) 전체 검정 화면으로 설정
	RECT rc;
	GetClientRect(&rc);
    FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));


    // 4) 현재 로드된 비트맵이 있으면 화면에 그림
    if (m_hBmp) {
        // 4-1) 메모리 DC 생성 (비트맵을 그리기 위한 임시 DC)
        HDC memDC = CreateCompatibleDC(hdc);

        // 4-2) 메모리 DC에 m_hBmp 비트맵 선택
        HBITMAP hOld = (HBITMAP)SelectObject(memDC, m_hBmp);

        // 4-3) 비트맵 크기 정보 얻기
        BITMAP bm;
        GetObject(m_hBmp, sizeof(bm), &bm);

        // 4-4) BitBlt() : 메모리 DC(memDC)에 있는 비트맵을
        //                 윈도우 DC(hdc)로 복사해서 화면에 출력
        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);

        // 4-5) 원래 비트맵을 복구하고 메모리 DC 해제
        SelectObject(memDC, hOld);
        DeleteDC(memDC);
    }

    // 5) EndPaint() : BeginPaint()와 반드시 짝을 맞춰 호출해야 함
    //    WM_PAINT 메시지 처리를 끝내고, DC 반환
    EndPaint(&ps);

    return 0;
}


// JPEG 로드
void CMainWnd::LoadAndDisplayJpeg(LPCWSTR filename)
{
	// 1) 이미 이전에 로드된 비트맵이 있으면 삭제 (메모리/리소스 누수 방지)
	if (m_hBmp) {
		DeleteObject(m_hBmp);
		m_hBmp = nullptr;
	}

	// 2) JPEG 파일 열기 (유니코드 경로 → _wfopen_s 사용)
	FILE* infile = nullptr;
	if (_wfopen_s(&infile, filename, L"rb") != 0 || !infile) {
		MessageBox(L"파일 열기 실패", L"에러", MB_ICONERROR);
		return;
	}

	// 3) libjpeg 구조체 초기화
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);       // 에러 핸들러 연결
	jpeg_create_decompress(&cinfo);          // 디컴프레서 객체 생성
	jpeg_stdio_src(&cinfo, infile);          // 파일을 입력 소스로 설정
	jpeg_read_header(&cinfo, TRUE);          // JPEG 헤더 읽기
	jpeg_start_decompress(&cinfo);           // 디코딩 시작

	// 4) 디코딩된 이미지의 크기와 채널 수 얻기
	int width = cinfo.output_width;         // 가로 픽셀 수
	int height = cinfo.output_height;        // 세로 픽셀 수
	int depth = cinfo.output_components;    // 채널 수 (보통 3 = RGB)

	// RGB 아닌 경우 에러 처리
	if (depth != 3) {
		MessageBox(L"RGB JPEG만 지원합니다", L"에러", MB_ICONERROR);
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return;
	}

	// 5) 출력용 DIBSection 비트맵 생성 (요구사항 3))
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;   // top-down 저장 (위에서 아래로)
	bmi.bmiHeader.biPlanes = 1;         // 항상 1
	bmi.bmiHeader.biBitCount = 24;        // 24비트 (R/G/B 8비트씩)
	bmi.bmiHeader.biCompression = BI_RGB;    // 압축 없음

	void* pBits = nullptr;
	m_hBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
	if (!m_hBmp || !pBits) {
		MessageBox(L"DIBSection 생성 실패", L"에러", MB_ICONERROR);
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return;
	}

	// 6) stride 계산 (한 줄이 4바이트 배수여야 함)
	int stride = ((width * 3 + 3) & ~3);

	// 7) JPEG → DIBSection 복사 (요구사항 4))
	//    JPEG는 RGB 순서, Windows DIBSection은 BGR 순서 → 변환 필요
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr)&cinfo, JPOOL_IMAGE, width * depth, 1);

	unsigned char* dstLine = (unsigned char*)pBits;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1); // 한 줄씩 디코딩

		unsigned char* src = buffer[0];
		unsigned char* dst = dstLine;

		for (int x = 0; x < width; x++) {
			dst[0] = src[2]; // B
			dst[1] = src[1]; // G
			dst[2] = src[0]; // R
			dst += 3;
			src += 3;
		}

		dstLine += stride; // 다음 줄로 이동
	}

	// 8) JPEG 디코딩 마무리
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	// 새로 그려주도록 무효화 → OnPaint 호출
	Invalidate(FALSE);
	// UpdateWindow();  // 선택: 바로 OnPaint 실행

	// 9) 화면에 출력 (요구사항 5, 6)
	HDC hdc = GetDC();                        // 윈도우 DC 얻기
	HDC memDC = CreateCompatibleDC(hdc);      // 메모리 DC 생성
	HBITMAP hOld = (HBITMAP)SelectObject(memDC, m_hBmp); // 비트맵 선택

	BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY); // 화면에 복사

	// 리소스 정리
	SelectObject(memDC, hOld);
	DeleteDC(memDC);
	ReleaseDC(hdc);
}
