#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Block.h"
#include "DiskSpaceMap.h"

#ifndef OOP_FILE_H
#define OOP_FILE_H

enum checksumAlgorithm {
    XOR, WEIGHTED, ADLER32
};

class BaseFile {
    protected:
        std::vector<Block> fileBlocks;
        std::vector<int> blockMap;
        int id;
        std::string name;
        static int totalFiles;
        bool isHighPriority, markedForDeletion, unmovable;

        virtual void doPrint(std::ostream &os) const = 0;

    private:
        checksumAlgorithm checksumAlgorithmUsed;

        static size_t calculateChecksumXOR(const std::vector<Block> &blocksToScan);

        static size_t calculateChecksumWeighted(const std::vector<Block> &blocksToScan);

        static size_t calculateChecksumAdler32(const std::vector<Block> &blocksToScan);

    public:
        explicit BaseFile(int id = 0, int size = 0, const std::string &name = "",
                          checksumAlgorithm alg = checksumAlgorithm::ADLER32, bool isHighPriority = false,
                          bool markedForDeletion = false, bool unmovable = false);

        BaseFile(const BaseFile& other);

        virtual void storagePriority(DiskSpaceMap &disk) = 0;

        virtual void applyMaintenance(DiskSpaceMap &disk) = 0;

        [[nodiscard]] int getNumBlocks() const;

        [[nodiscard]] int getId() const;

        [[nodiscard]]const std::string &getName() const;

        [[nodiscard]] const std::vector<int> &getBlockMap() const;

        [[nodiscard]] const Block &getBlock(int index) const;

        void truncate(int newSize, DiskSpaceMap &disk);

        bool extend(int blocksToAdd, DiskSpaceMap &disk);

        void updatePhysicalAddress(int oldIndex, int newIndex);

        void setBlockMap(const std::vector<int> &map);

        [[nodiscard]] size_t getMasterChecksum() const;

        void verifyChecksum(const DiskSpaceMap &disk) const;

        [[nodiscard]] virtual std::unique_ptr<BaseFile> clone() const = 0;

        void display(std::ostream &os) const;

        void setUnmovable();

        [[nodiscard]] bool isUnmovable() const;

        void setMarkedForDeletion();

        [[nodiscard]] bool isMarkedForDeletion() const;

        static int getTotalFiles() {
            return totalFiles;
        };

        void setIsHighPriority();

        friend std::ostream &operator<<(std::ostream &os, const BaseFile &file);

        virtual ~BaseFile();
};

#endif //OOP_FILE_H