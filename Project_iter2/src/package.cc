#include <EntityProject/simple_UMN_route_manager.h>
#include <EntityProject/entity_observer.h>
#include "src/package.h"
#include "src/json_helper.h"
#include "src/entity_isubject.h"
#include <vector>
#include <string>
#include <iostream>

namespace csci3081 {

Package::Package(const picojson::object& newDetails) {
  // Set the json details
  details_ = newDetails;
  // Initialize the rest of the fields from details_
  InitPack();
}

Package::Package() {
  // Set the json details "type" key to "package" so that it can be identified as entity
  details_["type"] = picojson::value("package");
  // Initialize the rest of the fields from details_
  InitPack();
}

void Package::InitPack() {
  // Add 'Package' to the list of entity types
  AddType<Package>();

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

  // Set the weight, if it exists
  if (JsonHelper::ContainsKey(details_, "weight")) {
    weight = static_cast<float>(JsonHelper::GetValue(details_, "weight").get<double>());
  } else {
    // Otherwise default to 1
    weight = 1;
  }

  // Set status to "not delivered" and "not scheduled"
  isDelivered = false;
  isScheduled = false;
}

bool Package::CheckPosition(entity_project::Entity* other) {
  // If the other is null, return false
  if (other == nullptr) {
    return false;
  }

  // Get our position and the other's position as SimpleUMNRoutePoints
  entity_project::SimpleUMNRoutePoint thisPos =
      entity_project::SimpleUMNRoutePoint(GetPosition()[0], GetPosition()[1], GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint otherPos =
      entity_project::SimpleUMNRoutePoint(other->GetPosition()[0], other->GetPosition()[1],
      other->GetPosition()[2]);

  // If the distance to the other is less than our radii combined, we are close to it
  return thisPos.DistanceBetween(otherPos) <= GetRadius() + other->GetRadius();
}

void Package::SetPosition(float x, float y, float z) {
  // Set the position
  position_[0] = x;
  position_[1] = y;
  position_[2] = z;
}

void Package::Update(float dt) {
  if (CheckPosition(customer)) {
    // If we're within pickup range of the customer, set our status to 'delivered'
    isDelivered = true;
    NotifyObservers("delivered", this);
  }
}

void Package::AddObserver(entity_project::EntityObserver* observer) {
  observers.push_back(observer);
}

void Package::RemoveObserver(entity_project::EntityObserver* observer) {
  for (int i = 0; i < observers.size(); i++) {
    if (observers.at(i) == observer) {
      observers.erase(observers.begin() + i);
    }
  }
}

void Package::NotifyObservers(std::string task, entity_project::Entity *entity) {
  const Entity &package = *entity;
  picojson::object obj;
  obj["type"] = picojson::value("notify");
  obj["value"] = picojson::value(task);
  const picojson::value& constObj = picojson::value(obj);
  for (int i = 0; i < observers.size(); i++) {
    observers.at(i)->OnEvent(constObj, package);
  }
}

}  // namespace csci3081
