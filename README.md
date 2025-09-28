# Win32_Task
## Win32 기반 이미지 뷰어 + String Class 구현

**폴더**

- LearnWin32 : VS 공식 문서 기반 Win32 공부용 프로젝트
- AtlWin32 : 이미지 뷰어 프로젝트
- StringClass : String 클래스 구현 및 유닛 테스트 진행

**기타**
- AI 검색을 활용하되, 코드 작성 및 디버깅은 직접 진행
- 함수의 응용 방법 등은 추가로 공부 중

---

<br><br>

## AtlWinApp 라이브러리 빌드 옵션
### [01] libjpeg-turbo
``` powershell
mkdir build
cd build

# Release
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded"

cmake --build . --config Release

# Debug
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebug"

cmake --build . --config Debug
```

### [02] zlib 1.3
``` powershell
# 새 build 디렉터리 생성
mkdir build
cd build

# Release
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded"

cmake --build . --config Release

# Debug
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebug"

cmake --build . --config Debug
```

### [03] libpng 1.6.x
``` powershell
mkdir build
cd build

# Release
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DPNG_SHARED=OFF -DPNG_STATIC=ON `
  -DZLIB_INCLUDE_DIR="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3" `
  -DZLIB_LIBRARY="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3/build/Release/zs.lib" `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded"

cmake --build . --config Release

# Debug
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DPNG_SHARED=OFF -DPNG_STATIC=ON `
  -DZLIB_INCLUDE_DIR="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3" `
  -DZLIB_LIBRARY="C:/Users/shmd02/Desktop/Coding/Win32_Task/AtlWinApp/zlib-1.3/build/Debug/zsd.lib" `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebug"

cmake --build . --config Debug
```

---

<br><br>

# 일지
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
- 라이브러리 - CRT 설정에 맞게 빌드

## 25.09.28
### [01] String Class 구현
- 일반 프로그램 + 유닛테스트 구현
- 일반 프로그램 : 메모리누수 체크 => _CrtDumpMemoryLeaks(); 함수 활용
- 유닛 테스트 : 클래스 내 함수 동작 확인

### [02] README 조정
- 프로젝트 총 정리