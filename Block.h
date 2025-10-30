#pragma once
#include  <iostream>

#ifndef OOP_BLOCK_H
#define OOP_BLOCK_H


class Block {
private:
    int id, size;
    bool occupied;
    unsigned long content;

    friend class File;
    friend class DiskSpaceMap;
public:
    explicit Block(int id = 0, int size = 0, bool occupied = false, unsigned long content = 0) {//asta e ala normalu
        this->id = id;
        this->size = size;
        this->occupied = occupied;
        this->content = content;
    }

    Block(const Block &other) { //asta e de copiere
        this->id = other.id;
        this->size = other.size;
        this->occupied = other.occupied;
        this->content = other.content;
    }

    Block &operator=(const Block &other) {//nebunia cu operatorul de atribuire
        if (this == &other) {
            return *this;
        }

        this->id = other.id;
        this->size = other.size;
        this->occupied = other.occupied;
        this->content = other.content;
        return *this;
    }

    ~Block() = default;

    friend std::ostream &operator<<(std::ostream &os, const Block &block) {
        if (block.occupied == true) {
            os<<"Block "<<block.id<< " : " << block.content << "; ";
        }
        else {
            os<<"Block not occupied";
        }

        return os;
    }

    [[nodiscard]] bool getOccupied() const {
        return this->occupied;
    }

protected:
    void setData(int blockId, bool isOccupied, unsigned long fileID) {
        this->id = blockId;
        this->occupied = isOccupied;
        this->content = fileID;
    }

};


#endif //OOP_BLOCK_H