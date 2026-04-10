# Hệ thống quản lý điểm và Phân tích học lực sinh viên

**Đề tài 3**

## Biên dịch
```bash
g++ -std=c++17 -Wall -Wextra -I include -o student_gpa.exe main.cpp src/file_io.cpp src/gpa.cpp src/algorithms.cpp src/statistics.cpp src/string_utils.cpp src/utils.cpp
```

## Chạy

```bash
./student_gpa.exe
```

## Menu chương trình

1. Đọc dữ liệu từ file (chọn bộ test 1/2/3)
2. Tính GPA & Phân loại học lực
3. Phân tích tiến bộ (Kadane + Dãy tăng liên tiếp)
4. Phân tích xu hướng (Sliding Window)
5. Truy vấn tín chỉ tích lũy (Prefix Sum)
6. Thống kê mô tả (mean/std/min/max/Z-score)
7. Xuất báo cáo (ghi tất cả file output)
0. Thoát

## Cấu trúc thư mục

```
BaiTapLon_DeTai3/
├── main.cpp            # Entry point
├── include/            # Header files (7 files)
├── src/                # Source files (6 files)
├── data/test1,2,3/     # Dữ liệu test (grades.csv + class.txt)
├── output/             # File output (gpa_report.csv, progress.txt, ranking.csv, statistics.txt)
├── docs/               # Báo cáo
├── Makefile
└── README.md
```

## Kỹ thuật sử dụng

- **Struct long 3 cap**: Cohort -> Student -> Grade
- **File I/O**: Đọc CSV + TXT, ghi 4 file output
- **Kadane's Algorithm**: Tìm đoạn kỳ học tốt nhất (trên mảng delta GPA)
- **Dãy tăng liên tiếp**: Tìm chuỗi kỳ GPA tăng dài nhất
- **Sliding Window**: GPA trung bình trượt, phát hiện sụt giảm
- **Prefix Sum**: Tín chỉ tích lũy, truy vấn O(1)
- **Thống kê mô tả**: mean/std/min/max/Z-score theo môn và lớp
- **Xử lý chuỗi**: trim, title case, chuẩn hóa tên

## Lưu ý

- File `grades.csv` phải có header dòng đầu tiên
- File `class.txt`: dong bat dau bang "LOP" la ma lop, dong bat dau bang "SV" la MSSV
- Tên CSV không được chứa dấu phẩy
- Cần chạy bước 1 (đọc dữ liệu) trước, rồi bước 2 (tính GPA) trước khi dùng các chức năng khác
