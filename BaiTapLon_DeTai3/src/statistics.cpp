#include "statistics.h"
#include <cmath>
#include <algorithm>
#include <map>

double mean(const vector<double>& data) {
    if (data.empty()) return 0.0;
    double sum = 0;
    for (double x : data) sum += x;
    return sum / data.size();
}

double stdDev(const vector<double>& data) {
    if (data.size() <= 1) return 0.0;
    double m = mean(data);
    double sumSq = 0;
    for (double x : data) sumSq += (x - m) * (x - m);
    return sqrt(sumSq / data.size());
}

double minVal(const vector<double>& data) {
    if (data.empty()) return 0.0;
    return *min_element(data.begin(), data.end());
}

double maxVal(const vector<double>& data) {
    if (data.empty()) return 0.0;
    return *max_element(data.begin(), data.end());
}

vector<double> zScore(const vector<double>& data) {
    vector<double> result;
    if (data.empty()) return result;
    double m = mean(data);
    double s = stdDev(data);
    for (double x : data) {
        result.push_back(s > 0 ? (x - m) / s : 0.0);
    }
    return result;
}

SubjectStats statsBySubject(const vector<Cohort>& cohorts) {
    SubjectStats result;
    map<string, vector<double>> scoresBySubject;
    map<string, vector<string>> studentsBySubject;

    for (const auto& cohort : cohorts) {
        for (const auto& student : cohort.students) {
            for (const auto& grade : student.grades) {
                scoresBySubject[grade.subject].push_back(grade.score);
                studentsBySubject[grade.subject].push_back(student.id);
            }
        }
    }

    for (auto& entry : scoresBySubject) {
        SubjectStatEntry e;
        e.subject = entry.first;
        e.meanScore = mean(entry.second);
        e.stdScore = stdDev(entry.second);
        e.minScore = minVal(entry.second);
        e.maxScore = maxVal(entry.second);
        e.zScores = zScore(entry.second);
        e.studentIds = studentsBySubject[entry.first];
        result.entries.push_back(e);
    }
    return result;
}

ClassStats statsByClass(const vector<Cohort>& cohorts) {
    ClassStats result;
    for (const auto& cohort : cohorts) {
        vector<double> gpas;
        for (const auto& student : cohort.students)
            gpas.push_back(student.gpa);

        ClassStatEntry e;
        e.classId = cohort.class_id;
        e.meanGPA = mean(gpas);
        e.stdGPA = stdDev(gpas);
        e.minGPA = minVal(gpas);
        e.maxGPA = maxVal(gpas);
        result.entries.push_back(e);
    }
    return result;
}
