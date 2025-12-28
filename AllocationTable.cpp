#include "AllocationTable.h"
#include "SystemFile.h"
#include "TempFile.h"
#include "UserFile.h"

AllocationTable::AllocationTable(const std::string &type, const int initialCapacity) {
    files.reserve(initialCapacity);
    this->formatType = type;
}

BaseFile *AllocationTable::findFileById(const int &fileId) const {
    for (auto & file : files) {
        if (file->getId() == fileId) {
            return file.get();
        }
    }

    return nullptr;
}

[[nodiscard]] const std::vector<std::unique_ptr<BaseFile>> &AllocationTable::getFiles() {
    return files;
}

void AllocationTable::updateBlockAddress(const int fileId, const int oldIndex, const int newIndex) const {
    for (auto & file : files) {
        if (file->getId() == fileId) {
            file->updatePhysicalAddress(oldIndex, newIndex);
        }
    }
}

void AllocationTable::addFile(const BaseFile &fileToAdd) {
    files.push_back(fileToAdd.clone());
}

bool AllocationTable::deleteFile(const int &fileId) {
    for (int i = 0; i < static_cast<int>(files.size()); i++) {
        if (files[i]->getId() == fileId) {
            std::swap(files[i], files.back());
            files.pop_back();
            return true;
        }
    }

    return false;
}

void AllocationTable::runMaintenance(DiskSpaceMap &disk) const {
    for (auto &file : files) {
        if (auto *system = dynamic_cast<SystemFile*>(file.get())) {
            if (system->getSecurityLevel() > 5) {
                system->setUnmovable();
                for (const auto &block : system->getBlockMap()) {
                    disk.getBlockRef(block).setLocked(true);
                }
            }
        }

        if (auto *user = dynamic_cast<UserFile*>(file.get())) {
            if (user->getOwnerName() == "Admin") {
                user->setIsHighPriority();
            }

            else if (user->getOwnerName() == "Guest" && user->getIsHidden() == true) {
                user->setMarkedForDeletion();
            }
        }

        if (auto *temp = dynamic_cast<TempFile*>(file.get())) {
            if (temp->getSourceProcess() == "WebBrowser") {
                temp->setMarkedForDeletion();
            }
        }
    }
}

AllocationTable::AllocationTable(const AllocationTable &other) : formatType(other.formatType) {
    formatType = other.formatType;
    files.reserve(other.files.size());
    for (const auto & file : other.files) {
        files.push_back(file->clone());
    }
}

AllocationTable &AllocationTable::operator=(AllocationTable other) {
    std::swap(this->files, other.files);
    std::swap(this->formatType, other.formatType);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const AllocationTable &allocationTable) {
    os<< "================ Tebelul de alocare (" << allocationTable.formatType <<") "
            << allocationTable.files.size() << "/" << allocationTable.files.capacity() << " fisiere ======";

    for (const auto & file : allocationTable.files) {
        os<< std::endl << *file;
    }

    os<< std::endl << "==================================";
    return os;
}