#include "../include/draw.h"

#define EMPTY_SPACE 0
#define WALL 1
#define IMMUNE_WALL 2
#define SNAKE_HEAD 3
#define SNAKE_BODY 4
#define GROWTH_ITEM 5
#define POISON_ITEM 6
#define GATE 7

void drawGame(int row, int col, vector<vector<int> > ary,
              const map<string, string>& score,
              const map<string, string>& mission) {
  clear();
  start_color();                                     // Color 사용 선언
  init_pair(EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE);  // 색 attribute 설정
  init_pair(WALL, WALL, WALL);
  init_pair(IMMUNE_WALL, IMMUNE_WALL, IMMUNE_WALL);
  init_pair(SNAKE_HEAD, SNAKE_HEAD, SNAKE_HEAD);
  init_pair(SNAKE_BODY, SNAKE_BODY, SNAKE_BODY);
  init_pair(GROWTH_ITEM, GROWTH_ITEM, GROWTH_ITEM);
  init_pair(POISON_ITEM, POISON_ITEM, POISON_ITEM);
  init_pair(GATE, GATE, GATE);

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      attron(COLOR_PAIR(ary[i][j]));
      mvprintw(i, j * 2, "  ");
      attroff(COLOR_PAIR(ary[i][j]));
    }
  }
  for (int i = 0; i < row; i++) {  // score/mission board 출력(세로)
    attron(COLOR_PAIR(6));
    move(i, col * 2 + 1);
    printw(" ");
    move(i, col * 4 + 1);
    printw(" ");
    attroff(COLOR_PAIR(6));
  }

  for (int i = 0; i < col * 2; i++) {  // score/mission board 출력(가로)
    attron(COLOR_PAIR(6));
    move(0, col * 2 + 1 + i);
    printw(" ");
    move(row / 2, col * 2 + 1 + i);
    printw(" ");
    move(row - 1, col * 2 + 1 + i);
    printw(" ");
    attroff(COLOR_PAIR(6));
  }
  //-------------------------------------------------------------------//
  attron(COLOR_PAIR(6));
  mvprintw(0, col * 8 / 3, "Score Board");
  attroff(COLOR_PAIR(6));

  attron(COLOR_PAIR(6));
  mvprintw(row / 2, col * 8 / 3, "Mission Board");  // Score/Mission 글씨 출력
  attroff(COLOR_PAIR(6));

  move(1, col * 2 + 2);
  printw("B : %s", score.at("length").c_str());
  move(2, col * 2 + 2);
  printw("+ : %s", score.at("growth").c_str());
  move(3, col * 2 + 2);
  printw("- : %s", score.at("poison").c_str());
  move(4, col * 2 + 2);
  printw("G : %s", score.at("gate").c_str());
  move(5, col * 2 + 2);
  printw("Seconds : %s", score.at("seconds").c_str());

  // 미션 출력
  move(col / 2 + 1, row * 2 + 2);
  printw("B : %s", mission.at("length").c_str());
  move(col / 2 + 2, row * 2 + 2);
  printw("+ : %s", mission.at("growth").c_str());
  move(col / 2 + 3, row * 2 + 2);
  printw("- : %s", mission.at("poison").c_str());
  move(col / 2 + 4, row * 2 + 2);
  printw("G : %s", mission.at("gate").c_str());
  move(col / 2 + 5, row * 2 + 2);
  printw("Seconds : %s", mission.at("seconds").c_str());

  refresh();  // 실제 스크린에 출력
}

bool drawGameOver(int row, int col) {
  const char* msg = "GAME OVER";
  int msg_len = strlen(msg);
  int win_height = 3;
  int win_width = msg_len + 4;
  int starty = (row - win_height) / 2;     // Window start y position
  int startx = (col * 2 - win_width) / 2;  // Window start x position

  // Create a new window
  WINDOW* win = newwin(win_height, win_width, starty, startx);
  box(win, 0, 0);

  // Print message in the center of the window
  mvwprintw(win, 1, 2, "%s", msg);

  // Refresh the window to show the box and the message
  wrefresh(win);
  while (true) {
    int g = getch();
    if (g == '\n') {
      delwin(win);
      return false;
      break;
    } else if (g == 'r') {
      delwin(win);
      return true;
      break;
    }
  }

  // Clean up
  delwin(win);
}

void drawNextStage(int row, int col) {
  const char* msg = "NEXT STAGE";
  int msg_len = strlen(msg);
  int win_height = 3;
  int win_width = msg_len + 4;
  int starty = (row - win_height) / 2;
  int startx = (col * 2 - win_width) / 2;

  WINDOW* win = newwin(win_height, win_width, starty, startx);
  box(win, 0, 0);

  mvwprintw(win, 1, 2, "%s", msg);

  wrefresh(win);
  while (true) {
    if (getch() == '\n') {
      break;
    }
  }
  delwin(win);
}

void drawGameClear(int row, int col) {
  const char* msg = "CLEAR";
  int msg_len = strlen(msg);
  int win_height = 3;
  int win_width = msg_len + 4;
  int starty = (row - win_height) / 2;
  int startx = (col * 2 - win_width) / 2;

  WINDOW* win = newwin(win_height, win_width, starty, startx);
  box(win, 0, 0);

  mvwprintw(win, 1, 2, "%s", msg);

  wrefresh(win);
  while (true) {
    if (getch() == '\n') {
      break;
    }
  }
  delwin(win);
}

// int main() {
//   int row = 10, col = 10;  // 행, 열 입력
//   // cin >> row >> col;

//   // 배열 초기화
//   int** ary = new int*[row];
//   for (int i = 0; i < row; i++) {
//     ary[i] = new int[col];
//     for (int j = 0; j < col; j++) {
//       ary[i][j] = 0;
//     }
//   }

//   draw(row, col, ary);
// }