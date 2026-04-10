#include "string_utils.h"
#include <cctype>

// Xóa khoảng trắng đầu và cuối
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Viết hoa chữ cái đầu mỗi từ, còn lại viết thường
string toTitleCase(const string& s) {
    string result = s;
    bool newWord = true;
    for (size_t i = 0; i < result.size(); i++) {
        if (isspace((unsigned char)result[i])) {
            newWord = true;
        } else if (newWord) {
            result[i] = toupper((unsigned char)result[i]);
            newWord = false;
        } else {
            result[i] = tolower((unsigned char)result[i]);
        }
    }
    return result;
}

// Kết hợp trim + xóa khoảng trắng thừa giữa các từ + title case
string normalizeName(const string& name) {
    string trimmed = trim(name);
    // Xóa khoảng trắng thừa giữa các từ
    string cleaned;
    bool prevSpace = false;
    for (char c : trimmed) {
        if (isspace((unsigned char)c)) {
            if (!prevSpace) cleaned += ' ';
            prevSpace = true;
        } else {
            cleaned += c;
            prevSpace = false;
        }
    }
    return toTitleCase(cleaned);
}
