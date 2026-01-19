#ifndef OOP_FILEFACTORY_H
#define OOP_FILEFACTORY_H
#include "BaseFile.h"

enum class FileType {
    User, System, Temp
};

class FileFactory {
public:
    static BaseFile *createFile(FileType type, int id, int size, const std::string &name);
};


#endif //OOP_FILEFACTORY_H