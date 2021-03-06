#include "particle_filter.hpp"
#include <iostream>
#include <helpers.hpp>

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  // create gaussian distributions
  std::normal_distribution<double> dist_x(x, std[0]);
  std::normal_distribution<double> dist_y(y, std[1]);
  std::normal_distribution<double> dist_t(theta, std[2]);

  // create N particles using gaussian distribution for initialization
  for(int i=0; i<num_particles_; i++) {
    particle_t p;
    p.id = i;
    p.x = dist_x(gen_);
    p.y = dist_y(gen_);
    p.theta = dist_t(gen_);
    p.weight = 1;
    particles_.push_back(p);
  }
  is_initialized_ = true;
}

void ParticleFilter::prediction(double delta_t, double velocity, double yaw_rate, double std[]) {
  // define normal distributions for sensor noise
  std::normal_distribution<double> noise_x(0.0, std[0]);
  std::normal_distribution<double> noise_y(0.0, std[1]);
  std::normal_distribution<double> noise_t(0.0, std[2]);

  for(int i=0; i<num_particles_; i++) {
    if(std::abs(yaw_rate) > 0.00001) { // non-zero yaw rate
      particles_[i].theta += yaw_rate * delta_t;
      particles_[i].x += velocity / yaw_rate * (std::sin(particles_[i].theta + yaw_rate * delta_t) -
                                                std::sin(particles_[i].theta));

      particles_[i].y += velocity / yaw_rate * (std::cos(particles_[i].theta) -
                                                std::cos(particles_[i].theta + yaw_rate * delta_t));
    } else { // zero yaw rate
      particles_[i].x += velocity * delta_t * std::cos(particles_[i].theta);
      particles_[i].y += velocity * delta_t * std::sin(particles_[i].theta);
    }
    particles_[i].x += noise_x(gen_);
    particles_[i].y += noise_y(gen_);
    particles_[i].theta = std::fmod(particles_[i].theta + noise_t(gen_), 2*M_PI);
  }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[],
                                   const std::vector<landmark_t> &observations,
                                   const std::vector<landmark_t> &map_landmarks) {
  for(auto& p : particles_) {
    // get all landmarks within range
    std::vector<landmark_t> predictions;
    for(auto const& landmark : map_landmarks) {
      if(dist(landmark.x, landmark.y, p.x, p.y) <= sensor_range) {
        predictions.push_back(landmark);
      }
    }

    // convert observations from vehicle to global coods
    std::vector<landmark_t> map_observations;
    for(auto const& obs : observations) {
      // 2D transformation matrix with particle theta and position
      double t_x = std::cos(p.theta) * obs.x - std::sin(p.theta) * obs.y + p.x;
      double t_y = std::sin(p.theta) * obs.x + std::cos(p.theta) * obs.y + p.y;
      map_observations.push_back(landmark_t{obs.id, t_x, t_y});
    }

    // perform associations between landmarks and observations
    // updates map_observations with associations
    dataAssociation(predictions, map_observations);

    // update association debug structures
    p.associations.clear();
    p.sense_x.clear();
    p.sense_y.clear();
    for(auto const& obs : map_observations) {
      p.associations.push_back(obs.id);
      p.sense_x.push_back(obs.x);
      p.sense_y.push_back(obs.y);
    }

    // calculate weights
    p.weight = 1.0;
    for(auto const& obs : map_observations) {
      // get associated prediction
      landmark_t prediction{};
      for(auto const& pred : predictions) {
        if(pred.id == obs.id) {
          prediction = pred;
        }
      }
      // update weight
      p.weight *= gaussian2d(obs.x, obs.y, prediction.x, prediction.y, std_landmark[0], std_landmark[1]);
    }
  }
}

void ParticleFilter::dataAssociation(std::vector<landmark_t> predicted, std::vector<landmark_t> &observations) {
  for(auto& obs : observations) {
    double minimum_dist = std::numeric_limits<double>::max();
    int id = -1;
    for(size_t i=0; i<predicted.size(); i++) {
      double d = dist(obs.x, obs.y, predicted[i].x, predicted[i].y);
      if(d < minimum_dist) {
        minimum_dist = d;
        id = predicted[i].id;
      }
    }
    obs.id = id;
  }
}

void ParticleFilter::resample() {
  // using resampling wheel method
  std::vector<particle_t> new_particles;

  std::vector<double> weights;
  for(auto const& p : particles_) {
    weights.push_back(p.weight);
  }
  double max_weight = *max_element(weights.begin(), weights.end());
  std::uniform_real_distribution<double> uniformdist_weight(0.0, 2 * max_weight);

  // generate random starting index for resampling wheel
  std::uniform_int_distribution<int> uniformdist_index(0, num_particles_-1);
  auto index = uniformdist_index(gen_);

  double beta = 0;
  for(int i=0; i<num_particles_; i++) {
    beta += uniformdist_weight(gen_);
    while(weights[index] < beta) {
      beta -= weights[index];
      index = (index + 1) % num_particles_;
    }
    new_particles.push_back(particles_[index]);
  }
  particles_ = new_particles;
}

particle_t ParticleFilter::get_best_particle() {
  double highest_weight = -1.0;
  particle_t best_particle{};
  double weight_sum = 0.0;
  for (auto const& p : particles_) {
    if (p.weight > highest_weight) {
      highest_weight = p.weight;
      best_particle = p;
    }
    weight_sum += p.weight;
  }
  return best_particle;
}

double ParticleFilter::weighted_error(double gt_x, double gt_y, double gt_theta) {
  double error_sum = 0;
  double weight_sum = 0;
  for(auto const& p : particles_) {
    error_sum += p.weight * getError(gt_x, gt_y, gt_theta, p.x, p.y, p.theta);
    weight_sum += p.weight;
  }
  return error_sum/weight_sum;
}




