#include "String.h"
#include <cstring>
#include <algorithm>

// ���� ��ƿ: capacity ���� (2�� ���� ��å)
void String::ensure_capacity(size_t newLen) {
    if (newLen + 1 > capacity) {
        size_t newCap = std::max(newLen + 1, capacity * 2);
        char* newData = new char[newCap];
        if (data) {
            std::strcpy(newData, data);
            delete[] data;
        }
        else {
            newData[0] = '\0';
        }
        data = newData;
        capacity = newCap;
    }
}

// �⺻ ������
String::String() : data(nullptr), length(0), capacity(0) {
    reserve(16); // �ʱ� �뷮 16
}

// C ���ڿ� ������
String::String(const char* str) {
    if (str) {
        length = std::strlen(str);
        capacity = length + 1;
        data = new char[capacity];
        std::strcpy(data, str);
    }
    else {
        data = nullptr;
        length = 0;
        capacity = 0;
    }
}

// ���� ������
String::String(const String& other) : length(other.length), capacity(other.capacity) {
    if (capacity > 0) {
        data = new char[capacity];
        std::strcpy(data, other.data);
    }
    else {
        data = nullptr;
    }
}

// �̵� ������
String::String(String&& other) noexcept
    : data(other.data), length(other.length), capacity(other.capacity) {
    other.data = nullptr;
    other.length = 0;
    other.capacity = 0;
}

// �Ҹ���
String::~String() {
    delete[] data;
}

// ���� ���� ������
String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] data;
        length = other.length;
        capacity = other.capacity;
        if (capacity > 0) {
            data = new char[capacity];
            std::strcpy(data, other.data);
        }
        else {
            data = nullptr;
        }
    }
    return *this;
}

// �̵� ���� ������
String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        length = other.length;
        capacity = other.capacity;
        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;
    }
    return *this;
}

// ���ڿ� ����
String String::operator+(const String& rhs) const {
    String result(*this);
    result.append(rhs.c_str());
    return result;
}

// �� ������
bool String::operator==(const String& rhs) const {
    return std::strcmp(c_str(), rhs.c_str()) == 0;
}
bool String::operator!=(const String& rhs) const {
    return !(*this == rhs);
}

// ���� ��ȯ
size_t String::size() const noexcept {
    return length;
}

// capacity ��ȯ
size_t String::get_capacity() const noexcept {
    return capacity;
}

// C ���ڿ� ������ ��ȯ
const char* String::c_str() const noexcept {
    return data ? data : "";
}

// reserve
void String::reserve(size_t newCap) {
    if (newCap <= length + 1) return; // ���� ����
    if (newCap > capacity) {
        char* newData = new char[newCap];
        if (data) {
            std::strcpy(newData, data);
            delete[] data;
        }
        else {
            newData[0] = '\0';
        }
        data = newData;
        capacity = newCap;
    }
}

// append
void String::append(const char* str) {
    if (!str) return;
    size_t addLen = std::strlen(str);
    ensure_capacity(length + addLen);
    std::strcat(data, str);
    length += addLen;
}

// �ε��� ���� (���� ����, ���� üũ ����)
char& String::operator[](size_t index) {
    return data[index];
}
const char& String::operator[](size_t index) const {
    return data[index];
}

// �ε��� ���� (���� üũ ����)
char& String::at(size_t index) {
    if (index >= length) throw std::out_of_range("String::at(): index out of range");
    return data[index];
}
const char& String::at(size_t index) const {
    if (index >= length) throw std::out_of_range("String::at(): index out of range");
    return data[index];
}

// ��� ������
std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}
