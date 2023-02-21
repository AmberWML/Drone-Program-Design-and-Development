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

class IntegrationTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    droneSimulation = new DroneSimulation();
    picojson::object obj;
    obj["type"] = picojson::value("drone");
    obj["position"] = picojson::value(picojson::array(
      { picojson::value(498.292), picojson::value(253.883), picojson::value(-228.623) }));
    Entity* entity = droneSimulation->CreateEntity(obj);
    drone = entity->AsType<Drone>();
    droneSimulation->AddEntity(drone);
    picojson::object obj2;
    obj2["type"] = picojson::value("customer");
    obj2["position"] = picojson::value(picojson::array(
      { picojson::value(-951.412), picojson::value(254.665), picojson::value(298.271) }));
    entity = droneSimulation->CreateEntity(obj2);
    customer = entity->AsType<Customer>();
    droneSimulation->AddEntity(customer);
    picojson::object obj3;
    obj3["type"] = picojson::value("package");
    obj3["position"] = picojson::value(picojson::array(
      { picojson::value(-255.994), picojson::value(257.798), picojson::value(-296.859) }));
    entity = droneSimulation->CreateEntity(obj3);
    package = entity->AsType<Package>();
    droneSimulation->AddEntity(package);
  }
  virtual void TearDown() {}

  DroneSimulation* droneSimulation;
  Drone* drone;
  Package* package;
  Customer* customer;
};

TEST_F(IntegrationTest, SetRoute) {
  picojson::object obj;
  droneSimulation->ScheduleDelivery(package, customer, obj);
  droneSimulation->Update(0.02);
  ASSERT_NE(drone->GetPosition()[0], 0);
  ASSERT_NE(drone->GetPosition()[1], 0);
  ASSERT_NE(drone->GetPosition()[2], 0);
}

TEST_F(IntegrationTest, PackagePickedUp) {
  picojson::object obj;
  droneSimulation->ScheduleDelivery(package, customer, obj);
  droneSimulation->Update(20.0);
  ASSERT_NE(drone->GetHasPackage(), true);
}

TEST_F(IntegrationTest, DeletePackage) {
  picojson::object obj;
  droneSimulation->ScheduleDelivery(package, customer, obj);
  int size = droneSimulation->GetEntities().size();
  package->SetPosition(-951.412, 254.665, 298.271);
  droneSimulation->Update(0.01);
  ASSERT_EQ(droneSimulation->GetEntities().size(), size-1);
}

}  // namespace csci3081
