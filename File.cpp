#include "File.h"

size_t File::calculateChecksumXOR(const std::vector<Block> &blocksToScan) {
    size_t sum = 0;
    for (const auto & block : blocksToScan) {
        sum ^= block.getContent();
    }

    return sum;
}

size_t File::calculateChecksumWeighted(const std::vector<Block> &blocksToScan) {
    size_t sum = 0;
    for (int i = 0; i < static_cast<int>(blocksToScan.size()); i++) {
        sum += blocksToScan[i].getContent() * (i + 1);
    }
    return sum;
}

size_t File::calculateChecksumAdler32(const std::vector<Block> &blocksToScan) {
    size_t sumA = 1;
    size_t sumB = 0;
    for (const auto & block : blocksToScan) {
        constexpr size_t prime = 65521;
        sumA += block.getContent();
        sumA %= prime;
        sumB += sumA;
        sumB %= prime;
    }

    return (sumB << 16) | sumA;
}

File::File(int id, int size, const std::string &name ,
                      checksumAlgorithm alg) {
    this->id = id;
    this->name = name;
    fileBlocks.resize(size);
    checksumAlgorithmUsed = alg;
    for (int i = 0; i < size; i++) {
        fileBlocks[i].setData( i + 1,true, this->id, 4096);
    }
}

[[nodiscard]] int File::getNumBlocks() const {
    return static_cast<int>(fileBlocks.size());
}

[[nodiscard]] int File::getId() const {
    return this->id;
}

[[nodiscard]] std::string File::getName() const {
    return name;
}


[[nodiscard]] const std::vector<int> &File::getBlockMap() const {
    return blockMap;
}

[[nodiscard]] const Block &File::getBlock(const int index) const {
    return fileBlocks[index];
}

void File::truncate(const int newSize, DiskSpaceMap &disk) {
    std::vector<int> blocksToFree;
    if (newSize >= static_cast<int>(fileBlocks.size())) {
        return;
    }

    for (int i = newSize; i < static_cast<int>(blockMap.size()); i++) {
        blocksToFree.push_back(blockMap[i]);
    }
    fileBlocks.resize(newSize);
    blockMap.resize(newSize);
    disk.freeBlocks(blocksToFree);
}

bool File::extend(const int blocksToAdd, DiskSpaceMap &disk) { //aici se produce fragmentarea
    if (blockMap.empty() == true) {
        return false;
    }

    const int currentSize = getNumBlocks();

    if (disk.isSpace(blocksToAdd) == false) {
        return false;
    }

    for (int i = 0; i < blocksToAdd; i++) {
        int newBlockIndex = disk.findFirstFreeBlock();
        const int newLogicalIndex = currentSize + i;
        Block newBlock(newLogicalIndex, 4096, true, this->id);
        disk.occupyBlock(newBlockIndex, newBlock);

        blockMap.push_back(newBlockIndex);
        fileBlocks.push_back(newBlock);

    }

    return true;
}

void File::updatePhysicalAddress(const int oldIndex, const int newIndex) {
    for (int & i : blockMap) {
        if (i == oldIndex) {
            i = newIndex;
        }
    }
}

void File::setBlockMap(const std::vector<int> &map) {
    blockMap = map;
}

[[nodiscard]] size_t File::getMasterChecksum() const {
    switch (checksumAlgorithmUsed) {
        case checksumAlgorithm::ADLER32: return calculateChecksumAdler32(this->fileBlocks);

        case checksumAlgorithm::WEIGHTED: return calculateChecksumWeighted(this->fileBlocks);

        case checksumAlgorithm::XOR: return calculateChecksumXOR(this->fileBlocks);

        default: return calculateChecksumAdler32(this->fileBlocks);
    }
}

[[nodiscard]] bool File::verifyChecksum(const DiskSpaceMap &disk) const {
    const size_t masterChecksum = getMasterChecksum();
    std::vector<Block> physicalBlocks;

    for (int blockIndex : blockMap) {
        const Block &block = disk.getBlock(blockIndex);
        physicalBlocks.push_back(block);
    }

    size_t physicalChecksum;
    switch (checksumAlgorithmUsed) {
        case ADLER32: physicalChecksum = calculateChecksumAdler32(physicalBlocks); break;

        case WEIGHTED: physicalChecksum = calculateChecksumWeighted(physicalBlocks); break;

        case XOR: physicalChecksum = calculateChecksumXOR(physicalBlocks); break;

        default: physicalChecksum = calculateChecksumAdler32(physicalBlocks);//consecvent cu getMasterChecksum
    }

    return physicalChecksum == masterChecksum;
}

std::ostream &operator<<(std::ostream &os, const File &file) {
    os<<"File "<<file.id<<": "<<file.name << " -> ";
    for (const auto & fileBlock : file.fileBlocks) {
        os<< fileBlock << " ";
    }
    return os;
}