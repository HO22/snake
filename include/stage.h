#ifndef STAGE_H
#define STAGE_H

#include <iostream>
#include <map>
#include <string>

class Stage {
 public:
  Stage(int _stage = 1);

 private:
  map<string, int> mission;
};

#endif