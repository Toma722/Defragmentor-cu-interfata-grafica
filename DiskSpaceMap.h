#pragma once
#include <iostream>
#include "Block.h"
#include <vector>

class BaseFile;
class AllocationTable;

#ifndef OOP_DISKSPACEMAP_H
#define OOP_DISKSPACEMAP_H

class DiskSpaceMap {
    private:
        std::vector<Block> diskBlocks;
    public:
        explicit DiskSpaceMap(int totalBlocks = 0);

        void markBlockAsDamaged(int blockIndex);

        //void defragment(AllocationTable &table);

        [[nodiscard]] int findSpace(const BaseFile& file) const;

        [[nodiscard]] int findSpace(int numOfBlocks) const;

        [[nodiscard]] bool isSpace(int numberOfBlocks) const;

        [[nodiscard]] std::vector<int> allocateFile(const BaseFile &file);

        [[nodiscard]] int findFirstFreeBlock() const;

        bool occupyBlock(int blockIndex, const Block &dataBlock);

        void freeBlocks(const std::vector<int> &blockMap);

        void relocateDamagedBlocks(const AllocationTable &table);

        [[nodiscard]] int getTotalFreeBlocks() const;

        [[nodiscard]] int getTotalUsedBlocks() const;

        [[nodiscard]] int getTotalBadBlocks() const;

        [[nodiscard]] double getFragmentationPercentage() const;

        [[nodiscard]] int getNumBlocks() const;

        [[nodiscard]] const Block &getBlock(int index) const;

        Block &getBlockRef(int index);


        friend std::ostream &operator<<(std::ostream &os, const DiskSpaceMap &diskSpaceMap);

};


#endif //OOP_DISKSPACEMAP_H