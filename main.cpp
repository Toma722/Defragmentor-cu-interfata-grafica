#include "DiskSpaceMap.h"
#include "AllocationTable.h"
#include "GUI.h"
//de pus liste de initializare
//momentan simulez content si id uri
//de implementat first-fit/best-fit/worst-fit
//de implementat format type urile

int main() {
    DiskSpaceMap disk(64);
    AllocationTable table("FAT32", 4);

    GUI gui(disk, table);
    gui.run();

}
