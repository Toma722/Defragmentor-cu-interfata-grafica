#pragma once
#include  <iostream>

#ifndef OOP_BLOCK_H
#define OOP_BLOCK_H


class Block {
private:
    int id, size;
    bool occupied;
    unsigned long content;
    bool isDamaged, locked;

public:
    explicit Block(int id = 0, int size = 0, bool occupied = false,
        unsigned long content = 0, bool isDamaged = false, bool locked = false);

    Block(const Block &other) {
        this->id = other.id;
        this->size = other.size;
        this->occupied = other.occupied;
        this->content = other.content;
        this->isDamaged = other.isDamaged;
        this->locked = other.locked;
    }

    Block &operator=(const Block &other);

    ~Block() = default;

    friend std::ostream &operator<<(std::ostream &os, const Block &block);

    [[nodiscard]] unsigned long getContent() const;

    [[nodiscard]] bool getOccupied() const;

    [[nodiscard]] int getSize() const;

    [[nodiscard]] bool isBad() const;

    void markAsBad();

    void clear();

    void setData(int blockId, bool isOccupied, unsigned long fileId, int bSize);

    [[nodiscard]] bool getLocked() const;

    void setLocked(bool lock);
};


#endif //OOP_BLOCK_H