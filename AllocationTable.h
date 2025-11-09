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
    explicit AllocationTable(const std::string &type = "", int initialCapacity = 2) {
        files.reserve(initialCapacity); //vreau doar sa aloc memoria
        this->formatType = type;
    }

    File *findFileById(const int &fileId) {
        for (auto & file : files) {
            if (file.getId() == fileId) {
                return &file;
            }
        }

        return nullptr;
    }

    // bool createNewFile(int id, int size, const std::string &name, DiskSpaceMap &disk) {
    //     File newFile(id, size, name);
    //     std::vector<int> map = disk.allocateFile(newFile);
    //     if (map.empty() == true) {
    //         std::cout<< "Crearea fisierului " << name << " a esuat!" << std::endl;
    //         return false;
    //     }
    //     newFile.setBlockMap(map);
    //     addFile(newFile);
    //     return true;
    // }

    [[nodiscard]] const std::vector<File> &getFiles() {
        return files;
    }

    void updateBlockAddress(const int fileId, const int oldIndex, const int newIndex) {
        for (auto & file : files) {
            if (file.getId() == fileId) {
                file.updatePhysicalAddress(oldIndex, newIndex);
            }
        }
    }

    void addFile(const File &fileToAdd) {
        files.push_back(fileToAdd);
    }

    bool deleteFile(const int &fileId) {
        for (int i = 0; i < static_cast<int>(files.size()); i++) {
            if (files[i].getId() == fileId) {
                std::swap(files[i], files.back());
                files.pop_back();
                return true;
            }
        }

        return false;
    }


    friend std::ostream &operator<<(std::ostream &os, const AllocationTable &allocationTable) {
        os<< "================ Tebelul de alocare (" << allocationTable.formatType <<") "
                << allocationTable.files.size() << "/" << allocationTable.files.capacity() << " fisiere ======";

        for (const auto & file : allocationTable.files) {
            os<< std::endl <<file;
        }

        os<< std::endl << "==================================";
        return os;
    }

};

#endif //OOP_ALLOCATIONTABLE_H