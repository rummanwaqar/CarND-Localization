#ifndef TYPES_H
#define TYPES_H

#include <vector>

// represents a single landmark
struct landmark_t {
  int id;     // landmark id
  float x;    // x-position
  float y;    // y-position
};

// represents a single particle state
struct particle_t {
  int id;
  double x;
  double y;
  double theta;
  double weight;
  std::vector<int> associations;
  std::vector<double> sense_x;
  std::vector<double> sense_y;
};



#endif
