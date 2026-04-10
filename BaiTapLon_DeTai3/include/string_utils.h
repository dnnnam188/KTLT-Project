#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

using namespace std;

// Xóa khoảng trắng đầu và cuối chuỗi
string trim(const string& s);

// Viết hoa chữ cái đầu mỗi từ
string toTitleCase(const string& s);

// Kết hợp trim + title case
string normalizeName(const string& name);

#endif // STRING_UTILS_H
