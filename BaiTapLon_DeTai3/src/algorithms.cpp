#include "algorithms.h"
#include <map>

// === KADANE'S ALGORITHM ===
// Áp dụng trên mảng delta (GPA_kỳ - GPA_TB)
// Tìm đoạn con có tổng lớn nhất
KadaneResult kadaneMaxGPA(const vector<double>& deltaGPAs) {
    KadaneResult result;
    if (deltaGPAs.empty()) return result;

    if (deltaGPAs.size() == 1) {
        result.start = 0;
        result.end = 0;
        result.maxSum = deltaGPAs[0];
        return result;
    }

    double maxEndingHere = deltaGPAs[0];
    double maxSoFar = deltaGPAs[0];
    int tempStart = 0;
    result.start = 0;
    result.end = 0;

    for (int i = 1; i < (int)deltaGPAs.size(); i++) {
        if (maxEndingHere + deltaGPAs[i] < deltaGPAs[i]) {
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

// === DÃY TĂNG LIÊN TIẾP ===
// Tìm chuỗi kỳ GPA tăng liên tiếp dài nhất
StreakResult longestIncreasingStreak(const vector<double>& semGPAs) {
    StreakResult result;
    if (semGPAs.empty()) return result;

    if (semGPAs.size() == 1) {
        result.start = 0;
        result.end = 0;
        result.length = 1;
        return result;
    }

    int curStart = 0, curLen = 1;
    int bestStart = 0, bestLen = 1;

    for (int i = 1; i < (int)semGPAs.size(); i++) {
        if (semGPAs[i] > semGPAs[i - 1]) {
            curLen++;
        } else {
            if (curLen > bestLen) {
                bestLen = curLen;
                bestStart = curStart;
            }
            curStart = i;
            curLen = 1;
        }
    }
    if (curLen > bestLen) {
        bestLen = curLen;
        bestStart = curStart;
    }

    result.start = bestStart;
    result.end = bestStart + bestLen - 1;
    result.length = bestLen;
    return result;
}

// === SLIDING WINDOW ===
// Tính GPA trung bình trượt với cửa sổ k kỳ
vector<double> slidingAvgGPA(const vector<double>& semGPAs, int k) {
    vector<double> result;
    int n = (int)semGPAs.size();
    if (n == 0 || k <= 0 || k > n) return result;

    double windowSum = 0;
    for (int i = 0; i < k; i++) windowSum += semGPAs[i];
    result.push_back(windowSum / k);

    for (int i = k; i < n; i++) {
        windowSum += semGPAs[i] - semGPAs[i - k];
        result.push_back(windowSum / k);
    }
    return result;
}

// Phát hiện sụt giảm: cần >= 2 lần giảm liên tiếp trong sliding avg
bool detectDecline(const vector<double>& slidingAvgs) {
    if (slidingAvgs.size() < 3) return false;

    int consecutiveDrops = 0;
    for (int i = 1; i < (int)slidingAvgs.size(); i++) {
        if (slidingAvgs[i] < slidingAvgs[i - 1]) {
            consecutiveDrops++;
            if (consecutiveDrops >= 2) return true;
        } else {
            consecutiveDrops = 0;
        }
    }
    return false;
}

// === PREFIX SUM ===
// Xây mảng prefix sum tín chỉ tích lũy theo kỳ
vector<int> prefixSumCredits(const vector<Grade>& grades, const vector<string>& semesters) {
    map<string, int> creditsBySem;
    for (const auto& g : grades) {
        creditsBySem[g.semester] += g.credit;
    }

    // prefix[0] = 0, prefix[i+1] = tổng tín chỉ từ kỳ 0 đến kỳ i
    vector<int> prefix(semesters.size() + 1, 0);
    for (int i = 0; i < (int)semesters.size(); i++) {
        prefix[i + 1] = prefix[i] + creditsBySem[semesters[i]];
    }
    return prefix;
}

// Truy vấn tổng tín chỉ đoạn [l, r] trong O(1)
int queryCredits(const vector<int>& prefix, int l, int r) {
    if (l < 0 || r < 0 || l >= (int)prefix.size() - 1 || r >= (int)prefix.size() - 1)
        return 0;
    if (l > r) return 0;
    return prefix[r + 1] - prefix[l];
}
