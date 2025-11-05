#pragma once
#include <SFML/Graphics.hpp>
#include "DiskSpaceMap.h"
//de modificat video mode
#ifndef OOP_GUI_H
#define OOP_GUI_H

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BLOCK_LENGTH 20.f

class GUI {
    private:
        sf::RenderWindow window;
        sf::RectangleShape blockShape;
        DiskSpaceMap &m_disk;

        void drawDiskMap() { //functie netriviala
            int blocksPerRow = SCREEN_WIDTH / static_cast<int>(BLOCK_LENGTH);

            for (int i = 0; i < m_disk.getNumBlocks(); i++) {
                const Block &block = m_disk.getBlock(i);

                if (block.getOccupied() == true) {
                    blockShape.setFillColor(sf::Color::Red);
                }
                else {
                    blockShape.setFillColor(sf::Color::Green);
                }

                int col = i % blocksPerRow;
                int row = i / blocksPerRow;
                blockShape.setPosition(static_cast<float>(col) * BLOCK_LENGTH, static_cast<float>(row) * BLOCK_LENGTH);
                window.draw(blockShape);
            }
        }
    public:

        explicit GUI(DiskSpaceMap &disk) : m_disk(disk) {
            window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Defragmentor v0.2");
            blockShape.setSize(sf::Vector2f(BLOCK_LENGTH, BLOCK_LENGTH));
            blockShape.setOutlineColor(sf::Color::Black);
            blockShape.setOutlineThickness(1);
        }

        void run() {
            while (window.isOpen()) {
                sf::Event event{};
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }

                window.clear(sf::Color(128, 128, 128));//gri
                drawDiskMap();
                window.display();
            }
        }


};


#endif //OOP_GUI_H