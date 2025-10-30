#pragma once
#include <iostream>
#include "Block.h"

#ifndef OOP_FILE_H
#define OOP_FILE_H


class File {
    private:
        Block *fileBlocks;
        int numBlocks, id;
        std::string name;

    public:
        explicit File(int id = 0, int size = 0, const std::string &name = "") {
            this->id = id;
            this->name = name;
            this->numBlocks = size;
            this->fileBlocks = new Block[size];

            for (int i = 0; i < size; i++) {
                this->fileBlocks[i].setData(i + 1, true, this->id);
            }
        }

        File(const File &other) { //cc
            this->id = other.id;
            this->name = other.name;
            this->fileBlocks = new Block[other.numBlocks];
            for (int i = 0; i < other.numBlocks; i++) {
                this->fileBlocks[i] = other.fileBlocks[i];
            }
            this->numBlocks = other.numBlocks;
        }

        File(File &&other) noexcept { //cm
            this->fileBlocks = other.fileBlocks;
            this->numBlocks = other.numBlocks;
            this->id = other.id;
            this->name = std::move(other.name);
            other.fileBlocks = nullptr;
            other.numBlocks = 0;
            other.id = 0;
            other.name = "";
        }

        File &operator=(const File &other) {
            if (this == &other) {
                return *this;
            }

            auto *tempBlocks = new Block[other.numBlocks];

            for (int i = 0; i < other.numBlocks; i++) {
                tempBlocks[i] = other.fileBlocks[i];
            }

            delete[] this->fileBlocks;
            this->fileBlocks = tempBlocks;
            this->numBlocks = other.numBlocks;
            this->id = other.id;
            this->name = other.name;

            return *this;
        }

        File &operator=(File &&other) noexcept {
            if (this == &other) {
                return *this;
            }
            delete[] this->fileBlocks;
            this->fileBlocks = other.fileBlocks;
            this->numBlocks = other.numBlocks;
            this->id = other.id;
            this->name = std::move(other.name);
            other.fileBlocks = nullptr;
            other.numBlocks = 0;
            other.id = 0;
            other.name = "";

            return *this;
        }


        [[nodiscard]] int getId() const {
            return this->id;
        }

        ~File() {
            delete[] this->fileBlocks;
        }

        [[nodiscard]] int getNumBlocks() const {
            return this->numBlocks;
        }

        friend std::ostream &operator<<(std::ostream &os, const File &file) {
            os<<"File "<<file.id<<": "<<file.name << " -> ";
            for (int i = 0; i < file.numBlocks; i++) {
                os<< file.fileBlocks[i] << " ";
            }
            return os;
        }
};

#endif //OOP_FILE_H