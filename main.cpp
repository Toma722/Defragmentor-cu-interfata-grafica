#include <iostream>
#include "File.h"
#include "DiskSpaceMap.h"
#include "AllcoationTable.h"

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
    std::cout << file1 << std::endl << file2 << std::endl << file3 << std::endl << file4 << std::endl;

    std::cout<< "Incercare alocare file1" << std::endl;
    if (disk.allocateFileBlocks(file1)) {
        table.addFile(file1);
        std::cout << "  -> SUCCES" << std::endl;
    }

    else {
        std::cout << "  -> EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Incercare alocare file2" << std::endl;
    if (disk.allocateFileBlocks(file2)) {
        table.addFile(file2);
        std::cout << "SUCCES" << std::endl;
    }

    else {
        std::cout << "EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Incercare alocare file3" << std::endl;
    if (disk.allocateFileBlocks(file3)) {
        table.addFile(file3);
        std::cout << "SUCCES" << std::endl;
    }

    else {
        std::cout << "EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Incercare alocare file4" << std::endl;
    if (disk.allocateFileBlocks(file4)) {
        table.addFile(file4);
        std::cout << "  -> SUCCES.\n";
    }

    else {
        std::cout << "EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Afisare table si disk: " << std::endl;
    std::cout<< table << std::endl;
    std::cout<< disk << std::endl;

    // //teste(COMENTATE DE OARECE RIDICA WARNING-URI)
    // std::cout<< "TESTE(cc, cm, op...): " << std::endl;
    // File emptyFile(800, 0, "gol.txt");
    // File copyEmptyFile = emptyFile; //cc
    // copyEmptyFile = emptyFile; //operator
    // File moveEmptyFile = std::move(emptyFile);//cm
    //
    // std::cout << "Testul cm pe file_gol a reusit. Stare file_gol: " << emptyFile << std::endl;
    //
    // std::cout << "Test cm: " << std::endl;
    // File file99(99, 4, "file99.txt");
    // std::cout<<"Inainte de mutare: " << file99 << std::endl;
    // File file99Moved = std::move(file99);
    // std::cout << "Dupa mutare(file99Moved): " << file99Moved << std::endl;
    // std::cout << "Dupa mutare(file99 -> ar trebui sa fie gol): " << file99 << std::endl;

}
