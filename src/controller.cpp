#include "../include/controller.h"

#include <deque>
#include <iostream>

#include "../include/gate.h"
#include "../include/item.h"
#include "../include/position.h"
#include "../include/snake.h"
#include "../include/stage.h"
using namespace std;

Controller::Controller() {
  field = vector<vector<int> >(mapColSize, vector<int>(mapRowSize, 0));
  snake = Snake(SNAKE_MAX_LENGTH, snakeInit());
  itemConsumptionCount[POISON] = 0;
  itemConsumptionCount[GROWTH] = 0;
  gateConsumptionCount = 0;
  stage = 0;
  snakeMaxLength = 5;
  snakeMinLength = 3;
  gateConsumptionMissionCount = 1;
  growthItemConsumptionMissionCount = 1;
  poisonItemConsumptionMissionCount = 1;
  minSeconds = 5;
  setMap();
  createWall();
  createItem(GROWTH);
  createItem(POISON);
  createGate();
  updateMap();
}

deque<Position> Controller::snakeInit(const int startX, const int startY) {
  deque<Position> body;
  body.push_front(Position(startX, startY));
  for (int i = 1; i < snakeInitLength; ++i) {
    vector<Position> n_p;
    bool isEmpty = false;
    Position p = body[0];
    n_p = p.nextPosition();
    vector<Position>::iterator it = n_p.begin();
    for (; it != n_p.end(); it++) {
      isEmpty = isPositionEmpty(*it);

      if (isEmpty) {
        body.push_front(*it);
        break;
      }
    }
    if (!isEmpty) throw "SNAKE INIT POSITION ERROR";
  }
  return body;
}

bool Controller::isPositionValid(const Position p) {
  return p.x >= 0 && p.y >= 0 && p.x < mapRowSize && p.y < mapColSize;
}

bool Controller::isPositionEmpty(const Position p) {
  return isPositionValid(p) && (field[p.x][p.y] == EMPTY_SPACE);
}

bool Controller::isPositionWall(const Position p) {
  return isPositionValid(p) && (field[p.x][p.y] == WALL);
}

int Controller::getPositionKind(const Position p) { return field[p.x][p.y]; }
vector<vector<int> > Controller::getMap() { return field; }
Snake& Controller::getSnake() { return snake; }

void Controller::nextTick() {
  setMap();
  updateMap();
  moveSnake();
  deque<Position> snakeBody = snake.getBody();
  deque<Position>::iterator it2 = snakeBody.begin();
  for (; it2 != snakeBody.end(); it2++) {
    field[it2->x][it2->y] = SNAKE_BODY;
  }
  Position snakeHead = snake.getHead();
  field[snakeHead.x][snakeHead.y] = SNAKE_HEAD;
}

int Controller::getMapRowSize() { return mapRowSize; }
int Controller::getMapColSize() { return mapColSize; }
vector<pair<chrono::steady_clock::time_point, ItemKind> >
Controller::getItemConsumptionTimes() {
  return itemConsumptionTimes;
}

int Controller::getItemConsumptionCount(ItemKind k) {
  return itemConsumptionCount[k];
}

int Controller::getGateConsumptionCount() { return gateConsumptionCount; }

Item Controller::findItemWithPosition(Position p) {
  vector<Item>::iterator it = items.begin();
  for (; it != items.end(); it++) {
    if (it->getPosition() == p) {
      return *it;
    }
  }
  return Item();
}

Gate Controller::findOtherGateWithPosition(Position p) {
  vector<pair<Gate, Gate> >::iterator it = gates.begin();
  for (; it != gates.end(); it++) {
    if (it->first.getPosition() == p) {
      return it->second;
    } else if (it->second.getPosition() == p) {
      return it->first;
    }
  }
  return Gate();
}

void Controller::setSnakeDirection(Direction d) { snake.setDirection(d); }
void Controller::setItemConsumptionTimes(
    vector<pair<chrono::steady_clock::time_point, ItemKind> > i_c_t) {
  itemConsumptionTimes = i_c_t;
}

void Controller::moveSnake() {
  Position head = snake.getHead();
  Direction d = snake.getDirection();
  // cout << d << endl;
  Position n_p = head.nextPosition(d);

  // cout << "<" << n_p.x << " " << n_p.y << ">" << endl;
  bool isEmpty = false;
  isEmpty = isPositionEmpty(n_p);
  if (isEmpty) {
    snake.move(n_p);
  } else {
    int p_k = getPositionKind(n_p);
    Item item;
    Gate gate;
    switch (p_k) {
      case GROWTH_ITEM:
      case POISON_ITEM:
        item = findItemWithPosition(n_p);
        handleItemConsumption(item);
        removeItem(item);
        break;

      case GATE:
        gate = findOtherGateWithPosition(n_p);
        handleGateConsumption(gate);
        break;

      case IMMUNE_WALL:
      case WALL:
        throw GameOverException();
        break;

      default:
        break;
    }
    // GAMEOVER
  }
}

void Controller::createItem(ItemKind k) {
  Position p = Position();
  bool isEmpty = false;
  while (true) {
    p.setRandomPosition(mapRowSize, mapColSize);
    isEmpty = isPositionEmpty(p);

    if (isEmpty) {
      Item item = Item(p, k);
      items.push_back(item);
      break;
    }
  }
}

void Controller::removeItem(Item i) {
  vector<Item>::iterator it = items.begin();
  while (it != items.end()) {
    if (it->getPosition() == i.getPosition()) {
      it = items.erase(it);
    } else {
      ++it;
    }
  }
}

void Controller::incItemConsumptionCount(ItemKind k) {
  itemConsumptionCount[k] += 1;
}
void Controller::incGateConsumptionCount() { gateConsumptionCount += 1; }

void Controller::handleItemConsumption(const Item& i) {
  const Position p = i.getPosition();
  const ItemKind k = i.getKind();
  itemConsumptionTimes.push_back(make_pair(chrono::steady_clock::now(), k));

  incItemConsumptionCount(k);
  switch (k) {
    case GROWTH:
      snake.incBody(p);

      break;
    case POISON:
      snake.decBody(p);
      break;
    default:
      break;
  }
}

Direction rotateRight(Direction d) {
  switch (d) {
    case Up:
      return Right;
    case Right:
      return Down;
    case Down:
      return Left;
    case Left:
      return Up;
  }
}

void Controller::handleGateConsumption(const Gate& g) {
  Position p = g.getPosition();
  Direction d = snake.getDirection();

  Position n_p = p.nextPosition(d);
  Direction n_d = rotateRight(d);

  incGateConsumptionCount();

  if (!isPositionEmpty(n_p)) {
    while (d != n_d) {
      n_p = p.nextPosition(n_d);
      if (isPositionEmpty(n_p)) {
        break;
      }
      n_d = rotateRight(n_d);
    }
  }
  snake.setDirection(n_d);
  snake.move(n_p);
}

Position Controller::getRandomWallPosition() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis_wall(0, walls.size() - 1);
  if (walls.empty()) {
    // throw error;
  }
  int wall_idx = dis_wall(gen);
  return walls[wall_idx];
}

void Controller::createGate() {
  Position p1 = getRandomWallPosition();
  Position p2 = getRandomWallPosition();
  while (p1 == p2) {
    p2 = getRandomWallPosition();
  }
  Gate gate1 = Gate(p1);
  Gate gate2 = Gate(p2);
  gates.push_back(make_pair(gate1, gate2));
}

void Controller::createWall() {
  for (int i = 0; i < mapColSize; ++i) {
    for (int j = 0; j < mapRowSize; ++j) {
      if (field[i][j] == WALL) {
        walls.push_back(Position(i, j));
      }
    }
  }
}

void Controller::updateMap() {
  // // WALL
  // for (int i = 0; i < mapColSize; ++i) {
  //   for (int j = 0; j < mapRowSize; ++j) {
  //     if (i == 0 || i == mapColSize - 1 || j == 0 || j == mapRowSize - 1) {
  //       if ((i == 0 && j == 0) || (i == 0 && j == mapRowSize - 1) ||
  //           (i == mapColSize - 1 && j == 0) ||
  //           (i == mapColSize - 1 && j == mapRowSize - 1)) {
  //         field[i][j] = IMMUNE_WALL;
  //       } else {
  //         field[i][j] = WALL;
  //       }
  //     } else {
  //       field[i][j] = EMPTY_SPACE;
  //     }
  //   }
  // }

  // ITEM
  vector<Item>::iterator it = items.begin();
  for (; it != items.end(); it++) {
    Position p = it->getPosition();
    ItemKind k = it->getKind();
    switch (k) {
      case POISON:
        field[p.x][p.y] = POISON_ITEM;
        break;
      case GROWTH:
        field[p.x][p.y] = GROWTH_ITEM;
        break;
    }
  }

  // GATE
  // cout << "GATE" << endl;
  vector<pair<Gate, Gate> >::iterator it3 = gates.begin();
  for (; it3 != gates.end(); it3++) {
    Gate& gate1 = it3->first;
    Position g_p1 = gate1.getPosition();
    // cout << g_p1.x << " " << g_p1.y << endl;

    field[g_p1.x][g_p1.y] = GATE;
    Gate& gate2 = it3->second;
    Position g_p2 = gate2.getPosition();
    field[g_p2.x][g_p2.y] = GATE;
  }
}

void Controller::setMap() {
  switch (stage) {
    case 0: {
      int f[21][21] = {
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
      };
      for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
          field[i][j] = f[i][j];
        }
      }
      break;
    }
    case 1: {
      int f[21][21] = {
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
      };
      for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
          field[i][j] = f[i][j];
        }
      }
      break;
    }

    case 2: {
      int f[21][21] = {
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
      };
      for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
          field[i][j] = f[i][j];
        }
      }
      break;
    }

    case 3: {
      int f[21][21] = {
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
      };
      for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
          field[i][j] = f[i][j];
        }
      }
      break;
    }

    case 4: {
      int f[21][21] = {
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
      };
      for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
          field[i][j] = f[i][j];
        }
      }
      break;
    }
    default:
      break;
  }

  // createItem(GROWTH);
  // createItem(POISON);
  // createWall();
  // createGate();
  // updateMap();
}

void Controller::nextStage() {
  if (stage >= 5) {
    // 클리어
  } else {
    // field = vector<vector<int> >(mapColSize, vector<int>(mapRowSize, 0));
    snake = Snake(SNAKE_MAX_LENGTH, snakeInit());
    itemConsumptionCount[POISON] = 0;
    itemConsumptionCount[GROWTH] = 0;
    stage += 1;
    snakeMaxLength += 2;
    gateConsumptionMissionCount += 1;
    growthItemConsumptionMissionCount += 1;
    poisonItemConsumptionMissionCount += 1;
    minSeconds += 5;
    items.clear();
    walls.clear();
    gates.clear();
    itemConsumptionTimes.clear();
    setMap();
    createWall();

    // <update map - sanke
    deque<Position> snakeBody = snake.getBody();
    deque<Position>::iterator it2 = snakeBody.begin();
    for (; it2 != snakeBody.end(); it2++) {
      field[it2->x][it2->y] = SNAKE_BODY;
    }
    Position snakeHead = snake.getHead();
    field[snakeHead.x][snakeHead.y] = SNAKE_HEAD;
    // update map - sanke>

    for (int _i = 0; _i <= stage; ++_i) {
      createItem(GROWTH);
      updateMap();
      createItem(POISON);
      updateMap();
      createGate();
      updateMap();
    }
  }
}

// int main() {
//   Controller c;
//   c.nextTick();
// }