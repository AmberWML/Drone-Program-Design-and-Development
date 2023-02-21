#ifndef DRONE_H_
#define DRONE_H_

#include <EntityProject/ANVIL2/drone.h>
#include <EntityProject/graph.h>
#include <EntityProject/ANVIL2/drone_delivery_system.h>
#include <EntityProject/simple_UMN_route_manager.h>
#include <EntityProject/entity_observer.h>
#include <vector>
#include <string>
#include "src/package.h"
#include "src/dijkstra_route.h"


namespace csci3081 {

/*******************************************************************************
* Class Definitions
******************************************************************************/
/**
* @brief This class is for tracking drones, which carry packages around the environment.
*
* The drone stores a speed, busy status and package-carrying status, a pointer to
* its current package, a route made up of SimpleUMNRoutePoints, and an index along
* that route. It has methods check its statuses and to set its package and route.
* It also has methods to check its position in comparison to an Entity and to
* a SimpleUMNRoutePoint. Its most important method is Update(), which is called by
* the DroneSimulation during every tick of the simulation and advances the drone's
* state based on how much time has passed since the last update.
*/
class Drone : public entity_project::Drone {
 public:
    /**
    * @brief Instantiate a drone from a picojson object.
    */
    explicit Drone(const picojson::object& newDetails);

    /**
     * @brief Instantiate a blank drone.
     */
    Drone();

    /**
     * @brief Set local variables from the details_ picojson.
     */
    void InitDrone();

    /**
     * @brief Set the drone's route using a vector of SimpleUMNRoutePoints.
     */
    void SetRoute(std::vector<entity_project::SimpleUMNRoutePoint> newRoute);

    /**
     * @brief Set the drone's target package to an existing package.
     */
    void SetPackage(Package* newPackage);

    /**
     * @brief Check if the drone's radius and another entity's radius overlap.
     */
    bool CheckPosition(entity_project::Entity* other);

    /**
     * @brief Check if a SimpleUMNRoutePoint falls within the drone's radius.
     */
    bool CheckPosition(entity_project::SimpleUMNRoutePoint other);

    /**
     * @brief Advance the drone's state based on the amount of time that has passed
     * since the last update.
     */
    void Update(float dt);

    /**
     * @brief Check if the drone is currently busy delivering a package.
     */
    bool GetIsBusy() { return isBusy; }

    /**
     * @brief Check if the drone is currently carrying a package.
     */
    bool GetHasPackage() { return hasPackage; }

    /**
     * @brief Check if the drone moves around much. It does.
     */
    bool IsDynamic() { return true; }

    /**
     * @brief Get the current route as a reference to a vector of strings.
     */
    const std::vector<std::string>& GetCurrentRoute() const { return *routeNew; }

    /**
     * @brief Get the current route as a vector of SimpleUMNRoutePoints
     */
    std::vector<entity_project::SimpleUMNRoutePoint> GetRoute() { return route; }

    /**
     * @brief Get the remaining battery in seconds as a float.
     */
    /// TODO: implement this properly
    float GetRemainingBattery() const { return max_batt; }

    /**
     * @brief Get current speed.
     */
    float GetCurrentSpeed() const { return speed; }


    /**
     * @brief Get maximum package capacity.
     */
    /// TODO: implement this properly
    float GetMaxCapacity() const { return capacity; }

    /**
     * @brief Get remaining package capacity.
     */
    /// TODO: implement this properly
    float GetRemainingCapacity() const { return capacity - package->GetWeight(); }

    /**
     * @brief Get base acceleration
     */
    /// TODO: implement this properly
    float GetBaseAcceleration() const { return accel; }

    /**
     * @brief Adds an observer to EntityObserver* vector
     */
    void AddObserver(entity_project::EntityObserver* observer);

    /**
     * @brief Removes observer to EntityObserver* vector
     */
    void RemoveObserver(entity_project::EntityObserver* observer);

    /**
     * @brief Calls all OnEvent() functions for observers in vector
     */
    void NotifyObservers(std::string task, entity_project::Entity *entity);

    // int minDistance(entity_project::IGraph* isGraph, int dist[], bool sptSet[]);
    // void printSolution(entity_project::IGraph* isGraph, int dist[]);
    // void dijkstra(entity_project::IGraph* isGraph, int src);

 private:
    // Track mass, max speed, speed, acceleration, weight capacity, battery maximum,
    // statuses, package, route, and progress along the route.
    float mass;
    float max_speed;
    float speed;
    float accel;
    float capacity;
    float max_batt;
    bool isBusy;
    bool hasPackage;
    Package* package;
    std::vector<entity_project::SimpleUMNRoutePoint> route;
    const std::vector<std::string>* routeNew;
    std::vector<entity_project::EntityObserver*> observers;

    int routeIndex;  // Index of which node we're headed toward on the route.

    /**
     * @brief Change the drone's direction to point at an existing entity.
     */
    void PointAt(entity_project::Entity* other);
    /**
     * @brief Change the drone's direction to point at a SimpleUMNRoutePoint.
     */
    void PointAt(entity_project::SimpleUMNRoutePoint);
};
}  // namespace csci3081
#endif
