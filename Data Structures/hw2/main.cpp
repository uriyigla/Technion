#include "UnionFind.h"
#include "worldcup23a2.h"
#include "HashTable.h"

int main() {
    world_cup_t world1;
    world1.add_team(1311);
    int a[5];
    world1.add_player(1,1311,a,0,9,6, false);
    world1.add_player(2,1311,a,0,9,6, true);
    world1.add_player(3,1311,a,0,9,6, false);
    world1.add_player(98,1311,a,0,9,6, false);
    world1.add_player(17,1311,a,0,9,6, false);
    world1.playersArray.print();
    world1.add_team(167);
    int b[5];
    world1.add_player(12,167,b,0,9,6, false);
    world1.add_player(28,167,b,0,9,6, true);
    world1.add_player(32,167,b,0,9,6, false);
    world1.playersArray.print();




}
