#pragma once
#include <iostream>
#include "Block.h"
#include "File.h"

#ifndef OOP_DISKSPACEMAP_H
#define OOP_DISKSPACEMAP_H


class DiskSpaceMap {
    private:
        Block *diskBlocks;
        int totalBlocks;
    public:
        explicit DiskSpaceMap(int totalBlocks = 0) {
            this->totalBlocks = totalBlocks;
            this->diskBlocks = new Block[totalBlocks];
        }

        DiskSpaceMap(const DiskSpaceMap &other) { //cc
            this->totalBlocks = other.totalBlocks;
            this->diskBlocks = new Block[other.totalBlocks];
            for (int i = 0; i < other.totalBlocks; i++) {
                this->diskBlocks[i] = other.diskBlocks[i];
            }
        }

        DiskSpaceMap(DiskSpaceMap &&other) noexcept { //cm
            this->totalBlocks = other.totalBlocks;
            this->diskBlocks = other.diskBlocks;
            other.diskBlocks = nullptr;
            other.totalBlocks = 0;
        }

        DiskSpaceMap &operator=(const DiskSpaceMap &other) {
            if (this == &other) {
                return *this;
            }

            auto *tempBlocks = new Block[other.totalBlocks];



            for (int i = 0; i < other.totalBlocks; i++) {
                tempBlocks[i] = other.diskBlocks[i];
            }

            delete[] this->diskBlocks;

            this->diskBlocks = tempBlocks;
            this->totalBlocks = other.totalBlocks;

            return *this;
        }

        DiskSpaceMap &operator=(DiskSpaceMap &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            delete[] this->diskBlocks;
            this->diskBlocks = other.diskBlocks;
            this->totalBlocks = other.totalBlocks;
            other.diskBlocks = nullptr;
            other.totalBlocks = 0;

            return *this;
        }

        //asta ar fi first-fit
        [[nodiscard]] int findSpace(const File& file) const { //functie netriviala
            int freeCount = 0;
            for (int i = 0; i < this->totalBlocks; i++) {
                if (diskBlocks[i].getOccupied() == false) {
                    freeCount++;
                    if (freeCount == file.getNumBlocks()) {
                        return i - file.getNumBlocks() + 1;
                    }
                }
                else {
                    freeCount = 0;
                }
            }
            return -1;
        }

        //functie netriviala
        [[nodiscard]] bool allocateFileBlocks(const File &file) const {
            int startIndex = findSpace(file);
            if (startIndex == -1) {
                return false;
            }
            int fileId = file.getId();
            for (int i = 0; i < file.getNumBlocks(); i++) {
                this->diskBlocks[startIndex + i].setData(startIndex + i, true, fileId);
            }
            return true;
        }

        ~DiskSpaceMap() {
            delete[] this->diskBlocks;
        }

        friend std::ostream &operator<<(std::ostream &os, const DiskSpaceMap &diskSpaceMap) {
            os<<"DiskSpaceMap: "<<diskSpaceMap.totalBlocks<<"total blocks -> ";
            for (int i = 0; i < diskSpaceMap.totalBlocks; i++) {
                os<<diskSpaceMap.diskBlocks[i]<<" ";
            }
            return os;
        }

};


#endif //OOP_DISKSPACEMAP_H