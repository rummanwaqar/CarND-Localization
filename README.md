# Localization with Particle Filter

[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)

<img src="output.gif" width="480" alt="Output" />

## Overview
This project implements a 2 dimensional particle filter in C++. We are given a map of the world, a (noisy) GPS estimate of its initial location, and lots of (noisy) sensor and control data.

## Dependencies
* [Simulator](https://github.com/udacity/self-driving-car-sim/releases)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
* cmake
* gcc/g++
* make
* openssl
* libuv
* zlib

## Build Instructions
1. Clone this repo.
2. Make a build directory: `mkdir build && cd $_`
3. Compile: `cmake .. && make`

## Run
`./run.sh`

---

### Map
`data/map_data.txt` includes the position of landmarks (in meters) on an arbitrary Cartesian coordinate system. Each row has three columns

1. x position
2. y position
3. landmark id
