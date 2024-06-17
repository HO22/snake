#include "../include/gate.h"

Gate::Gate() {}
Gate::Gate(const Position& pos) : position(pos) {
  // determineExitDirection();
}

// void Gate::determineExitDirection() {
//   if (position.x == 0) {
//     exitDirection = Right;
//   } else if (position.x == rowSize - 1) {
//     exitDirection = Left;
//   } else if (position.y == 0) {
//     exitDirection = Down;
//   } else if (position.y == colSize - 1) {
//     exitDirection = Up;
//   } else {
//     switch (exitDirection) {
//       case Up:
//         if (position.y > colSize / 2) {
//           exitDirection = Down;
//         }
//         break;
//       case Right:
//         if (position.x < rowSize / 2) {
//           exitDirection = Left;
//         }
//         break;
//       case Down:
//         if (position.y < colSize / 2) {
//           exitDirection = Up;
//         }
//         break;
//       case Left:
//         if (position.x > rowSize / 2) {
//           exitDirection = Right;
//         }
//         break;
//       default:
//         break;
//     }
//   }
// }

// Position Gate::determineExitPosition(const Position& gate1_pos,
//                                      Direction gate1_exit_dir) {
//   Position exit_pos = position;

//   if (position.x == 0) {
//     exit_pos.x = rowSize - 2;
//   } else if (position.x == rowSize - 1) {
//     exit_pos.x = 1;
//   } else if (position.y == 0) {
//     exit_pos.y = colSize - 2;
//   } else if (position.y == colSize - 1) {
//     exit_pos.y = 1;
//   } else {
//     switch (exitDirection) {
//       case Up:
//         exit_pos.y++;
//         break;
//       case Right:
//         exit_pos.x--;
//         break;
//       case Down:
//         exit_pos.y--;
//         break;
//       case Left:
//         exit_pos.x++;
//         break;
//       default:
//         break;
//     }
//   }

//   return exit_pos;
// }

// Direction Gate::getNextDirection(Direction current_dir, bool clockwise) {
//   if (clockwise) {
//     switch (current_dir) {
//       case Up:
//         return Right;
//       case Right:
//         return Down;
//       case Down:
//         return Left;
//       case Left:
//         return Up;
//       default:
//         return Up;
//     }
//   } else {
//     switch (current_dir) {
//       case Up:
//         return Left;
//       case Left:
//         return Down;
//       case Down:
//         return Right;
//       case Right:
//         return Up;
//       default:
//         return Up;
//     }
//   }
// }
