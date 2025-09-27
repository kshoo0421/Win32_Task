# Win32_Task
Win32 기반 이미지 뷰어 +a

## 25.09.23-24
### [01] 라이브러리 빌드 및 프로젝트 연결
1. libjpeg-turbo 라이브러리 빌드 (w/Visual Studio 2022)
2. AtlWinApp.sln 프로젝트 속성 수정 및 연결

### [02] Win32 공부
1.  [VS 공식문서](https://learn.microsoft.com/ko-kr/windows/win32/learnwin32/learn-to-program-for-windows) 기반, 동작 원리 공부
2. 예제 코드 분석
- 03_OpenDialogBox까지
- 그 이후는 Direct2D 기반이기에 제외

## 25.09.25-26 
### [01] 1차 기능 완성
- JPEG 로딩 구현
- 각종 오류 수정
### [02] PNG 관련 라이브러리 빌드 및 링크
- 추가 기능 구현용 라이브러리 빌드

## 25.09.27
### [01] 라이브러리 관련 GIT 조정
- JPEG 폴더가 깃허브에 안올라가는 오류 수정
### [02] PNG 관련 기능 구현
라이브러리 - CRT 설정에 맞게 빌드
# Debug 빌드 (정적 CRT /MTd)
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebug" `
  -DPNG_SHARED=OFF -DPNG_STATIC=ON `
  -DZLIB_INCLUDE_DIR="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3" `
  -DZLIB_LIBRARY="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3/build/Debug/zsd.lib"

cmake --build . --config Debug


# Release 빌드 (정적 CRT /MT)
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded" `
  -DPNG_SHARED=OFF -DPNG_STATIC=ON `
  -DZLIB_INCLUDE_DIR="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3" `
  -DZLIB_LIBRARY="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3/build/Release/zs.lib"

cmake --build . --config Release
