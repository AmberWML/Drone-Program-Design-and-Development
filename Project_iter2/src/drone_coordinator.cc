#include <EntityProject/ANVIL/drone_delivery_system.h>
#include <vector>
#include "src/drone_coordinator.h"
#include "src/drone_simulation.h"
#include "src/json_helper.h"
using entity_project::SimpleUMNRouteManager;
using entity_project::SimpleUMNRoutePoint;
using entity_project::IGraph;
using entity_project::IGraphNode;
namespace csci3081 {
DroneCoordinator::DroneCoordinator() { }

Drone* DroneCoordinator::GetFreeDrone(std::vector<entity_project::Entity*> entities_) {
  // Return the first Drone in a list of entities that isn't marked busy
  for (int i = 0; i < entities_.size(); i++) {
    if (GetType(entities_[i]) == "drone") {
      if (!entities_[i]->AsType<Drone>()->GetIsBusy()) {
        return entities_[i]->AsType<Drone>();
      }
    }
  }

  // If we get this far, there are no non-busy drones, so return a null pointer
  return nullptr;
}

void DroneCoordinator::ScheduleDelivery(entity_project::Package* package,
    entity_project::Customer* dest, const picojson::object& details,
    std::vector<entity_project::Entity*> entities_) {
  // Print the key details with JsonHelper
  // JsonHelper::PrintKeyValues(details);

  // Cast the entity_project types to our own csci3081 types
  Package* ourPackage = package->AsType<Package>();
  Customer* ourDest = dest->AsType<Customer>();
  Drone* drone = GetFreeDrone(entities_);

  // Set the package its destination and its priority details
  ourPackage->SetCustomer(ourDest);
  ourPackage->SetPriorityDetails(details);

  // If we have a free drone:
  if (drone != nullptr) {
    // Assigns the drone to delivery, and sets the route
    AssignDroneToDelivery(drone, ourPackage);

  } else {
    // If there is no drone currently available, push the package to the queue
    pQueue.push(ourPackage);
  }
}

void DroneCoordinator::AssignNextPackageToDrone(Drone* drone) {
  if (!pQueue.empty()) {
    // If the queue is not empty, assign the drone to the next package in the queue
    AssignDroneToDelivery(drone, pQueue.top());
    // Pop the package off of the queue
    pQueue.pop();
  }
}

void DroneCoordinator::AssignDroneToDelivery(Drone* drone, Package* package) {
  // Get the destination from the package
  Customer* dest = package->GetCustomer();
  // Assign the package to the drone
  drone->SetPackage(package);
  // Let the package know that it has been scheduled
  package->SetIsScheduled(true);

  // Notify subscribed observers that package is scheduled
  package->NotifyObservers("scheduled", package);

  // Get the start, middle, and end points of the routes as SimpleUMNRoutePoints
  // Using the drone, package, and customer's positions in that order
  entity_project::SimpleUMNRoutePoint start = entity_project::SimpleUMNRoutePoint
      (drone->GetPosition()[0], drone->GetPosition()[1], drone->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint mid = entity_project::SimpleUMNRoutePoint
      (package->GetPosition()[0], package->GetPosition()[1], package->GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint end = entity_project::SimpleUMNRoutePoint
      (dest->GetPosition()[0], dest->GetPosition()[1], dest->GetPosition()[2]);

  // Get the first part of the route using the RouteManager
  std::vector<entity_project::SimpleUMNRoutePoint> routeFirst = routeManager.GetRoute(start, mid);
  // Get the second part of the route using the RouteManager
  std::vector<entity_project::SimpleUMNRoutePoint> routeSecond = routeManager.GetRoute(mid, end);
  // Append the second part to the first part
  routeFirst.insert(routeFirst.end(), routeSecond.begin(), routeSecond.end());
  // Set the drone's route to this route
  drone->SetRoute(routeFirst);
  drone->NotifyObservers("moving", drone);
  // auto node_src = dijkstraroute.getGraphNode(position_src);S
  // auto node_dest = dijkstraroute.getGraphNode(position_dest);
  // auto route = dijkstraroute.GetRoute(node_src, node_dest);

/*  int graph[V][V] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };*/
  // drone->dijkstra(graph,0);
  // DijkstraRouteManager(const IGraph *igraph) : graph(igraph);
}


std::vector<IGraphNode*> DroneCoordinator::GenerateRoute(entity_project::Entity* src,
  entity_project::Entity* dest) {
  auto my_position = src->GetPosition();
  auto my_dest_position = dest->GetPosition();
  auto my_node = dijkstraroute.getGraphNode(my_position);
  auto my_dest = dijkstraroute.getGraphNode(my_dest_position);
  auto result_route = dijkstraroute.GetRoute(my_node, my_dest);
  return result_route;
}

std::string DroneCoordinator::GetType(entity_project::Entity* ent) {
  // Get the details from an entity
  const picojson::object& val = ent->GetDetails();
  // Return the type in the entity, if it exists
  if (JsonHelper::ContainsKey(val, "type")) {
    return JsonHelper::GetString(val, "type");
  } else {
    // Otherwise, return an error type
    return "no type";
  }
}
}  // namespace csci3081
