#include <iostream>
#include "helpers.hpp"
#include "types.hpp"
#include "io.hpp"

const std::string MAP_FILE = "../data/map_data.txt";
const int PORT = 4567;

int main() {
  // Time elapsed between measurements [sec]
  double delta_t = 0.1;
  // Sensor range [m]
  double sensor_range = 50;
  // GPS measurement uncertainty [x [m], y [m], theta [rad]]
  double sigma_pos [3] = {0.3, 0.3, 0.01};
  // Landmark measurement uncertainty [x [m], y [m]]
  double sigma_landmark [2] = {0.3, 0.3};

  // read map data
  std::vector<landmark_t> map;
  try {
    map = read_map(MAP_FILE);
  } catch(std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  }

  std::cout << "Connecting to simulator" << std::endl;
  SimIO simulator(PORT, [&](double sense_x, double sense_y, double sense_theta, double prev_velocity, double prev_yawrate, std::vector<landmark_t> observations) {
    particle_t best_particle;
    best_particle.x = sense_x;
    best_particle.y = sense_y;
    best_particle.theta = sense_theta;
    best_particle.associations = {1,2,3};
    return best_particle;
  });

  simulator.run();

  return 0;
}
