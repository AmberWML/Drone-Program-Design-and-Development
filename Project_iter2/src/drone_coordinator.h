#ifndef COORDINATOR_H_
#define COORDINATOR_H_

#include <EntityProject/ANVIL2/drone_delivery_system.h>
#include <EntityProject/graph.h>
#include <EntityProject/simple_UMN_route_manager.h>
#include <string>
#include <vector>
#include <queue>
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"
#include "src/dijkstra_route.h"
using entity_project::SimpleUMNRouteManager;
using entity_project::SimpleUMNRoutePoint;
using entity_project::IGraph;
using entity_project::IGraphNode;

namespace csci3081 {

/*******************************************************************************
* Class Definitions
******************************************************************************/
/**
* @brief This class coordinates the drones and deliveries.
*
* This class has methods for creating entities, getting the first free drone
* from an entity vector, and getting the type of an entity.
*/
class DroneCoordinator {
 public:
  /**
   * @brief Instantiate the DroneCoordinator.
   */
  DroneCoordinator();

  /**
   * @brief Get the first free drone from a vector of entities.
   */
  Drone* GetFreeDrone(std::vector<entity_project::Entity*> entities_);

  /**
   * @brief Schedule an existing package to be delivered to an existing customer,
   * using a route manager.
   *
   * The picojson object details is used to add more details to the package, such
   * as priority and maximum time.
   */
  void ScheduleDelivery(entity_project::Package* package,
      entity_project::Customer* dest, const picojson::object& details,
      std::vector<entity_project::Entity*> entities_);

  /**
   * @brief Find the next package in the queue and assign it to the given drone.
   */
  void AssignNextPackageToDrone(Drone* drone);

  /**
   * @brief Assigns a drone to deliver a package.
   */
  void AssignDroneToDelivery(Drone* drone, Package* package);

  /**
   * @brief Get the type of an entity (using its picojson object details).
   */
  std::string GetType(entity_project::Entity* ent);
  void SetGraph(const entity_project::IGraph* graph) {
    // std::cout<<"SetGraph1"<<std::endl;
    dijkstraroute.setGraph(graph);
  }

  std::vector<IGraphNode*> GenerateRoute(entity_project::Entity* src,
  entity_project::Entity* dest);

 private:
  entity_project::SimpleUMNRouteManager routeManager;
  std::priority_queue<Package*> pQueue;
  entity_project::DijkstraRoute dijkstraroute;
};
}  // namespace csci3081

#endif
