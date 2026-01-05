#pragma once
#include "BaseFile.h"

#ifndef OOP_USERFILE_H
#define OOP_USERFILE_H


class UserFile : public BaseFile {
    private:
        std::string ownerName;
        bool isHidden;
    public:
        explicit UserFile(int id = 0, int size = 0, const std::string &name = "",
                  checksumAlgorithm alg = checksumAlgorithm::ADLER32, bool isHidden = false, std::string ownerName = "");

        [[nodiscard]] std::unique_ptr<BaseFile> clone() const override;

        void doPrint(std::ostream &os) const override;

        [[nodiscard]] const std::string &getOwnerName() const;

        [[nodiscard]] bool getIsHidden() const;

        void storagePriority(DiskSpaceMap &disk) override;

        void applyMaintenance(DiskSpaceMap &disk) override;
};


#endif //OOP_USERFILE_H