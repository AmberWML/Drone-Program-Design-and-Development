#ifndef DRONE_SIMULATION_H_
#define DRONE_SIMULATION_H_

#include <EntityProject/ANVIL2/drone_delivery_system.h>
#include <EntityProject/graph.h>
#include <EntityProject/simple_UMN_route_manager.h>
#include <vector>
#include <string>
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"
#include "src/factory.h"
#include "src/drone_coordinator.h"
#include "src/json_helper.h"
#include "src/dijkstra_route.h"

namespace csci3081 {
/*******************************************************************************
* Class Definitions
******************************************************************************/
/**
* @brief This class is the facade in the facade pattern; it takes input and directs the simulation.
*
* This class a list of entities and methods for creating and adding them. It has methods for
* adding and removing Observer entities. It has an Update() method, which calls the
* update method of every entity in its list. It has a Factory to help with these tasks.
*/
class DroneSimulation : public entity_project::DroneDeliverySystem {
 public:
    /**
     * @brief Instantiate the drone simulation.
     */
    DroneSimulation() { }

    /**
     * @brief Destroy the drone simulation. Free the entity pointers.
     */
    ~DroneSimulation();

    /**
     * @brief Get the team name.
     */
    const std::string& GetTeamName() const { return teamName_; }

    /**
     * @brief Create an entity from a picojson object. Uses Factory's CreateEntity() method.
     */
    entity_project::Entity* CreateEntity(const picojson::object& val) {
      // Use the Factory CreateEntity method
      fact.CreateEntity(val);
    }

    /**
     * @brief Add an existing entity to the master list of entities.
     */
    void AddEntity(entity_project::Entity* entity) {
      // Simply push the entity onto the back of the list
      entities_.push_back(entity);
    }

    /**
     * @brief Schedule an existing package to be delivered to an existing customer.
     *
     * The picojson object does not do anything.
     */
    void ScheduleDelivery(entity_project::Package* package,
        entity_project::Customer* dest, const picojson::object& details) {
      // Use the DroneCoordinator ScheduleDelivery method
      coord.ScheduleDelivery(package, dest, details, entities_);
      // const float *package_points = package->GetPosition();
      // const float *dest_points = dest->GetPosition();
      // std::vector<float> package_position(package_points, package_points + 3);
      // std::vector<float> dest_position(dest_points, dest_points + 3);
      // const entity_project::IGraphNode *package_node =
      //    dijkstraRouteManager.getGraphNode(package_position);
      // const entity_project::IGraphNode *dest_node =
      //    dijkstraRouteManager.getGraphNode(dest_position);
      // std::vector<entity_project::IGraphNode *> route =
      //    dijkstraRouteManager.GetRoute(package_node, dest_node);
    }

    /**
     * @brief Add an existing observer to the list of observers for an existing entity.
     */
    void AddObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

    /**
     * @brief Remove an existing observer from the list of observers for an existing entity.
     */
    void RemoveObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

    /**
     * @brief Get the list of entities as a vector.
     */
    const std::vector<entity_project::Entity*>& GetEntities() const { return entities_; }

    /**
     * @brief Set the graph of the simulation, which represents the
     * navigable routes on the simulation for the drones.
     */
    /// TODO: implement this properly

    void SetGraph(const entity_project::IGraph* graph) {
    // std::cout<<"SetGraph"<<std::endl;
    coord.SetGraph(graph);
    }
    /**
     * @brief Update the simulation by calling the Update() method of every entity in the entities list,
     * based on how much time has passed since the last update.
     */
    void Update(float dt);

 private:
    // const entity_project::IGraph *graph;
    std::string teamName_ = "Samuel Peterson";
    // entity_project::DijkstraRouteManager dijkstraRouteManager;
    // Helper classes
    std::vector<entity_project::Entity*> entities_;
    Factory fact;
    DroneCoordinator coord;
};
}  // namespace csci3081

#endif  // DRONE_SIMULATION_H_
