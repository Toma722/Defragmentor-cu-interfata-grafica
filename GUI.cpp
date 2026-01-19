#include "GUI.h"
#include "BaseFile.h"
#include "AllocationTable.h"
#include <cassert>
#include <cmath>
#include "FileFactory.h"
#include "Exceptions.h"
#include "SystemFile.h"
#include "Utils.h"

void GUI::handleSubmitExtend() {
    if (inputText.empty()) {
        return;
    }

    try {
        tempFileSize = static_cast<int>(std::stoul(inputText));
    } catch (const std::invalid_argument &) {
        std::cout << "Numarul de Blocuri trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }
    BaseFile *fileToExtend = table.findFileById(tempFileId);

    if (fileToExtend == nullptr) {
        inputPromptText.setString("Fisierul nu exista (Esc = RETURN)");
        return;
    }

    try {
        fileToExtend->extend(tempFileSize, disk);
        currentState = NORMAL;
        inputText = "";
    } catch (const DiskFullException &e) {
        inputPromptText.setString(std::string(e.what()) + " (ESC = RETURN)");
        inputBackground.setOutlineColor(sf::Color::Red);
    }
}

void GUI::handleSubmitExtendAddId() {
    if (inputText.empty()) {
        return;
    }

    try {
        tempFileId = static_cast<int>(std::stoul(inputText));
    } catch (const std::invalid_argument &) {
        std::cout << "Id-ul trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }
    if (table.findFileById(tempFileId) == nullptr) {
        inputPromptText.setString("Fisierul nu exista (Esc = RETURN)");
        return;
    }
    currentState = INPUT_EXTEND;
    inputPromptText.setString("Introduceti cu cate blocuri sa fie extins fisierul: ");
    inputText = "";
}

void GUI::handleSubmitTruncate() {
    if (inputText.empty()) {
        return;
    }

    try {
        tempFileSize = static_cast<int>(std::stoul(inputText));
    } catch (const std::invalid_argument &) {
        std::cout << "Numarul de Blocuri trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }
    BaseFile *fileToTruncate = table.findFileById(tempFileId);
    if (fileToTruncate == nullptr) {
        inputPromptText.setString("Fisierul Nu Exista! (ESC = RETURN)");
        return;
    }
    if (tempFileSize < 1) {
        inputPromptText.setString("Nu se poate Trunchia la mai putin de 1 bloc! (ESC = RETURN)");
        return;
    } else if (tempFileSize >= fileToTruncate->getNumBlocks()) {
        inputPromptText.setString("Nu se poate Trunchia la sau peste dimensiunea Fisierului! (ESC = Return)");
        return;
    }

    fileToTruncate->truncate(tempFileSize, disk);
    currentState = NORMAL;
    inputText = "";
}

void GUI::handleSubmitTruncateAddId() {
    if (inputText.empty()) {
        return;
    }

    try {
        tempFileId = static_cast<int>(std::stoul(inputText));
    } catch (const std::invalid_argument &) {
        std::cout << "Id-ul trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }

    if (table.findFileById(tempFileId) == nullptr) {
        inputPromptText.setString("Fisierul Nu Exista! (ESC = RETURN)");
        return;
    }

    currentState = INPUT_TRUNCATE;
    inputPromptText.setString("Introduceti Noua Dimensiune in Blocuri: ");
    inputText = "";
}

void GUI::handleSubmitAddId() {
    if (inputText.empty()) {
        return;
    }

    try {
        tempFileId = static_cast<int>(std::stoul(inputText));
    } catch (const std::invalid_argument &) {
        std::cout << "ID Invalid! Trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }

    if (table.findFileById(tempFileId) != nullptr) {
        inputPromptText.setString("Fisierul este deja existent! (ESC = RETURN)");
        return;
    }

    currentState = INPUT_ADD_SIZE;
    inputPromptText.setString("Introduceti Dimensiunea(In blocuri): ");
    inputText = "";
}

void GUI::handleSubmitAddSize() {
    if (inputText.empty()) {
        return;
    }

    try {
        tempFileSize = static_cast<int>(std::stoul(inputText));
    } catch (const std::invalid_argument &) {
        std::cout << "Dimensiune Invalida! Trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }

    if (!validateRange<int>(tempFileSize, 1, disk.getNumBlocks())) {
        inputPromptText.setString("Dimensiune invalida (ESC = RETURN)");
        return;
    }

    if (disk.findSpace(tempFileSize) == -1) {
        inputPromptText.setString("Fisierul nu incape! (Defragmentare sau marire disc! ESC = RETURN)");
        return;
    }

    fileSizeStats.addValue(tempFileSize);

    currentState = INPUT_ADD_NAME;
    inputPromptText.setString("Introduceti Numele Fisierului: ");
    inputText = "";
}

void GUI::handleSubmitAddName() {
    try {
        auto type = FileType::User;

        if (inputText.find(".sys") != std::string::npos) {
            type = FileType::System;
        } else if (inputText.find(".tmp") != std::string::npos) {
            type = FileType::Temp;
        }

        if (BaseFile *newFile = FileFactory::createFile(type, tempFileId, tempFileSize, inputText);
            newFile != nullptr) {
            const std::vector<int> map = disk.allocateFile(*newFile);

            if (map.empty()) {
                delete newFile;
                throw DiskFullException();
            }

            newFile->setBlockMap(map);
            table.addFile(*newFile);
        }

        currentState = NORMAL;
        inputText = "";
    } catch (const DiskFullException &e) {
        inputPromptText.setString(std::string(e.what()) + " (ESC = RETURN)");
        inputBackground.setOutlineColor(sf::Color::Red);
    }
}

void GUI::handleDeleteInput() {
    if (inputText.empty()) {
        return;
    }

    unsigned long fileId = 0;

    try {
        fileId = std::stoul(inputText);
    } catch (const std::invalid_argument &) {
        std::cout << "ID Invalid! Trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }

    const BaseFile *fileToDelete = table.findFileById(static_cast<int>(fileId));
    if (fileToDelete == nullptr) {
        inputPromptText.setString("Fisierul Nu Exista! (ESC = RETURN)");
        return;
    }

    const std::vector<int> mapToDelete = fileToDelete->getBlockMap();

    try {
        if (table.deleteFile(static_cast<int>(fileId))) {
            disk.freeBlocks(mapToDelete);

            std::cout << "Fisierul a fost sters!" << std::endl;

            inputText = "";
            currentState = NORMAL;
        }
    } catch (const SecurityException &e) {
        inputPromptText.setString(std::string(e.what()) + " (ESC = RETURN)");
        inputBackground.setOutlineColor(sf::Color::Red);
    }
}

void GUI::drawDiskMap(const float &pulse) {
    for (int i = 0; i < disk.getNumBlocks(); i++) {
        blockShape.setOutlineColor(sf::Color::Black);
        blockShape.setOutlineThickness(1.f);
        const Block &block = disk.getBlock(i);

        if (currentState == DEFRAGMENTATION_ANIMATION &&
            (i == defragBlockToScan || i == defragEmptySlot)) {
            continue;
        }
        if (i == hoveredBlockIndex) {
            continue;
        }

        if (block.isBad() == true) {
            blockShape.setFillColor(sf::Color::Black);
        } else if (block.getOccupied() == true) {
            const unsigned long fileId = block.getContent();
            int colorIndex = static_cast<int>(fileId) % 5;
            switch (colorIndex) {
                case 0: {
                    blockShape.setFillColor(sf::Color(255, 128, 0));
                    break;
                }
                case 1: {
                    blockShape.setFillColor(sf::Color(0, 255, 255));
                    break;
                }
                case 2: {
                    blockShape.setFillColor(sf::Color(255, 0, 128));
                    break;
                }
                case 3: {
                    blockShape.setFillColor(sf::Color(255, 200, 0));
                    break;
                }
                case 4: {
                    blockShape.setFillColor(sf::Color(60, 150, 255));
                    break;
                }
                default: {
                    blockShape.setFillColor(sf::Color(120, 160, 255, 100));
                    break;
                }
            }
        } else {
            blockShape.setFillColor(sf::Color(120, 160, 255, 100));
        }

        if (currentState == DEFRAGMENTING) {
            if (i == defragBlockToScan) {
                blockShape.setFillColor(sf::Color(static_cast<sf::Uint8>(pulse * 255),
                                                  static_cast<sf::Uint8>(pulse * 255), 255));
            }
        }


        const int col = i % blocksPerRow;
        const int row = i / blocksPerRow;
        const float pozX = static_cast<float>(col) * BLOCK_LENGTH;
        const float pozY = static_cast<float>(row) * BLOCK_LENGTH;
        blockShape.setPosition(pozX, pozY);
        window.draw(blockShape);
        if (block.getOccupied() == true && block.isBad() == false) {
            std::string id = std::to_string(block.getContent());
            blockText.setString(id);
            blockText.setPosition(pozX + 2.f, pozY + 2.f);
            window.draw(blockText);
        }
    }

    if (hoveredBlockIndex != -1) {
        blockShape.setOutlineColor(sf::Color::Black);
        blockShape.setOutlineThickness(1.f);
        const Block &block = disk.getBlock(hoveredBlockIndex);

        if (block.isBad() == true) {
            blockShape.setFillColor(sf::Color::Black);
        } else if (block.getOccupied() == true) {
            const unsigned long fileId = block.getContent();
            int colorIndex = static_cast<int>(fileId) % 5;
            switch (colorIndex) {
                case 0: {
                    blockShape.setFillColor(sf::Color(255, 128, 0));
                    break;
                }
                case 1: {
                    blockShape.setFillColor(sf::Color(0, 255, 255));
                    break;
                }
                case 2: {
                    blockShape.setFillColor(sf::Color(255, 0, 128));
                    break;
                }
                case 3: {
                    blockShape.setFillColor(sf::Color(255, 200, 0));
                    break;
                }
                case 4: {
                    blockShape.setFillColor(sf::Color(60, 150, 255));
                    break;
                }
                default: {
                    blockShape.setFillColor(sf::Color(120, 160, 255, 100));
                    break;
                }
            }
        } else {
            blockShape.setFillColor(sf::Color(120, 160, 255, 100));
        }

        if (currentState == DEFRAGMENTING) {
            if (hoveredBlockIndex == defragBlockToScan) {
                blockShape.setFillColor(sf::Color(static_cast<sf::Uint8>(pulse * 255),
                                                  static_cast<sf::Uint8>(pulse * 255), 255));
            }
        }
        blockShape.setOutlineColor(sf::Color::White);
        blockShape.setOutlineThickness(1.f + pulse * 3.f);


        const int col = hoveredBlockIndex % blocksPerRow;
        const int row = hoveredBlockIndex / blocksPerRow;
        const float pozX = static_cast<float>(col) * BLOCK_LENGTH;
        const float pozY = static_cast<float>(row) * BLOCK_LENGTH;
        blockShape.setPosition(pozX, pozY);
        window.draw(blockShape);
        if (block.getOccupied() == true && block.isBad() == false) {
            const std::string id = std::to_string(block.getContent());
            blockText.setString(id);
            blockText.setPosition(pozX + 2.f, pozY + 2.f);
            window.draw(blockText);
        }
    }
}

void GUI::drawInputBox() {
    inputUserText.setString(inputText + "_");

    window.draw(inputBackground);
    window.draw(inputPromptText);
    window.draw(inputUserText);
}

void GUI::handleTextInput(const sf::Uint32 unicode, const GuiState state) {
    if (state == INPUT_DELETE || state == INPUT_ADD_ID ||
        state == INPUT_ADD_SIZE || state == INPUT_TRUNCATE ||
        state == INPUT_TRUNCATE_ADD_ID || state == INPUT_EXTEND ||
        state == INPUT_EXTEND_ADD_ID) {
        if (unicode >= 48 && unicode <= 57) {
            inputBackground.setOutlineColor(sf::Color::White);
            if (inputText.length() < 10) {
                inputText += static_cast<char>(unicode);
            }
        } else if (!inputText.empty()) {
            inputBackground.setOutlineColor(sf::Color::Red);
        }
    } else if (state == INPUT_ADD_NAME) {
        if (unicode >= 32 && unicode <= 127) {
            if (inputText.length() < 10) {
                inputText += static_cast<char>(unicode);
            }
        }
    }
}

sf::Vector2f GUI::getPixelPosition(const int index) const {
    const int col = index % blocksPerRow;
    const int row = index / blocksPerRow;
    return {
        static_cast<float>(col) * BLOCK_LENGTH,
        static_cast<float>(row) * BLOCK_LENGTH
    };
}

void GUI::runDefragmentStep() {
    if (defragBlockToScan >= disk.getNumBlocks()) {
        currentState = NORMAL;
        std::cout << "Defragmentare Terminata" << std::endl;
        return;
    }

    while (defragEmptySlot < disk.getNumBlocks() && disk.getBlockRef(defragEmptySlot).isBad() == true) {
        defragEmptySlot++;
    }

    if (defragBlockToScan < defragEmptySlot) {
        defragBlockToScan = defragEmptySlot;
    }

    if (disk.getBlockRef(defragBlockToScan).isBad() == true) {
        defragBlockToScan++;
        return;
    }

    if (const Block &blockToScan = disk.getBlockRef(defragBlockToScan); blockToScan.getOccupied() == true) {
        if (defragBlockToScan != defragEmptySlot) {
            const BaseFile *file = table.findFileById(static_cast<int>(blockToScan.getContent()));
            if (file && file->isUnmovable()) {
                defragBlockToScan++;
                return;
            }
            currentState = DEFRAGMENTATION_ANIMATION;

            animationDefragmentationStartPos = getPixelPosition(defragBlockToScan);
            animationDefragmentationEndPos = getPixelPosition(defragEmptySlot);

            const Block &block = disk.getBlock(defragBlockToScan);

            const unsigned long fileId = block.getContent();
            int colorIndex = static_cast<int>(fileId) % 5;
            switch (colorIndex) {
                case 0: animatedDefragmentationBlock.setFillColor(sf::Color(255, 128, 0));
                    break;
                case 1: animatedDefragmentationBlock.setFillColor(sf::Color(0, 255, 255));
                    break;
                case 2: animatedDefragmentationBlock.setFillColor(sf::Color(255, 0, 128));
                    break;
                case 3: animatedDefragmentationBlock.setFillColor(sf::Color(255, 200, 0));
                    break;
                case 4: animatedDefragmentationBlock.setFillColor(sf::Color(60, 150, 255));
                    break;
                default: animatedDefragmentationBlock.setFillColor(sf::Color(120, 160, 255, 100));
                    break;
            }

            animatedDefragmentationBlock.setPosition(animationDefragmentationStartPos);

            animationDefragmentationProgress = 0.f;

            return;
        }
        defragEmptySlot++;
    }
    defragBlockToScan++;
}

void GUI::runDefragmentAnimation() {
    const float dt = animationDefragmentation.restart().asSeconds();
    animationDefragmentationProgress += dt * animationDefragmentationSpeed;

    if (animationDefragmentationProgress >= 1.f) {
        animationDefragmentationProgress = 1.f;

        Block &blockToScan = disk.getBlockRef(defragBlockToScan);
        Block &emptyBlock = disk.getBlockRef(defragEmptySlot);

        const unsigned long fileId = blockToScan.getContent();
        const int size = blockToScan.getSize();

        emptyBlock.setData(defragEmptySlot, true, fileId, size);
        table.updateBlockAddress(static_cast<int>(fileId), defragBlockToScan, defragEmptySlot);
        blockToScan.clear();

        defragEmptySlot++;
        defragBlockToScan++;

        currentState = DEFRAGMENTING;
        return;
    }

    const float currentX = animationDefragmentationStartPos.x + (
                               animationDefragmentationEndPos.x - animationDefragmentationStartPos.x) *
                           animationDefragmentationProgress;
    const float currentY = animationDefragmentationStartPos.y + (
                               animationDefragmentationEndPos.y - animationDefragmentationStartPos.y) *
                           animationDefragmentationProgress;

    animatedDefragmentationBlock.setPosition(currentX, currentY);
};

void GUI::drawToolTip() {
    if (hoveredBlockIndex == -1) {
        return;
    }

    const Block &block = disk.getBlock(hoveredBlockIndex);
    std::string infoString = "Index fizic: " + std::to_string(hoveredBlockIndex) + "\n";
    if (block.isBad() == true) {
        infoString += "Status: STRICAT";
    } else if (block.getOccupied() == true) {
        const std::string fileName = table.findFileById(static_cast<int>(block.getContent()))->getName();
        infoString += "Status: OCUPAT DE:\nFisier: " + fileName + "\n" + "ID:" + std::to_string(block.getContent());
    } else {
        infoString += "Status: LIBER";
    }

    BaseFile *file = table.findFileById(static_cast<int>(block.getContent()));
    if (file) {
        const auto *sysFile = dynamic_cast<SystemFile *>(file);

        if (sysFile) {
            infoString += "\nNivel Securitate: " + std::to_string(sysFile->getSecurityLevel());
        }
    }

    const sf::Vector2i mouseIntPoz = sf::Mouse::getPosition(window);
    const sf::Vector2f mousePoz(mouseIntPoz);
    const float mouseX = mousePoz.x;
    const float mouseY = mousePoz.y;
    const float initalX = mouseX + 15.f;
    const float initalY = mouseY + 15.f;
    float finalX = initalX;
    const float finalY = initalY;
    toolTipText.setString(infoString);

    const sf::FloatRect textBounds = toolTipText.getGlobalBounds();

    if (const float toolTipWidth = textBounds.width + 10.f; initalX + toolTipWidth + 55.f > SCREEN_WIDTH) {
        finalX = SCREEN_WIDTH - toolTipWidth - 60.f;
    }

    toolTipBackground.setSize(sf::Vector2f(textBounds.width + 10.f, textBounds.height + 10.f));

    toolTipText.setPosition(finalX, finalY);
    toolTipBackground.setPosition(finalX, finalY);

    window.draw(toolTipBackground);
    window.draw(toolTipText);
}

void GUI::updateAndDrawDashBoard() {
    const int numOfTotalBlocks = disk.getNumBlocks();
    const int numOfFreeBlocks = disk.getTotalFreeBlocks();
    const int numOfUsedBlocks = disk.getTotalUsedBlocks();
    const int numOfBadBlocks = disk.getTotalBadBlocks();
    const int activeFiles = BaseFile::getTotalFiles();
    const double fragmentationPercentage = disk.getFragmentationPercentage() * 100;

    fragmentationStats.addValue(fragmentationPercentage);

    std::string statsStr = "STATISTICI DISC:\nFisiere Active: " + std::to_string(activeFiles) +
                           "\nTotal: " + std::to_string(numOfTotalBlocks) +
                           " blocuri\nLiber: " + std::to_string(numOfFreeBlocks) + " blocuri\nOcupate: " +
                           std::to_string(numOfUsedBlocks)
                           + " blocuri\nStricate: " + std::to_string(numOfBadBlocks) + " blocuri\nFragmentare: " +
                           std::to_string(fragmentationPercentage) + "%";

    statsStr += "\nISTORIC \n";
    statsStr += "Fragmentare maxima: " + std::to_string(fragmentationStats.getMax()).substr(0, 4) +
            "%\nFragmentare medie: " + std::to_string(fragmentationStats.getAverage()).substr(0, 4) +
            "%\n" + "Cel mai mare fisier creat: " + std::to_string(fileSizeStats.getMax()) + " blocuri\n";

    if (fileSizeStats.getCount() > 0) {
        statsStr += "Cel mai mic fisier creat: " + std::to_string(fileSizeStats.getMin()) + " blocuri\n";
        statsStr += "Medie fisiere: " + std::to_string(fileSizeStats.getAverage()).substr(0, 4) + " blocuri\n";
    }


    dashBoardText.setString(statsStr);

    window.draw(dashBoardText);
}

void GUI::drawFragmentationBar() {
    const double fragmentationPer = disk.getFragmentationPercentage();
    if (validateRange<double>(fragmentationPer, 0.0, 0.3)) {
        fragmentationBarFill.setFillColor(sf::Color(100, 255, 170));
    } else if (validateRange<double>(fragmentationPer, 0.30001, 0.65)) {
        fragmentationBarFill.setFillColor(sf::Color(255, 180, 0));
    } else {
        fragmentationBarFill.setFillColor(sf::Color(200, 50, 50));
    }

    const float maxBarWidth = fragmentationBarBackground.getSize().x;
    const float currentWidth = static_cast<float>(fragmentationPer) * maxBarWidth;

    fragmentationBarFill.setSize(sf::Vector2f(currentWidth, fragmentationBarBackground.getSize().y));

    window.draw(fragmentationBarBackground);
    window.draw(fragmentationBarFill);
}

void GUI::drawMiniMap(const float &pulse) {
    window.draw(miniMapBackground);
    const sf::Vector2f miniMapPosition = miniMapBackground.getPosition();
    for (int i = 0; i < disk.getNumBlocks(); i++) {
        const Block &block = disk.getBlock(i);

        if (currentState == DEFRAGMENTATION_ANIMATION &&
            (i == defragBlockToScan || i == defragEmptySlot)) {
            continue;
        }

        if (block.isBad() == true) {
            miniMapBlock.setFillColor(sf::Color::Black);
        } else if (block.getOccupied() == true) {
            const unsigned long fileId = block.getContent();
            int colorIndex = static_cast<int>(fileId) % 5;
            switch (colorIndex) {
                case 0: {
                    miniMapBlock.setFillColor(sf::Color(255, 128, 0));
                    break;
                }
                case 1: {
                    miniMapBlock.setFillColor(sf::Color(0, 255, 255));
                    break;
                }
                case 2: {
                    miniMapBlock.setFillColor(sf::Color(255, 0, 128));
                    break;
                }
                case 3: {
                    miniMapBlock.setFillColor(sf::Color(255, 200, 0));
                    break;
                }
                case 4: {
                    miniMapBlock.setFillColor(sf::Color(60, 150, 255));
                    break;
                }
                default: {
                    miniMapBlock.setFillColor(sf::Color(120, 160, 255, 100));
                    break;
                }
            }
        } else {
            miniMapBlock.setFillColor(sf::Color(120, 160, 255, 100));
        }

        if (currentState == DEFRAGMENTING) {
            if (i == defragBlockToScan) {
                miniMapBlock.setFillColor(sf::Color(static_cast<sf::Uint8>(pulse * 255),
                                                    static_cast<sf::Uint8>(pulse * 255), 255));
            }
        }


        const int col = i % blocksPerRow;
        const int row = i / blocksPerRow;
        const float pozX = miniMapPosition.x + (static_cast<float>(col) * 2.f);
        const float pozY = miniMapPosition.y + (static_cast<float>(row) * 2.f);

        miniMapBlock.setPosition(pozX, pozY);
        window.draw(miniMapBlock);
    }
}

GUI::GUI(DiskSpaceMap &disk, AllocationTable &table) : disk(disk), table(table) {
    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Defragmentor v0.3");
    blocksPerRow = SCREEN_WIDTH / static_cast<int>(BLOCK_LENGTH);
    window.setFramerateLimit(60);

    blockShape.setSize(sf::Vector2f(BLOCK_LENGTH, BLOCK_LENGTH));
    blockShape.setOutlineColor(sf::Color::Black);
    blockShape.setOutlineThickness(1);
    animatedDefragmentationBlock.setSize(sf::Vector2f(BLOCK_LENGTH, BLOCK_LENGTH));
    animatedDefragmentationBlock.setOutlineColor(sf::Color::Black);
    animatedDefragmentationBlock.setOutlineThickness(1);

    if (font.loadFromFile("fonts/arial.ttf") == false) {
        std::cout << "EROARE: Nu s-a putut incarca fontul" << std::endl;
        window.close();
    }

    blockText.setFont(font);
    blockText.setCharacterSize(18);
    blockText.setFillColor(sf::Color::Black);

    currentState = NORMAL;

    inputBackground.setSize(sf::Vector2f(600.f, 100.f));
    inputBackground.setFillColor(sf::Color(50, 50, 50, 220));
    inputBackground.setOutlineColor(sf::Color::White);
    inputBackground.setOutlineThickness(4.f);

    inputPromptText.setFont(font);
    inputPromptText.setCharacterSize(16);
    inputPromptText.setFillColor(sf::Color::White);
    inputPromptText.setString("Introduceti ID-ul fisierului de sters (Enter = Confirma, Esc = Anuleaza): ");
    inputPromptText.setPosition(
        inputBackground.getPosition().x + 10.f,
        inputBackground.getPosition().y + 10.f
    );

    inputUserText.setFont(font);
    inputUserText.setCharacterSize(20);
    inputUserText.setFillColor(sf::Color::White);
    inputUserText.setPosition(
        inputPromptText.getPosition().x,
        inputPromptText.getPosition().y + 40.f
    );

    legendText.setFont(font);
    legendText.setCharacterSize(20);
    legendText.setFillColor(sf::Color::White);
    legendText.setString("CONTROALE:\n "
        "A - Adauga Fisier\n"
        " S - Sterge Fisier\n"
        " D - Defragmenteaza\n"
        " T - Trunchiaza Fisier\n"
        " E - Extinde Fisier\n"
        " MOUSELEFT - Strica un bloc\n"
        " MOUSEWHEEL - ZOOM si Mutare\n"
        " R - Repara/Mentenanta\n"
        " V - Verifica Checksum\n"
        " X - Exit");
    legendText.setPosition(SCREEN_WIDTH - legendText.getGlobalBounds().width - 100.f,
                           SCREEN_HEIGHT - legendText.getGlobalBounds().height - 100.f);

    toolTipText.setFont(font);
    toolTipText.setCharacterSize(14);
    toolTipText.setFillColor(sf::Color::White);

    toolTipBackground.setFillColor({0, 0, 0, 170});

    dashBoardText.setFont(font);
    dashBoardText.setCharacterSize(20);
    dashBoardText.setFillColor(sf::Color::White);
    dashBoardText.setPosition(10.f,SCREEN_HEIGHT - 420.f);

    constexpr float ySpacing = 150.f;

    const float barX = dashBoardText.getPosition().x;
    const float barY = dashBoardText.getGlobalBounds().height + dashBoardText.getPosition().y + 160.f + ySpacing;

    fragmentationBarBackground.setSize({300.f, 30.f});
    fragmentationBarBackground.setPosition(barX, barY);
    fragmentationBarBackground.setOutlineThickness(1.f);
    fragmentationBarBackground.setOutlineColor(sf::Color::White);
    fragmentationBarBackground.setFillColor(sf::Color(50, 50, 50));

    fragmentationBarFill.setPosition(barX, barY);

    tempFileId = 0;
    tempFileSize = 0;
    defragBlockToScan = 0;
    defragEmptySlot = 0;
    hoveredBlockIndex = -1;
    animationDefragmentationSpeed = 5.f;
    animationDefragmentationProgress = 0.f;

    isPanning = false;
    camera.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    camera.setCenter(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

    float miniMapWidth = static_cast<float>(blocksPerRow) * 2.f;
    float miniMapHeight = (static_cast<float>(disk.getNumBlocks()) / static_cast<float>(blocksPerRow)) * 2.f;

    miniMapBackground.setSize({miniMapWidth, miniMapHeight});
    miniMapBackground.setFillColor(sf::Color(50, 50, 50, 220));
    miniMapBackground.setPosition(0.95f * SCREEN_WIDTH - miniMapBackground.getSize().x, 10.f);

    miniMapBlock.setSize(sf::Vector2f(2.f, 2.f));
    miniMapBlock.setFillColor(sf::Color(120, 160, 255, 100));
}

void GUI::run() {
    while (window.isOpen()) {
        sf::Event event{};
        const double pulse = (std::sin(globalClock.getElapsedTime().asSeconds() * 5.f) + 1.f) / 2.f;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (currentState == NORMAL) {
                    switch (event.key.code) {
                        case sf::Keyboard::A: {
                            inputBackground.setOutlineColor(sf::Color::White);
                            currentState = INPUT_ADD_ID;
                            inputPromptText.setString("Introduceti ID ul noului fisier (Enter = Urmatorul Pas):");
                            inputText = "";
                            break;
                        }

                        case sf::Keyboard::D: {
                            std::cout << "Defragmentare... " << std::endl;
                            currentState = DEFRAGMENTING;
                            defragBlockToScan = 0;
                            defragEmptySlot = 0;
                            break;
                        }

                        case sf::Keyboard::T: {
                            inputBackground.setOutlineColor(sf::Color::White);
                            currentState = INPUT_TRUNCATE_ADD_ID;
                            inputPromptText.setString("Introduceti Id-ul (Enter = Mai departe): ");
                            inputText = "";
                            break;
                        }

                        case sf::Keyboard::E: {
                            inputBackground.setOutlineColor(sf::Color::White);
                            currentState = INPUT_EXTEND_ADD_ID;
                            inputPromptText.setString("Introduceti Id-ul (Enter = Mai departe): ");
                            inputText = "";
                            break;
                        }

                        case sf::Keyboard::S: {
                            inputBackground.setOutlineColor(sf::Color::White);
                            currentState = INPUT_DELETE;
                            inputPromptText.setString("Introduceti Id ul fisierul de sters: (Enter: Stergere)");
                            inputText = "";
                            break;
                        }

                        case sf::Keyboard::V: {
                            for (const auto &i: table.getFiles()) {
                                try {
                                    i->verifyChecksum(disk);
                                    std::cout << "Fisierul " << i->getName() << " este valid! \n";
                                } catch (const CorruptedDataException &e) {
                                    std::cout << e.what() << " la " << i->getName() << std::endl;
                                }
                            }
                            break;
                        }

                        case sf::Keyboard::R: {
                            table.runMaintenance(disk);
                            disk.relocateDamagedBlocks(table);
                            break;
                        }

                        case sf::Keyboard::X: {
                            window.close();
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_ADD_ID) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleSubmitAddId();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_EXTEND_ADD_ID) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleSubmitExtendAddId();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_EXTEND) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleSubmitExtend();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_TRUNCATE_ADD_ID) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleSubmitTruncateAddId();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_TRUNCATE) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleSubmitTruncate();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_ADD_SIZE) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleSubmitAddSize();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_ADD_NAME) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleSubmitAddName();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                } else if (currentState == INPUT_DELETE) {
                    switch (event.key.code) {
                        case sf::Keyboard::Enter: {
                            handleDeleteInput();
                            break;
                        }

                        case sf::Keyboard::Backspace: {
                            if (inputText.empty() == false) {
                                inputText.pop_back();
                            }
                            break;
                        }

                        case sf::Keyboard::Escape: {
                            currentState = NORMAL;
                            break;
                        }

                        default: break;
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (currentState == NORMAL) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                        const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, camera);

                        const int col = static_cast<int>(worldPos.x / BLOCK_LENGTH);
                        const int row = static_cast<int>(worldPos.y / BLOCK_LENGTH);

                        if (const int index = row * blocksPerRow + col;
                            index >= 0 && index < disk.getNumBlocks() && col < blocksPerRow && col >= 0) {
                            disk.markBlockAsDamaged(index);
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    isPanning = true;
                    lastPanPos = sf::Mouse::getPosition(window);
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    isPanning = false;
                }
            } else if (event.type == sf::Event::TextEntered) {
                handleTextInput(event.text.unicode, currentState);
            } else if (event.type == sf::Event::MouseMoved) {
                if (currentState == NORMAL) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, camera);

                    const int col = static_cast<int>(worldPos.x / BLOCK_LENGTH);
                    const int row = static_cast<int>(worldPos.y / BLOCK_LENGTH);

                    if (const int index = row * blocksPerRow + col; index >= 0 && index < disk.getNumBlocks() &&
                                                                    col < blocksPerRow && col >= 0) {
                        hoveredBlockIndex = index;
                    } else {
                        hoveredBlockIndex = -1;
                    }
                }

                if (isPanning == true) {
                    const sf::Vector2i newMousePos = sf::Mouse::getPosition(window);

                    sf::Vector2f worldOldPos = window.mapPixelToCoords(lastPanPos, camera);

                    sf::Vector2f worldNewPos = window.mapPixelToCoords(newMousePos, camera);

                    const sf::Vector2f delta = worldOldPos - worldNewPos;

                    camera.move(delta);

                    lastPanPos = newMousePos;
                }
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    camera.zoom(0.9f);
                } else if (event.mouseWheelScroll.delta < 0) {
                    camera.zoom(1.1f);
                }
            }
        }

        if (currentState == DEFRAGMENTING) {
            runDefragmentStep();
        } else if (currentState == DEFRAGMENTATION_ANIMATION) {
            runDefragmentAnimation();
        }

        window.clear(sf::Color(30, 30, 50));

        window.setView(camera);

        drawDiskMap(static_cast<float>(pulse));

        if (currentState == DEFRAGMENTATION_ANIMATION) {
            window.draw(animatedDefragmentationBlock);
        }

        window.setView(window.getDefaultView());

        if (currentState != NORMAL && currentState != DEFRAGMENTING && currentState != DEFRAGMENTATION_ANIMATION) {
            drawInputBox();
        }

        updateAndDrawDashBoard();
        drawFragmentationBar();
        window.draw(legendText);
        drawToolTip();
        drawMiniMap(static_cast<float>(pulse));
        window.display();
    }
}
