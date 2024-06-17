

// #include "../include/map.h"

// #include <ncurses.h>
// #include <unistd.h>

// #include <cstdlib>
// #include <ctime>

// #include "../include/position.h"
// #include "../include/snake.h"
// #include "../include/wall.h"  // Include wall.h to access place_wall function

// #define GROWTH_ITEM_SYMBOL '+'
// #define POISON_ITEM_SYMBOL '-'

// int map[MAP_SIZE][MAP_SIZE];

// int items[MAX_ITEMS];
// int num_items = 0;

// void initialize_map() {
//   for (int i = 0; i < MAP_SIZE; ++i) {
//     for (int j = 0; j < MAP_SIZE; ++j) {
//       if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1) {
//         place_wall(Position(i, j));  // Use place_wall function from wall.cpp
//       } else {
//         map[i][j] = EMPTY_SPACE;
//       }
//     }
//   }

//   place_wall(Position(0, 0), true);
//   place_wall(Position(0, MAP_SIZE - 1), true);
//   place_wall(Position(MAP_SIZE - 1, 0), true);
//   place_wall(Position(MAP_SIZE - 1, MAP_SIZE - 1), true);
// }

// void initialize_items() {
//   srand(time(NULL));

//   for (int i = 0; i < INITIAL_GROWTH_ITEMS; ++i) {
//     int x = rand() % (MAP_SIZE - 2) + 1;
//     int y = rand() % (MAP_SIZE - 2) + 1;
//     place_item(x, y, GROWTH_ITEM);
//   }

//   for (int i = 0; i < INITIAL_POISON_ITEMS; ++i) {
//     int x = rand() % (MAP_SIZE - 2) + 1;
//     int y = rand() % (MAP_SIZE - 2) + 1;
//     place_item(x, y, POISON_ITEM);
//   }
// }

// void place_item(int x, int y, int item) { map[x][y] = item; }

// void clear_item(int x, int y) { map[x][y] = EMPTY_SPACE; }

// void print_map() {
//   initscr();
//   clear();

//   for (int i = 0; i < MAP_SIZE; ++i) {
//     for (int j = 0; j < MAP_SIZE; ++j) {
//       switch (map[i][j]) {
//         case EMPTY_SPACE:
//           printw(" ");
//           break;
//         case WALL:
//           printw("#");
//           break;
//         case IMMUNE_WALL:
//           printw("X");
//           break;
//         case SNAKE_HEAD:
//           printw("O");
//           break;
//         case SNAKE_BODY:
//           printw("o");
//           break;
//         case GROWTH_ITEM:
//           attron(COLOR_PAIR(1));
//           printw("%c", GROWTH_ITEM_SYMBOL);
//           attroff(COLOR_PAIR(1));
//           break;
//         case POISON_ITEM:
//           attron(COLOR_PAIR(2));
//           printw("%c", POISON_ITEM_SYMBOL);
//           attroff(COLOR_PAIR(2));
//           break;
//         case GATE:
//           printw("I");
//           break;
//         default:
//           printw(" ");
//           break;
//       }
//     }
//     printw("\n");
//   }

//   refresh();
//   usleep(500000);
// }