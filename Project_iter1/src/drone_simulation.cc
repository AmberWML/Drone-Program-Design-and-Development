#include "src/drone_simulation.h"
#include <iostream>

namespace csci3081 {

DroneSimulation::~DroneSimulation() {
  for (auto iter : entities_) {
    delete iter;
  }
}

void DroneSimulation::AddObserver(entity_project::Entity* entity,
    entity_project::EntityObserver* observer) {
  if (coord.GetType(entity) == "drone") {
    // call drone AddObserver
    Drone *drone = entity->AsType<Drone>();
    drone->AddObserver(observer);
  } else if (coord.GetType(entity) == "package") {
    // call package AddObserver
    Package *package = entity->AsType<Package>();
    package->AddObserver(observer);
  }
}

void DroneSimulation::RemoveObserver(entity_project::Entity* entity,
    entity_project::EntityObserver* observer) {
  if (coord.GetType(entity) == "drone") {
    // call drone RemoveObserver
    Drone *drone = entity->AsType<Drone>();
    drone->RemoveObserver(observer);
  } else if (coord.GetType(entity) == "package") {
    // call package RemoveObserver
    Package *package = entity->AsType<Package>();
    package->RemoveObserver(observer);
  }
}

void DroneSimulation::Update(float dt) {
  // Iterate over the entire list of entities

  for (int i = 0; i < entities_.size(); i++) {
    // Cast and Update each entity, checking its type to know how to cast it
    if (coord.GetType(entities_[i]) == "drone") {
      Drone* drone = entities_[i]->AsType<Drone>();
      drone->Update(dt);

      // If the drone is not busy, assign the next package to the drone.
      if (!drone->GetIsBusy()) {
        coord.AssignNextPackageToDrone(drone);
      }
    } else if (coord.GetType(entities_[i]) == "customer") {
      entities_[i]->AsType<Customer>()->Update(dt);
    } else if (coord.GetType(entities_[i]) == "package" &&
        entities_[i]->AsType<Package>()->IsDynamic()) {
      entities_[i]->AsType<Package>()->Update(dt);

      // For packages, if they've been delivered, free their memory,
      // and erase them from the entities list
      if (entities_[i]->AsType<Package>()->GetIsDelivered()) {
        delete entities_[i];
        entities_.erase(entities_.begin() + i);
      }
    }
  }
}
}  // namespace csci3081
