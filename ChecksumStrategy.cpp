#include "ChecksumStrategy.h"

size_t XorStrategy::calculate(const std::vector<Block> &blocks) const {
    size_t sum = 0;
    for (const auto & block : blocks) {
        sum ^= block.getContent();
        sum += (block.isBad() ? 1 : 0);
    }

    return sum;
}

size_t Adler32Strategy::calculate(const std::vector<Block> &blocks) const {
    size_t sumA = 1;
    size_t sumB = 0;
    for (const auto & block : blocks) {
        constexpr size_t prime = 65521;
        sumA += block.getContent();
        sumA += (block.isBad() ? 1 : 0);
        sumA %= prime;
        sumB += sumA;
        sumB %= prime;
    }

    return (sumB << 16) | sumA;
}

size_t WeightedStrategy::calculate(const std::vector<Block> &blocks) const {
    size_t sum = 0;
    for (int i = 0; i < static_cast<int>(blocks.size()); i++) {
        sum += blocks[i].getContent() * (i + 1);
        sum += (blocks[i].isBad() ? 1 : 0);
    }
    return sum;
}
