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

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>


namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class routeIndexTest : public ::testing::Test {
 protected:
        virtual void SetUp() {
            droneSimulation = new DroneSimulation;
            droneObj["type"] = picojson::value("drone");
            packObj["type"] = picojson::value("package");
            droneEntity = droneSimulation->CreateEntity(droneObj);
            drone = droneEntity->AsType<csci3081::Drone>();
            droneSimulation->AddEntity(drone);
            packageEntity = droneSimulation->CreateEntity(packObj);
            package = packageEntity->AsType<csci3081::Package>();
            droneSimulation->AddEntity(package);
        }
        virtual void TearDown() {}
        Entity* packageEntity;
        Entity* droneEntity;
        DroneSimulation* droneSimulation;
        Drone* drone;
        Package* package;
        picojson::object droneObj, packObj;
};

TEST_F(routeIndexTest, colorIsCorrect) {
    std::vector<entity_project::SimpleUMNRoutePoint> route;
    route.push_back(entity_project::SimpleUMNRoutePoint(0, 0, 0));
    route.push_back(entity_project::SimpleUMNRoutePoint(0, 1, 0));
    route.push_back(entity_project::SimpleUMNRoutePoint(0, 2, 0));
    route.push_back(entity_project::SimpleUMNRoutePoint(1, 2, 0));
    route.push_back(entity_project::SimpleUMNRoutePoint(2, 2, 0));

    drone->SetPackage(package);
    drone->SetRoute(route);
    drone->Update(0.068);  // 0.136 should put the drone at the second point in our route


    int index = drone->GetRouteIndex();
    int routeSize = route.size();
    const picojson::object& droneDetails = drone->GetDetails();

    std::string hexVal = "0xcc3300";

    ASSERT_EQ(hexVal, JsonHelper::GetString(droneDetails, "color"));
    // 0xcc3300 is the color val when the drone is 20% throught the route
}
}  // namespace csci3081
