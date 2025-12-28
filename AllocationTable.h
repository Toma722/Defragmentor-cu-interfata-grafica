#pragma once
#include <iostream>
#include "BaseFile.h"

#ifndef OOP_ALLOCATIONTABLE_H
#define OOP_ALLOCATIONTABLE_H

class AllocationTable {
private:
    std::vector<std::unique_ptr<BaseFile>> files;
    std::string formatType;

public:
    explicit AllocationTable(const std::string &type = "", int initialCapacity = 2);

    AllocationTable(const AllocationTable &other);

    [[nodiscard]] BaseFile *findFileById(const int &fileId) const;

    [[nodiscard]] const std::vector<std::unique_ptr<BaseFile>> &getFiles();

    void updateBlockAddress(int fileId, int oldIndex, int newIndex) const;

    void addFile(const BaseFile &fileToAdd);

    bool deleteFile(const int &fileId);

    void runMaintenance(DiskSpaceMap &disk) const;

    AllocationTable &operator=(AllocationTable other);

    friend std::ostream &operator<<(std::ostream &os, const AllocationTable &allocationTable);

};

#endif //OOP_ALLOCATIONTABLE_H