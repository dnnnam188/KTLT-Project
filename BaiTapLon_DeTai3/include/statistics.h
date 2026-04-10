#ifndef STATISTICS_H
#define STATISTICS_H

#include "structs.h"
#include <vector>

using namespace std;

// Tính trung bình
double mean(const vector<double>& data);

// Tính độ lệch chuẩn (population std)
double stdDev(const vector<double>& data);

// Giá trị nhỏ nhất
double minVal(const vector<double>& data);

// Giá trị lớn nhất
double maxVal(const vector<double>& data);

// Chuẩn hóa Z-score: z[i] = (x[i] - mean) / std
vector<double> zScore(const vector<double>& data);

// Thống kê theo môn học: mean/std/min/max/Z-score điểm theo từng môn
SubjectStats statsBySubject(const vector<Cohort>& cohorts);

// Thống kê theo lớp: mean/std/min/max GPA theo từng lớp
ClassStats statsByClass(const vector<Cohort>& cohorts);

#endif // STATISTICS_H
