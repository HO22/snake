#include <ncurses.h>

#include <chrono>
#include <map>
#include <string>
#include <thread>

#include "../include/controller.h"
#include "../include/draw.h"

int main() {
  initscr();  // ncurses 초기화
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  timeout(0);  // 비차단 입력 모드 설정

  Controller c;
  int lastInput = ERR;  // 마지막 입력을 저장할 변수, 초기값은 ERR로 설정
  std::chrono::steady_clock::time_point lastMoveTime =
      std::chrono::steady_clock::now();
  std::chrono::steady_clock::time_point stageStartTime =
      std::chrono::steady_clock::now();

  while (true) {
    try {
      std::chrono::steady_clock::time_point currentTime =
          std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed =
          std::chrono::duration_cast<std::chrono::duration<double> >(
              currentTime - lastMoveTime);

      // 1초 동안 키 입력을 처리
      while (elapsed.count() < 0.5) {
        int ch = getch();
        if (ch != ERR) {
          lastInput = ch;  // 최신 입력 저장
        }

        currentTime = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::duration<double> >(
            currentTime - lastMoveTime);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(50));  // CPU 점유율을 낮추기 위해 대기
      }

      vector<pair<chrono::steady_clock::time_point, ItemKind> >
          itemConsumptionTimes = c.getItemConsumptionTimes();
      vector<pair<chrono::steady_clock::time_point, ItemKind> >::iterator it =
          itemConsumptionTimes.begin();

      while (it != itemConsumptionTimes.end()) {
        if (std::chrono::duration_cast<std::chrono::duration<double> >(
                lastMoveTime - it->first)
                .count() > 2) {
          c.createItem(it->second);
          it = itemConsumptionTimes.erase(it);
        } else {
          ++it;
        }
      }
      c.setItemConsumptionTimes(itemConsumptionTimes);

      // 마지막 입력 처리
      if (lastInput != ERR) {
        Snake& snake = c.getSnake();  // `Controller`의 뱀 객체 참조 사용
        Direction d = snake.getDirection();

        switch (lastInput) {
          case KEY_UP:
            if (snake.getDirection() != Right)
              c.setSnakeDirection(Left);  // 올바른 방향으로 설정
            else
              throw GameOverException();
            break;
          case KEY_RIGHT:
            if (snake.getDirection() != Up)
              c.setSnakeDirection(Down);  // 올바른 방향으로 설정
            else
              throw GameOverException();
            break;
          case KEY_DOWN:
            if (snake.getDirection() != Left)
              c.setSnakeDirection(Right);  // 올바른 방향으로 설정
            else
              throw GameOverException();
            break;
          case KEY_LEFT:
            if (snake.getDirection() != Down)
              c.setSnakeDirection(Up);  // 올바른 방향으로 설정
            else
              throw GameOverException();
            break;
          default:
            break;
        }
        lastInput = ERR;  // 입력 처리 후 초기화
      }

      // 뱀 이동 및 맵 업데이트
      c.nextTick();
      std::vector<std::vector<int> > ary = c.getMap();
      std::map<std::string, std::string> score;
      std::map<std::string, std::string> mission;
      score["length"] = std::to_string(c.getSnake().getLength()) + " / " +
                        std::to_string(c.getSnakeMaxLegth());
      score["growth"] = std::to_string(c.getItemConsumptionCount(GROWTH));
      score["poison"] = std::to_string(c.getItemConsumptionCount(POISON));
      score["gate"] = std::to_string(c.getGateConsumptionCount());
      score["seconds"] = std::to_string(
          (std::chrono::duration_cast<std::chrono::duration<int> >(
               currentTime - stageStartTime)
               .count()));

      if (c.getSnakeMaxLegth() > c.getSnake().getLength()) {
        mission["length"] = std::to_string(c.getSnakeMaxLegth()) + " (X)";
      } else {
        mission["length"] = std::to_string(c.getSnakeMaxLegth()) + " (O)";
      }

      if (c.getGrowthItemConsumptionMissionCount() >
          c.getItemConsumptionCount(GROWTH)) {
        mission["growth"] =
            std::to_string(c.getGrowthItemConsumptionMissionCount()) + " (X)";
      } else {
        mission["growth"] =
            std::to_string(c.getGrowthItemConsumptionMissionCount()) + " (O)";
      }

      if (c.getGrowthItemConsumptionMissionCount() >
          c.getItemConsumptionCount(POISON)) {
        mission["poison"] =
            std::to_string(c.getGrowthItemConsumptionMissionCount()) + " (X)";
      } else {
        mission["poison"] =
            std::to_string(c.getGrowthItemConsumptionMissionCount()) + " (O)";
      }

      if (c.getGateConsumptionMissionCount() > c.getGateConsumptionCount()) {
        mission["gate"] =
            std::to_string(c.getGateConsumptionMissionCount()) + " (X)";
      } else {
        mission["gate"] =
            std::to_string(c.getGateConsumptionMissionCount()) + " (O)";
      }

      if (c.getMinSeconds() >
          std::chrono::duration_cast<std::chrono::duration<int> >(
              currentTime - stageStartTime)
              .count()) {
        mission["seconds"] = std::to_string(c.getMinSeconds()) + " (X)";
      } else {
        mission["seconds"] = std::to_string(c.getMinSeconds()) + " (O)";
      }
      drawGame(c.getMapRowSize(), c.getMapColSize(), ary, score, mission);

      // 마지막 이동 시간 갱신
      lastMoveTime = std::chrono::steady_clock::now();
      if (c.getSnake().getLength() < c.getSnakeMinLegth()) {
        throw GameOverException();
      }
      if ((c.getSnakeMaxLegth() <= c.getSnake().getLength()) &&
          (c.getGrowthItemConsumptionMissionCount() <=
           c.getItemConsumptionCount(GROWTH)) &&
          (c.getGrowthItemConsumptionMissionCount() <=
           c.getItemConsumptionCount(POISON)) &&
          (c.getGateConsumptionMissionCount() <= c.getGateConsumptionCount()) &&
          (c.getMinSeconds() <=
           std::chrono::duration_cast<std::chrono::duration<int> >(
               currentTime - stageStartTime)
               .count())) {
        if (c.getStage() == 4) {
          drawGameClear(c.getMapRowSize(), c.getMapColSize());
          break;
        } else {
          drawNextStage(c.getMapRowSize(), c.getMapColSize());
          c.nextStage();
        }
        stageStartTime = std::chrono::steady_clock::now();
      }
    } catch (const GameOverException& e) {
      if (!drawGameOver(c.getMapRowSize(), c.getMapColSize())) {
        break;
      }
    }
  }

  endwin();  // ncurses 종료
  return 0;
}

// int main() {
//   initscr();
//   start_color();
//   init_pair(1, COLOR_GREEN, COLOR_BLACK);
//   init_pair(2, COLOR_RED, COLOR_BLACK);

//   Snake snake;  // Create an instance of Snake

//   endwin();  // End NCurses
//   return 0;
// }
