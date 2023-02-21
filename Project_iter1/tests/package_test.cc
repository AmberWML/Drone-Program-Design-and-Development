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
class PackageTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    droneSimulation = new DroneSimulation();
    picojson::object obj;
    obj["type"] = picojson::value("package");
    Entity* entity = droneSimulation->CreateEntity(obj);
    package = entity->AsType<Package>();
    droneSimulation->AddEntity(package);
  }
  virtual void TearDown() {}

  DroneSimulation* droneSimulation;
  Package* package;
};


TEST_F(PackageTest, SetIsScheduled) {
  ASSERT_EQ(package->IsDynamic(), false);
  package->SetIsScheduled(true);
  ASSERT_EQ(package->IsDynamic(), true);
}

TEST_F(PackageTest, SetCustomer) {
  picojson::object obj;
  obj["type"] = picojson::value("customer");
  Entity* entity = droneSimulation->CreateEntity(obj);
  Customer* customer = entity->AsType<Customer>();
  droneSimulation->AddEntity(customer);
  package->SetCustomer(customer);
  ASSERT_NE(package->GetCustomer(), nullptr);
}

}  // namespace csci3081
