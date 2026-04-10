#ifndef FILE_IO_H
#define FILE_IO_H

#include "structs.h"
#include <string>
#include <vector>

using namespace std;

// Đọc dữ liệu từ grades.csv và class.txt, nhóm thành Cohort -> Student -> Grade
vector<Cohort> readData(const string& gradesFile, const string& classFile);

// Tách dòng CSV bằng dấu phẩy
vector<string> splitCSVLine(const string& line);

// Ghi file gpa_report.csv
void writeGPAReport(const string& path, vector<Cohort>& cohorts);

// Ghi file progress.txt (Kadane, dãy tăng, Sliding Window, Prefix Sum)
void writeProgress(const string& path, vector<Cohort>& cohorts, int k);

// Ghi file ranking.csv (sắp xếp GPA giảm dần)
void writeRanking(const string& path, vector<Cohort>& cohorts);

// Ghi file statistics.txt (thống kê theo môn + lớp + Z-score)
void writeStatistics(const string& path, vector<Cohort>& cohorts);

#endif // FILE_IO_H
