#pragma once
#include <SFML/Graphics.hpp>
#include "DiskSpaceMap.h"
#include "StatTracker.h"
//de modificat video mode
#ifndef OOP_GUI_H
#define OOP_GUI_H

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BLOCK_LENGTH 60.f

enum GuiState {
    NORMAL, INPUT_DELETE, INPUT_ADD_ID, INPUT_ADD_SIZE, INPUT_ADD_NAME, INPUT_TRUNCATE_ADD_ID, INPUT_TRUNCATE, INPUT_EXTEND,
    INPUT_EXTEND_ADD_ID, DEFRAGMENTING, DEFRAGMENTATION_ANIMATION
};

class GUI {
    private:
        sf::View camera;
        bool isPanning;
        sf::Vector2i lastPanPos;
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
        sf::Text dashBoardText;
        sf::Text toolTipText;
        sf::RectangleShape toolTipBackground;
        sf::RectangleShape miniMapBackground;
        sf::RectangleShape miniMapBlock;
        sf::RectangleShape fragmentationBarBackground;
        sf::RectangleShape fragmentationBarFill;
        sf::RectangleShape animatedDefragmentationBlock;
        sf::Vector2f animationDefragmentationStartPos;
        sf::Vector2f animationDefragmentationEndPos;
        float animationDefragmentationProgress;
        float animationDefragmentationSpeed;
        sf::Clock globalClock, animationDefragmentation;
        int tempFileId;
        int tempFileSize;
        int defragEmptySlot;
        int defragBlockToScan;
        int blocksPerRow;
        int hoveredBlockIndex;
        StatTracker<int> fileSizeStats;
        StatTracker<double> fragmentationStats;

        void drawMiniMap(const float &pulse);

        void runDefragmentStep();

        void runDefragmentAnimation();

        sf::Vector2f getPixelPosition(int index) const;

        void handleSubmitExtend();

        void handleSubmitExtendAddId();

        void handleSubmitTruncate();

        void handleSubmitTruncateAddId();

        void handleSubmitAddId();

        void handleSubmitAddSize();

        void handleSubmitAddName();

        void handleDeleteInput();

        void drawDiskMap(const float &pulse);

        void drawInputBox();

        void handleTextInput(sf::Uint32 unicode, GuiState state);

        void drawToolTip();

        void updateAndDrawDashBoard();

        void drawFragmentationBar();

    public:

        explicit GUI(DiskSpaceMap &disk, AllocationTable &table);

        void run();

};

#endif //OOP_GUI_H