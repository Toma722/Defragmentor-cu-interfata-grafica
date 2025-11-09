#include "DiskSpaceMap.h"

#include "AllocationTable.h"
#include "File.h"

int DiskSpaceMap::findSpace(const File& file) const{
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

std::vector<int> DiskSpaceMap::allocateFile(const File& file) {
    std::vector<int> allocatedBlocks;
    int startIndex = findSpace(file);
    if (startIndex == -1) {
        return allocatedBlocks;
    }
    for (int i = 0; i < file.getNumBlocks(); i++) {
        const Block &sourceBlock = file.getBlock(i);
        unsigned long dataToCopy = sourceBlock.getContent();
        diskBlocks[startIndex + i].setData(startIndex + i, true, dataToCopy, 4096);
        allocatedBlocks.push_back(startIndex + i);
    }
    return allocatedBlocks;
}

// void DiskSpaceMap::relocateDamagedBlocks(AllocationTable &table) {
//     for (int i = 0; i < static_cast<int>(diskBlocks.size()); i++) {
//         if (diskBlocks[i].isBad() == true && diskBlocks[i].getOccupied() == true) {
//             int fileId = static_cast<int>(diskBlocks[i].getContent());
//             int oldIndex = i;
//             int newIndex = findFirstFreeBlock();
//             if (newIndex == -1) {
//                 std::cout<< "Discul este plin! Nu se poate reloca blocul " << i << std::endl;
//                 return;
//             }
//
//             occupyBlock(newIndex, diskBlocks[oldIndex]);
//             table.updateBlockAddress(fileId, oldIndex, newIndex);
//             diskBlocks[oldIndex].markAsBad();
//         }
//     }
// };

void DiskSpaceMap::defragment(AllocationTable &table) { //Compacteaza blocurile complet
    int emptySlot = 0;
    for (int blockToScan = 0; blockToScan < static_cast<int>(diskBlocks.size()); blockToScan++) {
        if (diskBlocks[blockToScan].getOccupied() == true) {
            if (blockToScan != emptySlot) {
                int fileId = static_cast<int>(diskBlocks[blockToScan].getContent());
                int size = diskBlocks[blockToScan].getSize();

                diskBlocks[emptySlot].setData(emptySlot, true, fileId, size);

                table.updateBlockAddress(fileId, blockToScan, emptySlot);

                diskBlocks[blockToScan].clear();

            }

            emptySlot++;
        }
    }
};