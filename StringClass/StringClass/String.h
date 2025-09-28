#pragma once
#include <cstddef>
#include <iostream>
#include <stdexcept> // for std::out_of_range

class String {
private:
    char* data;        // 문자열 저장 버퍼
    size_t length;     // 실제 문자열 길이
    size_t capacity;   // 버퍼 용량 (null 포함 공간)

    void ensure_capacity(size_t newLen);

public:
    // 생성자 & 소멸자
    String();
    String(const char* str);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    // 연산자
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String operator+(const String& rhs) const;
    bool operator==(const String& rhs) const;
    bool operator!=(const String& rhs) const;

    // 멤버 함수
    size_t size() const noexcept;
    size_t get_capacity() const noexcept;
    const char* c_str() const noexcept;

    void reserve(size_t newCap);
    void append(const char* str);

    // 인덱스 접근
    char& operator[](size_t index);
    const char& operator[](size_t index) const;
    char& at(size_t index);              // 범위 체크 버전
    const char& at(size_t index) const;  // 범위 체크 버전

    // 출력 연산자 (friend)
    friend std::ostream& operator<<(std::ostream& os, const String& str);
};
