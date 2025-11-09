#include "AllocationTable.h"

AllocationTable::AllocationTable(const std::string &type, int initialCapacity) {
    files.reserve(initialCapacity); //vreau doar sa aloc memoria
    this->formatType = type;
}

File *AllocationTable::findFileById(const int &fileId) {
    for (auto & file : files) {
        if (file.getId() == fileId) {
            return &file;
        }
    }

    return nullptr;
}

[[nodiscard]] const std::vector<File> &AllocationTable::getFiles() {
    return files;
}

void AllocationTable::updateBlockAddress(const int fileId, const int oldIndex, const int newIndex) {
    for (auto & file : files) {
        if (file.getId() == fileId) {
            file.updatePhysicalAddress(oldIndex, newIndex);
        }
    }
}

void AllocationTable::addFile(const File &fileToAdd) {
    files.push_back(fileToAdd);
}

bool AllocationTable::deleteFile(const int &fileId) {
    for (int i = 0; i < static_cast<int>(files.size()); i++) {
        if (files[i].getId() == fileId) {
            std::swap(files[i], files.back());
            files.pop_back();
            return true;
        }
    }

    return false;
}

std::ostream &operator<<(std::ostream &os, const AllocationTable &allocationTable) {
    os<< "================ Tebelul de alocare (" << allocationTable.formatType <<") "
            << allocationTable.files.size() << "/" << allocationTable.files.capacity() << " fisiere ======";

    for (const auto & file : allocationTable.files) {
        os<< std::endl <<file;
    }

    os<< std::endl << "==================================";
    return os;
}