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

class DroneTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    droneSimulation = new DroneSimulation();
    picojson::object obj;
    obj["type"] = picojson::value("package");
    Entity* entity = droneSimulation->CreateEntity(obj);
    package = entity->AsType<Package>();
    droneSimulation->AddEntity(package);
    obj["type"] = picojson::value("drone");
    entity = droneSimulation->CreateEntity(obj);
    drone = entity->AsType<Drone>();
    droneSimulation->AddEntity(drone);
  }
  virtual void TearDown() {}

  DroneSimulation* droneSimulation;
  Drone* drone;
  Package* package;
};

TEST_F(DroneTest, SetPackage) {
  ASSERT_EQ(drone->GetIsBusy(), false);
  drone->SetPackage(package);
  ASSERT_EQ(drone->GetIsBusy(), true);
}

TEST_F(DroneTest, Update) {
  std::vector<entity_project::SimpleUMNRoutePoint> route;
  route.push_back(entity_project::SimpleUMNRoutePoint(1, 1, 1));
  route.push_back(entity_project::SimpleUMNRoutePoint(2, 2, 2));
  route.push_back(entity_project::SimpleUMNRoutePoint(3, 3, 3));
  route.push_back(entity_project::SimpleUMNRoutePoint(4, 4, 4));
  route.push_back(entity_project::SimpleUMNRoutePoint(5, 5, 5));
  picojson::object obj;
  drone->SetPackage(package);
  drone->SetRoute(route);
  drone->Update(0.02);
  ASSERT_NE(drone->GetPosition()[0], 0);
  ASSERT_NE(drone->GetPosition()[1], 0);
  ASSERT_NE(drone->GetPosition()[2], 0);
}

}  // namespace csci3081
