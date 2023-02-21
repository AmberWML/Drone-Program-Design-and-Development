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
#include <string>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class MutantTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
    obj["type"] = picojson::value("drone");
    obj["position"] = picojson::value(picojson::array(
      { picojson::value(498.292), picojson::value(253.883), picojson::value(-228.623) }));
    obj2["type"] = picojson::value("customer");
    obj2["position"] = picojson::value(picojson::array(
      { picojson::value(-951.412), picojson::value(254.665), picojson::value(298.271) }));
    obj3["type"] = picojson::value("package");
    obj3["position"] = picojson::value(picojson::array(
      { picojson::value(-255.994), picojson::value(257.798), picojson::value(-296.859) }));
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
  Drone* drone;
  Package* package;
  Customer* customer;
  picojson::object obj, obj2, obj3;
};

class TestObserver : public entity_project::EntityObserver {
 public:
  Entity ent;
  picojson::object ev;
  void OnEvent(const picojson::value& event, const Entity& entity) {
    ent = entity;
    ev = event.get<picojson::object>();
  }
};

TEST_F(MutantTest, UpdateIsZero) {
  Entity* entity = system->CreateEntity(obj);
  drone = entity->AsType<Drone>();
  system->AddEntity(drone);
  entity = system->CreateEntity(obj2);
  customer = entity->AsType<Customer>();
  system->AddEntity(customer);
  entity = system->CreateEntity(obj3);
  package = entity->AsType<Package>();
  system->AddEntity(package);
  float x = drone->GetPosition()[0];
  float y = drone->GetPosition()[1];
  float z = drone->GetPosition()[2];
  system->ScheduleDelivery(package, customer, obj);
  system->Update(0.2);
  ASSERT_NE(drone->GetPosition()[0], x);
  ASSERT_NE(drone->GetPosition()[1], y);
  ASSERT_NE(drone->GetPosition()[2], z);
}

TEST_F(MutantTest, WrongCustomer) {
  Entity* entity = system->CreateEntity(obj2);
  customer = entity->AsType<Customer>();
  system->AddEntity(customer);
  entity = system->CreateEntity(obj3);
  package = entity->AsType<Package>();
  system->AddEntity(package);
  picojson::object obj;
  system->ScheduleDelivery(package, customer, obj);
  ASSERT_EQ(package->GetCustomer()->GetId(), customer->GetId());
}

TEST_F(MutantTest, WrongEntity) {
  system->CreateEntity(obj);
  system->CreateEntity(obj2);
  system->CreateEntity(obj3);
  string s = typeid(drone).name();
  ASSERT_EQ(s, "PN8csci30815DroneE");
  s = typeid(customer).name();
  ASSERT_EQ(s, "PN8csci30818CustomerE");
  s = typeid(package).name();
  ASSERT_EQ(s, "PN8csci30817PackageE");
}

TEST_F(MutantTest, ObserverNotAdded) {
  Entity* entity = system->CreateEntity(obj);
  drone = entity->AsType<Drone>();
  system->AddEntity(drone);
  entity = system->CreateEntity(obj2);
  customer = entity->AsType<Customer>();
  system->AddEntity(customer);
  entity = system->CreateEntity(obj3);
  package = entity->AsType<Package>();
  system->AddEntity(package);
  TestObserver* observer = new TestObserver();
  package->AddObserver(observer);
  system->ScheduleDelivery(package, customer, obj);
  ASSERT_EQ(observer->ent.GetId(), package->GetId());
  picojson::object event = observer->ev;
  if (event.find("type") != event.end()) {
    ASSERT_EQ(event.find("type")->second.get<std::string>(), "notify");
  } else {
    ASSERT_EQ(true, false);
  }
  if (event.find("value") != event.end()) {
    ASSERT_EQ(event.find("value")->second.get<std::string>(), "scheduled");
  } else {
    ASSERT_EQ(true, false);
  }
}

}  // namespace csci3081
