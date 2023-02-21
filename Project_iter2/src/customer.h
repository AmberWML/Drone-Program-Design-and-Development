#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <EntityProject/ANVIL2/customer.h>

namespace csci3081 {
/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief This class is for tracking customers.
 *
 * It can be instantiated from a picojson object and has an Update() method for
 * the DroneSimulation to call during every tick of the simulation.
 */
class Customer : public entity_project::Customer {
 public:
    /**
     * @brief Instantiate a customer from picojson object.
     */
    explicit Customer(const picojson::object& newDetails);

    /**
     * @brief Instantiate a blank customer.
     */
    Customer();

    /**
     * @brief Set local variables from the details_ picojson.
     */
    void InitCust();

    /**
     * @brief Update the customer based on how much time has passed since the last update.
     *
     * This method is currently empty.
     */
    void Update(float dt);

    /**
     * @brief Check if the Customer moves around much. It does not.
     */
    bool IsDynamic() { return false; }
};
}  // namespace csci3081

#endif
