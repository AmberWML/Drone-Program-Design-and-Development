#ifndef COLOR_DRONE_H_
#define COLOR_DRONE_H_

#include "src/drone_decorator.h"
#include <vector>
#include <string>

namespace csci3081 {
/*******************************************************************************
* Class Definitions
******************************************************************************/
/**
* @brief This decorator adds color to drones.
*/
class ColorDrone : public DroneDecorator {
 public:
    /**
     * @brief Add this decorator to an existing drone.
     * @param [out] newDrone - A Drone* is required to instantiate a ColorDrone.
     * @return ColorDrone - a decorated Drone.
     */
    explicit ColorDrone(Drone* newDrone) {
      drone = newDrone;
      details_ = drone->GetDetails();
      InitDrone();
      routeSize = 0;
    }

    /**
     * @brief Set the drone's route using a vector of SimpleUMNRoutePoints.
     * @param [in] newRoute - a vector<SimpleUMNRoutePoint> is passed in to set a route.
     * @return void - there is no return value.
     */
    void SetRoute(std::vector<entity_project::SimpleUMNRoutePoint> newRoute) {
      drone->SetRoute(newRoute);
      routeSize = newRoute.size();
    }

    /**
     * @brief Set the drone's target package to an existing package.
     * @param [in] Package* - a Package is passed in to set the package.
     * @return void - there is no return value.
     */
    void SetPackage(Package* newPackage) { drone->SetPackage(newPackage); }

    /**
     * @brief Check if the drone's radius and another entity's radius overlap.
     * @param [in] Entity* - an Entity is passed in to check the position in relation to the drone.
     * @return bool - a bool is returned indicating if the radius overlaps.
     */
    bool CheckPosition(entity_project::Entity* other) { return drone->CheckPosition(other); }

    /**
     * @brief Check if a SimpleUMNRoutePoint falls within the drone's radius.
     * @param [in] SimpleUMNRoutePoint - a SimpleUMNRoutePoint is passed in to check the position
     * in relation to the drone.
     * @return bool - a bool is returned indicating if the radius overlaps.
     */
    bool CheckPosition(entity_project::SimpleUMNRoutePoint other) {
      return drone->CheckPosition(other);
    }

    /**
     * @brief Advance the drone's state based on the amount of time that has passed
     * since the last update. This is where the color decorator does its work:
     * it updates its local drone and copies its values, then updates its own color.
     * @param [in] float - a float is passed in which is the time that passes with the update.
     * @return void - there is no return value.
     */
    void Update(float dt);

    /**
     * @brief Check if the drone is currently busy delivering a package.
     * @param None - there are no parameters needed.
     * @return bool - a bool is returned indicating if the drone is busy.
     */
    bool GetIsBusy() { return drone->GetIsBusy(); }

    /**
     * @brief Check if the drone is currently carrying a package.
     * @param None - there are no parameters needed.
     * @return bool - a bool is returned indicating if the drone has a package.
     */
    bool GetHasPackage() { return drone->GetHasPackage(); }

    /**
     * @brief Check if the drone moves around much. It does.
     * @param None - there are no parameters needed.
     * @return bool - a bool is returned indicating if the drone is dynamic.
     */
    bool IsDynamic() { return drone->IsDynamic(); }

    /**
     * @brief Get the current route as a reference to a vector of strings.
     * @param None - there are no parameters needed.
     * @return vector<string>& - a vector of strings is returned for the route.
     */
    const std::vector<std::string>& GetCurrentRoute() const {
      return drone->GetCurrentRoute();
    }

    /**
     * @brief Get the current route as a vector of SimpleUMNRoutePoints
     * @param None - there are no parameters needed.
     * @return vector<SimpleUMNRoutePoint> - a vector of SimpleUMNRoutePoints is returned
     * for the route.
     */
    std::vector<entity_project::SimpleUMNRoutePoint> GetRoute() {
      return drone->GetRoute();
    }

    /**
     * @brief Get the remaining battery in seconds as a float.
     * @param None - there are no parameters needed
     * @return float - a float representing the remaining battery is returned.
     */
    float GetRemainingBattery() const { return drone->GetRemainingBattery(); }

    /**
     * @brief Get current speed.
     * @param None - there are no parameters needed.
     * @return float - a float representing the current speed is returned.
     */
    float GetCurrentSpeed() const { return drone->GetCurrentSpeed(); }

    /**
     * @brief Get maximum package capacity.
     * @param None - there are no parameters needed.
     * @return float - a float representing the max capacity is returned.
     */
    float GetMaxCapacity() const { return drone->GetMaxCapacity(); }

    /**
     * @brief Get remaining package capacity.
     * @param None - there are no parameters needed.
     * @return float - a float representing the remaining capacity is returned.
     */
    float GetRemainingCapacity() const { return drone->GetRemainingCapacity(); }

    /**
     * @brief Get base acceleration
     * @param None - there are no parameters needed.
     * @return float - a float representing the base acceleration is returned.
     */
    float GetBaseAcceleration() const { return drone->GetBaseAcceleration(); }

 private:
    Drone* drone;
    int routeSize;
};
}  // namespace csci3081

#endif
