#include <iostream>
#include <utility>
//DEFRAGMENTORUL per se nu este inca implementat
//de pus liste de initializare
//momentan simulez content si id uri
//de implementat first-fit/best-fit/worst-fit
//de implementat format type urile
//functiile netriviale sunt comentate ca atare
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

class File {
    private:
        Block *fileBlocks;
        int numBlocks, id;
        std::string name;

    public:
        explicit File(int id = 0, int size = 0, const std::string &name = "") {
            this->id = id;
            this->name = name;
            this->numBlocks = size;
            this->fileBlocks = new Block[size];

            for (int i = 0; i < size; i++) {
                this->fileBlocks[i].setData(i + 1, true, this->id);
            }
        }

        File(const File &other) { //cc
            this->id = other.id;
            this->name = other.name;
            this->fileBlocks = new Block[other.numBlocks];
            for (int i = 0; i < other.numBlocks; i++) {
                this->fileBlocks[i] = other.fileBlocks[i];
            }
            this->numBlocks = other.numBlocks;
        }

        File(File &&other) noexcept { //cm
            this->fileBlocks = other.fileBlocks;
            this->numBlocks = other.numBlocks;
            this->id = other.id;
            this->name = std::move(other.name);
            other.fileBlocks = nullptr;
            other.numBlocks = 0;
            other.id = 0;
            other.name = "";
        }

        File &operator=(const File &other) {
            if (this == &other) {
                return *this;
            }

            auto *tempBlocks = new Block[other.numBlocks];

            for (int i = 0; i < other.numBlocks; i++) {
                tempBlocks[i] = other.fileBlocks[i];
            }

            delete[] this->fileBlocks;
            this->fileBlocks = tempBlocks;
            this->numBlocks = other.numBlocks;
            this->id = other.id;
            this->name = other.name;

            return *this;
        }

        File &operator=(File &&other) noexcept {
            if (this == &other) {
                return *this;
            }
            delete[] this->fileBlocks;
            this->fileBlocks = other.fileBlocks;
            this->numBlocks = other.numBlocks;
            this->id = other.id;
            this->name = std::move(other.name);
            other.fileBlocks = nullptr;
            other.numBlocks = 0;
            other.id = 0;
            other.name = "";

            return *this;
        }


        [[nodiscard]] int getId() const {
            return this->id;
        }

        ~File() {
            delete[] this->fileBlocks;
        }

        [[nodiscard]] int getNumBlocks() const {
            return this->numBlocks;
        }

        friend std::ostream &operator<<(std::ostream &os, const File &file) {
            os<<"File "<<file.id<<": "<<file.name << " -> ";
            for (int i = 0; i < file.numBlocks; i++) {
                os<< file.fileBlocks[i] << " ";
            }
            return os;
        }
};

class DiskSpaceMap {
    private:
        Block *diskBlocks;
        int totalBlocks;
    public:
        explicit DiskSpaceMap(int totalBlocks = 0) {
            this->totalBlocks = totalBlocks;
            this->diskBlocks = new Block[totalBlocks];
        }

        DiskSpaceMap(const DiskSpaceMap &other) { //cc
            this->totalBlocks = other.totalBlocks;
            this->diskBlocks = new Block[other.totalBlocks];
            for (int i = 0; i < other.totalBlocks; i++) {
                this->diskBlocks[i] = other.diskBlocks[i];
            }
        }

        DiskSpaceMap(DiskSpaceMap &&other) noexcept { //cm
            this->totalBlocks = other.totalBlocks;
            this->diskBlocks = other.diskBlocks;
            other.diskBlocks = nullptr;
            other.totalBlocks = 0;
        }

        DiskSpaceMap &operator=(const DiskSpaceMap &other) {
            if (this == &other) {
                return *this;
            }

            auto *tempBlocks = new Block[other.totalBlocks];



            for (int i = 0; i < other.totalBlocks; i++) {
                tempBlocks[i] = other.diskBlocks[i];
            }

            delete[] this->diskBlocks;

            this->diskBlocks = tempBlocks;
            this->totalBlocks = other.totalBlocks;

            return *this;
        }

        DiskSpaceMap &operator=(DiskSpaceMap &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            delete[] this->diskBlocks;
            this->diskBlocks = other.diskBlocks;
            this->totalBlocks = other.totalBlocks;
            other.diskBlocks = nullptr;
            other.totalBlocks = 0;

            return *this;
        }

        //asta ar fi first-fit
        [[nodiscard]] int findSpace(const File& file) const { //functie netriviala
            int freeCount = 0;
            for (int i = 0; i < this->totalBlocks; i++) {
                if (diskBlocks[i].getOccupied() == false) {
                    freeCount++;
                    if (freeCount == file.getNumBlocks()) {
                        return i - file.getNumBlocks() + 1;
                    }
                }
                else {
                    freeCount = 0;
                }
            }
            return -1;
        }

        //functie netriviala
        [[nodiscard]] bool allocateFileBlocks(const File &file) const {
            int startIndex = findSpace(file);
            if (startIndex == -1) {
                return false;
            }
            int fileId = file.getId();
            for (int i = 0; i < file.getNumBlocks(); i++) {
                this->diskBlocks[startIndex + i].setData(startIndex + i, true, fileId);
            }
            return true;
        }

        ~DiskSpaceMap() {
            delete[] this->diskBlocks;
        }

        friend std::ostream &operator<<(std::ostream &os, const DiskSpaceMap &diskSpaceMap) {
            os<<"DiskSpaceMap: "<<diskSpaceMap.totalBlocks<<"total blocks -> ";
            for (int i = 0; i < diskSpaceMap.totalBlocks; i++) {
                os<<diskSpaceMap.diskBlocks[i]<<" ";
            }
            return os;
        }

};

class AllocationTable {
    private:
        File *files;
        std::string formatType;
        int fileCount, fileCapacity;

        void resize() { //functie netriviala
            int newFileCapacity = fileCapacity * 2;
            File *newFiles = new File[newFileCapacity];
            for (int i = 0; i < fileCount; i++) {
                newFiles[i] = std::move(files[i]);
            }
            delete[] files;
            files = newFiles;
            fileCapacity = newFileCapacity;
        }

    public:
        explicit AllocationTable(const std::string &type = "", int initialCapacity = 2) {
            this->files = new File[initialCapacity];
            this->fileCount = 0;
            this->fileCapacity = initialCapacity;
            this->formatType = type;
        }

        AllocationTable(const AllocationTable &other) { //cc
            this->fileCount = other.fileCount;
            this->formatType = other.formatType;
            this->fileCapacity = other.fileCapacity;
            this->files = new File[other.fileCapacity];
            for (int i = 0; i < this->fileCount; i++) {
                this->files[i] = other.files[i];
            }
        }

        AllocationTable(AllocationTable &&other)  noexcept { //cm
            this->files = other.files;
            this->fileCount = other.fileCount;
            this->formatType = std::move(other.formatType);
            this->fileCapacity = other.fileCapacity;
            other.files = nullptr;
            other.fileCount = 0;
            other.fileCapacity = 0;
            other.formatType = "";

        }

        AllocationTable &operator=(const AllocationTable &other) {
            if (this == &other) {
                return *this;
            }

            File *tempFiles = new File[other.fileCapacity];

            for (int i = 0; i < other.fileCount; i++) {
                tempFiles[i] = other.files[i];
            }

            delete[] this->files;

            this->files = tempFiles;
            this->fileCapacity = other.fileCapacity;
            this->formatType = other.formatType;
            this->fileCount = other.fileCount;


            return *this;
        }

        AllocationTable &operator=(AllocationTable &&other) noexcept {
            if (this == &other) {
                return *this;
            }
            delete[] this->files;
            this->files = other.files;
            this->fileCapacity = other.fileCapacity;
            this->formatType = std::move(other.formatType);
            this->fileCount = other.fileCount;

            other.files = nullptr;
            other.fileCount = 0;
            other.fileCapacity = 0;
            other.formatType = "";


            return *this;
        }

        void addFile(const File &fileToAdd) { //functie netriviala
            if (this->fileCount == this->fileCapacity) {
                this->resize();
            }
            this->files[this->fileCount] = fileToAdd;
            this->fileCount++;
        }

        ~AllocationTable() {
            delete[] this->files;
        }

        friend std::ostream &operator<<(std::ostream &os, const AllocationTable &allocationTable) {
            os<< "================ Tebelul de alocare (" << allocationTable.formatType <<") "
            << allocationTable.fileCount << "/" << allocationTable.fileCapacity << " fisiere ======";

            for (int i = 0; i < allocationTable.fileCount; i++) {
                os<< std::endl <<allocationTable.files[i];
            }

            os<< std::endl << "==================================";
            return os;
        }

};

int main() {
    DiskSpaceMap disk(32);
    AllocationTable table("FAT32", 2);

    File file1(101, 5, "doc.txt");
    File file2(102, 8, "driver.dll");
    File file3(103, 3, "text.txt");
    File file4(104, 20, "video.mkv");//nu o sa incapa asta
    std::cout << file1 << std::endl << file2 << std::endl << file3 << std::endl << file4 << std::endl;

    std::cout<< "Incercare alocare file1" << std::endl;
    if (disk.allocateFileBlocks(file1)) {
        table.addFile(file1);
        std::cout << "  -> SUCCES" << std::endl;
    }

    else {
        std::cout << "  -> EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Incercare alocare file2" << std::endl;
    if (disk.allocateFileBlocks(file2)) {
        table.addFile(file2);
        std::cout << "SUCCES" << std::endl;
    }

    else {
        std::cout << "EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Incercare alocare file3" << std::endl;
    if (disk.allocateFileBlocks(file3)) {
        table.addFile(file3);
        std::cout << "SUCCES" << std::endl;
    }

    else {
        std::cout << "EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Incercare alocare file4" << std::endl;
    if (disk.allocateFileBlocks(file4)) {
        table.addFile(file4);
        std::cout << "  -> SUCCES.\n";
    }

    else {
        std::cout << "EROARE: Spatiu insuficient" << std::endl;
    }

    std::cout<< "Afisare table si disk: " << std::endl;
    std::cout<< table << std::endl;
    std::cout<< disk << std::endl;

    // //teste(COMENTATE DE OARECE RIDICA WARNING-URI)
    // std::cout<< "TESTE(cc, cm, op...): " << std::endl;
    // File emptyFile(800, 0, "gol.txt");
    // File copyEmptyFile = emptyFile; //cc
    // copyEmptyFile = emptyFile; //operator
    // File moveEmptyFile = std::move(emptyFile);//cm
    //
    // std::cout << "Testul cm pe file_gol a reusit. Stare file_gol: " << emptyFile << std::endl;
    //
    // std::cout << "Test cm: " << std::endl;
    // File file99(99, 4, "file99.txt");
    // std::cout<<"Inainte de mutare: " << file99 << std::endl;
    // File file99Moved = std::move(file99);
    // std::cout << "Dupa mutare(file99Moved): " << file99Moved << std::endl;
    // std::cout << "Dupa mutare(file99 -> ar trebui sa fie gol): " << file99 << std::endl;

}