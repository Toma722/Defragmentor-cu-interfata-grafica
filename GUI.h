#pragma once
#include <SFML/Graphics.hpp>
#include "DiskSpaceMap.h"
//de modificat video mode
#ifndef OOP_GUI_H
#define OOP_GUI_H

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define BLOCK_LENGTH 60.f

enum GuiState {
    NORMAL, INPUT_DELETE, INPUT_ADD_ID, INPUT_ADD_SIZE, INPUT_ADD_NAME, INPUT_TRUNCATE_ADD_ID, INPUT_TRUNCATE, INPUT_EXTEND,
    INPUT_EXTEND_ADD_ID
};

class GUI {
    private:
        sf::RenderWindow window;
        sf::RectangleShape blockShape;
        DiskSpaceMap &disk;
        AllocationTable &table;
        sf::Font font;
        sf::Text blockText;
        GuiState currentState;
        std::string inputText;
        sf::RectangleShape inputBackground;
        sf::Text inputPromptText;
        sf::Text inputUserText;
        sf::Text legendText;
        int tempFileId;
        int tempFileSize;

        void handleSubmitExtend();

        void handleSubmitExtendAddId();

        void handleSubmitTruncate();

        void handleSubmitTruncateAddId();

        void handleSubmitAddId();

        void handleSubmitAddSize();

        void handleSubmitAddName();

        void handleDeleteInput();

        void drawDiskMap();

        void drawInputBox();

        void handleTextInput(sf::Uint32 unicode, GuiState state);

    public:

        explicit GUI(DiskSpaceMap &disk, AllocationTable &table);

        void run();

};


#endif //OOP_GUI_H