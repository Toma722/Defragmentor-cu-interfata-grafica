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

    if (checksumAlgorithmUsed == XOR) {
        strategy = std::make_unique<XorStrategy>();
    }
    else if (checksumAlgorithmUsed == WEIGHTED) {
        strategy = std::make_unique<WeightedStrategy>();
    }
    else {
        strategy = std::make_unique<Adler32Strategy>();
    }

    totalFiles++;
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
    if (alg == XOR) {
        strategy = std::make_unique<XorStrategy>();
    }
    else if (alg == WEIGHTED) {
        strategy = std::make_unique<WeightedStrategy>();
    }
    else {
        strategy = std::make_unique<Adler32Strategy>();
    }
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
    return strategy->calculate(fileBlocks);
}

void BaseFile::verifyChecksum(const DiskSpaceMap &disk) const {
    const size_t masterChecksum = getMasterChecksum();
    std::vector<Block> physicalBlocks;

    for (const int blockIndex : blockMap) {
        const Block &block = disk.getBlock(blockIndex);

        physicalBlocks.push_back(block);
    }

    size_t physicalChecksum = strategy->calculate(physicalBlocks);

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
