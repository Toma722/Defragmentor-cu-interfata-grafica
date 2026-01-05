#include "SystemFile.h"

SystemFile::SystemFile(int id, int size, const std::string &name, checksumAlgorithm alg,
    int securityLevel) : BaseFile(id, size, name, alg), securityLevel(securityLevel) {
    if (this->securityLevel > 5) {
        unmovable = true;
    }
}

void SystemFile::doPrint(std::ostream &os) const {
    os<< "Security Level: " << securityLevel;
}

void SystemFile::storagePriority(DiskSpaceMap &disk) {
    if (securityLevel > 5) {
        for (int physcialIndex : blockMap) {
            disk.getBlockRef(physcialIndex).setLocked(true);
        }
    }
    else {
        for (int physcialIndex : blockMap) {
            disk.getBlockRef(physcialIndex).setLocked(false);
        }
    }
}

void SystemFile::applyMaintenance(DiskSpaceMap &disk) {
    if (securityLevel > 5) {
        unmovable = true;
        for (const auto &block : blockMap) {
            disk.getBlockRef(block).setLocked(true);
        }
    }
}

std::unique_ptr<BaseFile> SystemFile::clone() const {
    return std::make_unique<SystemFile>(*this);
};