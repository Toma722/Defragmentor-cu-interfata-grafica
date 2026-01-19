#ifndef OOP_UTILS_H
#define OOP_UTILS_H

template <typename T>
bool validateRange(T value, T min, T max) {
    return (value >= min && value <= max);
}

#endif //OOP_UTILS_H