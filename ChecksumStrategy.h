#ifndef OOP_CHECKSUMSTRATEGY_H
#define OOP_CHECKSUMSTRATEGY_H
#include <vector>
#include "Block.h"


class ChecksumStrategy {
public:
    [[nodiscard]] virtual size_t calculate(const std::vector<Block>&blocks) const = 0;
    virtual ~ChecksumStrategy() = default;
};

class XorStrategy : public ChecksumStrategy {
public:
    [[nodiscard]] size_t calculate(const std::vector<Block> &blocks) const override;
};

class Adler32Strategy : public ChecksumStrategy {
public:
    [[nodiscard]] size_t calculate(const std::vector<Block> &blocks) const override;
};

class WeightedStrategy : public ChecksumStrategy {
public:
    [[nodiscard]] size_t calculate(const std::vector<Block> &blocks) const override;
};
#endif //OOP_CHECKSUMSTRATEGY_H