#ifndef UTILS_H
#define UTILS_H

#include "structs.h"
#include <string>
#include <vector>

using namespace std;

// Kiểm tra điểm hợp lệ: score thuộc [0, 10]
bool isValidScore(double score);

// Kiểm tra tín chỉ hợp lệ: credit > 0
bool isValidCredit(int credit);

// Kiểm tra + cảnh báo/loại bỏ dữ liệu không hợp lệ trong toàn bộ Cohort
void validateAndClean(vector<Cohort>& cohorts);

// Trích + sắp xếp danh sách kỳ học theo thứ tự thời gian
// Từ vector<Grade> lấy ra các semester duy nhất, đã sort
vector<string> getSortedSemesters(const vector<Grade>& grades);

// So sánh 2 semester string (vd: "HK1_2024" < "HK2_2024" < "HK1_2025")
// Trả về: -1 nếu a < b, 0 nếu a == b, 1 nếu a > b
int compareSemester(const string& a, const string& b);

#endif // UTILS_H
