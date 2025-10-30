#include <iostream>
#include "File.h"
#include "DiskSpaceMap.h"
#include "AllcoationTable.h"
#include "Test.h"

//DEFRAGMENTORUL per se nu este inca implementat
//de pus liste de initializare
//momentan simulez content si id uri
//de implementat first-fit/best-fit/worst-fit
//de implementat format type urile
//functiile netriviale sunt comentate ca atare

int main() {
    DiskSpaceMap disk(32);
    AllocationTable table("FAT32", 2);

    File file1(101, 5, "doc.txt");
    File file2(102, 8, "driver.dll");
    File file3(103, 3, "text.txt");
    File file4(104, 20, "video.mkv");//nu o sa incapa asta

    const File *filesToAllocate[4] = {&file1, &file2, &file3, &file4};

    for (const File *i : filesToAllocate) {
        std::cout << *i << std::endl;
    }

    for (const File *i : filesToAllocate) {
        std::cout << "Incercare alocare " << i->getName() << std::endl;
        if (disk.allocateFileBlocks(*i)) {
            table.addFile(*i);
            std::cout<< "SUCCES" << std::endl;
        }
        else {
            std::cout<< "EROARE: Spatiu insuficient" << std::endl;
        }
    }

    std::cout<< "Afisare table si disk: " << std::endl;
    std::cout<< table << std::endl;
    std::cout<< disk << std::endl;

    Test::testMove();
    Test::testCopy(disk, table);

}
