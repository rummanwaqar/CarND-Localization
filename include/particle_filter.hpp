#ifndef PARTICLE_FILTER_H
#define PARTICLE_FILTER_H

#include <random>
#include <cmath>
#include <limits>
#include <algorithm>
#include "json.h"

#include "types.hpp"

class ParticleFilter {
 public:
  /*
   * Constructor
   * @param num_particles Number of particles
   */
  explicit ParticleFilter(int num_particles) :
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
   * prediction Predicts the state for the next time step
   *   using the process model.
   * @param delta_t Time between time step t and t+1 in measurements [s]
   * @param std_pos[] Array of dimension 3 [standard deviation of x [m],
   *   standard deviation of y [m], standard deviation of yaw [rad]]
   * @param velocity Velocity of car from t to t+1 [m/s]
   * @param yaw_rate Yaw rate of car from t to t+1 [rad/s]
   */
  void prediction(double delta_t, double velocity, double yaw_rate, double std[]);

  /**
   * dataAssociation Finds which observations correspond to which landmarks
   *   (by using a nearest-neighbors data association).
   * @param predicted Vector of predicted landmark observations (map)
   * @param observations Vector of landmark observations
   */
  void dataAssociation(std::vector<landmark_t> predicted,
                       std::vector<landmark_t>& observations);

  /**
   * updateWeights Updates the weights for each particle based on the likelihood
   *   of the observed measurements.
   * @param sensor_range Range [m] of sensor
   * @param std_landmark[] Array of dimension 2
   *   [Landmark measurement uncertainty [x [m], y [m]]]
   * @param observations Vector of landmark observations
   * @param map Vector containing map landmarks
   */
  void updateWeights(double sensor_range, double std_landmark[],
                     const std::vector<landmark_t> &observations,
                     const std::vector<landmark_t> &map_landmarks);

  /**
   * resamples from the updated set of particles to form
   *   the new set of particles.
   */
  void resample();

  /*
   * returns the current particle state as json string
   */
  std::string particles_json();

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

  // Set of current particles
  std::vector<particle_t> particles_;

  // generator for random distributions
  std::default_random_engine gen_;
};


#endif
