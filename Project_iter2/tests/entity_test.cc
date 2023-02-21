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

class EntityTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
    picojson::object obj;
    obj["type"] = picojson::value("drone");
    Entity* entity = system->CreateEntity(obj);
    drone = entity->AsType<Drone>();
    system->AddEntity(drone);
    obj["type"] = picojson::value("customer");
    entity = system->CreateEntity(obj);
    customer = entity->AsType<Customer>();
    system->AddEntity(customer);
    obj["type"] = picojson::value("package");
    entity = system->CreateEntity(obj);
    package = entity->AsType<Package>();
    system->AddEntity(package);
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
  Drone* drone;
  Package* package;
  Customer* customer;
};

TEST_F(EntityTest, IsDynamic) {
  ASSERT_EQ(drone->IsDynamic(), true);
  ASSERT_EQ(customer->IsDynamic(), false);
  ASSERT_EQ(package->IsDynamic(), false);
}

TEST_F(EntityTest, SetRadius) {
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  obj["radius"] = picojson::value(2.0);
  Entity* d = system->CreateEntity(obj);
  obj["type"] = picojson::value("package");
  Entity* p = system->CreateEntity(obj);
  obj["type"] = picojson::value("customer");
  Entity* c = system->CreateEntity(obj);

  ASSERT_EQ(d->GetRadius(), 2);
  ASSERT_EQ(p->GetRadius(), 2);
  ASSERT_EQ(c->GetRadius(), 2);

  ASSERT_LT(drone->GetRadius(), 2);
  ASSERT_LT(package->GetRadius(), 2);
  ASSERT_LT(customer->GetRadius(), 2);
}

TEST_F(EntityTest, UniqueID) {
  ASSERT_NE(drone->GetId(), customer->GetId());
  ASSERT_NE(package->GetId(), customer->GetId());
  ASSERT_NE(drone->GetId(), package->GetId());
  picojson::object obj;
  obj["type"] = picojson::value("drone");
  Entity* drone2 = system->CreateEntity(obj);
  obj["type"] = picojson::value("package");
  Entity* package2 = system->CreateEntity(obj);
  obj["type"] = picojson::value("customer");
  Entity* customer2 = system->CreateEntity(obj);
  ASSERT_NE(drone->GetId(), drone2->GetId());
  ASSERT_NE(customer->GetId(), customer2->GetId());
  ASSERT_NE(package->GetId(), package2->GetId());
}

}  // namespace csci3081
