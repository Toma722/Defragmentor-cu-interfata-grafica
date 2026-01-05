#ifndef OOP_TEMPFILE_H
#define OOP_TEMPFILE_H
#include "BaseFile.h"


class TempFile : public BaseFile {
    private:
        std::string sourceProcess;
    public:
        explicit TempFile(int id = 0, int size = 0, const std::string &name = "",
                  checksumAlgorithm alg = checksumAlgorithm::ADLER32, std::string sourceProcess = "");

        [[nodiscard]] std::unique_ptr<BaseFile> clone() const override;

        void doPrint(std::ostream &os) const override;

        void applyMaintenance(DiskSpaceMap &disk) override;

        void storagePriority(DiskSpaceMap &disk) override;
};


#endif //OOP_TEMPFILE_H