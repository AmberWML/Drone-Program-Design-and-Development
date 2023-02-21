#include "src/customer.h"
#include "src/json_helper.h"

namespace csci3081 {

Customer::Customer(const picojson::object& newDetails) {
  // Set the json details
  details_ = newDetails;
  // Initialize the rest of the fields from details_
  InitCust();
}

Customer::Customer() {
  // Set the json details "type" key to "package" so that it can be identified as entity
  details_["type"] = picojson::value("customer");
  // Initialize the rest of the fields from details_
  InitCust();
}

void Customer::InitCust() {
  // Add 'Customer' to the list of entity types
  AddType<Customer>();

  // Set the position and direction vectors, if they exist
  if (JsonHelper::ContainsKey(details_, "position")) {
    const picojson::array& newPosition = JsonHelper::GetArray(details_, "position");
    // For up to 3 elements in the json position array, assign them to
    // the appropriate index of our own position.
    for (int i = 0; i < newPosition.size() && i < 3; i++) {
      position_[i] = static_cast<float>(newPosition[i].get<double>());
    }
  } else {
    // Otherwise default to [0, 0, 0]
    position_[0] = 0;
    position_[1] = 0;
    position_[2] = 0;
  }

  if (JsonHelper::ContainsKey(details_, "direction")) {
    const picojson::array& newDirection = JsonHelper::GetArray(details_, "direction");
    // For up to 3 elements in the json direction array, assign them to
    // the appropriate index of our own direction.
    for (int i = 0; i < newDirection.size() && i < 3; i++) {
      direction_[i] = static_cast<float>(newDirection[i].get<double>());
    }
  } else {
    // Otherwise default to [1, 0, 0]
    direction_[0] = 1;
    direction_[1] = 0;
    direction_[2] = 0;
  }

  // Set the radius, if it exists
  if (JsonHelper::ContainsKey(details_, "radius")) {
    radius_ = static_cast<float>(JsonHelper::GetValue(details_, "radius").get<double>());
  } else {
    // Otherwise default to 1
    radius_ = 1;
  }
}

void Customer::Update(float dt) { }
}  // namespace csci3081
