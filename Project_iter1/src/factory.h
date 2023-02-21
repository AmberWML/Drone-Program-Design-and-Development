#ifndef FACTORY_H_
#define FACTORY_H_

#include <string>
#include <vector>
#include "src/drone.h"
#include "src/customer.h"
#include "src/package.h"

namespace csci3081 {
/*******************************************************************************
* Class Definitions
******************************************************************************/
/**
* @brief This class is the factory in the factory pattern. It can create Drone, Customer, and Package entities.
*
* This class has methods for creating entities and dealing with CSV input. 
*/
class Factory {
 public:
    /**
     * @brief Instantiate the Factory. Read in the drone model CSV file.
     */
    Factory();

    /**
     * @brief Create an entity based on a picojson object. Supports Customer, Drone, and Package types.
     */
    entity_project::Entity* CreateEntity(const picojson::object& val);

 private:
    /**
     * @brief Read in the drone model CSV file.
     */
    void ReadCSV();

    /**
     * @brief Get the index of a model name in the model column.
     */
    int GetModelIndex(std::string name);

    /**
     * @brief Create a picojson value containing drone model information.
     */
    picojson::object ModelInfoJson(const picojson::object& val);

    // The path to the drone model CSV
    std::string csv_path = "data/planet-x.csv";

    // The indices of the columns of the drone model CSV file, default to -1
    int model_index = -1;
    int mass_index = -1;
    int max_speed_index = -1;
    int accel_index = -1;
    int capacity_index = -1;
    int batt_index = -1;

    // The columns of the drone model CSV file
    std::vector<std::string> model_col;
    std::vector<float> mass_col;
    std::vector<float> max_speed_col;
    std::vector<float> accel_col;
    std::vector<float> capacity_col;
    std::vector<float> batt_col;
};
}  // namespace csci3081

#endif
