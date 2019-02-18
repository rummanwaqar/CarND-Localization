#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <fstream>
#include <sstream>
#include <exception>

#include "types.hpp"

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

#endif
