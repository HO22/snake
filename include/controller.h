#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

#include "gate.h"
#include "item.h"
#include "snake.h"
#include "stage.h"

#define A_MISSION 10
#define B_MISSION 10
#define C_MISSION 10
#define D_MISSION 10
#define SNAKE_INIT_LENGTH 3
#define SNAKE_MAX_LENGTH 15
#define MIN_MAP_COL_SIZE 21
#define MIN_MAP_ROW_SIZE 21
#define SNAKE_START_X 4
#define SNAKE_START_Y 4

#define EMPTY_SPACE 0
#define WALL 1
#define IMMUNE_WALL 2
#define SNAKE_HEAD 3
#define SNAKE_BODY 4
#define GROWTH_ITEM 5
#define POISON_ITEM 6
#define GATE 7

// #define KEY_UP 260
// #define KEY_DOWN 258
// #define KEY_LEFT 259
// #define KEY_RIGHT 261

// enum PositionKind {
//   EMPTY_SPACE,
//   WALL,
//   IMMUNE_WALL,
//   SNAKE_HEAD,
//   SNAKE_BODY,
//   GROWTH_ITEM,
//   POISON_ITEM,
//   GATE,
// };

class GameOverException : public std::exception {};

class Controller {
 public:
  Controller();

  deque<Position> snakeInit(const int startX = SNAKE_START_X,
                            const int startY = SNAKE_START_Y);
  bool isPositionEmpty(const Position p);
  bool isPositionWall(const Position p);
  bool isPositionValid(const Position p);

  int getPositionKind(const Position p);
  vector<vector<int> > getMap();
  Snake& getSnake();
  int getMapColSize();
  int getMapRowSize();
  Position getRandomWallPosition();
  vector<pair<chrono::steady_clock::time_point, ItemKind> >
  getItemConsumptionTimes();
  int getItemConsumptionCount(ItemKind k);
  int getGateConsumptionCount();

  Item findItemWithPosition(const Position p);
  Gate findOtherGateWithPosition(const Position p);
  void setSnakeDirection(Direction d);
  void setItemConsumptionTimes(
      vector<pair<chrono::steady_clock::time_point, ItemKind> > i_c_t);
  void setMap();
  void mapGenerator();
  void nextTick();
  void moveSnake();
  void createItem(ItemKind k);
  void handleItemConsumption(const Item& i);
  void handleGateConsumption(const Gate& g);
  void createGate();
  void createWall();
  void updateMap();
  void removeItem(Item i);
  void incItemConsumptionCount(ItemKind k);
  void incGateConsumptionCount();
  int getSnakeMaxLegth() { return snakeMaxLength; };
  int getSnakeMinLegth() { return snakeMinLength; };
  int getGateConsumptionMissionCount() { return gateConsumptionMissionCount; };
  int getGrowthItemConsumptionMissionCount() {
    return growthItemConsumptionMissionCount;
  };
  int getPoisonItemConsumptionMissionCount() {
    return poisonItemConsumptionMissionCount;
  };
  int getMinSeconds() { return minSeconds; };
  int getStage() { return stage; };
  void nextStage();

 private:
  // stage별 map, mission, tick 관리
  // Stage stage;
  // map<string, int> score;
  Snake snake;
  // Gate gate;
  // Item item;
  vector<Item> items;
  static const int snakeInitLength = SNAKE_INIT_LENGTH;
  vector<vector<int> > field;
  vector<pair<Gate, Gate> > gates;
  vector<Position> walls;
  vector<pair<chrono::steady_clock::time_point, ItemKind> >
      itemConsumptionTimes;
  map<ItemKind, int> itemConsumptionCount;
  int gateConsumptionCount;
  int stage;
  int snakeMaxLength;
  int snakeMinLength;
  int gateConsumptionMissionCount;
  int growthItemConsumptionMissionCount;
  int poisonItemConsumptionMissionCount;
  int minSeconds;
  static const int mapColSize = MIN_MAP_COL_SIZE;
  static const int mapRowSize = MIN_MAP_ROW_SIZE;
};