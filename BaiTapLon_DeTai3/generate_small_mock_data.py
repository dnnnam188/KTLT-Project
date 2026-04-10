import os
import random
import csv

# Danh sách họ, tên đệm, tên
ho_list = ["Nguyen", "Tran", "Le", "Pham", "Hoang", "Phan", "Vu", "Vo", "Dang", "Bui", "Do", "Ho", "Ngo", "Duong", "Ly"]
dem_list = ["Van", "Thi", "Huu", "Tuan", "Gia", "Xuan", "Quang", "Minh", "Thanh", "Ngoc", "Bao", "Anh", "Hai", "Quoc", "Manh", "Thuy"]
ten_list = ["An", "Binh", "Cuong", "Dung", "Dat", "Khoa", "Nghia", "Thang", "Truong", "Vinh", "Hoa", "Lan", "Mai", "Phuong", "Trang", "Yen", "Linh", "Kien", "Son", "Phuc", "Sang", "Luan", "Hieu"]

# Danh sách môn học và số tín chỉ
subjects_dict = {
    "Toan Cao Cap": 3, "Vat Ly": 2, "Hoa Hoc": 3, "Triet Hoc": 2, "Lap Trinh C++": 4, 
    "Tieng Anh 1": 2, "Tieng Anh 2": 2, "Nhap Mon CNTT": 3, "CTDL & GT": 4, 
    "Co So Du Lieu": 3, "He Dieu Hanh": 3, "Tien Te Ngan Hang": 2, "Toan Roi Rac": 3,
    "Kien Truc May Tinh": 3, "Mang May Tinh": 3, "Phat Trien Web": 3, "Cong Nghe PM": 3,
    "Tu Tuong HCM": 2, "Mac - Lenin": 2, "Kinh Te Vi Mo": 2
}
all_subjects = list(subjects_dict.keys())

def generate_name():
    return f"{random.choice(ho_list)} {random.choice(dem_list)} {random.choice(ten_list)}"

def generate_test_data(test_num, num_classes, num_students_per_class, num_semesters):
    base_dir = f"data/test{test_num}"
    os.makedirs(base_dir, exist_ok=True)
    
    class_file = os.path.join(base_dir, "class.txt")
    grades_file = os.path.join(base_dir, "grades.csv")
    
    student_id_counter = 1
    
    with open(class_file, "w", encoding="utf-8") as fc:
        for c in range(1, num_classes + 1):
            class_id = f"LOP{test_num}_{c:02d}"
            fc.write(f"{class_id}\n")
            for _ in range(num_students_per_class):
                student_id = f"SV_{test_num}_{student_id_counter:04d}"
                fc.write(f"{student_id}\n")
                student_id_counter += 1

    # Reset counter for grades
    student_id_counter = 1
    
    with open(grades_file, "w", encoding="utf-8", newline='') as fg:
        writer = csv.writer(fg)
        writer.writerow(["student_id","name","subject","score","credit","semester"])
        
        for c in range(1, num_classes + 1):
            for _ in range(num_students_per_class):
                student_id = f"SV_{test_num}_{student_id_counter:04d}"
                student_id_counter += 1
                name = generate_name()
                
                # Base performance of the student to create realistic profiles
                base_score = random.gauss(6.5, 1.5)
                base_score = max(3.0, min(9.5, base_score)) 
                
                # Sometime student performance trend up or down
                trend = random.choice([-0.2, -0.1, 0, 0.1, 0.2, 0.3])
                
                for sem in range(1, num_semesters + 1):
                    semester_name = f"HK{sem}_{2020 + (sem-1)//2}"
                    
                    # Number of subjects this semester
                    num_subjects = random.randint(3, 5)
                    random.shuffle(all_subjects)
                    sem_subjects = all_subjects[:num_subjects]
                    
                    current_base = base_score + (trend * sem)
                    
                    for subj in sem_subjects:
                        credit = subjects_dict[subj]
                        score = random.gauss(current_base, 1.0)
                        score = round(max(0.0, min(10.0, score)), 1)
                        writer.writerow([student_id, name, subj, score, credit, semester_name])

# Drastically reduced sizes
generate_test_data(1, 1, 5, 3)   # Small: 1 class, 5 students, 3 semesters
generate_test_data(2, 2, 8, 4)   # Medium: 2 classes, 8 students, 4 semesters
generate_test_data(3, 3, 20, 6)  # Large: 3 classes, 20 students, 6 semesters

# Add edge cases directly back into test 2
class_file_test2 = "data/test2/class.txt"
grades_file_test2 = "data/test2/grades.csv"

with open(class_file_test2, "a", encoding="utf-8") as fc:
    fc.write("SV_EDGE_001\n") 
    fc.write("SV_EDGE_002\n") 
    fc.write("SV_EDGE_003\n") 

with open(grades_file_test2, "a", encoding="utf-8", newline='') as fg:
    writer = csv.writer(fg)
    writer.writerow(["SV_EDGE_002", "Le Van Khong", "Toan Cao Cap", 0.0, 3, "HK1_2024"])
    writer.writerow(["SV_EDGE_002", "Le Van Khong", "Vat Ly", 0.0, 2, "HK1_2024"])
    writer.writerow(["SV_EDGE_003", "Mot Ky Duy Nhat", "Toan Cao Cap", 8.5, 3, "HK1_2024"])
    writer.writerow(["SV_EDGE_003", "Mot Ky Duy Nhat", "Vat Ly", 7.0, 2, "HK1_2024"])

print("Small data generated successfully!")
