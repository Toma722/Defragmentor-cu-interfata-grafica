#pragma once
#include <iostream>
#include "File.h"
#include "DiskSpaceMap.h"
#include "AllcoationTable.h"

#ifndef OOP_TEST_H
#define OOP_TEST_H


class Test {
    public:
        static void testEmpty() {
            std::cout<< "TESTE(cc, cm, op...): " << std::endl;
            File emptyFile(800, 0, "gol.txt");
            File copyEmptyFile = emptyFile; //cc
            copyEmptyFile = emptyFile; //operator
            File moveEmptyFile = std::move(emptyFile);//cm
            std::cout << "Testul cm pe file_gol a reusit. Stare file_gol: " << emptyFile << std::endl;
        }

        static void testMove() {
            std::cout << "Test cm: " << std::endl;
            File file99(99, 4, "file99.txt");
            std::cout<<"Inainte de mutare: " << file99 << std::endl;
            File file99Moved = std::move(file99);
            std::cout << "Dupa mutare(file99Moved): " << file99Moved << std::endl;
            std::cout << "Dupa mutare(file99 -> ar trebui sa fie gol): " << file99 << std::endl;
        }

        static void testCopy(DiskSpaceMap &disk, AllocationTable &table) {
            std::cout<< "Test Copy: " << std::endl;

            AllocationTable copyTable = table;
            File fileCopy(201, 2, "copy.txt");
            if (disk.allocateFileBlocks(fileCopy)) {
                table.addFile(fileCopy);
            }

            std::cout<<"Originalul (3 fisiere): " << std::endl;
            std::cout<< copyTable << std::endl;
            std::cout<< "Copia: (4 fisiere)" << std::endl;
            std::cout<< table << std::endl;

            std::cout<< "Test op=: " << std::endl;
            AllocationTable table2("Test");
            table2 = table;

            std::cout<< "Dupa atribuire: " << std::endl;
            std::cout<< table2 << std::endl;

        }
};


#endif //OOP_TEST_H