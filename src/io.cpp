#include "io.hpp"

SimIO::SimIO(int port, ProcessCb cb) : port_(port), callbackFunc_(cb) {
  /*
   * Register event handlers for uWS
   */
  h_.onMessage([&](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      std::string s = hasData(std::string(data));

      if(s != "") { // data available
        // parse json
        auto j = nlohmann::json::parse(s);
        std::string event = j[0].get<std::string>();

        if(event == "telemetry") {
          // Sense noisy position data from the simulator (used for init)
          double sense_x = std::stod(j[1]["sense_x"].get<std::string>());
          double sense_y = std::stod(j[1]["sense_y"].get<std::string>());
          double sense_theta = std::stod(j[1]["sense_theta"].get<std::string>());
          // Predict the vehicle's next state from previous (noiseless control) data.
          double previous_velocity = std::stod(j[1]["previous_velocity"].get<std::string>());
          double previous_yawrate = std::stod(j[1]["previous_yawrate"].get<std::string>());
          // noisy observation data from the simulator
          std::vector<float> x_sense = string_to_vec(j[1]["sense_observations_x"]);
          std::vector<float> y_sense = string_to_vec(j[1]["sense_observations_y"]);
          assert(x_sense.size() == y_sense.size());
          std::vector<landmark_t> noisy_observations;
          for(int i = 0; i < x_sense.size(); ++i) {
            landmark_t obs;
            obs.x = x_sense[i];
            obs.y = y_sense[i];
            noisy_observations.push_back(obs);
          }

          // process
          particle_t best_particle = callbackFunc_(sense_x, sense_y, sense_theta, previous_velocity, previous_yawrate, std::move(noisy_observations));

          // send output
          nlohmann::json msgJson;
          msgJson["best_particle_x"] = best_particle.x;
          msgJson["best_particle_y"] = best_particle.y;
          msgJson["best_particle_theta"] = best_particle.theta;
          // Optional message data used for debugging particle's sensing and associations
          msgJson["best_particle_associations"] = vec_to_string(best_particle.associations);
          msgJson["best_particle_sense_x"] = vec_to_string(best_particle.sense_x);
          msgJson["best_particle_sense_y"] = vec_to_string(best_particle.sense_y);
          auto msg = "42[\"best_particle\"," + msgJson.dump() + "]";
          // std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  h_.onConnection([](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h_.onDisconnection([](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });
}

void SimIO::run() {
  // listen and wait for connection
  if (h_.listen(port_)) {
    std::cout << "Listening to port " << port_ << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return;
  }
  // endless loop until application exists
  h_.run();
}

std::string SimIO::hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_first_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}
