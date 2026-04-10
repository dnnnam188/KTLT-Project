#ifndef GPA_H
#define GPA_H

#include "structs.h"
#include <string>
#include <vector>
#include <utility> // pair

using namespace std;

// Tính GPA tổng = sum(score * credit) / sum(credit)
double calcGPA(const vector<Grade>& grades);

// Tính GPA theo từng kỳ, đã sắp xếp theo thứ tự thời gian
// Trả về vector<pair<semester, gpa>> thay vì map để đảm bảo thứ tự
vector<pair<string, double>> calcGPABySemester(const vector<Grade>& grades);

// Phân loại học lực theo GPA
// >= 9.0: "Xuất Sắc", >= 8.0: "Giỏi", >= 7.0: "Khá", >= 5.0: "Trung Bình", < 5.0: "Yếu"
string classifyRank(double gpa);

// Tính GPA + rank cho toàn bộ SV trong tất cả Cohort
void computeAllGPA(vector<Cohort>& cohorts);

#endif // GPA_H
