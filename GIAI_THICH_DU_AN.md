# 📚 GIẢI THÍCH DỰ ÁN: HỆ THỐNG QUẢN LÝ ĐIỂM VÀ PHÂN TÍCH HỌC LỰC

## 📋 TỔNG QUAN DỰ ÁN

Đây là **Hệ thống quản lý điểm và phân tích học lực sinh viên** viết bằng **C++17**. Chương trình đọc dữ liệu từ file CSV, tính GPA, phân tích tiến bộ học tập bằng các thuật toán hiện đại, và xuất báo cáo chi tiết. Sử dụng **7 thuật toán quan trọng** trong Khoa Học Máy Tính.

---

## 📁 CẤU TRÚC THƯ MỤC VÀ GIẢI THÍCH

```
BaiTapLon_DeTai3/
├── include/           ← Tất cả file header (.h) - chứa khai báo hàm & struct
├── src/               ← Tất cả file code (.cpp) - chứa thực hiện hàm
├── data/test1,2,3/    ← Dữ liệu đầu vào (3 bộ test)
├── output/            ← Kết quả xuất ra (4 file báo cáo)
├── main.cpp           ← Chương trình chính (menu và xử lý lựa chọn)
├── Makefile           ← Tập lệnh biên dịch tự động
├── generate_small_mock_data.py  ← Script Python tạo dữ liệu test
└── README.md          ← Hướng dẫn sử dụng
```

---

## 🏗️ CẤP ĐỘ DỮ LIỆU - STRUCT LỒNG 3 CẤP

### **Cấu trúc lồng 3 cấp:**

```cpp
// ============================================================
// Cấp 3: Grade (một điểm môn học)
// ============================================================
struct Grade {
    string subject;    // Tên môn: "Lập Trình C++"
    double score;      // Điểm: 8.5 (từ 0 đến 10)
    int credit;        // Tín chỉ: 4 (phải > 0)
    string semester;   // Kỳ học: "HK1_2020"
};

// ============================================================
// Cấp 2: Student (một sinh viên)
// ============================================================
struct Student {
    string id;              // MSSV: "SV_1_0001"
    string name;            // Họ tên: "Bùi Tuấn Phúc"
    vector<Grade> grades;   // Danh sách các môn (LỒNG Grade)
    double gpa;             // GPA tổng: 6.8
    string rank;            // Xếp loại: "Khá"
};

// ============================================================
// Cấp 1: Cohort (một lớp học)
// ============================================================
struct Cohort {
    string class_id;           // Mã lớp: "LOP1_01"
    vector<Student> students;  // Danh sách sinh viên (LỒNG Student)
};
```

**Ví dụ cụ thể:**
- Cohort = `LOP1_01` (Lớp)
  - Student = `SV_1_0001` (Sinh viên Bùi Tuấn Phúc)
    - Grade 1: Lập Trình C++, 7.3 điểm, 4 tín chỉ, HK1_2020
    - Grade 2: Mac - Lenin, 8.6 điểm, 2 tín chỉ, HK1_2020
    - Grade 3: Phát Triển Web, 7.4 điểm, 3 tín chỉ, HK2_2020
    - ...

---

## 📄 GIẢI THÍCH CHI TIẾT TỪNG FILE

### **1. 📊 STRUCTS.H** - Định nghĩa cấu trúc dữ liệu

**Nội dung:**
- Khai báo `struct Grade` (cấp 3): subject, score, credit, semester
- Khai báo `struct Student` (cấp 2): id, name, grades[], gpa, rank
- Khai báo `struct Cohort` (cấp 1): class_id, students[]
- Khai báo các struct kết quả thuật toán:
  - `KadaneResult`: start, end, maxSum
  - `StreakResult`: start, end, length
  - `SubjectStatEntry`: thống kê theo môn
  - `ClassStatEntry`: thống kê theo lớp

**Mục đích:** Lưu trữ dữ liệu trong bộ nhớ một cách có tổ chức

---

### **2. 📥 FILE_IO.H & FILE_IO.CPP** - Đọc/ghi file

#### **2.1 FILE_IO.H** - Khai báo hàm

```cpp
readData(gradesFile, classFile)
    // Đọc 2 file → tạo struct Cohort, Student, Grade
    // Trả về: vector<Cohort>

splitCSVLine(line)
    // Tách dòng CSV bằng dấu phẩy
    // "SV_1_0001,Bui Tuan Phuc,Mac - Lenin,8.6,2,HK1_2020"
    // → ["SV_1_0001", "Bui Tuan Phuc", "Mac - Lenin", "8.6", "2", "HK1_2020"]

writeGPAReport(path, cohorts)
    // Ghi file gpa_report.csv
    // Nội dung: student_id, name, class, gpa, rank

writeProgress(path, cohorts, k)
    // Ghi file progress.txt
    // Nội dung: Kadane, dãy tăng, sliding window, prefix sum

writeRanking(path, cohorts)
    // Ghi file ranking.csv
    // Nội dung: Sắp xếp SV theo GPA giảm dần

writeStatistics(path, cohorts)
    // Ghi file statistics.txt
    // Nội dung: Thống kê theo môn + lớp + Z-score
```

#### **2.2 FILE_IO.CPP** - Thực hiện hàm - GIẢI THÍCH CHI TIẾT

##### **A. HÀM `splitCSVLine()` - Tách dòng CSV**

```cpp
vector<string> splitCSVLine(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string field;
    while (getline(ss, field, ',')) {           // ① Đọc từng trường, phân tách bằng ','
        result.push_back(trim(field));           // ② Xóa khoảng trắng, thêm vào result
    }
    return result;
}
```

**Giải thích từng dòng:**
- `stringstream ss(line)` → Chuyển string thành stream để đọc từng phần
- `getline(ss, field, ',')` → Đọc cho đến khi gặp ',' (phân tách bằng đấu phẩy)
- `trim(field)` → Xóa khoảng trắng đầu/cuối
- `result.push_back()` → Thêm field vào vector

**Ví dụ xử lý:**
```
Input:  "SV_1_0001,Bui Tuan Phuc,Mac - Lenin,8.6,2,HK1_2020"
        ↓ getline(..., ',')
Output: ["SV_1_0001", "Bui Tuan Phuc", "Mac - Lenin", "8.6", "2", "HK1_2020"]
```

---

##### **B. HÀM `readData()` - Đọc & xây dựng dữ liệu - STEP-BY-STEP**

**Bước 1: Đọc class.txt → Xây dựng 2 map**

```cpp
map<string, string> studentToClass;          // MSSV → Lớp
map<string, vector<string>> classToStudents;  // Lớp → [MSSV1, MSSV2, ...]

string currentClass = "";
while (getline(classIn, line)) {
    line = trim(line);
    if (line.empty()) continue;  // Bỏ qua dòng trống
    
    if (line.substr(0, 3) == "LOP") {        // ① Nếu dòng bắt đầu bằng "LOP"
        currentClass = line;                  // ② Là mã lớp mới
    } else {
        studentToClass[line] = currentClass;           // ③ Gán MSSV cho lớp hiện tại
        classToStudents[currentClass].push_back(line); // ④ Thêm MSSV vào danh sách lớp
    }
}
```

**Ví dụ:**
```
File class.txt:
LOP1_01        ← currentClass = "LOP1_01"
SV_1_0001      ← studentToClass["SV_1_0001"] = "LOP1_01"
               ← classToStudents["LOP1_01"].push_back("SV_1_0001")
SV_1_0002      ← studentToClass["SV_1_0002"] = "LOP1_01"
               ← classToStudents["LOP1_01"].push_back("SV_1_0002")
...
LOP1_02        ← currentClass = "LOP1_02" (lớp mới)
SV_1_0011      ← studentToClass["SV_1_0011"] = "LOP1_02"
```

**Kết quả:**
- `studentToClass = {"SV_1_0001":"LOP1_01", "SV_1_0002":"LOP1_01", ...}`
- `classToStudents = {"LOP1_01":["SV_1_0001","SV_1_0002",...], "LOP1_02":["SV_1_0011",...], ...}`

---

**Bước 2: Đọc grades.csv → Lưu điểm từng SV**

```cpp
map<string, string> studentNames;           // MSSV → Tên
map<string, vector<Grade>> studentGrades;   // MSSV → [Grade1, Grade2, ...]

bool isHeader = true;
while (getline(gradesIn, line)) {
    if (isHeader) { isHeader = false; continue; }  // ① Bỏ qua header
    
    vector<string> fields = splitCSVLine(line);    // ② Tách dòng CSV
    if (fields.size() < 6) continue;               // ③ Kiểm tra đủ trường (6 trường)
    
    string studentId = fields[0];                  // ④ Trích MSSV
    string name = normalizeName(fields[1]);        // ⑤ Chuẩn hóa tên
    string subject = fields[2];                    // ⑥ Trích tên môn
    
    double score = stod(fields[3]);                // ⑦ Parse điểm (string → double)
    int credit = stoi(fields[4]);                  // ⑧ Parse tín chỉ (string → int)
    
    Grade grade;
    grade.subject = subject;
    grade.score = score;
    grade.credit = credit;
    grade.semester = fields[5];                    // ⑨ Lưu thông tin Grade
    
    studentNames[studentId] = name;                // ⑩ Lưu MSSV → Tên
    studentGrades[studentId].push_back(grade);     // ⑪ Thêm Grade vào danh sách
}
```

**Ví dụ xử lý dòng dữ liệu:**
```
Input dòng CSV:
SV_1_0001,Bui Tuan Phuc,Lap Trinh C++,7.3,4,HK1_2020
         ↓
fields = ["SV_1_0001", "Bui Tuan Phuc", "Lap Trinh C++", "7.3", "4", "HK1_2020"]
         ↓
Grade{subject="Lap Trinh C++", score=7.3, credit=4, semester="HK1_2020"}
↓
studentGrades["SV_1_0001"].push_back(grade)

Nếu SV_1_0001 có 10 môn → studentGrades["SV_1_0001"] sẽ có 10 Grade
```

---

**Bước 3: Ghép lại thành Cohort → Student → Grade**

```cpp
map<string, Cohort> cohortMap;

// ① Từ classToStudents, xây dựng từng Cohort
for (auto& entry : classToStudents) {
    Cohort cohort;
    cohort.class_id = entry.first;  // entry.first = "LOP1_01"
    
    // ② Cho mỗi MSSV trong lớp
    for (const string& sid : entry.second) {  // entry.second = ["SV_1_0001", "SV_1_0002", ...]
        Student s;
        s.id = sid;
        s.name = studentNames[sid];  // ③ Lấy tên từ map đã lưu
        s.grades = studentGrades[sid];  // ④ Gán tất cả Grade cho SV
        
        cohort.students.push_back(s);  // ⑤ Thêm Student vào Cohort
    }
    
    cohortMap[entry.first] = cohort;  // ⑥ Lưu Cohort vào map
}

vector<Cohort> result;
for (auto& entry : cohortMap) result.push_back(entry.second);  // ⑦ Chuyển thành vector
return result;
```

**Ví dụ kết quả:**
```
result[0] = Cohort {
    class_id = "LOP1_01",
    students = [
        {id="SV_1_0001", name="Bui Tuan Phuc", grades=[Grade1, Grade2, ...]},
        {id="SV_1_0002", name="Vu Thuy Cuong", grades=[Grade1, Grade2, ...]},
        ...
    ]
}
```

---

**Xử lý trường hợp ngoại lệ:**

```cpp
// ① SV trong grades.csv nhưng không có trong class.txt
for (auto& entry : studentGrades) {
    if (studentToClass.find(entry.first) == studentToClass.end()) {
        // Không tìm thấy lớp → gom vào "UNKNOWN"
        cout << "[CẢNH BÁO] " << entry.first << " không thuộc lớp nào. Gom vào UNKNOWN." << endl;
        
        if (cohortMap.find("UNKNOWN") == cohortMap.end()) {
            Cohort u;
            u.class_id = "UNKNOWN";
            cohortMap["UNKNOWN"] = u;
        }
        
        Student s;
        s.id = entry.first;
        s.name = studentNames[entry.first];
        s.grades = entry.second;
        cohortMap["UNKNOWN"].students.push_back(s);
    }
}
```

---

##### **C. HÀM `writeProgress()` - Xuất báo cáo chi tiết**

```cpp
void writeProgress(const string& path, vector<Cohort>& cohorts, int k) {
    for (const auto& student : cohorts) {
        
        // ① Tính GPA từng kỳ
        auto semGPAs = calcGPABySemester(student.grades);
        // semGPAs = [{"HK1_2020", 7.18}, {"HK2_2020", 6.42}, ...]
        
        vector<double> gpaVals;
        vector<string> semNames;
        
        for (int i = 0; i < semGPAs.size(); i++) {
            gpaVals.push_back(semGPAs[i].second);    // Lấy giá trị GPA
            semNames.push_back(semGPAs[i].first);    // Lấy tên kỳ
        }
        
        // ② Tính GPA trung bình
        double avg = 0;
        for (double g : gpaVals) avg += g;
        avg /= gpaVals.size();  // avg = (7.18 + 6.42 + ...) / số kỳ
        
        // ③ Tính Delta GPA
        vector<double> delta;
        for (int i = 0; i < gpaVals.size(); i++) {
            double d = gpaVals[i] - avg;  // d = GPA_kỳ - GPA_TB
            delta.push_back(d);
        }
        
        // ④ Chạy Kadane → tìm giai đoạn tốt nhất
        KadaneResult kadane = kadaneMaxGPA(delta);
        out << "Đoạn kỳ học tốt nhất: [" << semNames[kadane.start] 
            << " -> " << semNames[kadane.end] << "]" << endl;
        
        // ⑤ Chạy Longest Increasing Streak → tìm chuỗi tăng
        StreakResult streak = longestIncreasingStreak(gpaVals);
        out << "Chuỗi GPA tăng dài nhất: " << streak.length << " kỳ" << endl;
        
        // ⑥ Tính Sliding Window → trung bình trượt
        if (gpaVals.size() >= k) {
            vector<double> avgs = slidingAvgGPA(gpaVals, k);
            // avgs có (gpaVals.size() - k + 1) phần tử
            out << "GPA trung bình trượt (k=" << k << "): ";
            for (int i = 0; i < avgs.size(); i++) {
                if (i > 0) out << ", ";
                out << avgs[i];
            }
        }
        
        // ⑦ Phát hiện sụt giảm
        bool decline = detectDecline(avgs);
        out << "Cảnh báo sụt giảm: " << (decline ? "Có" : "Không") << endl;
        
        // ⑧ Tính Prefix Sum → tín chỉ tích lũy
        vector<string> sortedSems = getSortedSemesters(student.grades);
        vector<int> prefix = prefixSumCredits(student.grades, sortedSems);
        
        // Truy vấn tín chỉ từng kỳ
        for (int i = 0; i < sortedSems.size(); i++) {
            int total = queryCredits(prefix, 0, i);  // Tổng từ kỳ 0 đến i
            out << sortedSems[i] << "=" << total << " tín chỉ" << endl;
        }
    }
}
```

---

### **3. 📈 GPA.H & GPA.CPP** - Tính GPA - GIẢI THÍCH CHI TIẾT

#### **Công thức GPA:**
$$\text{GPA} = \frac{\sum (\text{điểm} \times \text{tín chỉ})}{\sum \text{tín chỉ}}$$

**Ví dụ:**
- Lập Trình C++: 7.3 điểm, 4 tín chỉ → 7.3 × 4 = 29.2
- Mac - Lenin: 8.6 điểm, 2 tín chỉ → 8.6 × 2 = 17.2
- Triết Học: 5.5 điểm, 2 tín chỉ → 5.5 × 2 = 11.0
- **GPA = (29.2 + 17.2 + 11.0) / (4 + 2 + 2) = 57.4 / 8 = 7.175**

---

#### **A. HÀM `calcGPA()` - Tính GPA từ danh sách điểm**

```cpp
double calcGPA(const vector<Grade>& grades) {
    if (grades.empty()) return 0.0;  // ① Nếu không có điểm → GPA = 0
    
    double totalWeighted = 0.0;      // ② Biến lưu tổng (điểm × tín chỉ)
    int totalCredits = 0;            // ③ Biến lưu tổng tín chỉ
    
    for (const auto& g : grades) {
        totalWeighted += g.score * g.credit;  // ④ Cộng (điểm × tín chỉ)
        totalCredits += g.credit;             // ⑤ Cộng tín chỉ
    }
    
    if (totalCredits == 0) return 0.0;  // ⑥ Tránh chia cho 0
    return totalWeighted / totalCredits;  // ⑦ Trả về GPA
}
```

**Ví dụ thực hiện từng bước:**
```
Dữ liệu: 
  Grade1: score=7.3, credit=4
  Grade2: score=8.6, credit=2
  Grade3: score=5.5, credit=2

Bước 1: totalWeighted = 0, totalCredits = 0
Bước 2: i=0 → totalWeighted = 0 + 7.3*4 = 29.2, totalCredits = 0 + 4 = 4
Bước 3: i=1 → totalWeighted = 29.2 + 8.6*2 = 46.4, totalCredits = 4 + 2 = 6
Bước 4: i=2 → totalWeighted = 46.4 + 5.5*2 = 57.4, totalCredits = 6 + 2 = 8
Bước 5: GPA = 57.4 / 8 = 7.175 ✓
```

---

#### **B. HÀM `calcGPABySemester()` - Tính GPA từng kỳ**

```cpp
vector<pair<string, double>> calcGPABySemester(const vector<Grade>& grades) {
    if (grades.empty()) return {};
    
    // ① Nhóm điểm theo kỳ học
    map<string, vector<Grade>> semGrades;
    for (const auto& g : grades) {
        semGrades[g.semester].push_back(g);
    }
    // semGrades = {"HK1_2020": [Grade1, Grade2, ...], "HK2_2020": [...], ...}
    
    // ② Tính GPA cho từng kỳ
    vector<pair<string, double>> result;
    for (const auto& entry : semGrades) {
        double gpa = calcGPA(entry.second);  // Gọi calcGPA() cho từng kỳ
        result.push_back({entry.first, gpa});  // Thêm {kỳ, GPA}
    }
    
    // ③ Sắp xếp theo thứ tự thời gian
    sort(result.begin(), result.end(),
        [](const pair<string,double>& a, const pair<string,double>& b) {
            return compareSemester(a.first, b.first) < 0;
        });
    
    return result;
}
```

**Ví dụ thực hiện:**
```
Input: Grades của SV_1_0001
  {subject="C++", score=7.3, credit=4, semester="HK1_2020"}
  {subject="ML", score=8.6, credit=2, semester="HK1_2020"}
  {subject="Web", score=7.4, credit=3, semester="HK2_2020"}
  {subject="DB", score=5.0, credit=4, semester="HK2_2020"}

Bước 1: Nhóm theo kỳ
  semGrades["HK1_2020"] = [{C++, 7.3, 4}, {ML, 8.6, 2}]
  semGrades["HK2_2020"] = [{Web, 7.4, 3}, {DB, 5.0, 4}]

Bước 2: Tính GPA từng kỳ
  GPA("HK1_2020") = (7.3*4 + 8.6*2) / (4+2) = 57.4 / 6 = 7.57
  GPA("HK2_2020") = (7.4*3 + 5.0*4) / (3+4) = 42.2 / 7 = 6.03

Bước 3: Sắp xếp → [{"HK1_2020", 7.57}, {"HK2_2020", 6.03}]

Output: [{"HK1_2020", 7.57}, {"HK2_2020", 6.03}]
```

---

#### **C. HÀM `classifyRank()` - Phân loại học lực**

```cpp
string classifyRank(double gpa) {
    if (gpa >= 9.0) return "Xuat Sac";      // ① GPA ≥ 9.0
    if (gpa >= 8.0) return "Gioi";          // ② GPA ≥ 8.0 và < 9.0
    if (gpa >= 7.0) return "Kha";           // ③ GPA ≥ 7.0 và < 8.0
    if (gpa >= 5.0) return "Trung Binh";    // ④ GPA ≥ 5.0 và < 7.0
    return "Yeu";                            // ⑤ GPA < 5.0
}
```

**Ví dụ:**
```
GPA = 8.5 → return "Gioi" (if gpa >= 8.0 ✓)
GPA = 6.8 → return "Kha" (if gpa >= 7.0 ✗, if gpa >= 5.0 ✓)
GPA = 4.2 → return "Yeu" (không thõa điều kiện nào)
```

---

#### **D. HÀM `computeAllGPA()` - Tính GPA toàn bộ SV**

```cpp
void computeAllGPA(vector<Cohort>& cohorts) {
    for (auto& cohort : cohorts) {              // ① Duyệt từng Cohort (lớp)
        for (auto& student : cohort.students) {  // ② Duyệt từng Student
            // ③ Tính GPA từ danh sách điểm
            student.gpa = calcGPA(student.grades);
            
            // ④ Xếp loại dựa vào GPA
            student.rank = classifyRank(student.gpa);
        }
    }
}
```

**Ví dụ:**
```
Trước: Student{id="SV_1_0001", gpa=0.0, rank="Chua xep loai"}
         ↓ calcGPA(grades) → 7.18
         ↓ classifyRank(7.18) → "Kha"
Sau  : Student{id="SV_1_0001", gpa=7.18, rank="Kha"}
```

---

### **4. 🔧 ALGORITHMS.H & ALGORITHMS.CPP** - 5 THUẬT TOÁN CHÍNH

#### **4.1 KADANE'S ALGORITHM** - Tìm giai đoạn học tập tốt nhất

**Ý tưởng:**
- Tìm đoạn kỳ học liên tiếp có tổng "delta GPA" (GPA_kỳ - GPA_trung_bình) lớn nhất
- Delta dương = tốt hơn trung bình, Delta âm = kém hơn trung bình

**Ví dụ:**
```
SV có GPA từng kỳ: [6.0, 7.0, 6.5, 5.5]
Trung bình TB = 6.25

Delta so với TB: [-0.25, +0.75, +0.25, -0.75]

Kadane: Đoạn từ kỳ 2→3 có delta = 0.75 + 0.25 = 1.0 (lớn nhất)
→ Kỳ 2-3 là giai đoạn sinh viên "bứt phá" tốt nhất
```

**Code:**
```cpp
KadaneResult kadaneMaxGPA(const vector<double>& deltaGPAs)
{
    // Thuật toán Kadane cổ điển
    // maxEndingHere = tổng từ start đến vị trí i hiện tại
    // maxSoFar = tổng lớn nhất tìm được
    
    double maxEndingHere = deltaGPAs[0];
    double maxSoFar = deltaGPAs[0];
    int tempStart = 0;
    result.start = 0;
    result.end = 0;
    
    for (int i = 1; i < deltaGPAs.size(); i++) {
        if (maxEndingHere + deltaGPAs[i] < deltaGPAs[i]) {
            // Bắt đầu lại từ i thay vì tiếp tục cộng
            maxEndingHere = deltaGPAs[i];
            tempStart = i;
        } else {
            maxEndingHere += deltaGPAs[i];
        }
        
        if (maxEndingHere > maxSoFar) {
            maxSoFar = maxEndingHere;
            result.start = tempStart;
            result.end = i;
        }
    }
    result.maxSum = maxSoFar;
    return result;
}
```

**Độ phức tạp:** O(n)

---

#### **4.2 LONGEST INCREASING STREAK** - Chuỗi kỳ GPA tăng liên tiếp dài nhất

**Ý tưởng:**
- Tìm chuỗi kỳ học mà GPA tăng liên tiếp: GPA[i] > GPA[i-1] > GPA[i-2]...

**Ví dụ:**
```
GPA từng kỳ: [6.0, 7.0, 7.5, 8.0, 7.5, 8.5]

Tìm chuỗi tăng:
- Kỳ 1-2: 6.0 < 7.0 ✓
- Kỳ 2-3: 7.0 < 7.5 ✓
- Kỳ 3-4: 7.5 < 8.0 ✓
- Kỳ 4-5: 8.0 > 7.5 ✗ (dừng)

Chuỗi tăng dài nhất = 3 kỳ liên tiếp (HK2→HK3→HK4: 7.0→7.5→8.0)
```

**Code:**
```cpp
StreakResult longestIncreasingStreak(const vector<double>& semGPAs)
{
    int curStart = 0, curLen = 1;
    int bestStart = 0, bestLen = 1;
    
    for (int i = 1; i < semGPAs.size(); i++) {
        if (semGPAs[i] > semGPAs[i - 1]) {
            // Tiếp tục chuỗi
            curLen++;
        } else {
            // Chuỗi bị cắt
            if (curLen > bestLen) {
                bestLen = curLen;
                bestStart = curStart;
            }
            curStart = i;
            curLen = 1;
        }
    }
    // Kiểm tra chuỗi cuối cùng
    if (curLen > bestLen) {
        bestLen = curLen;
        bestStart = curStart;
    }
    
    return {bestStart, bestStart + bestLen - 1, bestLen};
}
```

**Độ phức tạp:** O(n)

---

#### **4.3 SLIDING WINDOW** - GPA trung bình trượt (phát hiện xu hướng)

**Ý tưởng:**
- Tính trung bình GPA của k kỳ liên tiếp
- Sử dụng "cửa sổ trượt" để tối ưu hóa

**Ví dụ (k=2):**
```
GPA từng kỳ: [6.0, 7.0, 7.5, 8.0]

Sliding avg với k=2:
- Cửa sổ [6.0, 7.0]: avg = 6.5
- Cửa sổ [7.0, 7.5]: avg = 7.25
- Cửa sổ [7.5, 8.0]: avg = 7.75

Xu hướng: 6.5 → 7.25 → 7.75 (tăng dần = sinh viên tiến bộ)
```

**Code:**
```cpp
vector<double> slidingAvgGPA(const vector<double>& semGPAs, int k)
{
    vector<double> result;
    
    // Tính tổng k phần tử đầu
    double windowSum = 0;
    for (int i = 0; i < k; i++)
        windowSum += semGPAs[i];
    result.push_back(windowSum / k);
    
    // Cửa sổ trượt
    for (int i = k; i < semGPAs.size(); i++) {
        windowSum += semGPAs[i] - semGPAs[i - k];  // Loại bỏ phần tử cũ, thêm phần tử mới
        result.push_back(windowSum / k);
    }
    return result;
}
```

**Độ phức tạp:** O(n)

#### **Phát hiện sụt giảm:**
```cpp
bool detectDecline(const vector<double>& slidingAvgs)
{
    // Nếu có ≥ 2 lần sliding avg giảm liên tiếp → cảnh báo
    int consecutiveDrops = 0;
    for (int i = 1; i < slidingAvgs.size(); i++) {
        if (slidingAvgs[i] < slidingAvgs[i - 1]) {
            consecutiveDrops++;
            if (consecutiveDrops >= 2) return true;  // ⚠️ Cảnh báo!
        } else {
            consecutiveDrops = 0;
        }
    }
    return false;
}
```

---

#### **4.4 PREFIX SUM** - Tín chỉ thích lũy (truy vấn O(1))

**Ý tưởng:**
- Xây mảng prefix sum để truy vấn nhanh "tổng tín chỉ từ kỳ l đến r"
- Vừa lưu thông tin, vừa tối ưu thời gian truy vấn

**Ví dụ:**
```
Tín chỉ mỗi kỳ:
- HK1_2020: 2 + 4 + 2 + 2 = 10 tín chỉ
- HK2_2020: 3 + 2 + 3 + 4 + 2 = 14 tín chỉ
- HK3_2021: 3 + 4 + 2 + 3 = 12 tín chỉ
- HK4_2021: 2 + 3 + 2 + 2 = 9 tín chỉ

Mảng prefix:
- prefix[0] = 0
- prefix[1] = 0 + 10 = 10
- prefix[2] = 10 + 14 = 24
- prefix[3] = 24 + 12 = 36
- prefix[4] = 36 + 9 = 45

Truy vấn: Tín chỉ từ kỳ 2 (HK2) đến kỳ 3 (HK3):
query(prefix, 1, 2) = prefix[3] - prefix[1] = 36 - 10 = 26 ✓
```

**Code:**
```cpp
vector<int> prefixSumCredits(const vector<Grade>& grades, const vector<string>& semesters)
{
    map<string, int> creditsBySem;
    for (const auto& g : grades) {
        creditsBySem[g.semester] += g.credit;
    }
    
    // prefix[0] = 0, prefix[i+1] = tổng tín chỉ từ kỳ 0 đến kỳ i
    vector<int> prefix(semesters.size() + 1, 0);
    for (int i = 0; i < semesters.size(); i++) {
        prefix[i + 1] = prefix[i] + creditsBySem[semesters[i]];
    }
    return prefix;
}

int queryCredits(const vector<int>& prefix, int l, int r)
{
    return prefix[r + 1] - prefix[l];  // O(1)
}
```

**Độ phức tạp:** Xây dựng O(n), Truy vấn O(1)

---

### **5. 📊 STATISTICS.H & STATISTICS.CPP** - Thống kê mô tả

#### **Các hàm thống kê:**

| Hàm | Công thức | Ý nghĩa |
|-----|-----------|---------|
| `mean(data)` | Σx / n | Trung bình cộng |
| `stdDev(data)` | √[Σ(x - mean)² / n] | Độ lệch chuẩn (phân tán dữ liệu) |
| `minVal(data)` | min(x) | Giá trị nhỏ nhất |
| `maxVal(data)` | max(x) | Giá trị lớn nhất |
| `zScore(data)` | (x - mean) / std | Điểm chuẩn hóa |

#### **Z-score - Chuẩn hóa dữ liệu:**

**Ý tưởng:**
- So sánh điểm giữa các môn khác nhau (vd: điểm Toán vs. Tiếng Anh)
- Z-score cho biết: điểm này "bao nhiêu độ lệch chuẩn" so với trung bình

**Ví dụ:**
```
Môn Lập Trình C++:
- Điểm SV: [7.3, 8.0, 6.5, 7.0, 9.0]
- Mean = 7.56, Std = 0.94

Z-score của 7.3: (7.3 - 7.56) / 0.94 = -0.276 (dưới trung bình một chút)
Z-score của 9.0: (9.0 - 7.56) / 0.94 = 1.532 (hơn trung bình 1.5σ)
```

#### **Hàm thống kê:**

```cpp
SubjectStats statsBySubject(const vector<Cohort>& cohorts)
{
    // Nhóm điểm theo subject
    map<string, vector<double>> scoresBySubject;
    
    for (mỗi Cohort) {
        for (mỗi Student) {
            for (mỗi Grade) {
                scoresBySubject[grade.subject].push_back(grade.score);
            }
        }
    }
    
    // Tính thống kê cho từng môn
    for (mỗi subject) {
        SubjectStatEntry entry;
        entry.meanScore = mean(scores);
        entry.stdScore = stdDev(scores);
        entry.minScore = minVal(scores);
        entry.maxScore = maxVal(scores);
        entry.zScores = zScore(scores);
    }
}

ClassStats statsByClass(const vector<Cohort>& cohorts)
{
    // Tương tự, nhóm GPA theo lớp
    for (mỗi Cohort) {
        ClassStatEntry entry;
        entry.meanGPA = mean(gpa của tất cả SV trong lớp);
        entry.stdGPA = stdDev(gpa);
        entry.minGPA = minVal(gpa);
        entry.maxGPA = maxVal(gpa);
    }
}
```

---

### **6. 🔤 STRING_UTILS.H & STRING_UTILS.CPP** - Xử lý chuỗi

| Hàm | Công dụng | Ví dụ |
|-----|-----------|-------|
| `trim(s)` | Xóa khoảng trắng đầu/cuối | "  Bùi Phúc  " → "Bùi Phúc" |
| `toTitleCase(s)` | Viết hoa chữ đầu mỗi từ | "bui tuan phuc" → "Bui Tuan Phuc" |
| `normalizeName(name)` | Kết hợp trim + title case + xóa khoảng trắng thừa | "  bui  TUAN  phuc  " → "Bui Tuan Phuc" |

**Code:**
```cpp
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string toTitleCase(const string& s) {
    string result = s;
    bool newWord = true;
    for (size_t i = 0; i < result.size(); i++) {
        if (isspace(result[i])) {
            newWord = true;
        } else if (newWord) {
            result[i] = toupper(result[i]);
            newWord = false;
        } else {
            result[i] = tolower(result[i]);
        }
    }
    return result;
}

string normalizeName(const string& name) {
    string trimmed = trim(name);
    // Xóa khoảng trắng thừa giữa các từ
    string cleaned;
    bool prevSpace = false;
    for (char c : trimmed) {
        if (isspace(c)) {
            if (!prevSpace) cleaned += ' ';
            prevSpace = true;
        } else {
            cleaned += c;
            prevSpace = false;
        }
    }
    return toTitleCase(cleaned);
}
```

---

### **7. ✅ UTILS.H & UTILS.CPP** - Các hàm tiện ích

| Hàm | Công dụng |
|-----|-----------|
| `isValidScore(score)` | Kiểm tra điểm ∈ [0, 10] |
| `isValidCredit(credit)` | Kiểm tra tín chỉ > 0 |
| `validateAndClean(cohorts)` | Loại bỏ dữ liệu sai, in cảnh báo |
| `getSortedSemesters(grades)` | Trích & sắp xếp danh sách kỳ theo thời gian |
| `compareSemester(a, b)` | So sánh 2 kỳ: "HK1_2020" < "HK2_2020" < "HK1_2021" |

**Code:**
```cpp
bool isValidScore(double score) {
    return score >= 0.0 && score <= 10.0;
}

bool isValidCredit(int credit) {
    return credit > 0;
}

int compareSemester(const string& a, const string& b) {
    auto pA = parseSemester(a);  // ("HK1_2020") → (2020, 1)
    auto pB = parseSemester(b);
    if (pA.first != pB.first) return (pA.first < pB.first) ? -1 : 1;
    if (pA.second != pB.second) return (pA.second < pB.second) ? -1 : 1;
    return 0;
}

void validateAndClean(vector<Cohort>& cohorts) {
    for (auto& cohort : cohorts) {
        for (auto& student : cohort.students) {
            vector<Grade> validGrades;
            for (const auto& g : student.grades) {
                if (!isValidScore(g.score)) {
                    cout << "[CẢNH BÁO] " << student.id << " - " << g.subject
                         << ": điểm " << g.score << " không hợp lệ. Bỏ qua." << endl;
                } else if (!isValidCredit(g.credit)) {
                    cout << "[CẢNH BÁO] " << student.id << " - " << g.subject
                         << ": tín chỉ " << g.credit << " không hợp lệ. Bỏ qua." << endl;
                } else {
                    validGrades.push_back(g);
                }
            }
            student.grades = validGrades;
        }
    }
}
```

---

### **8. 🎮 MAIN.CPP** - Chương trình chính

**Cấu trúc chương trình:**

```cpp
int main() {
    vector<Cohort> cohorts;           // Lưu dữ liệu
    bool dataLoaded = false;           // Kiểm tra đã đọc dữ liệu?
    bool gpaComputed = false;          // Kiểm tra đã tính GPA?
    int k = 2;                         // Kích thước cửa sổ sliding window (mặc định)
    string outputDir = "output/";      // Thư mục output
    
    do {
        printMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: // Đọc dữ liệu
            case 2: // Tính GPA
            case 3: // Kadane + Dãy tăng
            case 4: // Sliding Window
            case 5: // Prefix Sum
            case 6: // Thống kê
            case 7: // Xuất báo cáo
            case 0: // Thoát
        }
    } while (choice != 0);
}
```

---

## 🎮 MENU CHƯƠNG TRÌNH - 7 TÍNH NĂNG

### **Lựa chọn 1: Đọc dữ liệu**

```
Chọn bộ test (1/2/3): 1

Đang đọc dữ liệu từ data/test1/...
[OK] Đã đọc 3 lớp, 27 sinh viên.
```

**Thực hiện:**
- Gọi `readData("data/test1/grades.csv", "data/test1/class.txt")`
- Xóa khoảng trắng, chuẩn hóa tên
- Kiểm tra & làm sạch dữ liệu (loại bỏ điểm sai)
- Lưu vào `vector<Cohort>`

---

### **Lựa chọn 2: Tính GPA & Phân loại**

```
MSSV    Họ Tên              Lớp     GPA     Xếp Loại
SV_1_0001  Bui Tuan Phuc    LOP1_01 6.82    Kha
SV_1_0002  Vu Thuy Cuong    LOP1_01 7.83    Kha
...
```

**Thực hiện:**
- Gọi `computeAllGPA(cohorts)` → tính GPA cho mỗi SV
- Gọi `classifyRank(gpa)` → xếp loại (Xuất Sắc/Giỏi/Khá/Trung Bình/Yếu)
- In bảng

---

### **Lựa chọn 3: Phân tích tiến bộ**

```
=== SV_1_0001 - Bui Tuan Phuc ===
  Kadane: [HK2_2020 -> HK4_2021], delta = +0.95
  Dãy tăng: 2 kỳ [HK1_2020 -> HK2_2020]

=== SV_1_0002 - Vu Thuy Cuong ===
  Kadane: [HK1_2020 -> HK4_2021], delta = +1.23
  Dãy tăng: 3 kỳ [HK2_2020 -> HK4_2021]
```

**Thực hiện:**
- Tính GPA từng kỳ: `calcGPABySemester()`
- Tính delta: GPA_kỳ - GPA_trung_bình
- Chạy **Kadane's Algorithm** → tìm giai đoạn tốt nhất
- Chạy **Longest Increasing Streak** → tìm chuỗi GPA tăng liên tiếp

---

### **Lựa chọn 4: Phân tích xu hướng**

```
Nhập kích thước cửa sổ k (mặc định 2): 2

=== SV_1_0001 - Bui Tuan Phuc ===
  GPA theo kỳ: HK1_2020=7.18, HK2_2020=6.42, HK3_2021=5.75, HK4_2021=6.55
  Sliding avg (k=2): 6.80 → 5.99 → 6.15
  Xu hướng: ⚠ Phát hiện sụt giảm!
```

**Thực hiện:**
- Tính trung bình trượt: `slidingAvgGPA(semGPAs, k)`
- Phát hiện xu hướng sụt giảm: `detectDecline(slidingAvgs)`

---

### **Lựa chọn 5: Truy vấn tín chỉ tích lũy**

```
=== SV_1_0001 - Bui Tuan Phuc ===
  Tín chỉ theo kỳ:
    HK1_2020: 10 tín chỉ
    HK2_2020: 14 tín chỉ
    HK3_2021: 12 tín chỉ
    HK4_2021: 9 tín chỉ
  
  Prefix Sum: [0, 10, 24, 36, 45]
  
  Truy vấn: Tín chỉ từ HK2_2020 (kỳ 1) đến HK3_2021 (kỳ 2)
  Result: 26 tín chỉ
```

**Thực hiện:**
- Xây mảng prefix sum: `prefixSumCredits()`
- Truy vấn O(1): `queryCredits(prefix, l, r)`

---

### **Lựa chọn 6: Thống kê mô tả**

```
=== THỐNG KÊ THEO MÔN ===
Môn: Lập Trình C++
  Mean: 7.02
  Std: 0.89
  Min: 5.20
  Max: 8.00
  Z-Score: [-0.28, 1.08, -2.04, -0.28, +1.10, ...]

=== THỐNG KÊ THEO LỚP ===
Lớp: LOP1_01
  Mean GPA: 7.15
  Std GPA: 0.67
  Min GPA: 5.82
  Max GPA: 8.50
```

**Thực hiện:**
- Gom dữ liệu theo môn/lớp
- Tính mean, std, min, max
- Tính z-score

---

### **Lựa chọn 7: Xuất báo cáo**

```
[OK] Đã ghi: output/gpa_report.csv
[OK] Đã ghi: output/progress.txt
[OK] Đã ghi: output/ranking.csv
[OK] Đã ghi: output/statistics.txt
```

**Thực hiện:**
- Gọi `writeGPAReport()` → file CSV
- Gọi `writeProgress()` → file TXT (tất cả kết quả Kadane, dãy tăng, sliding window, prefix sum)
- Gọi `writeRanking()` → file CSV (sắp xếp GPA)
- Gọi `writeStatistics()` → file TXT (thống kê)

---

## 💾 CẤU TRÚC DỮ LIỆU ĐẦU VÀO

### **Data/test1/grades.csv**

Header:
```
student_id,name,subject,score,credit,semester
```

Dữ liệu:
```
SV_1_0001,Bui Tuan Phuc,Mac - Lenin,8.6,2,HK1_2020
SV_1_0001,Bui Tuan Phuc,Lap Trinh C++,7.3,4,HK1_2020
SV_1_0001,Bui Tuan Phuc,Triet Hoc,5.5,2,HK1_2020
SV_1_0001,Bui Tuan Phuc,Tieng Anh 2,6.7,2,HK1_2020
SV_1_0001,Bui Tuan Phuc,Phat Trien Web,7.4,3,HK2_2020
...
```

**Quy tắc:**
- Dòng 1: Header (bỏ qua)
- Từ dòng 2: Dữ liệu (MSSV, Tên, Môn, Điểm, Tín chỉ, Kỳ học)

### **Data/test1/class.txt**

```
LOP1_01
SV_1_0001
SV_1_0002
SV_1_0003
...
SV_1_0010
LOP1_02
SV_1_0011
...
LOP1_03
SV_1_0021
...
```

**Quy tắc:**
- Dòng bắt đầu "LOP" → mã lớp mới
- Dòng tiếp theo → MSSV trong lớp đó

---

## 📊 CẤU TRÚC FILE ĐẦU RA (Output/)

### **1. gpa_report.csv** - Báo cáo GPA

```
student_id,name,class,gpa,rank
SV_1_0001,Bui Tuan Phuc,LOP1_01,6.82,Kha
SV_1_0002,Vu Thuy Cuong,LOP1_01,7.83,Kha
SV_1_0003,To Minh Duc,LOP1_01,7.47,Kha
...
```

### **2. ranking.csv** - Xếp hạng sinh viên theo GPA giảm dần

```
student_id,name,class,gpa,rank
SV_1_0009,Dang Minh Tam,LOP1_01,8.78,Gioi
SV_1_0002,Vu Thuy Cuong,LOP1_01,7.83,Kha
SV_1_0003,To Minh Duc,LOP1_01,7.47,Kha
...
```

### **3. progress.txt** - Chi tiết phân tích tiến bộ

```
=== SV_1_0001 - Bui Tuan Phuc ===
GPA theo kỳ: HK1_2020=7.18, HK2_2020=6.42, HK3_2021=5.75, HK4_2021=6.55
Delta GPA (so với TB=6.48): -0.70, -0.06, -0.73, +0.07
Đoạn kỳ học tốt nhất (Kadane): [HK4_2021], delta = +0.07
Dãy tăng: 1 kỳ
GPA trung bình trượt (k=2): 6.80, 5.99, 6.15
Xu hướng: Phát hiện sụt giảm!
Tín chỉ tích lũy:
  Kỳ 0 (HK1_2020): 10 tín chỉ
  Kỳ 1 (HK2_2020): 24 tín chỉ
  Kỳ 2 (HK3_2021): 36 tín chỉ
  Kỳ 3 (HK4_2021): 45 tín chỉ
```

### **4. statistics.txt** - Thống kê toàn bộ

```
=== THỐNG KÊ THEO MÔN ===

Môn: Cong Nghe PM
  Mean: 7.40 (Khá)
  Std: 0.65
  Min: 6.00
  Max: 8.50
  Z-Scores:
    SV_1_0009: +1.69
    SV_1_0014: +0.15
    SV_1_0024: -2.15
    ...

=== THỐNG KÊ THEO LỚP ===

Lớp: LOP1_01
  Mean GPA: 7.15 (Khá)
  Std GPA: 0.67
  Min GPA: 5.82
  Max GPA: 8.78
```

---

## 🔄 QUIN TRÌNH CHẠY CHƯƠNG TRÌNH

```bash
# Bước 1: Biên dịch
make
# Hoặc: g++ -std=c++17 -Wall -Wextra -I include -o student_gpa main.cpp src/file_io.cpp src/gpa.cpp src/algorithms.cpp src/statistics.cpp src/string_utils.cpp src/utils.cpp

# Bước 2: Chạy
./student_gpa.exe

# Bước 3: Chọn menu (thứ tự bắt buộc)
1. Đọc dữ liệu (chọn test 1/2/3)
2. Tính GPA & Phân loại
3-7. (Có thể chạy nếu đã hoàn thành 1, 2)
0. Thoát
```

---

## 🛠️ MAKEFILE - Tập lệnh biên dịch

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = main.cpp \
       $(SRC_DIR)/file_io.cpp \
       $(SRC_DIR)/gpa.cpp \
       $(SRC_DIR)/algorithms.cpp \
       $(SRC_DIR)/statistics.cpp \
       $(SRC_DIR)/string_utils.cpp \
       $(SRC_DIR)/utils.cpp

TARGET = student_gpa

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Các rules biên dịch...
```

---

## ✨ ĐIỂM NỔI BẬT CỦA DỰ ÁN

✅ **Struct 3 cấp rõ ràng:** Cohort → Student → Grade  
✅ **7 thuật toán quan trọng:**
  - Kadane's Algorithm (tìm max subarray)
  - Longest Increasing Streak (dãy con tăng)
  - Sliding Window (cửa sổ trượt)
  - Prefix Sum (truy vấn O(1))
  - Thống kê (mean, std, min, max, z-score)
  - File I/O (CSV + TXT)
  - Xử lý chuỗi (trim, title case, normalize)

✅ **Xử lý dữ liệu kỹ lưỡng:** Validate, clean, normalize, phát hiện lỗi  
✅ **Code modular:** Tách header/source, dễ bảo trì  
✅ **Ghi chép chi tiết:** Báo cáo 4 file  
✅ **Độ phức tạp tối ưu:** Kadane, LIS, Sliding Window O(n), Prefix Sum O(1)  

---

## 📚 TÓMT LẠI

Dự án này là một **hệ thống quản lý và phân tích học lực sinh viên** hoàn chỉnh, sử dụng:
- **Cấu trúc dữ liệu:** Struct lồng 3 cấp
- **Thuật toán:** Kadane, LIS, Sliding Window, Prefix Sum, thống kê
- **Xử lý file:** CSV read/write, TXT output
- **Xử lý chuỗi:** Normalize tên, chuẩn hóa dữ liệu
- **Phân tích:** GPA, phân loại, tiến bộ, xu hướng, tín chỉ, thống kê

**Đáp ứng đầy đủ 25+ yêu cầu** của đề tài! 🎉
