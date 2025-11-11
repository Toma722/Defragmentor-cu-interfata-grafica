#include "GUI.h"
#include "File.h"
#include "AllocationTable.h"
#include <cassert>

void GUI::handleSubmitExtend() {
    if (inputText.empty()) {
        return;
    }

    try {
        tempFileSize = static_cast<int>(std::stoul(inputText));
    }
    catch (const std::invalid_argument &) {
        std::cout<< "Numarul de Blocuri trebuie sa fie un numar!" << std::endl;
        inputText = "";
        currentState = NORMAL;
        return;
    }
    File *fileToExtend = table.findFileById(tempFileId);
    if (fileToExtend == nullptr) {
        inputPromptText.setString("Fisierul nu exista (Esc = RETURN)");
        return;
    }
    if (const File temp(0, tempFileSize); disk.findSpace(temp) == -1) {
        inputPromptText.setString("Nu Exista Loc (Defragmentare sau marire disc! ESC = RETURN)");
        return;
    }
    fileToExtend->extend(tempFileSize, disk);
    currentState = NORMAL;
    inputText = "";
}

void GUI::handleSubmitExtendAddId() {
            if (inputText.empty()) {
                return;
            }

            try {
                tempFileId = static_cast<int>(std::stoul(inputText));
            }
            catch (const std::invalid_argument &) {
                std::cout<< "Id-ul trebuie sa fie un numar!" << std::endl;
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
            }
            catch (const std::invalid_argument &) {
                std::cout<< "Numarul de Blocuri trebuie sa fie un numar!" << std::endl;
                inputText = "";
                currentState = NORMAL;
                return;
            }
            File *fileToTruncate = table.findFileById(tempFileId);
            if (fileToTruncate == nullptr) {
                inputPromptText.setString("Fisierul Nu Exista! (ESC = RETURN)");
                return;
            }
            if (tempFileSize < 1) {
                inputPromptText.setString("Nu se poate Trunchia la mai putin de 1 bloc! (ESC = RETURN)");
                return;
            }
            else if (tempFileSize >= fileToTruncate->getNumBlocks()) {
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
            }
            catch (const std::invalid_argument &) {
                std::cout<< "Id-ul trebuie sa fie un numar!" << std::endl;
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
            }
            catch (const std::invalid_argument &) {
                std::cout<< "ID Invalid! Trebuie sa fie un numar!" << std::endl;
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
                }

                catch (const std::invalid_argument &) {
                    std::cout<< "Dimensiune Invalida! Trebuie sa fie un numar!" << std::endl;
                    inputText = "";
                    currentState = NORMAL;
                    return;
                }

                if (const File temp(0, tempFileSize); disk.findSpace(temp) == -1) {
                    inputPromptText.setString("Fisierul nu incape! (Defragmentare sau marire disc! ESC = RETURN)");
                    return;
                }

                currentState = INPUT_ADD_NAME;
                inputPromptText.setString("Introduceti Numele Fisierului: ");
                inputText = "";

            }

void GUI::handleSubmitAddName() {
                File newFile(this->tempFileId, this->tempFileSize, inputText);
                const std::vector<int> map = disk.allocateFile(newFile);
                if (map.empty()) {
                    std::cout<< "Nu s-a putut adauga fisierul!" << std::endl;
                    return;
                }
                newFile.setBlockMap(map);
                table.addFile(newFile);

                currentState = NORMAL;
                inputText = "";

            }

void GUI::handleDeleteInput() {
            if (inputText.empty()) {
                return;
            }

            unsigned long fileId = 0;

            try {
                fileId = std::stoul(inputText);
            }
            catch (const std::invalid_argument &) {
                std::cout<< "ID Invalid! Trebuie sa fie un numar!" << std::endl;
                inputText = "";
                currentState = NORMAL;
                return;
            }

            const File *fileToDelete = table.findFileById(static_cast<int>(fileId));
            if (fileToDelete == nullptr) {
                inputPromptText.setString("Fisierul Nu Exista! (ESC = RETURN)");
                return;
            }

            const std::vector<int>mapToDelete = fileToDelete->getBlockMap();
            if (table.deleteFile(static_cast<int>(fileId)) == true) {
                disk.freeBlocks(mapToDelete);
                std::cout<< "Fisierul a fost sters" << std::endl;
            }
            else {
                std::cout<< "Fisierul nu a putut fi sters" << std::endl;
            }

            inputText = "";
            currentState = NORMAL;

        }

void GUI::drawDiskMap() {
            for (int i = 0; i < disk.getNumBlocks(); i++) {
                const Block &block = disk.getBlock(i);

                if (block.isBad() == true) {
                    blockShape.setFillColor(sf::Color::Black);
                }

                else if(block.getOccupied() == true) {
                    const unsigned long fileId = block.getContent();
                    int colorIndex = static_cast<int>(fileId) % 5; //ADAUGAT CULORI
                    switch (colorIndex) {
                        case 0: {
                            blockShape.setFillColor(sf::Color::Red);
                            break;
                        }
                        case 1: {
                            blockShape.setFillColor(sf::Color::Yellow);
                            break;
                        }
                        case 2: {
                            blockShape.setFillColor(sf::Color::Magenta);
                            break;
                        }
                        case 3: {
                            blockShape.setFillColor(sf::Color::White);
                            break;
                        }
                        case 4: {
                            blockShape.setFillColor(sf::Color::Cyan);
                            break;
                        }
                        default: {
                            blockShape.setFillColor(sf::Color::Green);
                            break;
                        }
                    }

                }
                else {
                    blockShape.setFillColor(sf::Color::Green);
                }

                if (currentState == DEFRAGMENTING) {
                    if (i == defragBlockToScan) {
                        blockShape.setFillColor(sf::Color::Blue);
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
                }
                else if (!inputText.empty()){
                    inputBackground.setOutlineColor(sf::Color::Red);
                }
            }
            else if (state == INPUT_ADD_NAME) {
                if (unicode >= 32 && unicode <= 127) {
                    if (inputText.length() < 10) {
                        inputText += static_cast<char>(unicode);
                    }
                }
            }
}

void GUI::runDefragmentStep() {
    if (defragBlockToScan >= disk.getNumBlocks()) {
        currentState = NORMAL;
        std::cout<< "Defragmentare Terminata" << std::endl;
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

    if (Block &blockToScan = disk.getBlockRef(defragBlockToScan); blockToScan.getOccupied() == true) {
        if (defragBlockToScan != defragEmptySlot) {
            Block &emptyBlock = disk.getBlockRef(defragEmptySlot);
            const unsigned long fileId = blockToScan.getContent();
            const int size = blockToScan.getSize();
            emptyBlock.setData(defragEmptySlot, true, fileId, size);
            table.updateBlockAddress(static_cast<int>(fileId), defragBlockToScan, defragEmptySlot);
            blockToScan.clear();

        }
        defragEmptySlot++;
    }
    defragBlockToScan++;
}

void GUI::drawToolTip() {
    if (hoveredBlockIndex == -1) {
        return;
    }

    const Block &block = disk.getBlock(hoveredBlockIndex);
    std::string infoString = "Index fizic: " + std::to_string(hoveredBlockIndex) + "\n";
    if (block.isBad() == true) {
        infoString += "Status: STRICAT";
    }
    else if (block.getOccupied() == true) {
        const std::string fileName = table.findFileById(static_cast<int>(block.getContent()))->getName();
        infoString += "Status: OCUPAT DE:\nFisier: " + fileName + "\n" + "ID:" + std::to_string(block.getContent());
    }
    else {
        infoString += "Status: LIBER";
    }

    const sf::Vector2i mouseIntPoz = sf::Mouse::getPosition(window);
    const sf::Vector2f mousePoz(mouseIntPoz);
    const float mouseX = mousePoz.x;
    const float mouseY = mousePoz.y;
    float initalX = mouseX + 15.f;
    float initalY = mouseY + 15.f;
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
    const int totalBlocks = disk.getNumBlocks();
    const int freeBlocks = disk.getTotalFreeBlocks();
    const int usedBlocks = disk.getTotalUsedBlocks();
    const int badBlocks = disk.getTotalBadBlocks();
    const double fragmentationPercentage = disk.getFragmentationPercentage() * 100;

    dashBoardText.setString("STATISTICI DISC:\nTotal: " + std::to_string(totalBlocks) +
        " blocuri\nLiber: " + std::to_string(freeBlocks) + " blocuri\nOcupate: " + std::to_string(usedBlocks)
        + " blocuri\nStricate: " + std::to_string(badBlocks) + " blocuri\nFragmentare: " + std::to_string(fragmentationPercentage) + "%");

    window.draw(dashBoardText);
}

GUI::GUI(DiskSpaceMap &disk, AllocationTable &table) : disk(disk), table(table){
            window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Defragmentor v0.3");
            blocksPerRow = SCREEN_WIDTH / static_cast<int>(BLOCK_LENGTH);
            window.setFramerateLimit(30);

            blockShape.setSize(sf::Vector2f(BLOCK_LENGTH, BLOCK_LENGTH));
            blockShape.setOutlineColor(sf::Color::Black);
            blockShape.setOutlineThickness(1);

            if (font.loadFromFile("fonts/arial.ttf") == false) {
                std::cout<< "EROARE: Nu s-a putut incarca fontul" << std::endl;
                window.close();
            }

            blockText.setFont(font);
            blockText.setCharacterSize(18);
            blockText.setFillColor(sf::Color::Black);

            currentState = GuiState::NORMAL;

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
            legendText.setString("CONTROALE:\n A - Adauga Fisier\n S - Sterge Fisier\n D - Defragmenteaza\n T - Trunchiaza Fisier\n E - Extinde Fisier\n MOUSELEFT - Strica un bloc\n R - Repara\n V - Verifica Checksum\n X - Exit");
            legendText.setPosition(SCREEN_WIDTH - legendText.getGlobalBounds().width - 100.f,
                                    SCREEN_HEIGHT - legendText.getGlobalBounds().height - 100.f);

            toolTipText.setFont(font);
            toolTipText.setCharacterSize(14);
            toolTipText.setFillColor(sf::Color::White);

            toolTipBackground.setFillColor({0, 0, 0, 170});

            dashBoardText.setFont(font);
            dashBoardText.setCharacterSize(20);
            dashBoardText.setFillColor(sf::Color::White);
            dashBoardText.setPosition(0.f,SCREEN_HEIGHT - 320.f);

            tempFileId = 0;
            tempFileSize = 0;
            defragBlockToScan = 0;
            defragEmptySlot = 0;
            hoveredBlockIndex = -1;
        }

void GUI::run() {
            while (window.isOpen()) {
                sf::Event event{};
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
                                    std::cout<< "Defragmentare... " << std::endl;
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

                                    for (const auto & i : table.getFiles()) {

                                        const bool checksumOK = i.verifyChecksum(disk);
                                        assert(checksumOK && "CHECKSUM A ESUAT");
                                        std::cout<< "CHECKSUM OK" << std::endl;
                                    }
                                    break;
                                }

                                case sf::Keyboard::R: {
                                    disk.relocateDamagedBlocks(table);
                                    break;
                                }

                                case sf::Keyboard::X: {
                                    window.close();
                                    break;
                                }

                                default: break;
                            }
                        }

                        else if (currentState == INPUT_ADD_ID) {
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
                        }

                        else if (currentState == INPUT_EXTEND_ADD_ID) {
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
                        }

                        else if (currentState == INPUT_EXTEND) {
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
                        }

                        else if (currentState == INPUT_TRUNCATE_ADD_ID) {
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
                        }

                        else if (currentState == INPUT_TRUNCATE) {
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
                        }

                        else if (currentState == INPUT_ADD_SIZE) {
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
                        }

                        else if (currentState == INPUT_ADD_NAME) {
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
                        }

                        else if (currentState == INPUT_DELETE) {
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

                    }

                    else if (event.type == sf::Event::MouseButtonPressed) {
                        if (currentState == NORMAL) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                const int mouseX = event.mouseButton.x;
                                const int mouseY = event.mouseButton.y;
                                const int col = mouseX / static_cast<int>(BLOCK_LENGTH);
                                const int row = mouseY / static_cast<int>(BLOCK_LENGTH);

                                if (const int index = row * blocksPerRow + col; index >= 0 && index < disk.getNumBlocks()) {
                                    disk.markBlockAsDamaged(index);
                                }
                            }
                        }
                    }

                    else if (event.type == sf::Event::TextEntered) {
                        handleTextInput(event.text.unicode, currentState);
                    }

                    else if (event.type == sf::Event::MouseMoved) {
                        if (currentState == NORMAL) {
                            const int mouseX = event.mouseMove.x;
                            const int mouseY = event.mouseMove.y;

                            const int col = mouseX / static_cast<int>(BLOCK_LENGTH);
                            const int row = mouseY / static_cast<int>(BLOCK_LENGTH);

                            const int index = row * blocksPerRow + col;

                            if (index >= 0 && index < disk.getNumBlocks()) {
                                hoveredBlockIndex = index;
                            }
                            else {
                                hoveredBlockIndex = -1;
                            }
                        }
                    }
                }

                if (currentState == DEFRAGMENTING) {
                    runDefragmentStep();
                }

                window.clear(sf::Color(128, 128, 128));
                drawDiskMap();
                if (currentState != NORMAL && currentState != DEFRAGMENTING) {
                    drawInputBox();
                }

                if (currentState == NORMAL) {
                    drawToolTip();
                }

                updateAndDrawDashBoard();
                window.draw(legendText);
                window.display();
            }
        }