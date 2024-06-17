#ifndef DRAW_H
#define DRAW_H
#include <ncurses.h>

#include <cstring>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <vector>
using namespace std;

void drawGame(int row, int col, vector<vector<int> > ary,
              const map<string, string>& score,
              const map<string, string>& mission);

bool drawGameOver(int row, int col);
void drawGameClear(int row, int col);
void drawNextStage(int row, int col);

#endif