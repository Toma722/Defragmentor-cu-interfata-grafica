#include "Block.h"

Block::Block(int id, int size, bool occupied, unsigned long content, bool isDamaged) {//constructor
    this->id = id;
    this->size = size;
    this->occupied = occupied;
    this->content = content;
    this->isDamaged = isDamaged;
}

Block &Block::operator=(const Block &other) {//op=
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

std::ostream &operator<<(std::ostream &os, const Block &block) {//op<<
    if (block.occupied == true) {
        os<<"Block "<<block.id<< " : " << block.content << "; ";
    }
    else {
        os<<"Block not occupied";
    }

    return os;
}

[[nodiscard]] unsigned long Block::getContent() const {
    return this->content;
}

[[nodiscard]] bool Block::getOccupied() const {
    return this->occupied;
}

[[nodiscard]] int Block::getSize() const {
    return this->size;
}

[[nodiscard]] bool Block::isBad() const {
    return this->isDamaged;
}

void Block::clear() {
    this->content = 0;
    occupied = false;
}

void Block::setData(const int blockId, const bool isOccupied, const unsigned long fileId, const int bSize) {
    this->id = blockId;
    this->occupied = isOccupied;
    this->content = fileId;
    this->size = bSize;
}

void Block::markAsBad() {
    isDamaged = true;
}

