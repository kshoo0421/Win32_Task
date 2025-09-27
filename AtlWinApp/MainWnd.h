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
#include "resource.h"

extern "C" {
#include "jpeglib.h"
}
#include <setjmp.h>
#include <vector>

class CMainWnd : public CWindowImpl<CMainWnd, CWindow>
{
public:
    CMainWnd() noexcept;
    ~CMainWnd() final;
    BOOL    CreateMainWindow();
    BOOL    PreTranslateMessage(MSG* pMsg) noexcept;

public:
    BEGIN_MSG_MAP(CMainWnd)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel) // 휠로 사진 이동
        MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)      // 키보드로 사진 이동
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)    // 배경 제거
        MESSAGE_HANDLER(WM_SIZE, OnSize)                // 사이즈 조정
        COMMAND_ID_HANDLER(IDM_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER(IDM_FILE_CLOSE, OnFileClose)
    END_MSG_MAP()

private:
    HBITMAP m_hBmp = nullptr;
    std::vector<WCHAR*> m_imageFiles; // 이미지 파일 목록 (동적 문자열)
    int m_currentIndex = -1;

    LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) noexcept;
    LRESULT OnMouseWheel(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnKeyDown(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnEraseBkgnd(UINT, WPARAM wParam, LPARAM, BOOL&);
    LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&);
    LRESULT OnFileClose(WORD, WORD, HWND, BOOL&);

    void LoadAndDisplayJpeg(LPCWSTR filename);
    void LoadAndDisplayPng(LPCWSTR filename);
    void LoadCurrentImage();

    void ClearImageList(); // 메모리 해제 함수
};
