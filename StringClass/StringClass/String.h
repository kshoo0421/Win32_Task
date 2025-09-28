#pragma once
#include <cstddef>
#include <iostream>
#include <stdexcept> // for std::out_of_range

class String {
private:
    char* data;        // ���ڿ� ���� ����
    size_t length;     // ���� ���ڿ� ����
    size_t capacity;   // ���� �뷮 (null ���� ����)

    void ensure_capacity(size_t newLen);

public:
    // ������ & �Ҹ���
    String();
    String(const char* str);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    // ������
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String operator+(const String& rhs) const;
    bool operator==(const String& rhs) const;
    bool operator!=(const String& rhs) const;

    // ��� �Լ�
    size_t size() const noexcept;
    size_t get_capacity() const noexcept;
    const char* c_str() const noexcept;

    void reserve(size_t newCap);
    void append(const char* str);

    // �ε��� ����
    char& operator[](size_t index);
    const char& operator[](size_t index) const;
    char& at(size_t index);              // ���� üũ ����
    const char& at(size_t index) const;  // ���� üũ ����

    // ��� ������ (friend)
    friend std::ostream& operator<<(std::ostream& os, const String& str);
};
