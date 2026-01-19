#ifndef OOP_STATTRACKER_H
#define OOP_STATTRACKER_H
#include <limits>

template<typename T>
class StatTracker {
private:
    T minValue, maxValue, sum;
    int count;
public:
    StatTracker() {
        minValue = std::numeric_limits<T>::max();
        maxValue = std::numeric_limits<T>::min();
        sum = 0;
        count = 0;
    }

    void addValue(T val) {
        if (val < minValue) minValue = val;
        if (val > maxValue) maxValue = val;
        sum += val;
        count++;
    }

    T getMin() const {
        if (count == 0) return 0;
        return minValue;
    }

    T getMax() const {
        if (count == 0) return 0;
        return maxValue;
    }

    [[nodiscard]] double getAverage() const {
        if (count == 0) return 0.0;
        return static_cast<double>(sum) / count;
    }

    [[nodiscard]] int getCount() const {
        return count;
    }
};


#endif //OOP_STATTRACKER_H