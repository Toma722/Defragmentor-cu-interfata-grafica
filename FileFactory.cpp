#include "FileFactory.h"

#include "SystemFile.h"
#include "TempFile.h"
#include "UserFile.h"

BaseFile *FileFactory::createFile(FileType type, int id, int size, const std::string &name) {
    switch (type) {
        case FileType::User:
            return new UserFile(id, size, name, ADLER32, false, "Guest");

        case FileType::System:
            return new SystemFile(id, size, name, ADLER32, 9);

        case FileType::Temp:
            return new TempFile(id, size, name, ADLER32, "WebBrowser");

        default:
            return nullptr;
    }
}
