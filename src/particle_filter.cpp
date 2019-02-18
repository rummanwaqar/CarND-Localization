#include "particle_filter.hpp"
#include <iostream>

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  // create gaussian distributions
  std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
  std::normal_distribution<double> dist_x(x, std[0]);
  std::normal_distribution<double> dist_y(y, std[1]);
  std::normal_distribution<double> dist_t(theta, std[2]);

  // create N particles using gaussian distribution for initialization
  for(int i=0; i<num_particles_; i++) {
    particle_t p;
    p.id = i;
    p.x = dist_x(gen);
    p.y = dist_y(gen);
    p.theta = dist_t(gen);
    p.weight = 1;
    particles_.push_back(p);
  }
  is_initialized_ = true;
}

void ParticleFilter::prediction(double delta_t, double velocity, double yaw_rate, double std_pos[]) {
  if(std::abs(yaw_rate) < 0.001) { // constant yaw_rate
    
  } else { // variable yaw_rate

  }

}
