#include "utils.h"
#include <algorithm>
#include <set>
#include <iostream>
#include <sstream>

using namespace std;

bool isValidScore(double score) {
    return score >= 0.0 && score <= 10.0;
}

bool isValidCredit(int credit) {
    return credit > 0;
}

// Parse semester "HKx_yyyy" thành (year, hk_number)
static pair<int, int> parseSemester(const string& sem) {
    int hk = 0, year = 0;
    size_t underscorePos = sem.find('_');
    if (underscorePos != string::npos && sem.size() > 2 && sem.substr(0, 2) == "HK") {
        try {
            hk = stoi(sem.substr(2, underscorePos - 2));
            year = stoi(sem.substr(underscorePos + 1));
        } catch (...) {
            // Nếu parse lỗi, giữ giá trị mặc định 0
        }
    }
    return {year, hk};
}

int compareSemester(const string& a, const string& b) {
    auto pA = parseSemester(a);
    auto pB = parseSemester(b);
    if (pA.first != pB.first) return (pA.first < pB.first) ? -1 : 1;
    if (pA.second != pB.second) return (pA.second < pB.second) ? -1 : 1;
    return 0;
}

vector<string> getSortedSemesters(const vector<Grade>& grades) {
    set<string> semSet;
    for (const auto& g : grades) {
        semSet.insert(g.semester);
    }
    vector<string> semesters(semSet.begin(), semSet.end());
    sort(semesters.begin(), semesters.end(), [](const string& a, const string& b) {
        return compareSemester(a, b) < 0;
    });
    return semesters;
}

void validateAndClean(vector<Cohort>& cohorts) {
    for (auto& cohort : cohorts) {
        for (auto& student : cohort.students) {
            vector<Grade> validGrades;
            for (const auto& g : student.grades) {
                bool valid = true;
                if (!isValidScore(g.score)) {
                    cout << "[CẢNH BÁO] " << student.id << " - Môn " << g.subject
                         << ": điểm " << g.score << " không hợp lệ (0-10). Bỏ qua." << endl;
                    valid = false;
                }
                if (!isValidCredit(g.credit)) {
                    cout << "[CẢNH BÁO] " << student.id << " - Môn " << g.subject
                         << ": tín chỉ " << g.credit << " không hợp lệ (>0). Bỏ qua." << endl;
                    valid = false;
                }
                if (valid) {
                    validGrades.push_back(g);
                }
            }
            student.grades = validGrades;
        }
    }
}
