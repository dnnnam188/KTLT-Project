---
marp: true
theme: default
paginate: true
size: 16:9
style: |
  section {
    font-family: 'Segoe UI', Arial, sans-serif;
  }
  h1 {
    color: #2c3e50;
    font-size: 2.5em;
    margin-bottom: 0.5em;
  }
  h2 {
    color: #34495e;
    font-size: 2em;
    border-bottom: 3px solid #3498db;
    padding-bottom: 0.3em;
    margin-bottom: 0.8em;
  }
  h3 {
    color: #16a085;
    font-size: 1.5em;
  }
  code {
    background-color: #ecf0f1;
    padding: 0.2em 0.4em;
    border-radius: 3px;
    font-family: 'Consolas', monospace;
  }
  pre {
    font-size: 0.85em;
  }
  .highlight {
    color: #e74c3c;
    font-weight: bold;
  }
  ul {
    margin: 0.5em 0;
    font-size: 0.9em;
    line-height: 1.6em;
  }
  table {
    width: 100%;
    border-collapse: collapse;
  }
  th {
    background-color: #3498db;
    color: white;
    padding: 10px;
  }
  td {
    padding: 10px;
    border-bottom: 1px solid #bdc3c7;
  }
---

<!-- _class: lead -->
# **HỆ THỐNG QUẢN LÝ ĐIỂM SINH VIÊN**
## Bài Tập Lớn - Môn Kỹ Thuật Lập Trình (KTLT)
*(Đoàn Nhật Nam - Đạt / Nhóm 5)*


---

## **Nội Dung Trình Bày**

1. **Mục tiêu & Yêu cầu**
2. **Cấu trúc dữ liệu**
3. **Thuật toán & Giải pháp**
4. **Tính năng chính**
5. **Test Cases & Kết quả**
6. **Hướng phát triển tương lai**

---

<!-- _class: lead -->
# **PHẦN 1: MỤC TIÊU & YÊU CẦU**

---

## **Mục tiêu Project**

Xây dựng hệ thống quản lý điểm sinh viên **toàn diện** với các tính năng:

- ✅ Quản lý dữ liệu học sinh theo **cấp độ lồng (Cohort → Student → Grade)**
- ✅ Tính toán **GPA** và **phân loại học lực**
- ✅ Phân tích dữ liệu bằng **06+ thuật toán tiên tiến**
- ✅ Đọc/ghi file **CSV, TXT** an toàn
- ✅ Sinh **báo cáo thống kê chi tiết**
- ✅ Xử lý **dữ liệu tiếng Việt** chuẩn chỉ

---

## **Yêu cầu Chức Năng (Phần 1)**

| **Yêu Cầu** | **Trạng Thái** | **Ghi chú** |
|-----------|:-----------:|-----------|
| Struct lồng 3 cấp | ✅ Done | Tổ chức dữ liệu logic |
| File I/O (CSV/TXT) | ✅ Done | Xử lý file đầu vào/ra |
| Tính GPA | ✅ Done | Trọng số theo tín chỉ |
| GPA Classification | ✅ Done | Dựa trên chuẩn Đại học |
| Xử lý Unicode Việt | ✅ Done | Hiển thị đúng font tiếng Việt |

---

## **Yêu Cầu Chức Năng (Phần 2)**

| **Yêu Cầu** | **Trạng Thái** | **Ghi chú** |
|-----------|:-----------:|-----------|
| Dữ liệu Validation | ✅ Done | Bắt lỗi dữ liệu dị thường |
| Thống kê (Mean/Std...) | ✅ Done | Tính toán độ lệch chuẩn, min, max |
| Z-score Analysis | ✅ Done | Phân tích phân phối chuẩn |
| String Processing | ✅ Done | Trim, Title case tự động |
| 3+ Test Cases | ✅ Done | Dữ liệu test từ nhỏ đến lớn |

---

<!-- _class: lead -->
# **PHẦN 2: CẤU TRÚC DỮ LIỆU**

---

## **Cấu Trúc Lồng 3 Cấp**

```cpp
struct Grade {
    std::string subject;
    double score;           // [0-10]
    int credit;             // > 0
};
struct Student {
    std::string id;
    std::string full_name;
    std::vector<Grade> grades;
};
struct Cohort {
    std::string cohort_id;
    std::string academic_year;
    std::vector<Student> students;
};
```

---

## **Đặc Điểm Cấu Trúc Dữ Liệu**

### **Lợi Ích của Cấu Trúc Lồng:**
- ✅ **Tổ chức logic**: Dữ liệu được sắp xếp theo cấp độ tự nhiên.
- ✅ **Tính modular**: Dễ dàng thêm, xóa, cập nhật sinh viên.
- ✅ **Linh hoạt**: Mỗi sinh viên có thể có số lượng điểm (môn học) khác nhau.
- ✅ **Bảo mật**: Dữ liệu có cấu trúc rõ ràng, dễ bảo trì.

### **Hạn Chế & Giải Pháp:**
- ❌ **Hạn chế**: Tiêu thụ bộ nhớ nếu dữ liệu quá lớn (overhead của vector).
- 💡 **Giải pháp**: Dùng **Prefix Sum** và tối ưu hóa cấp phát bộ nhớ (reserve) để tăng tốc độ truy vấn.

---

<!-- _class: lead -->
# **PHẦN 3: THUẬT TOÁN & GIẢI PHÁP**

---

## **1️⃣ Kadane Algorithm - Max Subarray Sum**

**Mục đích**: Tìm dãy điểm liên tục có tổng cao nhất. Thường áp dụng để đánh giá giai đoạn bứt phá của sinh viên.

```text
Input:  [3, -2, 5, -1, 4, -3, 2]
Output: 9 (từ subarray [5, -1, 4])
```

- **Độ phức tạp**: O(n) - Linear time
- **Ứng dụng**: 
  - Tìm chu kỳ/giai đoạn học tập đạt hiệu suất tốt nhất.
  - Phát hiện xu hướng tích cực ngắn hạn.

---

## **2️⃣ Longest Increasing Subsequence (LIS)**

**Mục đích**: Tìm dãy điểm tăng dần dài nhất trong suốt quá trình học.

```text
Input:  [2, 3, 1, 5, 4, 8, 6]
Output: 4 (subseq: [2, 3, 5, 8] hoặc [2, 3, 4, 8])
```

- **Độ phức tạp**: O(n log n) - Optimized DP / Binary Search
- **Ứng dụng**:
  - Theo dõi mức độ tiến bộ ổn định của sinh viên.
  - Nhận diện pattern cải thiện đường dài.

---

## **3️⃣ Sliding Window - Moving Average**

**Mục đích**: Tính trung bình điểm trên một "cửa sổ" gồm $k$ môn học liên tiếp.

```text
Input:  scores=[7, 8, 6, 9, 7], window(k)=3
Output: [7.0, 7.67, 7.33]
```

- **Độ phức tạp**: O(n) - Single pass
- **Ứng dụng**:
  - Tính GPA cục bộ theo từng cụm môn học (ví dụ: các môn chuyên ngành).
  - Làm mịn dữ liệu để vẽ biểu đồ dễ nhìn hơn.

---

## **4️⃣ Sliding Window - Decline Detection**

**Mục đích**: Phát hiện các giai đoạn có dấu hiệu sa sút.

**Điều kiện**:
- Đặt cửa sổ kích thước $k$.
- Nếu có $\ge L$ điểm rơi vào "vùng đỏ" (ví dụ $< 4.0$) trong cửa sổ đó.
- Output: Đánh dấu cửa sổ đó là **"Risky"** ⚠️

- **Ứng dụng**:
  - Hệ thống cảnh báo sớm (Early Warning System).
  - Hỗ trợ cố vấn học tập can thiệp kịp thời.

---

## **5️⃣ Prefix Sum - Cumulative Credits**

**Mục đích**: Tính tổng số tín chỉ tích lũy trong một khoảng thời gian với tốc độ O(1).

```text
Grades:  [3cr, 4cr, 3cr, 2cr, 3cr]
Prefix:  [0,   3,   7,   10,  12,  15]

Query: Tổng tín chỉ từ môn thứ 2 đến 4 = prefix[4] - prefix[1] = 12 - 3 = 9
```

- **Độ phức tạp**: 
  - Tiền xử lý (Preprocessing): O(n)
  - Truy vấn (Query): **O(1)** ✨

---

## **6️⃣ String Processing Utilities**

Các hàm tiện ích tự xây dựng giúp làm sạch dữ liệu đầu vào:

1. `trim(string)` - Xóa khoảng trắng thừa ở đầu/cuối chuỗi.
2. `to_title_case(string)` - Viết hoa chữ cái đầu của mỗi từ (VD: `nguyễn văn a` $\rightarrow$ `Nguyễn Văn A`).
3. `to_uppercase(string)` - Chuyển toàn bộ sang IN HOA.
4. `to_lowercase(string)` - Chuyển toàn bộ sang in thường.

**Áp dụng**: Đảm bảo tính nhất quán khi in báo cáo tiếng Việt.

---

<!-- _class: lead -->
# **PHẦN 4: TÍNH NĂNG CHÍNH**

---

## **Tính Năng 1: Tính GPA & Phân Loại**

**Công thức**:
```text
GPA = Σ(Score × Credit) / Σ(Credit)
```

**Bộ quy tắc phân loại**:
- 🥇 **Xuất sắc**: GPA $\ge$ 3.6
- ⭐ **Giỏi**: 3.2 $\le$ GPA $<$ 3.6
- ✅ **Khá**: 2.5 $\le$ GPA $<$ 3.2
- ⚠️ **Trung bình / Yếu**: GPA $<$ 2.5

---

## **Tính Năng 2: Thống Kê Toàn Diện**

Hệ thống cung cấp góc nhìn tổng quan qua các chỉ số:

- **Mean**: Điểm trung bình cộng của toàn khóa.
- **Standard Deviation**: Độ lệch chuẩn (đánh giá mức độ phân tán điểm số).
- **Min/Max**: Biên độ điểm (thấp nhất - cao nhất).
- **Median**: Giá trị trung vị (loại bỏ nhiễu từ các điểm quá cao/quá thấp).
- **Z-score**: Đánh giá vị trí của một sinh viên so với mặt bằng chung.

---

## **Tính Năng 3: File I/O Mạnh Mẽ**

### **Đọc File (Input)**:
- `class.txt` - Chứa danh sách sinh viên chuẩn.
- `grades.csv` - Chứa điểm số thô.

### **Ghi File (Output)**:
- `gpa_report.csv` - Danh sách tổng hợp GPA & Xếp loại.
- `ranking.csv` - Bảng xếp hạng từ cao xuống thấp.
- `statistics.txt` - File text chứa báo cáo phân tích toàn khóa.

---

## **Tính Năng 4: Data Validation (Xác Thực)**

Bảo vệ hệ thống khỏi crash do dữ liệu xấu:

| **Trường Kiểm Tra** | **Điều Kiện Hợp Lệ** | **Hành Động Khi Lỗi** |
|-----------|-----------|-----------|
| Score (Điểm) | `0 <= score <= 10` | ❌ Bỏ qua & Ghi log |
| Credit (Tín chỉ) | `credit > 0` | ❌ Reject môn học |
| ID (Mã SV) | `length > 0` | ❌ Báo lỗi định dạng |
| Tên Sinh Viên | Không chứa ký tự lạ | ✨ Tự động `trim` |

---

## **Tính Năng 5: CLI Menu (Giao diện dòng lệnh)**

```text
╔════════════════════════════════════════╗
║    HỆ THỐNG QUẢN LÝ ĐIỂM SINH VIÊN     ║
╚════════════════════════════════════════╝

1. Load dữ liệu từ file
2. Tính GPA tất cả sinh viên
3. Xếp hạng sinh viên
4. Phân tích thống kê
5. Phát hiện xu hướng (LIS, Sliding Window)
6. Xuất báo cáo tổng hợp
0. Thoát hệ thống

Vui lòng chọn chức năng: _
```

---

<!-- _class: lead -->
# **PHẦN 5: TEST CASES & KẾT QUẢ**

---

## **Kịch Bản Test (Test Datasets)**

```text
📊 Test 1 (Small):   5 students,   15 grades  (Unit test cơ bản)
📊 Test 2 (Medium):  50 students,  300 grades (Kiểm tra I/O)
📊 Test 3 (Large):   500 students, 3000 grades (Stress test)
```

**Mục đích đạt được**:
- ✅ Kiểm chứng tính chính xác của các công thức toán học.
- ✅ Đảm bảo thuật toán không bị quá tải với dữ liệu mảng lớn.
- ✅ Bắt thành công các "Edge cases" (Ví dụ: sinh viên không có điểm, điểm bị âm...).

---

## **Minh Họa Dữ Liệu Xuất (Output files)**

### **📄 gpa_report.csv**
```csv
student_id,full_name,gpa,classification
SV001,Nguyễn Văn A,3.75,Xuất sắc
SV002,Trần Thị B,3.20,Khá
```

### **📄 ranking.csv**
```csv
rank,student_id,full_name,gpa
1,SV001,Nguyễn Văn A,3.75
2,SV003,Lê Văn C,3.50
```

---

<!-- _class: lead -->
# **CẢM ƠN THẦY CÔ VÀ CÁC BẠN ĐÃ LẮNG NGHE!**

### Q&A - Hỏi Đáp
```