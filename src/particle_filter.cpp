#include "particle_filter.hpp"
#include <iostream>

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  std::cout << "initialized" << std::endl;
  is_initialized_ = true;
}
