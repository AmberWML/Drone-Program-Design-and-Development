#ifndef ENTITY_ISUBJECT_H_
#define ENTITY_ISUBJECT_H_

#include <EntityProject/entity_observer.h>
#include <string>
namespace csci3081 {

/*******************************************************************************
* Class Definitions
******************************************************************************/
/**
* @brief This is the subject interface class of an observer pattern.
*
* This class is inherited by Drone and Package to implement the observer pattern.
*/
class EntityISubject {
 public:
  /**
   * @brief Empty Destructor
   */
  virtual ~EntityISubject() {}
  /**
   * @brief To be defined in Drone and Package, they will be called by DroneSimulation's AddObserver function.
   */
  virtual void AddObserver(entity_project::EntityObserver* observer) = 0;

   /**
   * @brief To be defined in Drone and Package, they will be called by DroneSimulation's RemoveObserver function.
   */
  virtual void RemoveObserver(entity_project::EntityObserver* observer) = 0;

   /**
   * @brief Calls the observers OnEvent() function with the string being the value passed for the "value" key
   * And the entity being a pointer to the drone, or package, itself.
   */
  virtual void NotifyObservers(std::string task, entity_project::Entity *entity) = 0;
};
}  // namespace csci3081
#endif
