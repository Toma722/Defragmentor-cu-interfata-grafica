#ifndef OOP_SYSTEMFILE_H
#define OOP_SYSTEMFILE_H
#include "BaseFile.h"


class SystemFile : public BaseFile{
    private:
        int securityLevel;
    public:
        explicit SystemFile(int id = 0, int size = 0, const std::string &name = "",
                  checksumAlgorithm alg = checksumAlgorithm::ADLER32, int securityLevel = 5);

        [[nodiscard]] std::unique_ptr<BaseFile> clone() const override;

        [[nodiscard]] int getSecurityLevel() const;

        void doPrint(std::ostream &os) const override;

        void storagePriority(DiskSpaceMap &disk) override;
};


#endif //OOP_SYSTEMFILE_H