#include "file_io.h"
#include "string_utils.h"
#include "utils.h"
#include "gpa.h"
#include "algorithms.h"
#include "statistics.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

// TÁCH DÒNG CSV
vector<string> splitCSVLine(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string field;
    while (getline(ss, field, ',')) {
        result.push_back(trim(field));
    }
    return result;
}

// ĐỌC DỮ LIỆU TỪ FILE
vector<Cohort> readData(const string& gradesFile, const string& classFile) {
    // Bước 1: Đọc class.txt
    map<string, string> studentToClass; // Chứa MSSV -> Lớp
    map< string, vector<string> > classToStudents; // Chứa Lớp -> Danh sách MSSV
    ifstream classIn(classFile);
    if (!classIn.is_open()) {
        cout << "[LỖI] Không thể mở file: " << classFile << endl;
        return {};
    }

    string currentClass = "";
    string line;
    while (getline(classIn, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (line.substr(0, 3) == "LOP") {
            currentClass = line;
        } else {
            studentToClass[line] = currentClass;
            classToStudents[currentClass].push_back(line);
        }
    }
    classIn.close();


    // Bước 2: Đọc grades.csv
    map<string, string> studentNames;
    map<string, vector<Grade>> studentGrades;

    ifstream gradesIn(gradesFile);
    if (!gradesIn.is_open()) {
        cout << "[LỖI] Không thể mở file: " << gradesFile << endl;
        return {};
    }

    bool isHeader = true;
    while (getline(gradesIn, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        if (isHeader) { isHeader = false; continue; }

        vector<string> fields = splitCSVLine(line);
        if (fields.size() < 6) {
            cout << "[CẢNH BÁO] Dòng không đủ trường: " << line << endl;
            continue;
        }

        string studentId = fields[0];
        string name = normalizeName(fields[1]);
        string subject = fields[2];

        double score = 0;
        int credit = 0;
        try {
            score = stod(fields[3]);
            credit = stoi(fields[4]);
        } catch (...) {
            cout << "[CẢNH BÁO] Không parse được: " << line << endl;
            continue;
        }

        Grade grade;
        grade.subject = subject;
        grade.score = score;
        grade.credit = credit;
        grade.semester = fields[5];

        studentNames[studentId] = name;
        studentGrades[studentId].push_back(grade);
    }
    gradesIn.close();

    // Bước 3: Xây dựng Cohort
    map<string, Cohort> cohortMap;

    for (auto& entry : classToStudents) {
        Cohort cohort;
        cohort.class_id = entry.first;
        for (const string& sid : entry.second) {
            Student s;
            s.id = sid;
            s.name = studentNames.count(sid) ? studentNames[sid] : "Unknown";
            if (studentGrades.count(sid)) s.grades = studentGrades[sid];
            cohort.students.push_back(s);
        }
        cohortMap[entry.first] = cohort;
    }

    // SV trong grades.csv nhưng không có trong class.txt
    for (auto& entry : studentGrades) {
        if (studentToClass.find(entry.first) == studentToClass.end()) {
            cout << "[CẢNH BÁO] " << entry.first << " không thuộc lớp nào. Gom vào UNKNOWN." << endl;
            if (cohortMap.find("UNKNOWN") == cohortMap.end()) {
                Cohort u; u.class_id = "UNKNOWN"; cohortMap["UNKNOWN"] = u;
            }
            Student s;
            s.id = entry.first;
            s.name = studentNames.count(entry.first) ? studentNames[entry.first] : "Unknown";
            s.grades = entry.second;
            cohortMap["UNKNOWN"].students.push_back(s);
        }
    }

    vector<Cohort> result;
    for (auto& entry : cohortMap) result.push_back(entry.second);
    return result;
}

// GHI GPA_REPORT.CSV
void writeGPAReport(const string& path, vector<Cohort>& cohorts) {
    ofstream out(path);
    if (!out.is_open()) { cout << "[LỖI] Không thể tạo: " << path << endl; return; }

    out << "student_id,name,class,gpa,rank" << endl;
    out << fixed << setprecision(2);

    for (const auto& c : cohorts)
        for (const auto& s : c.students)
            out << s.id << "," << s.name << "," << c.class_id << "," << s.gpa << "," << s.rank << endl;

    out.close();
    cout << "[OK] Đã ghi: " << path << endl;
}

// GHI PROGRESS.TXT
void writeProgress(const string& path, vector<Cohort>& cohorts, int k) {
    ofstream out(path);
    if (!out.is_open()) { cout << "[LỖI] Không thể tạo: " << path << endl; return; }
    out << fixed << setprecision(2);

    for (const auto& cohort : cohorts) {
        for (const auto& student : cohort.students) {
            out << "=== " << student.id << " - " << student.name << " ===" << endl;

            auto semGPAs = calcGPABySemester(student.grades);
            if (semGPAs.empty()) {
                out << "Không có dữ liệu điểm." << endl << endl;
                continue;
            }

            // GPA theo kỳ
            vector<double> gpaVals;
            vector<string> semNames;
            out << "GPA theo kỳ: ";
            for (int i = 0; i < (int)semGPAs.size(); i++) {
                if (i > 0) out << ", ";
                out << semGPAs[i].first << "=" << semGPAs[i].second;
                gpaVals.push_back(semGPAs[i].second);
                semNames.push_back(semGPAs[i].first);
            }
            out << endl;

            // Delta GPA
            double avg = 0;
            for (double g : gpaVals) avg += g;
            avg /= gpaVals.size();

            vector<double> delta;
            out << "Delta GPA (so với TB=" << avg << "): ";
            for (int i = 0; i < (int)gpaVals.size(); i++) {
                double d = gpaVals[i] - avg;
                delta.push_back(d);
                if (i > 0) out << ", ";
                if (d >= 0) out << "+";
                out << d;
            }
            out << endl;

            // Kadane
            KadaneResult kadane = kadaneMaxGPA(delta);
            out << "Đoạn kỳ học tốt nhất (Kadane): ["
                << semNames[kadane.start] << " -> " << semNames[kadane.end]
                << "], delta = ";
            if (kadane.maxSum >= 0) out << "+";
            out << kadane.maxSum << endl;

            // Dãy tăng liên tiếp
            StreakResult streak = longestIncreasingStreak(gpaVals);
            out << "Chuỗi GPA tăng dài nhất: " << streak.length << " kỳ liên tiếp";
            if (streak.length > 1)
                out << " [" << semNames[streak.start] << " -> " << semNames[streak.end] << "]";
            out << endl;

            // Sliding Window
            if ((int)gpaVals.size() >= k) {
                vector<double> avgs = slidingAvgGPA(gpaVals, k);
                out << "GPA trung bình trượt (k=" << k << "): ";
                for (int i = 0; i < (int)avgs.size(); i++) {
                    if (i > 0) out << ", ";
                    out << avgs[i];
                }
                out << endl;
                out << "Cảnh báo sụt giảm: " << (detectDecline(avgs) ? "Có" : "Không") << endl;
            } else {
                out << "GPA trung bình trượt: không đủ kỳ (cần >= " << k << ")" << endl;
                out << "Cảnh báo sụt giảm: Không đủ dữ liệu" << endl;
            }

            // Prefix Sum tín chỉ
            vector<string> sortedSems = getSortedSemesters(student.grades);
            vector<int> prefix = prefixSumCredits(student.grades, sortedSems);
            out << "Tín chỉ tích lũy: ";
            for (int i = 0; i < (int)sortedSems.size(); i++) {
                if (i > 0) out << ", ";
                int total = queryCredits(prefix, 0, i);
                if (i == 0) out << sortedSems[i] << "=" << total;
                else out << sortedSems[0] << "-" << sortedSems[i] << "=" << total;
            }
            out << endl << endl;
        }
    }
    out.close();
    cout << "[OK] Đã ghi: " << path << endl;
}

// GHI RANKING.CSV
void writeRanking(const string& path, vector<Cohort>& cohorts) {
    struct Entry { string id, name, classId, rank; double gpa; };
    vector<Entry> all;

    for (const auto& c : cohorts)
        for (const auto& s : c.students)
            all.push_back({s.id, s.name, c.class_id, s.rank, s.gpa});

    sort(all.begin(), all.end(), [](const Entry& a, const Entry& b) { return a.gpa > b.gpa; });

    ofstream out(path);
    if (!out.is_open()) { cout << "[LỖI] Không thể tạo: " << path << endl; return; }

    out << "rank,student_id,name,class,gpa,classif      ication" << endl;
    out << fixed << setprecision(2);
    for (int i = 0; i < (int)all.size(); i++)
        out << (i+1) << "," << all[i].id << "," << all[i].name << ","
            << all[i].classId << "," << all[i].gpa << "," << all[i].rank << endl;

    out.close();
    cout << "[OK] Đã ghi: " << path << endl;
}

// GHI STATISTICS.TXT
void writeStatistics(const string& path, vector<Cohort>& cohorts) {
    ofstream out(path);
    if (!out.is_open()) { cout << "[LỖI] Không thể tạo: " << path << endl; return; }
    out << fixed << setprecision(2);

    SubjectStats subStats = statsBySubject(cohorts);
    out << "   THỐNG KÊ THEO MÔN HỌC" << endl;
    out << string(61, '-') << endl;
    out << "| " << left << setw(22) << "Môn học"
        << "| " << setw(7) << "Mean" 
        << "| " << setw(7) << "Std" 
        << "| " << setw(7) << "Min" 
        << "| " << setw(7) << "Max" << " |" << endl;
    out << string(61, '-') << endl;
    for (const auto& e : subStats.entries)
        out << "| " << left << setw(22) << e.subject 
            << "| " << setw(7) << fixed << setprecision(2) << e.meanScore 
            << "| " << setw(7) << e.stdScore 
            << "| " << setw(7) << e.minScore 
            << "| " << setw(7) << e.maxScore << " |" << endl;
    out << string(61, '-') << endl << endl;

    ClassStats clsStats = statsByClass(cohorts);
    out << "   THỐNG KÊ THEO LỚP" << endl;
    out << string(61, '-') << endl;
    out << "| " << left << setw(22) << "Lớp"
        << "| " << setw(7) << "Mean" 
        << "| " << setw(7) << "Std" 
        << "| " << setw(7) << "Min" 
        << "| " << setw(7) << "Max" << " |" << endl;
    out << string(61, '-') << endl;
    for (const auto& e : clsStats.entries)
        out << "| " << left << setw(22) << e.classId 
            << "| " << setw(7) << fixed << setprecision(2) << e.meanGPA 
            << "| " << setw(7) << e.stdGPA 
            << "| " << setw(7) << e.minGPA 
            << "| " << setw(7) << e.maxGPA << " |" << endl;
    out << string(61, '-') << endl << endl;

    out << "   Z-SCORE NGOẠI LỆ (|Z| >= 2.0)" << endl;
    out << string(61, '-') << endl;
    for (const auto& e : subStats.entries) {
        out << "  " << left << setw(22) << e.subject << " | ";
        bool hasOutlier = false;
        for (int i = 0; i < (int)e.studentIds.size(); i++) {
            if (e.zScores[i] >= 2.0 || e.zScores[i] <= -2.0) {
                if (hasOutlier) out << ", ";
                out << e.studentIds[i] << " (";
                if (e.zScores[i] >= 0) out << "+";
                out << e.zScores[i] << ")";
                hasOutlier = true;
            }
        }
        if (!hasOutlier) out << "(Không có)";
        out << endl;
    }
    out << string(61, '-') << endl;

    out.close();
    cout << "[OK] Đã ghi: " << path << endl;
}
