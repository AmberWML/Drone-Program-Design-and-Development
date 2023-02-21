#include <EntityProject/ANVIL2/drone_delivery_system.h>
#include <fstream>
#include <sstream>
#include "src/factory.h"
#include "src/json_helper.h"
#include "src/color_drone.h"

namespace csci3081 {

Factory::Factory() {
  ReadCSV();
}

void Factory::ReadCSV() {
  // Open the drone model file
  std::ifstream modelFile(csv_path);

  // Get the first line as a string stream
  std::string firstLine;
  std::getline(modelFile, firstLine);
  std::stringstream firstLineStream(firstLine);

  // Find the index of each of the expected column names
  std::string colName;
  int colIndex = 0;
  while (std::getline(firstLineStream, colName, ',')) {
    if (colName == "Model #") {
      model_index = colIndex;
    } else if (colName == "Mass (kg)") {
      mass_index = colIndex;
    } else if (colName == "Max Speed (km/h)") {
      max_speed_index = colIndex;
    } else if (colName == "Base Acceleration (m/s^2)") {
      accel_index = colIndex;
    } else if (colName == "WeightCapacity (kg)") {
      capacity_index = colIndex;
    } else if (colName == "Base Battery Capacity (seconds)") {
      batt_index = colIndex;
    }

    colIndex++;
  }

  // Now save the values of each column to the appropriate vector
  std::string line;
  while (std::getline(modelFile, line)) {
    colIndex = 0;
    std::string value = "";
    std::stringstream lineStream(line);

    while (std::getline(lineStream, value, ',')) {
      if (colIndex == model_index) {
        model_col.push_back(value);
      } else if (colIndex == mass_index) {
        mass_col.push_back(std::stof(value));
      } else if (colIndex == max_speed_index) {
        max_speed_col.push_back(std::stof(value));
      } else if (colIndex == accel_index) {
        accel_col.push_back(std::stof(value));
      } else if (colIndex == capacity_index) {
        capacity_col.push_back(std::stof(value));
      } else if (colIndex == batt_index) {
        batt_col.push_back(std::stof(value));
      }

      colIndex++;
    }
  }

  /* deprecated: print all of the columns

  std::cout << std::endl;
  for (auto i = model_col.begin(); i != model_col.end(); ++i) {
    std::cout << *i << ", ";
  }
  std::cout << std::endl;
  for (auto i = mass_col.begin(); i != mass_col.end(); ++i) {
    std::cout << *i << ", ";
  }
  std::cout << std::endl;
  for (auto i = max_speed_col.begin(); i != max_speed_col.end(); ++i) {
    std::cout << *i << ", ";
  }
  std::cout << std::endl;
  for (auto i = accel_col.begin(); i != accel_col.end(); ++i) {
    std::cout << *i << ", ";
  }
  std::cout << std::endl;
  for (auto i = capacity_col.begin(); i != capacity_col.end(); ++i) {
    std::cout << *i << ", ";
  }
  std::cout << std::endl;
  for (auto i = batt_col.begin(); i != batt_col.end(); ++i) {
    std::cout << *i << ", ";
  }
  std::cout << std::endl << std::endl;
  */

  // Close the drone model file
  modelFile.close();
}

int Factory::GetModelIndex(std::string name) {
  // Find the index of the model name in the model column
  auto iter = find(model_col.begin(), model_col.end(), name);
  if (!(iter == model_col.end())) {
    // If we found it, save the index
    return iter - model_col.begin();
  } else {
    return -1;
  }
}

entity_project::Entity* Factory::CreateEntity(const picojson::object& val) {
    // Investigate json object that is passed in
    // JsonHelper::PrintEntityDetails(val);

    // Get the type of the object from the json
    std::string type;
    if (JsonHelper::ContainsKey(val, "type")) {
      type = JsonHelper::GetString(val, "type");
    } else {
      type = "no type";
    }

    // Create and return a pointer of the appropriate type, depending on the input type
    if (type == "drone") {
      // Create a new picojson with appropriate model info
      picojson::object newVal = Factory::ModelInfoJson(val);
      Drone* newDrone = new Drone(newVal);
      ColorDrone* newColorDrone = new ColorDrone(newDrone);
      return newColorDrone;
    } else if (type == "customer") {
      Customer* newCustomer = new Customer(val);
      return newCustomer;
    } else if (type == "package") {
      Package* newPackage = new Package(val);
      return newPackage;
    } else {
      entity_project::Entity* newEntity = new entity_project::Entity();
      return newEntity;
    }

    // If we get this far, return a null pointer because the type is not recognized
    return nullptr;
}

picojson::object Factory::ModelInfoJson(const picojson::object& val) {
    picojson::object newVal = val;

    // Get the model name
    std::string model = "";
    if (JsonHelper::ContainsKey(newVal, "model")) {
      model = (JsonHelper::GetString(newVal, "model"));
    } else {
      // If there is no model in the json, just return it unchanged
      return val;
    }

    // Get the index of the model
    int index = GetModelIndex(model);
    if (index == -1) {
      // If we can't find it, update the CSV and try again
      ReadCSV();
      index = GetModelIndex(model);
      if (index == -1) {
        // If we still can't find it, return the json unchanged
        return val;
      }
    }

    // If we get this far, put the appropriate values into the picojson
    newVal["mass"] = picojson::value(mass_col[index]);
    newVal["max_speed"] = picojson::value(max_speed_col[index]);
    newVal["accel"] = picojson::value(accel_col[index]);
    newVal["capacity"] = picojson::value(capacity_col[index]);
    newVal["batt"] = picojson::value(batt_col[index]);

    return newVal;
}
}  // namespace csci3081
