#include "structs.h"
#include "file_io.h"
#include "gpa.h"
#include "algorithms.h"
#include "statistics.h"
#include "string_utils.h"
#include "utils.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

void printMenu() {
    cout << endl;
    cout << "==========================================================================" << endl;
    cout << "                     HỆ THỐNG QUẢN LÝ ĐIỂM SINH VIÊN" << endl;
    cout << "                           Đề Tài 3 - BTL KTLT" << endl;
    cout << "==========================================================================" << endl;
    cout << "  1. Đọc dữ liệu từ file" << endl;
    cout << "     -> Nạp dữ liệu điểm (grades.csv) và danh sách lớp (class.txt) từ thư mục test." << endl;
    cout << "  2. Tính GPA & Phân loại học lực" << endl;
    cout << "     -> Tính điểm trung bình (GPA) và xếp loại học lực cho từng sinh viên." << endl;
    cout << "  3. Phân tích tiến bộ (Kadane + Dãy tăng)" << endl;
    cout << "     -> Tìm giai đoạn kỳ học bứt phá nhất và chuỗi số kỳ học tăng điểm liên tiếp." << endl;
    cout << "  4. Phân tích xu hướng (Sliding Window)" << endl;
    cout << "     -> Tìm trung bình trượt của k kỳ liên tiếp và theo dõi cảnh báo học lực sụt giảm." << endl;
    cout << "  5. Truy vấn tín chỉ tích lũy (Prefix Sum)" << endl;
    cout << "     -> Tra cứu tổng số tín chỉ tích lũy đạt được qua từng học kỳ (đã qua O(1) tra cứu)." << endl;
    cout << "  6. Thống kê mô tả (mean/std/min/max/Z-score)" << endl;
    cout << "     -> Hiển thị thống kê chuyên sâu về điểm theo môn học và lớp, tính z-score." << endl;
    cout << "  7. Xuất báo cáo (ghi tất cả file output)" << endl;
    cout << "     -> Lưu tất cả kết quả tính toán trên ra các file báo cáo (.txt và .csv)." << endl;
    cout << "  0. Thoát" << endl;
    cout << "     -> Đóng chương trình." << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    cout << "  Lưu ý: cần thực hiện bước 1, 2 trước khi thực hiện các chức năng khác" << endl;
    cout << "==========================================================================" << endl;
    cout << "  Lựa chọn: ";
}

int main() {
    vector<Cohort> cohorts;
    bool dataLoaded = false;
    bool gpaComputed = false;
    int k = 2;
    string outputDir = "output/";
    int choice;

    do {
        printMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
        // 1. ĐỌC DỮ LIỆU
        case 1: {
            cout << "Chọn bộ test (1/2/3): ";
            int testNum;
            cin >> testNum;
            cin.ignore();
            string dataDir = "data/test" + to_string(testNum) + "/";

            cout << "Đang đọc dữ liệu từ " << dataDir << "..." << endl;
            cohorts = readData(dataDir + "grades.csv", dataDir + "class.txt");

            if (cohorts.empty()) {
                cout << "[LỖI] Không đọc được dữ liệu!" << endl;
            } else {
                validateAndClean(cohorts);
                for (auto& c : cohorts)
                    for (auto& s : c.students)
                        s.name = normalizeName(s.name);

                int total = 0;
                for (const auto& c : cohorts) total += (int)c.students.size();
                cout << "[OK] Đã đọc " << cohorts.size() << " lớp, " << total << " sinh viên." << endl;
                dataLoaded = true;
                gpaComputed = false;
            }
            break;
        }

        // 2. TÍNH GPA & PHÂN LOẠI
        case 2: {
            if (!dataLoaded) { cout << "[LỖI] Chưa đọc dữ liệu! Chọn 1 trước." << endl; break; }

            computeAllGPA(cohorts);
            gpaComputed = true;

            cout << endl << left << setw(8) << "MSSV" << setw(25) << "Họ Tên"
                 << setw(10) << "Lớp" << setw(8) << "GPA" << "Xếp Loại" << endl;
            cout << string(60, '-') << endl;

            for (const auto& c : cohorts)
                for (const auto& s : c.students)
                    cout << left << setw(8) << s.id << setw(25) << s.name
                         << setw(10) << c.class_id << fixed << setprecision(2)
                         << setw(8) << s.gpa << s.rank << endl;
            break;
        }

        // 3. KADANE + DÃY TĂNG
        case 3: {
            if (!gpaComputed) { cout << "[LỖI] Chưa tính GPA! Chọn 2 trước." << endl; break; }

            cout << endl;
            for (const auto& c : cohorts) {
                for (const auto& s : c.students) {
                    auto semGPAs = calcGPABySemester(s.grades);
                    if (semGPAs.empty()) continue;

                    vector<double> vals;
                    vector<string> names;
                    for (const auto& p : semGPAs) { vals.push_back(p.second); names.push_back(p.first); }

                    double avg = 0;
                    for (double g : vals) avg += g;
                    avg /= vals.size();

                    vector<double> delta;
                    for (double g : vals) delta.push_back(g - avg);

                    KadaneResult kadane = kadaneMaxGPA(delta);
                    StreakResult streak = longestIncreasingStreak(vals);

                    cout << "=== " << s.id << " - " << s.name << " ===" << endl;
                    cout << "  Kadane: [" << names[kadane.start] << " -> " << names[kadane.end]
                         << "], delta = " << fixed << setprecision(2);
                    if (kadane.maxSum >= 0) cout << "+";
                    cout << kadane.maxSum << endl;
                    cout << "  Dãy tăng: " << streak.length << " kỳ";
                    if (streak.length > 1)
                        cout << " [" << names[streak.start] << " -> " << names[streak.end] << "]";
                    cout << endl << endl;
                }
            }
            break;
        }

        // 4. SLIDING WINDOW
        case 4: {
            if (!gpaComputed) { cout << "[LỖI] Chưa tính GPA! Chọn 2 trước." << endl; break; }

            cout << "Nhập kích thước cửa sổ k (mặc định 2): ";
            cin >> k;
            cin.ignore();
            if (k <= 0) k = 2;

            cout << endl;
            for (const auto& c : cohorts) {
                for (const auto& s : c.students) {
                    auto semGPAs = calcGPABySemester(s.grades);
                    if (semGPAs.empty()) continue;

                    vector<double> vals;
                    for (const auto& p : semGPAs) vals.push_back(p.second);

                    cout << "=== " << s.id << " ===" << endl;
                    if ((int)vals.size() >= k) {
                        auto avgs = slidingAvgGPA(vals, k);
                        cout << "  TB trượt (k=" << k << "): ";
                        for (int i = 0; i < (int)avgs.size(); i++) {
                            if (i > 0) cout << ", ";
                            cout << fixed << setprecision(2) << avgs[i];
                        }
                        cout << endl;
                        cout << "  Sụt giảm: " << (detectDecline(avgs) ? "Có" : "Không") << endl;
                    } else {
                        cout << "  Không đủ kỳ (cần >= " << k << ")" << endl;
                    }
                    cout << endl;
                }
            }
            break;
        }

        // 5. PREFIX SUM
        case 5: {
            if (!dataLoaded) { cout << "[LỖI] Chưa đọc dữ liệu! Chọn 1 trước." << endl; break; }

            cout << endl;
            for (const auto& c : cohorts) {
                for (const auto& s : c.students) {
                    if (s.grades.empty()) continue;

                    vector<string> sems = getSortedSemesters(s.grades);
                    vector<int> prefix = prefixSumCredits(s.grades, sems);

                    cout << "=== " << s.id << " - " << s.name << " ===" << endl;
                    cout << "  Tín chỉ tích lũy: ";
                    for (int i = 0; i < (int)sems.size(); i++) {
                        if (i > 0) cout << ", ";
                        cout << sems[0] << "-" << sems[i] << "=" << queryCredits(prefix, 0, i);
                    }
                    cout << endl << endl;
                }
            }
            break;
        }

        // 6. THỐNG KÊ MÔ TẢ
        case 6: {
            if (!gpaComputed) { cout << "[LỖI] Chưa tính GPA! Chọn 2 trước." << endl; break; }

            SubjectStats sub = statsBySubject(cohorts);
            cout << endl << "   THỐNG KÊ THEO MÔN HỌC" << endl;
            cout << string(61, '-') << endl;
            cout << "| " << left << setw(22) << "Môn học"
                 << "| " << setw(7) << "Mean" 
                 << "| " << setw(7) << "Std" 
                 << "| " << setw(7) << "Min" 
                 << "| " << setw(7) << "Max" << " |" << endl;
            cout << string(61, '-') << endl;
            for (const auto& e : sub.entries)
                cout << "| " << left << setw(22) << e.subject 
                     << "| " << setw(7) << fixed << setprecision(2) << e.meanScore 
                     << "| " << setw(7) << e.stdScore 
                     << "| " << setw(7) << e.minScore 
                     << "| " << setw(7) << e.maxScore << " |" << endl;
            cout << string(61, '-') << endl;

            ClassStats cls = statsByClass(cohorts);
            cout << endl << "   THỐNG KÊ THEO LỚP" << endl;
            cout << string(61, '-') << endl;
            cout << "| " << left << setw(22) << "Lớp"
                 << "| " << setw(7) << "Mean" 
                 << "| " << setw(7) << "Std" 
                 << "| " << setw(7) << "Min" 
                 << "| " << setw(7) << "Max" << " |" << endl;
            cout << string(61, '-') << endl;
            for (const auto& e : cls.entries)
                cout << "| " << left << setw(22) << e.classId 
                     << "| " << setw(7) << fixed << setprecision(2) << e.meanGPA 
                     << "| " << setw(7) << e.stdGPA 
                     << "| " << setw(7) << e.minGPA 
                     << "| " << setw(7) << e.maxGPA << " |" << endl;
            cout << string(61, '-') << endl;

            cout << endl << "   Z-SCORE NGOẠI LỆ (|Z| >= 2.0)" << endl;
            cout << string(61, '-') << endl;
            for (const auto& e : sub.entries) {
                cout << "  " << left << setw(22) << e.subject << " | ";
                bool hasOutlier = false;
                for (int i = 0; i < (int)e.studentIds.size(); i++) {
                    if (e.zScores[i] >= 2.0 || e.zScores[i] <= -2.0) {
                        if (hasOutlier) cout << ", ";
                        cout << e.studentIds[i] << " (";
                        if (e.zScores[i] >= 0) cout << "+";
                        cout << fixed << setprecision(2) << e.zScores[i] << ")";
                        hasOutlier = true;
                    }
                }
                if (!hasOutlier) cout << "(Không có)";
                cout << endl;
            }
            cout << string(61, '-') << endl;
            break;
        }

        // 7. XUẤT BÁO CÁO
        case 7: {
            if (!gpaComputed) { cout << "[LỖI] Chưa tính GPA! Chọn 2 trước." << endl; break; }

            writeGPAReport(outputDir + "gpa_report.csv", cohorts);
            writeProgress(outputDir + "progress.txt", cohorts, k);
            writeRanking(outputDir + "ranking.csv", cohorts);
            writeStatistics(outputDir + "statistics.txt", cohorts);

            cout << endl << "Đã xuất tất cả báo cáo vào: " << outputDir << endl;
            break;
        }

        case 0:
            cout << "Xong" << endl;
            break;

        default:
            cout << "Lựa chọn không hợp lệ!" << endl;
        }
    } while (choice != 0);

    return 0;
}
