#ifndef HELPERS_H
#define HELPERS_H

#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>

#include "types.hpp"

/**
 * Computes the Euclidean distance between two 2D points.
 * @param (x1,y1) x and y coordinates of first point
 * @param (x2,y2) x and y coordinates of second point
 * @output Euclidean distance between two 2D points
 */
inline double dist(double x1, double y1, double x2, double y2) {
  return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

/**
 * Calculates 2D gaussian probability
 */
inline double gaussian2d(double x, double y, double ux, double uy, double sigmax, double sigmay) {
  return (std::exp(-((x - ux) * (x - ux) / (2 * sigmax * sigmax) + (y - uy) * (y - uy) / (2 * sigmay * sigmay))))
         / (2 * M_PI * sigmax * sigmay);
}

/**
 * Reads map data from a file.
 * @param filename Name of file containing map data.
 * @output map as a vector of landmark_t
 */
inline std::vector<landmark_t> read_map(std::string filename) {
  std::vector<landmark_t> map;
  std::ifstream in_file(filename.c_str(), std::ifstream::in);
  if(!in_file) {
    throw std::runtime_error("Map file not found.");
  }

  // run over each line
  std::string line;
  while(getline(in_file, line)) {
    std::istringstream iss(line);

    landmark_t single_landmark;
    // read data from current line
    iss >> single_landmark.x;
    iss >> single_landmark.y;
    iss >> single_landmark.id;

    map.push_back(single_landmark);
  }
  return map;
}

/*
 * Converts a space seperated string to vector of floats
 */
inline std::vector<float> string_to_vec(std::string str) {
  std::vector<float> vec;
  std::istringstream iss(str);
  std::copy(std::istream_iterator<float>(iss), std::istream_iterator<float>(), std::back_inserter(vec));
  return vec;
}

/*
 * Convert a vector to space seperated string
 */
template <class T>
inline std::string vec_to_string(std::vector<T> v) {
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<T>(ss, " "));
  std::string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

#endif
