#include "SystemFile.h"

SystemFile::SystemFile(int id, int size, const std::string &name, checksumAlgorithm alg,
    int securityLevel) : BaseFile(id, size, name, alg), securityLevel(securityLevel){}

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

int SystemFile::getSecurityLevel() const {
    return securityLevel;
}

std::unique_ptr<BaseFile> SystemFile::clone() const {
    return std::make_unique<SystemFile>(*this);
};