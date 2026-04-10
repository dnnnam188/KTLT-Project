#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "structs.h"
#include <vector>
#include <string>

using namespace std;

// Kadane's Algorithm trên mảng delta (GPA_kỳ - GPA_TB)
// Tìm đoạn kỳ học tốt nhất so với trung bình của chính SV đó
KadaneResult kadaneMaxGPA(const vector<double>& deltaGPAs);

// Tìm chuỗi kỳ GPA tăng liên tiếp dài nhất
// Trả StreakResult { start, end, length }
StreakResult longestIncreasingStreak(const vector<double>& semGPAs);

// Sliding Window: tính GPA trung bình trượt với cửa sổ k kỳ
vector<double> slidingAvgGPA(const vector<double>& semGPAs, int k);

// Phát hiện xu hướng sụt giảm: khi có >= 2 giá trị trung bình trượt liên tiếp giảm
bool detectDecline(const vector<double>& slidingAvgs);

// Prefix Sum: tín chỉ tích lũy theo kỳ (đã sắp xếp)
vector<int> prefixSumCredits(const vector<Grade>& grades, const vector<string>& semesters);

// Truy vấn tổng tín chỉ đoạn [l, r] trong O(1)
// l, r là index (0-based) trong mảng prefix
int queryCredits(const vector<int>& prefix, int l, int r);

#endif // ALGORITHMS_H
