////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// 메인 윈도우
/// 
/// @author   parkkh
/// @date     Friday, November 19, 2010  4:15:21 PM
/// 
/// Copyright(C) 2010 Bandisoft, All rights reserved.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "resource.h" // local 우선 참조

extern "C" {
#include "jpeglib.h"
}
#include <setjmp.h>


// 변경 종료



class CMainWnd  : public CWindowImpl<CMainWnd, CWindow>
{
public:
	CMainWnd() noexcept;
	~CMainWnd() final;
	BOOL		CreateMainWindow();
	BOOL		PreTranslateMessage(MSG* pMsg) noexcept;

public :
	BEGIN_MSG_MAP(CMainWnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		COMMAND_ID_HANDLER(IDM_FILE_OPEN, OnFileOpen) // 요구사항 2)
		COMMAND_ID_HANDLER(IDM_FILE_EXIT, OnFileExit)
	END_MSG_MAP()


private:
	LRESULT		OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) noexcept;
	
	// 메뉴 핸들러
	LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);

	// JPEG
	HBITMAP m_hBmp = nullptr;
	void LoadAndDisplayJpeg(LPCWSTR filename);
};

