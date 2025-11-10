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
        std::cout<< "Fisierul nu exista!" << std::endl;
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
                std::cout<< "Fisierul nu exista!" << std::endl;
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
                std::cout<< "Fisier cu Id deja existent" << std::endl;
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

                currentState = INPUT_ADD_NAME;
                inputPromptText.setString("Introduceti Numele Fisierului: ");
                inputText = "";

            }

void GUI::handleSubmitAddName() {
                File newFile(static_cast<int>(this->tempFileId), this->tempFileSize, inputText);
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
                std::cout<< "Fiserul nu este gasit!" << std::endl;
                currentState = NORMAL;
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
            constexpr int blocksPerRow = SCREEN_WIDTH / static_cast<int>(BLOCK_LENGTH);

            for (int i = 0; i < disk.getNumBlocks(); i++) {
                const Block &block = disk.getBlock(i);

                if (block.isBad() == true) {
                    blockShape.setFillColor(sf::Color::Black);
                }

                else if(block.getOccupied() == true) {
                    blockShape.setFillColor(sf::Color::Red);
                }
                else {
                    blockShape.setFillColor(sf::Color::Green);
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
                    if (inputText.length() < 10) {
                        inputText += static_cast<char>(unicode);
                    }
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

GUI::GUI(DiskSpaceMap &disk, AllocationTable &table) : disk(disk), table(table){
            window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Defragmentor v0.3");
            window.setFramerateLimit(60);

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
            inputBackground.setOutlineThickness(1.f);

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
            legendText.setString("CONTROALE:\n A - Adauga Fisier\n S - Sterge Fisier\n D - Defragmenteaza\n T - Trunchiaza Fisier\n E - Extinde Fisier\n V - Verifica Checksum\n X - Exit");
            legendText.setPosition(SCREEN_WIDTH - legendText.getGlobalBounds().width - 10.f,
                                    SCREEN_HEIGHT - legendText.getGlobalBounds().height - 10.f);

            tempFileId = 0;
            tempFileSize = 0;
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
                                    currentState = INPUT_ADD_ID;
                                    inputPromptText.setString("Introduceti ID ul noului fisier (Enter = Urmatorul Pas):");
                                    inputText = "";
                                    break;
                                }

                                case sf::Keyboard::D: {
                                    std::cout<< "Defragmentare... " << std::endl;
                                    disk.defragment(table);
                                    std::cout<<"Defragmentare Terminata" << std::endl;
                                    break;
                                }

                                case sf::Keyboard::T: {
                                    currentState = INPUT_TRUNCATE_ADD_ID;
                                    inputPromptText.setString("Introduceti Id-ul (Enter = Mai departe): ");
                                    inputText = "";
                                    break;
                                }

                                case sf::Keyboard::E: {
                                    currentState = INPUT_EXTEND_ADD_ID;
                                    inputPromptText.setString("Introduceti Id-ul (Enter = Mai departe): ");
                                    inputText = "";
                                    break;
                                }

                                case sf::Keyboard::S: {
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

                    else if (event.type == sf::Event::TextEntered) {
                        handleTextInput(event.text.unicode, currentState);
                    }
                }

                window.clear(sf::Color(128, 128, 128));
                drawDiskMap();
                if (currentState != NORMAL) {
                    drawInputBox();
                }
                window.draw(legendText);
                window.display();
            }
        }