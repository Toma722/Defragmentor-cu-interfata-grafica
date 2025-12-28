#include "DiskSpaceMap.h"
#include "AllocationTable.h"
#include "GUI.h"

int main() {
    DiskSpaceMap disk(1000);
    AllocationTable table("FAT32", 4);

    GUI gui(disk, table);
    gui.run();
}
