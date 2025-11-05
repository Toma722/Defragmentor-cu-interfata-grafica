#pragma once
#include <iostream>
#include "File.h"

#ifndef OOP_ALLOCATIONTABLE_H
#define OOP_ALLOCATIONTABLE_H

class AllocationTable {
private:
    File *files;
    std::string formatType;
    int fileCount, fileCapacity;

    void resize() { //functie netriviala
        int newFileCapacity = fileCapacity * 2;
        File *newFiles = new File[newFileCapacity];
        for (int i = 0; i < fileCount; i++) {
            newFiles[i] = std::move(files[i]);
        }
        delete[] files;
        files = newFiles;
        fileCapacity = newFileCapacity;
    }

public:
    explicit AllocationTable(const std::string &type = "", int initialCapacity = 2) {
        this->files = new File[initialCapacity];
        this->fileCount = 0;
        this->fileCapacity = initialCapacity;
        this->formatType = type;
    }

    AllocationTable(const AllocationTable &other) { //cc
        this->fileCount = other.fileCount;
        this->formatType = other.formatType;
        this->fileCapacity = other.fileCapacity;
        this->files = new File[other.fileCapacity];
        for (int i = 0; i < this->fileCount; i++) {
            this->files[i] = other.files[i];
        }
    }

    AllocationTable(AllocationTable &&other)  noexcept { //cm
        this->files = other.files;
        this->fileCount = other.fileCount;
        this->formatType = std::move(other.formatType);
        this->fileCapacity = other.fileCapacity;
        other.files = nullptr;
        other.fileCount = 0;
        other.fileCapacity = 0;
        other.formatType = "";

    }

    AllocationTable &operator=(const AllocationTable &other) {
        if (this == &other) {
            return *this;
        }

        File *tempFiles = new File[other.fileCapacity];

        for (int i = 0; i < other.fileCount; i++) {
            tempFiles[i] = other.files[i];
        }

        delete[] this->files;

        this->files = tempFiles;
        this->fileCapacity = other.fileCapacity;
        this->formatType = other.formatType;
        this->fileCount = other.fileCount;


        return *this;
    }

    AllocationTable &operator=(AllocationTable &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] this->files;
        this->files = other.files;
        this->fileCapacity = other.fileCapacity;
        this->formatType = std::move(other.formatType);
        this->fileCount = other.fileCount;

        other.files = nullptr;
        other.fileCount = 0;
        other.fileCapacity = 0;
        other.formatType = "";


        return *this;
    }

    void addFile(const File &fileToAdd) { //functie netriviala
        if (this->fileCount == this->fileCapacity) {
            this->resize();
        }
        this->files[this->fileCount] = fileToAdd;
        this->fileCount++;
    }

    ~AllocationTable() {
        delete[] this->files;
    }

    friend std::ostream &operator<<(std::ostream &os, const AllocationTable &allocationTable) {
        os<< "================ Tebelul de alocare (" << allocationTable.formatType <<") "
                << allocationTable.fileCount << "/" << allocationTable.fileCapacity << " fisiere ======";

        for (int i = 0; i < allocationTable.fileCount; i++) {
            os<< std::endl <<allocationTable.files[i];
        }

        os<< std::endl << "==================================";
        return os;
    }

};

#endif //OOP_ALLOCATIONTABLE_H