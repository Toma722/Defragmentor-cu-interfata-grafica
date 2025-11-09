#pragma once
#include <iostream>
#include <vector>
#include "Block.h"
#include "DiskSpaceMap.h"

#ifndef OOP_FILE_H
#define OOP_FILE_H

enum checksumAlgorithm {
    XOR, WEIGHTED, ADLER32
};

class File {
    private:
        std::vector<Block> fileBlocks;
        std::vector<int> blockMap;
        int id;
        std::string name;
        checksumAlgorithm checksumAlgorithmUsed;

        static size_t calculateChecksumXOR(const std::vector<Block> &blocksToScan);

        static size_t calculateChecksumWeighted(const std::vector<Block> &blocksToScan);

        static size_t calculateChecksumAdler32(const std::vector<Block> &blocksToScan);

    public:
        explicit File(int id = 0, int size = 0, const std::string &name = "",
                      checksumAlgorithm alg = checksumAlgorithm::ADLER32);


        [[nodiscard]] int getNumBlocks() const;

        [[nodiscard]] int getId() const;

        [[nodiscard]] const std::vector<int> &getBlockMap() const;

        [[nodiscard]] const Block &getBlock(int index) const;

        void truncate(int newSize, DiskSpaceMap &disk);

        bool extend(int blocksToAdd, DiskSpaceMap &disk);

        void updatePhysicalAddress(int oldIndex, int newIndex);

        void setBlockMap(const std::vector<int> &map);

        [[nodiscard]] size_t getMasterChecksum() const;

        [[nodiscard]] bool verifyChecksum(const DiskSpaceMap &disk) const;

        friend std::ostream &operator<<(std::ostream &os, const File &file);
};

#endif //OOP_FILE_H