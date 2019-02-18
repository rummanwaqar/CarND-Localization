#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#include "types.hpp"

class ParticleFilter {
 public:
  /*
   * Constructor
   * @param num_particles Number of particles
   */
  ParticleFilter(int num_particles) :
    num_particles_(num_particles), is_initialized_(false) {}

  /*
   * Destructor
   */
  ~ParticleFilter() = default;

  /**
   * init Initializes particle filter by initializing particles to Gaussian
   *   distribution around first position and all the weights to 1.
   * @param x Initial x position [m] (simulated estimate from GPS)
   * @param y Initial y position [m]
   * @param theta Initial orientation [rad]
   * @param std[] Array of dimension 3 [standard deviation of x [m],
   *   standard deviation of y [m], standard deviation of yaw [rad]]
   */
  void init(double x, double y, double theta, double std[]);

  /**
   * initialized returns whether particle filter is initialized yet or not.
   */
  const bool initialized() const {
    return is_initialized_;
  }


 private:
  // Number of particles to draw
  int num_particles_;

  // Flag, if filter is initialized
  bool is_initialized_;
};


#endif
