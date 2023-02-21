#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"
#include <EntityProject/simple_UMN_route_manager.h>
#include "src/drone_simulation.h"
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"
#include "src/factory.h"
#include "src/drone_coordinator.h"
#include <EntityProject/project_settings.h>

#include <iostream>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class DroneCoordinatorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    droneSimulation = new DroneSimulation();
    picojson::object obj;
    obj["type"] = picojson::value("drone");
    drone = new Drone(obj);
    obj["type"] = picojson::value("customer");
    customer = new Customer(obj);
    obj["type"] = picojson::value("package");
    package = new Package(obj);
  }
  virtual void TearDown() {}

  DroneSimulation* droneSimulation;
  Drone* drone;
  Package* package;
  Customer* customer;
};

TEST_F(DroneCoordinatorTest, DroneCoordinatorGetFreeDrone) {
  DroneCoordinator coord;
  std::vector<entity_project::Entity*> entities;
  entities.push_back(drone);
  ASSERT_NE(coord.GetFreeDrone(entities), nullptr);
}

TEST_F(DroneCoordinatorTest, DroneCoordinatorScheduleDelivery) {
  entity_project::SimpleUMNRouteManager manager;
  std::vector<entity_project::Entity*> entities;
  entities.push_back(drone);

  ASSERT_EQ(package->GetCustomer(), nullptr);
  ASSERT_NE(drone->GetIsBusy(), true);
  ASSERT_NE(package->IsDynamic(), true);

  DroneCoordinator coord;
  picojson::object obj;
  coord.ScheduleDelivery(package, customer, obj, entities);

  ASSERT_NE(package->GetCustomer(), nullptr);
  ASSERT_EQ(drone->GetIsBusy(), true);
  ASSERT_EQ(package->IsDynamic(), true);
}

}  // namespace csci3081
