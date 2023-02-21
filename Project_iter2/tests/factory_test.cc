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

class FactoryTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
    droneSimulation = new DroneSimulation();
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
  DroneSimulation* droneSimulation;
};

/*******************************************************************************
 * Test Cases
 ******************************************************************************/

TEST_F(FactoryTest, DroneCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  Entity* entity = droneSimulation->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Drone* drone = entity->AsType<Drone>();
  ASSERT_NE(drone, nullptr);
  ASSERT_EQ(droneSimulation->GetEntities().size(), 0);
  droneSimulation->AddEntity(drone);
  ASSERT_EQ(droneSimulation->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(droneSimulation->GetEntities()[0]->GetDetails()).serialize(),
    picojson::value(obj).serialize());
}

TEST_F(FactoryTest, CustomerCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("customer");
  Entity* entity = droneSimulation->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Customer* customer = entity->AsType<Customer>();
  ASSERT_NE(customer, nullptr);
  ASSERT_EQ(droneSimulation->GetEntities().size(), 0);
  droneSimulation->AddEntity(customer);
  ASSERT_EQ(droneSimulation->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(droneSimulation->GetEntities()[0]->GetDetails()).serialize(),
    picojson::value(obj).serialize());
}

TEST_F(FactoryTest, PackageCreated) {
  picojson::object obj;
  obj["type"] = picojson::value("package");
  Entity* entity = droneSimulation->CreateEntity(obj);
  ASSERT_NE(entity, nullptr);
  Package* package = entity->AsType<Package>();
  ASSERT_NE(package, nullptr);
  ASSERT_EQ(droneSimulation->GetEntities().size(), 0);
  droneSimulation->AddEntity(package);
  ASSERT_EQ(droneSimulation->GetEntities().size(), 1);
  ASSERT_EQ(picojson::value(droneSimulation->GetEntities()[0]->GetDetails()).serialize(),
    picojson::value(obj).serialize());
}

TEST_F(FactoryTest, Factory) {
  Factory f;
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  Entity* entity = f.CreateEntity(obj);
  Drone* drone = entity->AsType<Drone>();
  obj["type"] = picojson::value("customer");
  entity = f.CreateEntity(obj);
  Customer* customer = entity->AsType<Customer>();
  obj["type"] = picojson::value("package");
  entity = f.CreateEntity(obj);
  Package* package = entity->AsType<Package>();
  ASSERT_EQ(drone->GetDetails().find("type")->second.get<std::string>(), "drone");
  ASSERT_EQ(customer->GetDetails().find("type")->second.get<std::string>(), "customer");
  ASSERT_EQ(package->GetDetails().find("type")->second.get<std::string>(), "package");
}

TEST_F(FactoryTest, BasicCSV) {
  Factory f;
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  obj["model"] = picojson::value("Q-36-02");
  Drone* d1 = f.CreateEntity(obj)->AsType<Drone>();
  obj["model"] = picojson::value("Q-36-04");
  Drone* d2 = f.CreateEntity(obj)->AsType<Drone>();

  ASSERT_EQ(d1->GetRemainingBattery(), 10);
  ASSERT_EQ(d1->GetMaxCapacity(), 20);
  ASSERT_EQ(d1->GetBaseAcceleration(), 4.5);
  ASSERT_EQ(d2->GetRemainingBattery(), 90);
  ASSERT_EQ(d2->GetMaxCapacity(), 19);
  ASSERT_EQ(d2->GetBaseAcceleration(), 6);
}

TEST_F(FactoryTest, DefaultCSV) {
  Factory f;
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  obj["model"] = picojson::value("nonsense model name");
  Drone* d1 = f.CreateEntity(obj)->AsType<Drone>();

  ASSERT_EQ(d1->GetRemainingBattery(), 300);
  ASSERT_EQ(d1->GetMaxCapacity(), 10);
  ASSERT_EQ(d1->GetBaseAcceleration(), 4);
}
}  // namespace csci3081
