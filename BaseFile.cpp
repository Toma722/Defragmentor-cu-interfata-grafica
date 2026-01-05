#include "BaseFile.h"

#include "Exceptions.h"

int BaseFile::totalFiles = 0;

BaseFile::BaseFile(const BaseFile &other) {
    this->id = other.id;
    this->name = other.name;
    this->fileBlocks = other.fileBlocks;
    this->blockMap = other.blockMap;
    this->isHighPriority = other.isHighPriority;
    this->markedForDeletion = other.markedForDeletion;
    this->unmovable = other.unmovable;
    this->checksumAlgorithmUsed = other.checksumAlgorithmUsed;

    totalFiles++;
}

size_t BaseFile::calculateChecksumXOR(const std::vector<Block> &blocksToScan) {
    size_t sum = 0;
    for (const auto & block : blocksToScan) {
        sum ^= block.getContent();
        sum += (block.isBad() ? 1 : 0);
    }

    return sum;
}

size_t BaseFile::calculateChecksumWeighted(const std::vector<Block> &blocksToScan) {
    size_t sum = 0;
    for (int i = 0; i < static_cast<int>(blocksToScan.size()); i++) {
        sum += blocksToScan[i].getContent() * (i + 1);
        sum += (blocksToScan[i].isBad() ? 1 : 0);
    }
    return sum;
}

size_t BaseFile::calculateChecksumAdler32(const std::vector<Block> &blocksToScan) {
    size_t sumA = 1;
    size_t sumB = 0;
    for (const auto & block : blocksToScan) {
        constexpr size_t prime = 65521;
        sumA += block.getContent();
        sumA += (block.isBad() ? 1 : 0);
        sumA %= prime;
        sumB += sumA;
        sumB %= prime;
    }

    return (sumB << 16) | sumA;
}

BaseFile::BaseFile(const int id, const int size, const std::string &name ,
                      const checksumAlgorithm alg, const bool isHighPriority, const bool markedForDeletion, const bool unmovable) {
    this->id = id;
    this->name = name;
    fileBlocks.resize(size);
    checksumAlgorithmUsed = alg;
    for (int i = 0; i < size; i++) {
        fileBlocks[i].setData( i + 1,true, this->id, 4096);
    }
    this->isHighPriority = isHighPriority;
    this->markedForDeletion = markedForDeletion;
    this->unmovable = unmovable;
    totalFiles++;
}

[[nodiscard]] int BaseFile::getNumBlocks() const {
    return static_cast<int>(fileBlocks.size());
}

[[nodiscard]] int BaseFile::getId() const {
    return this->id;
}

[[nodiscard]]const std::string &BaseFile::getName() const {
    return name;
}

[[nodiscard]] const std::vector<int> &BaseFile::getBlockMap() const {
    return blockMap;
}

[[nodiscard]] const Block &BaseFile::getBlock(const int index) const {
    return fileBlocks[index];
}

void BaseFile::truncate(const int newSize, DiskSpaceMap &disk) {
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

bool BaseFile::extend(const int blocksToAdd, DiskSpaceMap &disk) { //aici se produce fragmentarea
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

void BaseFile::updatePhysicalAddress(const int oldIndex, const int newIndex) {
    for (int & i : blockMap) {
        if (i == oldIndex) {
            i = newIndex;
        }
    }
}

void BaseFile::setBlockMap(const std::vector<int> &map) {
    blockMap = map;
}

[[nodiscard]] size_t BaseFile::getMasterChecksum() const {
    switch (checksumAlgorithmUsed) {
        case checksumAlgorithm::ADLER32: return calculateChecksumAdler32(this->fileBlocks);

        case checksumAlgorithm::WEIGHTED: return calculateChecksumWeighted(this->fileBlocks);

        case checksumAlgorithm::XOR: return calculateChecksumXOR(this->fileBlocks);

        default: return calculateChecksumAdler32(this->fileBlocks);
    }
}

void BaseFile::verifyChecksum(const DiskSpaceMap &disk) const {
    const size_t masterChecksum = getMasterChecksum();
    std::vector<Block> physicalBlocks;

    for (const int blockIndex : blockMap) {
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

    if (physicalChecksum != masterChecksum) {
        throw CorruptedDataException();
    }
}

void BaseFile::display(std::ostream &os) const {
    os<<"File "<<this->id<<": "<<this->name << " -> ";
    doPrint(os);
    for (const auto & fileBlock : this->fileBlocks) {
        os<< fileBlock << " ";
    }
};

void BaseFile::setUnmovable() {
    unmovable = true;
}

void BaseFile::setMarkedForDeletion() {
    markedForDeletion = true;
}

void BaseFile::setIsHighPriority() {
    isHighPriority = true;
}

bool BaseFile::isUnmovable() const {
    return unmovable;
}

bool BaseFile::isMarkedForDeletion() const {
    return markedForDeletion;
}


BaseFile::~BaseFile() {
    totalFiles--;
}

std::ostream &operator<<(std::ostream &os, const BaseFile &file) {
    file.display(os);
    return os;
}
