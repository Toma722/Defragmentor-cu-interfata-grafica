#pragma once
#include  <iostream>

#ifndef OOP_BLOCK_H
#define OOP_BLOCK_H


class Block {
private:
    int id, size;
    bool occupied;
    unsigned long content;
    bool isDamaged;

public:
    explicit Block(int id = 0, int size = 0, bool occupied = false, unsigned long content = 0, bool isDamaged = false) {//constructor
        this->id = id;
        this->size = size;
        this->occupied = occupied;
        this->content = content;
        this->isDamaged = isDamaged;
    }

    Block(const Block &other) { //cc
        this->id = other.id;
        this->size = other.size;
        this->occupied = other.occupied;
        this->content = other.content;
        this->isDamaged = other.isDamaged;
    }

    Block &operator=(const Block &other) {//op=
        if (this == &other) {
            return *this;
        }

        this->id = other.id;
        this->size = other.size;
        this->occupied = other.occupied;
        this->content = other.content;
        this->isDamaged = other.isDamaged;
        return *this;
    }

    ~Block() = default;//destr

    friend std::ostream &operator<<(std::ostream &os, const Block &block) {//op<<
        if (block.occupied == true) {
            os<<"Block "<<block.id<< " : " << block.content << "; ";
        }
        else {
            os<<"Block not occupied";
        }

        return os;
    }

    [[nodiscard]] unsigned long getContent() const {
        return this->content;
    }

    [[nodiscard]] bool getOccupied() const {
        return this->occupied;
    }

    [[nodiscard]] int getSize() const {
        return this->size;
    }

    [[nodiscard]] bool isBad() const {
        return this->isDamaged;
    }

    void markAsBad() {
        isDamaged = true;
        occupied = true;
        content = 0;
    }

    void clear() {
        this->content = 0;
        occupied = false;
    }

    void setData(int blockId, bool isOccupied, unsigned long fileId, int bSize) {
        this->id = blockId;
        this->occupied = isOccupied;
        this->content = fileId;
        this->size = bSize;
    }

};


#endif //OOP_BLOCK_H