#pragma once
#include <iostream>
#include "File.h"

#ifndef OOP_ALLOCATIONTABLE_H
#define OOP_ALLOCATIONTABLE_H

class AllocationTable {
private:
    std::vector<File> files;
    std::string formatType;

public:
    explicit AllocationTable(const std::string &type = "", int initialCapacity = 2);

    File *findFileById(const int &fileId);

    [[nodiscard]] const std::vector<File> &getFiles();

    void updateBlockAddress(int fileId, int oldIndex, int newIndex);

    void addFile(const File &fileToAdd);

    bool deleteFile(const int &fileId);

    friend std::ostream &operator<<(std::ostream &os, const AllocationTable &allocationTable);

};

#endif //OOP_ALLOCATIONTABLE_H