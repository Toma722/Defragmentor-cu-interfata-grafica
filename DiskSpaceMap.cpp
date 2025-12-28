#include "DiskSpaceMap.h"
#include "AllocationTable.h"
#include "BaseFile.h"
#include "Exceptions.h"

DiskSpaceMap::DiskSpaceMap(const int totalBlocks) {
    diskBlocks.resize(totalBlocks);
}

int DiskSpaceMap::findSpace(const BaseFile& file) const{
    int freeCount = 0;
    for (int i = 0; i < static_cast<int>((diskBlocks).size()); i++) {
        if (diskBlocks[i].getOccupied() == false && diskBlocks[i].isBad() == false) {
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

int DiskSpaceMap::findSpace(const int numOfBlocks) const{
    int freeCount = 0;
    for (int i = 0; i < static_cast<int>((diskBlocks).size()); i++) {
        if (diskBlocks[i].getOccupied() == false && diskBlocks[i].isBad() == false) {
            freeCount++;
            if (freeCount == numOfBlocks) {
                return i - numOfBlocks + 1;
            }
        }
        else {
            freeCount = 0;
        }
    }
    return -1;
}

std::vector<int> DiskSpaceMap::allocateFile(const BaseFile& file) {
    std::vector<int> allocatedBlocks;
    const int startIndex = findSpace(file);
    if (startIndex == -1) {
        return allocatedBlocks;
    }
    for (int i = 0; i < file.getNumBlocks(); i++) {
        const Block &sourceBlock = file.getBlock(i);
        const unsigned long dataToCopy = sourceBlock.getContent();
        diskBlocks[startIndex + i].setData(startIndex + i, true, dataToCopy, 4096);
        allocatedBlocks.push_back(startIndex + i);
    }
    return allocatedBlocks;
}

void DiskSpaceMap::relocateDamagedBlocks(const AllocationTable &table) {
    for (int i = 0; i < static_cast<int>(diskBlocks.size()); i++) {
        if (diskBlocks[i].isBad() == true && diskBlocks[i].getOccupied() == true) {
            const int fileId = static_cast<int>(diskBlocks[i].getContent());
            const int oldIndex = i;
            const int newIndex = findFirstFreeBlock();
            if (newIndex == -1) {
                std::cout<< "Discul este plin! Nu se poate reloca blocul " << i << std::endl;
                return;
            }

            occupyBlock(newIndex, diskBlocks[oldIndex]);
            table.updateBlockAddress(fileId, oldIndex, newIndex);
            diskBlocks[oldIndex].clear();

            diskBlocks[oldIndex].markAsBad();

        }
    }
};

// void DiskSpaceMap::defragment(AllocationTable &table) { //Compacteaza blocurile complet
//     int emptySlot = 0;
//     for (int blockToScan = 0; blockToScan < static_cast<int>(diskBlocks.size()); blockToScan++) {
//         if (diskBlocks[blockToScan].getOccupied() == true) {
//             if (blockToScan != emptySlot) {
//                 const int fileId = static_cast<int>(diskBlocks[blockToScan].getContent());
//                 const int size = diskBlocks[blockToScan].getSize();
//
//                 diskBlocks[emptySlot].setData(emptySlot, true, fileId, size);
//
//                 table.updateBlockAddress(fileId, blockToScan, emptySlot);
//
//                 diskBlocks[blockToScan].clear();
//
//             }
//
//             emptySlot++;
//         }
//     }
// }

Block &DiskSpaceMap::getBlockRef(const int index) {
    return diskBlocks[index];
}

[[nodiscard]] bool DiskSpaceMap::isSpace(const int numberOfBlocks) const {
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

[[nodiscard]] int DiskSpaceMap::getTotalFreeBlocks() const {
    int countFreeBlocks = 0;
    for (const auto & diskBlock : diskBlocks) {
        if (diskBlock.isBad() == false && diskBlock.getOccupied() == false) {
            countFreeBlocks++;
        }
    }
    return countFreeBlocks;
}

[[nodiscard]] int DiskSpaceMap::getTotalUsedBlocks() const {
    int usedBlocks = 0;
    for (const auto & diskBlock: diskBlocks) {
        if (diskBlock.getOccupied() == true) {
            usedBlocks++;
        }
    }
    return usedBlocks;
}

[[nodiscard]] int DiskSpaceMap::getTotalBadBlocks() const {
    int badBlocks = 0;
    for (const auto & diskBlock: diskBlocks) {
        if (diskBlock.isBad() == true) {
            badBlocks++;
        }
    }
    return badBlocks;
}

[[nodiscard]] double DiskSpaceMap::getFragmentationPercentage() const {
    int countFreeBlocks = 0, largestContiguousSpace = 0;
    for (const auto & diskBlock: diskBlocks) {
        if (diskBlock.isBad() == true || diskBlock.getOccupied() == true) {
            if (countFreeBlocks > largestContiguousSpace) {
                largestContiguousSpace = countFreeBlocks;
                countFreeBlocks = 0;
            }
        }
        else {
            countFreeBlocks++;
        }
    }
    if (countFreeBlocks > largestContiguousSpace) {
        largestContiguousSpace = countFreeBlocks;
    }
    return 1.0 - static_cast<double>(largestContiguousSpace) / static_cast<double>(getTotalFreeBlocks());
};

[[nodiscard]] int DiskSpaceMap::findFirstFreeBlock() const {
    for (int i = 0; i < static_cast<int>(diskBlocks.size()); i++) {
        if (diskBlocks[i].getOccupied() == false && diskBlocks[i].isBad() == false) {
            return i;
        }
    }
    return -1;//discul este plin
}

bool DiskSpaceMap::occupyBlock(const int blockIndex, const Block &dataBlock) {
    if (diskBlocks[blockIndex].getOccupied() == true) {
        throw DiskFullException();
    }

    const unsigned long dataToCopy = dataBlock.getContent();

    diskBlocks[blockIndex].setData(blockIndex, true, dataToCopy, 4096);
    return true;
}

void DiskSpaceMap::freeBlocks(const std::vector<int> &blockMap) {
    for (const int blockIndex : blockMap) {
        Block &block = diskBlocks[blockIndex];
        block.clear();
    }
}

[[nodiscard]] int DiskSpaceMap::getNumBlocks() const {
    return static_cast<int>(diskBlocks.size());
}

[[nodiscard]] const Block &DiskSpaceMap::getBlock(const int index) const {
    return diskBlocks[index];
}

std::ostream &operator<<(std::ostream &os, const DiskSpaceMap &diskSpaceMap) {
    os<<"DiskSpaceMap: "<<diskSpaceMap.diskBlocks.size()<<"total blocks -> ";
    for (int i = 0; i < static_cast<int>(diskSpaceMap.diskBlocks.size()); i++) {
        os<<diskSpaceMap.diskBlocks[i]<<" ";
        if (i % 10 == 0) {
            os<<std::endl;
        }
    }
    return os;
}

void DiskSpaceMap::markBlockAsDamaged(const int blockIndex) {
    diskBlocks[blockIndex].markAsBad();
}