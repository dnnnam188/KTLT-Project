#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>

using namespace std;

// ============================================================
// Struct cấp 3: Grade (điểm từng môn)
// ============================================================
struct Grade {
    string subject;    // Tên môn học
    double score;      // Điểm (0 - 10)
    int credit;        // Số tín chỉ (> 0)
    string semester;   // Kỳ học, vd: "HK1_2024"
};

// ============================================================
// Struct cấp 2: Student (sinh viên)
// ============================================================
struct Student {
    string id;                // MSSV
    string name;              // Họ tên
    vector<Grade> grades;     // Danh sách điểm (chứa nhiều Grade)
    double gpa;               // GPA tổng
    string rank;              // Phân loại: "Xuất Sắc", "Giỏi", ...

    Student() : gpa(0.0), rank("Chua xep loai") {} // Constructor mặc định
};

// ============================================================
// Struct cấp 1: Cohort (lớp học)
// ============================================================
struct Cohort {
    string class_id;           // Mã lớp, vd: "LOP01"
    vector<Student> students;  // Danh sách sinh viên
};

// ============================================================
// Struct phụ trợ cho thuật toán
// ============================================================

// Kết quả Kadane's Algorithm
struct KadaneResult {
    int start;       // Vị trí bắt đầu (index trong mảng GPA theo kỳ)
    int end;         // Vị trí kết thúc
    double maxSum;   // Tổng delta lớn nhất

    KadaneResult() : start(0), end(0), maxSum(0.0) {}
};

// Kết quả dãy tăng liên tiếp
struct StreakResult {
    int start;       // Vị trí bắt đầu
    int end;         // Vị trí kết thúc
    int length;      // Độ dài chuỗi tăng

    StreakResult() : start(0), end(0), length(0) {}
};

// Kết quả thống kê theo môn
struct SubjectStatEntry {
    string subject;
    double meanScore;
    double stdScore;
    double minScore;
    double maxScore;
    vector<double> zScores;      // Z-score của từng SV trong môn này
    vector<string> studentIds;   // MSSV tương ứng với zScores
};

// Kết quả thống kê theo lớp
struct ClassStatEntry {
    string classId;
    double meanGPA;
    double stdGPA;
    double minGPA;
    double maxGPA;
};

// Tập hợp thống kê theo môn
struct SubjectStats {
    vector<SubjectStatEntry> entries;
};

// Tập hợp thống kê theo lớp
struct ClassStats {
    vector<ClassStatEntry> entries;
};

#endif // STRUCTS_H
