#include "UserFile.h"

#include <utility>

UserFile::UserFile(int id, int size, const std::string &name, checksumAlgorithm alg,
    bool isHidden, std::string ownerName) : BaseFile(id, size, name, alg), ownerName(std::move(ownerName)), isHidden(isHidden){}

void UserFile::doPrint(std::ostream &os) const {
    os<< "Owner name: " << ownerName;
    if (isHidden) {
        os << " (hidden)";
    }
    else {
        os << " (unhidden)";
    }
}

void UserFile::storagePriority(DiskSpaceMap &disk) {
    if (blockMap.size() < 2) {
        return;
    }

    unsigned long fragmentations = 0;
    for (unsigned long i = 1; i < blockMap.size(); i++) {
        if (blockMap[i] != blockMap[i - 1] + 1) {
            fragmentations++;
        }
    }

    const double fileFragRatio = static_cast<double>(fragmentations) / static_cast<double>(blockMap.size());
    double globalDiskFrag = disk.getFragmentationPercentage();

    if (globalDiskFrag > 0.5) {
        if (fileFragRatio > 0.10) {
            isHighPriority = true;
        } else {
            isHighPriority = false;
        }
    }
    else {
        if (fileFragRatio > 0.25) {
            isHighPriority = true;
        } else {
            isHighPriority = false;
        }
    }
}

void UserFile::applyMaintenance(DiskSpaceMap &disk) {
    if (disk.getFragmentationPercentage() > 0.5 && this->getOwnerName() == "Guest") {
        markedForDeletion = true;
    }

    if (this->ownerName == "Admin") {
        isHighPriority = true;
    }

    else if (this->ownerName == "Guest" && this->isHidden == true) {
        markedForDeletion = true;
    }
}

const std::string &UserFile::getOwnerName() const {
    return ownerName;
}

std::unique_ptr<BaseFile> UserFile::clone() const {
    return std::make_unique<UserFile>(*this);
};

