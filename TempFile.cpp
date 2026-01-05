#include "TempFile.h"

TempFile::TempFile(int id, int size, const std::string &name, checksumAlgorithm alg,
    std::string sourceProcess) : BaseFile(id, size, name, alg), sourceProcess(std::move(sourceProcess)){}

void TempFile::doPrint(std::ostream &os) const {
    os<< "Source Process: " << sourceProcess;
}

void TempFile::storagePriority(DiskSpaceMap &disk) {
    if (static_cast<double>(disk.getTotalFreeBlocks()) / static_cast<double>(disk.getNumBlocks()) <= 0.1) {
        markedForDeletion = true;
    }
    else {
        markedForDeletion = false;
    }
}

void TempFile::applyMaintenance(DiskSpaceMap &disk) {
    double freePercent = static_cast<double>(disk.getTotalFreeBlocks()) / disk.getNumBlocks();

    if (freePercent < 0.1 && this->sourceProcess == "WebBrowser") {
        markedForDeletion = true;
    }
}

std::unique_ptr<BaseFile> TempFile::clone() const {
    return std::make_unique<TempFile>(*this);
};