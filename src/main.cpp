#include <iostream>

#include "types.hpp"
#include "io.hpp"
#include "particle_filter.hpp"

const std::string MAP_FILE = "../data/map_data.txt";
const int PORT = 4567;

int main() {
  // read map data
  std::vector<landmark_t> map;
  try {
    map = read_map(MAP_FILE);
  } catch(std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  // Time elapsed between measurements [sec]
  double delta_t = 0.1;
  // Sensor range [m]
  double sensor_range = 50;
  // GPS measurement uncertainty [x [m], y [m], theta [rad]]
  double sigma_pos [3] = {0.3, 0.3, 0.01};
  // Landmark measurement uncertainty [x [m], y [m]]
  double sigma_landmark [2] = {0.3, 0.3};
  // number of particles
  int num_particles = 100;

  // create particle filter
  ParticleFilter particle_filter(num_particles);

  std::cout << "Connecting to simulator" << std::endl;
  SimIO simulator(PORT, [&](double sense_x, double sense_y, double sense_theta, double prev_velocity, double prev_yawrate, std::vector<landmark_t> observations) {
    if(!particle_filter.initialized()) {
      // if not initialized, initialize with GPS data
      particle_filter.init(sense_x, sense_y, sense_theta, sigma_pos);
    } else {
      // run the prediction step
      particle_filter.prediction(delta_t, prev_velocity, prev_yawrate, sigma_pos);
    }

    // Update the weights and resample
    particle_filter.updateWeights(sensor_range, sigma_landmark, observations, map);
    particle_filter.resample();

    particle_t best_particle = particle_filter.get_best_particle();
    return best_particle;
  });

  simulator.run();

  return 0;
}
