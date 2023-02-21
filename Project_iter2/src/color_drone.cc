#include "src/color_drone.h"
#include <string>
#include <iomanip>
#include <sstream>

namespace csci3081 {

void ColorDrone::Update(float dt) {
  drone->Update(dt);

  if (drone->GetIsBusy()) {
    position_ = drone->GetPosition();
    // Offset the y position by +12, so that the drone appears to float like normal
    // drones do. It is unclear why ColorDrones render sunk into the ground
    position_[1] += 11;
    direction_ = drone->GetDirection();
    details_ = drone->GetDetails();
    int index = drone->GetRouteIndex();

    float g = (static_cast<float>(index) / static_cast<float>(routeSize)) * 255;
    int green = static_cast<int>(g);
    int red = 255 - green;

    std::stringstream gStream;
    std::stringstream rStream;
    rStream << std::setfill('0') << std::setw(2) << std::hex << red;
    std::string redString = rStream.str();
    gStream << std::setfill('0') << std::setw(2) << std::hex << green;  // stores as hex num
    std::string greenString = gStream.str();
    std::string colorString;


    colorString = "0x" + redString + greenString + "00";

    details_["color"] = picojson::value(colorString);
    NotifyObservers("updateDetails", this);
  }
}
}  // namespace csci3081
