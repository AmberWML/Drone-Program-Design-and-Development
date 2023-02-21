#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <EntityProject/ANVIL2/package.h>
#include <EntityProject/entity_observer.h>
#include "src/customer.h"
#include "src/entity_isubject.h"
#include <vector>
#include <string>

namespace csci3081 {
/*******************************************************************************
* Class Definitions
******************************************************************************/
/**
* @brief This class is for tracking packages to be delivered.
*
* This class tracks its destination Customer, and has methods for getting and
* setting it. It also has a method to set its position, so that the Drone
* can 'carry' it. Finally, it has an Update() method, called by the DroneSimulation
* during every tick of the simulation.
*/
class Package : public entity_project::Package, EntityISubject {
 public:
   /**
    * @brief Instantiate a package from a picojson object.
    */
    explicit Package(const picojson::object& newDetails);

    /**
     * @brief Instantiate a blank package.
     */
    Package();

    /**
     * @brief Set local variables from the details_ picojson.
     */
    void InitPack();

    /**
     * @brief Set the destination customer to an existing customer.
     */
    void SetCustomer(Customer* newCustomer) { customer = newCustomer; }

    /**
     * @brief Get the destination customer.
     */
    Customer* GetCustomer() { return customer; }

    /**
     * @brief Check if the package's radius and another entity's radius overlap.
     */
    bool CheckPosition(entity_project::Entity* other);

    /**
     * @brief Set the drone's position in 3D space.
     */
    void SetPosition(float x, float y, float z);

    /**
     * @brief Check if the package has been delivered.
     */
    bool GetIsDelivered() { return isDelivered; }

    /**
     * @brief Set whether the package has been scheduled.
     */
    void SetIsScheduled(bool newIsScheduled) { isScheduled = newIsScheduled; }

    /**
     * @brief Check if the drone moves around much. If it has been scheduled, it does.
     */
    bool IsDynamic() { return isScheduled; }

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

    /**
     * @brief Get the weight as a float.
     */
    float GetWeight() const { return weight; }

    /**
     * @brief Sets the priority details for the package
     */
    void SetPriorityDetails(const picojson::object& details) { priorityDetails = details; }

    /**
     * @brief Update the package based on how much time has passed since the last update.
     *
     * If it is within pickup range of its customer, mark itself as delivered.
     */
    void Update(float dt);

 private:
    Customer* customer;  // The destination customer
    bool isDelivered;
    bool isScheduled;
    float weight;
    picojson::object priorityDetails;
    std::vector<entity_project::EntityObserver*> observers;
};
}  // namespace csci3081
#endif
