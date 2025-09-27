#include "stdafx.h"
#include <commdlg.h> // GetOpenFileName, OPENFILENAME
#include "MainWnd.h"
#include "resource.h"
#include <png.h>
#include <zlib.h>
#include <vector>

#define	APP_NAME				L"AtlWinApp"
#define APP_CLASS_NAME			L"AtlWinAppClass"
#define APP_TITLE				L"AtlWinApp"

/* 생성 & 소멸자 */
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
	ClearImageList();
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

BOOL CMainWnd::PreTranslateMessage(MSG* /*pMsg*/) noexcept
{
	return FALSE;
}

/* MESSAGE_HANDLER */
// 그리기
LRESULT CMainWnd::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);
	// BeginPaint: WM_PAINT 처리 시작, 화면에 그리기 위한 DC(HDC) 얻기

	// 1) 전체 클라이언트 영역을 검정색으로 초기화
	RECT rc;
	GetClientRect(&rc);
	// 현재 윈도우 클라이언트 영역 좌표 얻기
	FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	// 클라이언트 전체를 검정색으로 칠해 "배경 초기화"

	// 2) 로드된 비트맵(m_hBmp)이 있으면 화면에 출력
	if (m_hBmp) {
		HDC memDC = CreateCompatibleDC(hdc);
		// 비트맵을 그리기 위한 메모리 DC 생성
		HBITMAP hOld = (HBITMAP)SelectObject(memDC, m_hBmp);
		// m_hBmp를 메모리 DC에 선택

		BITMAP bm;
		GetObject(m_hBmp, sizeof(bm), &bm);
		// 비트맵 정보(가로/세로 크기 등) 얻기
		int imgW = bm.bmWidth;
		int imgH = bm.bmHeight;

		int winW = rc.right - rc.left;
		int winH = rc.bottom - rc.top;

		// 3) 비율 유지 계산
		double imgRatio = (double)imgW / imgH;  // 이미지 가로/세로 비율
		double winRatio = (double)winW / winH;  // 창의 가로/세로 비율

		int drawW, drawH;
		if (winRatio > imgRatio) {
			// 창이 더 "가로로 넓은" 경우: 높이를 창 크기에 맞추고, 폭은 비율 유지
			drawH = winH;
			drawW = (int)(winH * imgRatio);
		}
		else {
			// 창이 더 "세로로 긴" 경우: 폭을 창 크기에 맞추고, 높이는 비율 유지
			drawW = winW;
			drawH = (int)(winW / imgRatio);
		}

		// 4) 화면 중앙에 출력되도록 좌표 계산
		int offsetX = (winW - drawW) / 2;
		int offsetY = (winH - drawH) / 2;

		// 5) StretchBlt 고품질 모드 설정
		SetStretchBltMode(hdc, HALFTONE);
		// HALFTONE: 픽셀 보간을 고품질로 (이미지 확대/축소 시 깨짐 방지)
		SetBrushOrgEx(hdc, 0, 0, NULL);

		// 6) StretchBlt로 이미지 출력
		StretchBlt(hdc, offsetX, offsetY, drawW, drawH,
			memDC, 0, 0, imgW, imgH, SRCCOPY);
		// memDC에 있는 원본 이미지를 창 DC(hdc)에 복사 (비율 유지, 중앙 정렬)

		// 7) 리소스 정리
		SelectObject(memDC, hOld);  // 이전 비트맵 복원
		DeleteDC(memDC);            // 메모리 DC 해제
	}

	EndPaint(&ps);
	// WM_PAINT 처리 종료, DC 반환
	return 0;
}

LRESULT CMainWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) noexcept
{
	PostQuitMessage(0);
	return 0;
}

// 마우스 휠 이벤트 처리
LRESULT CMainWnd::OnMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	short delta = GET_WHEEL_DELTA_WPARAM(wParam); // 휠 이동량 (양수=위로, 음수=아래로)

	if (delta > 0 && m_currentIndex > 0) {
		// 위로 스크롤 → 이전 이미지
		m_currentIndex--;
		LoadCurrentImage();
	}
	else if (delta < 0 && m_currentIndex + 1 < (int)m_imageFiles.size()) {
		// 아래로 스크롤 → 다음 이미지
		m_currentIndex++;
		LoadCurrentImage();
	}
	return 0;
}

// 키보드 방향키 처리
LRESULT CMainWnd::OnKeyDown(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	if (wParam == VK_LEFT && m_currentIndex > 0) {
		// ← 키 → 이전 이미지
		m_currentIndex--;
		LoadCurrentImage();
	}
	else if (wParam == VK_RIGHT && m_currentIndex + 1 < (int)m_imageFiles.size()) {
		// → 키 → 다음 이미지
		m_currentIndex++;
		LoadCurrentImage();
	}
	return 0;
}

LRESULT CMainWnd::OnEraseBkgnd(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	HDC hdc = (HDC)wParam;
	RECT rc;
	GetClientRect(&rc);
	FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	return 1; // 배경 지웠다고 알림
}

LRESULT CMainWnd::OnSize(UINT, WPARAM, LPARAM, BOOL&)
{
	Invalidate(TRUE); // 창 전체를 무효화 → OnPaint가 다시 실행되며 전체 새 그림
	return 0;
}


/* COMMAND_ID_HANDLE */
// 파일 → 열기
LRESULT CMainWnd::OnFileOpen(WORD, WORD, HWND, BOOL&)
{
	WCHAR szFile[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	// 다이얼로그에서 표시할 파일 필터: JPEG/PNG + 모든 파일
	ofn.lpstrFilter =
		L"Image Files (*.jpg;*.jpeg;*.png)\0*.jpg;*.jpeg;*.png\0All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile = szFile;       // 선택된 파일 경로가 저장될 버퍼
	ofn.nMaxFile = MAX_PATH;      // 버퍼 크기
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	// 존재하는 파일만, 유효한 경로만 선택 가능

	if (GetOpenFileName(&ofn)) {
		// 사용자가 정상적으로 파일을 선택한 경우 실행됨
		WCHAR folder[MAX_PATH];
		wcscpy_s(folder, szFile);      // 전체 경로 복사
		PathRemoveFileSpec(folder);    // 파일 이름 제거 → 폴더 경로만 남김

		// 이전 이미지 목록 해제
		ClearImageList();

		// "C:\경로\*.*" 패턴으로 해당 폴더의 모든 파일 검색
		WCHAR searchPattern[MAX_PATH];
		wsprintf(searchPattern, L"%s\\*.*", folder);

		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(searchPattern, &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// 디렉터리는 무시하고 파일만 처리
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					// 확장자 추출
					LPCWSTR ext = PathFindExtension(fd.cFileName);
					// JPG/JPEG/PNG 만 허용
					if (_wcsicmp(ext, L".jpg") == 0 ||
						_wcsicmp(ext, L".jpeg") == 0 ||
						_wcsicmp(ext, L".png") == 0) {
						WCHAR fullPath[MAX_PATH];
						wsprintf(fullPath, L"%s\\%s", folder, fd.cFileName);
						// 경로 문자열을 복사해서 목록에 추가
						m_imageFiles.push_back(_wcsdup(fullPath));
					}
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}

		// 사용자가 선택한 파일이 목록에서 몇 번째인지 찾기
		for (int i = 0; i < (int)m_imageFiles.size(); i++) {
			if (_wcsicmp(m_imageFiles[i], szFile) == 0) {
				m_currentIndex = i;
				break;
			}
		}

		// 현재 인덱스의 이미지 로드 및 표시
		LoadCurrentImage();
	}
	return 0;
}


// 파일 → 닫기
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


/* 내부 함수 */
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

// PNG 로드 & 표시
void CMainWnd::LoadAndDisplayPng(LPCWSTR filename)
{
	// 1) 기존 비트맵 해제 (새 이미지 로드를 위해)
	if (m_hBmp) {
		DeleteObject(m_hBmp);
		m_hBmp = nullptr;
	}

	// 2) 파일 열기
	FILE* fp = nullptr;
	if (_wfopen_s(&fp, filename, L"rb") != 0 || !fp) {
		MessageBox(L"PNG 파일 열기 실패", L"에러", MB_ICONERROR);
		return;
	}

	// 3) libpng 구조체 생성
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return;
	}

	// 에러 발생 시 setjmp로 이 지점으로 돌아옴
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return;
	}

	// 4) PNG 파일 읽기 준비
	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);

	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	// 5) 색상/비트 변환 및 처리
	// 16비트 → 8비트
	if (bit_depth == 16) png_set_strip_16(png_ptr);
	// 팔레트 → RGB
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	// 1/2/4비트 그레이스케일 → 8비트 그레이스케일로 확장
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
	// 투명 정보(tRNS) → 알파 채널로 변환
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);

	// 그레이스케일 → RGB
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	// 3바이트(RGB) → 4바이트(RGBA)로 만들고, 32비트 DIBSection에 맞추기 위해 
	// filler (0xFF)를 맨 뒤(PNG_FILLER_AFTER)에 추가하여 RGBA로 만듦
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

	// 최종 이미지 정보 업데이트
	png_read_update_info(png_ptr, info_ptr);

	// 최종적으로 32비트 RGBA 형식이 되며, 이를 DIBSection의 BGRA로 변환할 예정

	// 6) 버퍼 준비 (vector 사용)
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr); // 한 줄의 바이트 수 (32비트 = 4바이트/픽셀)
	std::vector<unsigned char> image_data(rowbytes * height);
	std::vector<png_bytep> row_pointers(height);

	for (int y = 0; y < height; y++) {
		// PNG는 보통 top-down이지만, DIBSection top-down과 변환이 필요할 수 있음. 
		// 여기서는 간단히 y축 순서대로 읽음.
		row_pointers[y] = image_data.data() + y * rowbytes;
	}

	// 7) PNG 이미지 읽기
	png_read_image(png_ptr, row_pointers.data());

	// 8) 정리
	fclose(fp);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	// 9) DIBSection 생성 (32비트 BGRA 포맷)
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // top-down DIB: 이미지가 위에서 아래로 저장
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;      // 32비트 (BGRA)
	bmi.bmiHeader.biCompression = BI_RGB;

	void* pBits = nullptr;
	m_hBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
	if (!m_hBmp || !pBits) {
		MessageBox(L"DIBSection 생성 실패", L"에러", MB_ICONERROR);
		return;
	}

	// 10) RGBA (PNG) → BGRA (DIBSection) 변환 복사
	unsigned char* dst = (unsigned char*)pBits;
	int dib_stride = ((width * 4 + 3) & ~3); // DIBSection의 한 줄 stride (4바이트 배수)

	for (int y = 0; y < height; y++) {
		png_bytep src = row_pointers[y]; // PNG 픽셀 데이터 (RGBA 순서)
		unsigned char* current_dst_row = dst + y * dib_stride;

		for (int x = 0; x < width; x++) {
			// PNG 픽셀 (R, G, B, A)
			unsigned char R = src[0];
			unsigned char G = src[1];
			unsigned char B = src[2];
			unsigned char A = src[3];

			// DIBSection 픽셀 (B, G, R, A)
			current_dst_row[0] = B; // B
			current_dst_row[1] = G; // G
			current_dst_row[2] = R; // R
			current_dst_row[3] = A; // A

			current_dst_row += 4;
			src += 4;
		}
	}

	// 11) 화면 갱신 (OnPaint 호출)
	Invalidate(FALSE);

	// 12) 화면에 출력 (JPEG와 동일하게 로드 후 바로 출력)
	HDC hdc = GetDC();                        // 윈도우 DC 얻기
	if (hdc) {
		HDC memDC = CreateCompatibleDC(hdc);      // 메모리 DC 생성
		if (memDC) {
			HBITMAP hOld = (HBITMAP)SelectObject(memDC, m_hBmp); // 비트맵 선택

			BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY); // 화면에 복사

			// 리소스 정리
			SelectObject(memDC, hOld);
			DeleteDC(memDC);
		}
		ReleaseDC(hdc);
	}
}

// 현재 인덱스(m_currentIndex)에 해당하는 이미지를 로드하고 화면에 표시
void CMainWnd::LoadCurrentImage()
{
	// 유효한 인덱스인지 검사
	if (m_currentIndex < 0 || m_currentIndex >= (int)m_imageFiles.size())
		return;

	// 현재 파일 경로
	LPCWSTR path = m_imageFiles[m_currentIndex];

	// 확장자 추출
	LPCWSTR ext = PathFindExtension(path);

	// 확장자에 따라 JPEG 또는 PNG 로드 함수 호출
	if (_wcsicmp(ext, L".jpg") == 0 || _wcsicmp(ext, L".jpeg") == 0)
		LoadAndDisplayJpeg(path);
	else if (_wcsicmp(ext, L".png") == 0)
		LoadAndDisplayPng(path);

	// 윈도우 제목 표시줄에 "파일명 [현재/전체]" 형태로 표시
	WCHAR title[MAX_PATH];
	wsprintf(title, L"%s [%d/%d]",
		PathFindFileName(path), m_currentIndex + 1, (int)m_imageFiles.size());
	SetWindowText(title);
}

// 이미지 파일 목록을 정리하는 함수
void CMainWnd::ClearImageList()
{
	// m_imageFiles 벡터 안에는 wcsdup()으로 복사한 문자열 포인터들이 들어 있음
	// → free()로 직접 해제해야 메모리 누수 방지
	for (auto ptr : m_imageFiles) {
		free(ptr);
	}
	// 벡터 비우기
	m_imageFiles.clear();
}
