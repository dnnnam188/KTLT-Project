#include "gpa.h"
#include "utils.h"
#include <algorithm>
#include <map>

double calcGPA(const vector<Grade>& grades) {
    if (grades.empty()) return 0.0;
    double totalWeighted = 0.0;
    int totalCredits = 0;
    for (const auto& g : grades) {
        totalWeighted += g.score * g.credit;
        totalCredits += g.credit;
    }
    if (totalCredits == 0) return 0.0;
    return totalWeighted / totalCredits;
}

vector<pair<string, double>> calcGPABySemester(const vector<Grade>& grades) {
    if (grades.empty()) return {};

    // Nhóm điểm theo kỳ
    map<string, vector<Grade>> semGrades;
    for (const auto& g : grades) {
        semGrades[g.semester].push_back(g);
    }

    // Tính GPA cho từng kỳ
    vector<pair<string, double>> result;
    for (const auto& entry : semGrades) {
        result.push_back({entry.first, calcGPA(entry.second)});
    }

    // Sắp xếp theo thứ tự thời gian
    sort(result.begin(), result.end(),
        [](const pair<string,double>& a, const pair<string,double>& b) {
            return compareSemester(a.first, b.first) < 0;
        });

    return result;
}

string classifyRank(double gpa) {
    if (gpa >= 9.0) return "Xuat Sac";
    if (gpa >= 8.0) return "Gioi";
    if (gpa >= 7.0) return "Kha";
    if (gpa >= 5.0) return "Trung Binh";
    return "Yeu";
}

void computeAllGPA(vector<Cohort>& cohorts) {
    for (auto& cohort : cohorts) {
        for (auto& student : cohort.students) {
            student.gpa = calcGPA(student.grades);
            student.rank = classifyRank(student.gpa);
        }
    }
}
