#pragma once
#include <iostream>
#include "Block.h"
#include <vector>

class File;
class AllocationTable;

#ifndef OOP_DISKSPACEMAP_H
#define OOP_DISKSPACEMAP_H

class DiskSpaceMap {
    private:
        std::vector<Block> diskBlocks;
    public:
        explicit DiskSpaceMap(const int totalBlocks = 0) {
            diskBlocks.resize(totalBlocks);
        }

        // void markBlockAsDamaged(const int blockIndex) {
        //     diskBlocks[blockIndex].markAsBad();
        // }

        void defragment(AllocationTable &table);

        //asta ar fi first-fit
        [[nodiscard]] int findSpace(const File& file) const;

        [[nodiscard]] bool isSpace(const int numberOfBlocks) const {
            int freeCount = 0;
            for (const auto & diskBlock : diskBlocks) {
                if (diskBlock.getOccupied() == false) {
                    freeCount++;
                }
                if (freeCount == numberOfBlocks) {
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]] std::vector<int> allocateFile(const File &file);

        [[nodiscard]] int findFirstFreeBlock() const {
            for (int i = 0; i < static_cast<int>(diskBlocks.size()); i++) {
                if (diskBlocks[i].getOccupied() == false && diskBlocks[i].isBad() == false) {
                    return i;
                }
            }
            return -1;//discul este plin
        }

        bool occupyBlock(const int blockIndex, const Block &dataBlock) {
            if (diskBlocks[blockIndex].getOccupied() == true) {
                return false;
            }

            const unsigned long dataToCopy = dataBlock.getContent();

            diskBlocks[blockIndex].setData(blockIndex, true, dataToCopy, 4096);
            return true;
        }

        void freeBlocks(const std::vector<int> &blockMap) {
            for (const int blockIndex : blockMap) {
                Block &block = diskBlocks[blockIndex];
                block.clear();
            }
        }

        //void relocateDamagedBlocks(AllocationTable &table);

        [[nodiscard]] int getNumBlocks() const {
            return static_cast<int>(diskBlocks.size());
        }

        [[nodiscard]] const Block &getBlock(const int index) const {
            return diskBlocks[index];
        }


        friend std::ostream &operator<<(std::ostream &os, const DiskSpaceMap &diskSpaceMap) {
            os<<"DiskSpaceMap: "<<diskSpaceMap.diskBlocks.size()<<"total blocks -> ";
            for (int i = 0; i < static_cast<int>(diskSpaceMap.diskBlocks.size()); i++) {
                os<<diskSpaceMap.diskBlocks[i]<<" ";
                if (i % 10 == 0) {
                    os<<std::endl;
                }
            }
            return os;
        }

};


#endif //OOP_DISKSPACEMAP_H