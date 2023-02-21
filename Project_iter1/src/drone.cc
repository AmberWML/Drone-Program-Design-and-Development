#include "src/drone.h"
#include "src/json_helper.h"

#include "src/entity_isubject.h"

// #include "src/dijkstra.h"

#include <math.h>
#include <limits.h>
#include <stdio.h>
namespace csci3081 {

Drone::Drone(const picojson::object& newDetails) {
  // Set the json details
  details_ = newDetails;
  // Initialize the rest of the fields from details_
  InitDrone();
}

Drone::Drone() {
  // Set the json details "type" key to "drone" so that it can be identified as entity
  details_["type"] = picojson::value("drone");
  // Initialize the rest of the fields from details_
  InitDrone();
}

void Drone::InitDrone() {
    // Add 'Drone' to the list of Entity types
    AddType<Drone>();

    // Set the position and direction vectors, if they exist
    if (JsonHelper::ContainsKey(details_, "position")) {
      const picojson::array& newPosition = JsonHelper::GetArray(details_, "position");
      // For up to 3 elements in the json position array, assign them to
      // the appropriate index of our own position.
      for (int i = 0; i < newPosition.size() && i < 3; i++) {
        position_[i] = static_cast<float>(newPosition[i].get<double>());
      }
    } else {
      // Otherwise default to [0, 0, 0]
      position_[0] = 0;
      position_[1] = 0;
      position_[2] = 0;
    }

    if (JsonHelper::ContainsKey(details_, "direction")) {
      const picojson::array& newDirection = JsonHelper::GetArray(details_, "direction");
      // For up to 3 elements in the json direction array, assign them to
      // the appropriate index of our own direction.
      for (int i = 0; i < newDirection.size() && i < 3; i++) {
        direction_[i] = static_cast<float>(newDirection[i].get<double>());
      }
    } else {
      // Otherwise default to [1, 0, 0]
      direction_[0] = 1;
      direction_[1] = 0;
      direction_[2] = 0;
    }

    // Set the radius and model information, if they exist
    if (JsonHelper::ContainsKey(details_, "radius")) {
      radius_ = static_cast<float>(JsonHelper::GetValue(details_, "radius").get<double>());
    } else {
      // Otherwise default to 1
      radius_ = 1;
    }
    if (JsonHelper::ContainsKey(details_, "mass")) {
      mass = static_cast<float>(JsonHelper::GetValue(details_, "mass").get<double>());
    } else {
      // Otherwise default to 50
      mass = 50;
    }
    if (JsonHelper::ContainsKey(details_, "max_speed")) {
      max_speed = static_cast<float>(JsonHelper::GetValue(details_, "max_speed").get<double>());
      speed = max_speed;
    } else {
      // Otherwise default to 60
      max_speed = 60;
      speed = max_speed;
    }
    if (JsonHelper::ContainsKey(details_, "accel")) {
      accel = static_cast<float>(JsonHelper::GetValue(details_, "accel").get<double>());
    } else {
      // Otherwise default to 4
      accel = 4;
    }
    if (JsonHelper::ContainsKey(details_, "capacity")) {
      capacity = static_cast<float>(JsonHelper::GetValue(details_, "capacity").get<double>());
    } else {
      // Otherwise default to 10
      capacity = 10;
    }
    if (JsonHelper::ContainsKey(details_, "batt")) {
      max_batt = static_cast<float>(JsonHelper::GetValue(details_, "batt").get<double>());
    } else {
      // Otherwise default to 300
      max_batt = 300;
    }

    // Default statuses to "not busy, no package"
    isBusy = false;
    hasPackage = false;
}

void Drone::SetRoute(std::vector<entity_project::SimpleUMNRoutePoint> newRoute) {
  // Set the route
  route = newRoute;
  // Set the progress along the route to the start
  routeIndex = 0;
}

void Drone::SetPackage(Package* newPackage) {
  // Set the package
  package = newPackage;
  // Note that we are now busy
  isBusy = true;
}


bool Drone::CheckPosition(entity_project::SimpleUMNRoutePoint other) {
  // Get our position as a SimpleUMNRoutePoint
  entity_project::SimpleUMNRoutePoint thisPos =
      entity_project::SimpleUMNRoutePoint(GetPosition()[0], GetPosition()[1], GetPosition()[2]);

  // If the distance to the point is less than our radius, we are close to it
  return thisPos.DistanceBetween(other) <= GetRadius();
}

bool Drone::CheckPosition(entity_project::Entity* other) {
  // Get our position and the other's position as SimpleUMNRoutePoints
  entity_project::SimpleUMNRoutePoint thisPos =
      entity_project::SimpleUMNRoutePoint(GetPosition()[0], GetPosition()[1], GetPosition()[2]);
  entity_project::SimpleUMNRoutePoint otherPos =
      entity_project::SimpleUMNRoutePoint(other->GetPosition()[0], other->GetPosition()[1],
      other->GetPosition()[2]);

  // If the distance to the other is less than our radii combined, we are close to it
  return thisPos.DistanceBetween(otherPos) <= GetRadius() + other->GetRadius();
}

void Drone::Update(float dt) {
  if (GetIsBusy()) {
    // If we're NOT busy, just do nothing. Otherwise, do all of this
    if (!GetHasPackage()) {
      if (CheckPosition(package)) {
        // If we are within pickup range of the package, set that we have it
        hasPackage = true;

        // Now that we have the package, notify subscribed Observers that we now have the package
        package->NotifyObservers("en route", package);
      }
    } else {
      // If we are carrying a package, set it to our position to carry it
      package->SetPosition(GetPosition()[0], GetPosition()[1], GetPosition()[2]);


      if (CheckPosition(package->GetCustomer())) {
        // If we have a package and are within dropoff range of a customer, drop off the package
        // So: change our status
        // And notify subscribed drone observers
        this->NotifyObservers("idle", this);
        isBusy = false;
        hasPackage = false;
        // And remove our reference to the pointer
        // (but don't free the memory, that's in DroneSimulation)
        package = nullptr;
      }
    }

    if (routeIndex < route.size()) {
      if (CheckPosition(route[routeIndex])) {
        // If we're close to the route point, move on to the next
        routeIndex++;
      }
    }
    if (routeIndex < route.size()) {
      // If we haven't reached the end of the route, point at the next route point
      PointAt(route[routeIndex]);
    }

    // Move forwards
    position_[0] += GetDirection()[0] * max_speed * dt;
    position_[1] += GetDirection()[1] * max_speed * dt;
    position_[2] += GetDirection()[2] * max_speed * dt;
  }
}

void Drone::PointAt(entity_project::Entity* other) {
  // Get our position as a SimpleUMNRoutePoint
  entity_project::SimpleUMNRoutePoint otherPoint = entity_project::SimpleUMNRoutePoint
      (other->GetPosition()[0], other->GetPosition()[1], other->GetPosition()[2]);
  // Call the SimpleUMNRoutePoint version of this method
  PointAt(otherPoint);
}

void Drone::PointAt(entity_project::SimpleUMNRoutePoint other) {
  // Initialize the new direction to the vector between other's position and ours
  float newDir[3];
  newDir[0] = other.x - GetPosition()[0];
  newDir[1] = other.y - GetPosition()[1];
  newDir[2] = other.z - GetPosition()[2];

  // Get the magnitude of that new direction
  float magnitude = sqrt((newDir[0] * newDir[0]) + (newDir[1] * newDir[1]) +
      (newDir[2] * newDir[2]));

  // Normalize the new direction by dividing it by its magnitude
  newDir[0] = newDir[0] / magnitude;
  newDir[1] = newDir[1] / magnitude;
  newDir[2] = newDir[2] / magnitude;

  // Set our direction to the new direction
  direction_[0] = newDir[0];
  direction_[1] = newDir[1];
  direction_[2] = newDir[2];
}

void Drone::AddObserver(entity_project::EntityObserver* observer) {
  // subscribe observer
  observers.push_back(observer);
}

void Drone::RemoveObserver(entity_project::EntityObserver* observer) {
  for (int i = 0; i < observers.size(); i++) {
    if (observers.at(i) == observer) {
      observers.erase(observers.begin() + i);
    }
  }
}

void Drone::NotifyObservers(std::string task, entity_project::Entity *entity) {
  const Entity &package = *entity;
  picojson::object obj;
  obj["type"] = picojson::value("notify");
  obj["value"] = picojson::value(task);
  if (task.compare("moving") == 0) {
    // if the drone is moving, we are going to also add the "path" key with
    // a 2d float vector as it's value
    std::vector<entity_project::SimpleUMNRoutePoint> routeUMNPoints = GetRoute();
    std::vector<std::vector<float>> routePoints;

    std::vector<entity_project::SimpleUMNRoutePoint>::iterator iter;

    for (iter = routeUMNPoints.begin(); iter != routeUMNPoints.end(); iter++) {
      // Grab the x, y, z, values at i and store them as floats
      std::vector<float> xyz = { iter->x, iter->y, iter->z };
      routePoints.push_back(xyz);
    }

    // const std::vector<std::vector<float>> arr = routePoints;
    obj["path"] = JsonHelper::EncodeArray(routePoints);
  }
  const picojson::value& constObj = picojson::value(obj);
  for (int i = 0; i < observers.size(); i++) {
    observers.at(i)->OnEvent(constObj, package);
  }
}

/*
int Drone::minDistance(entity_project::IGraph* isGraph, int dist[], bool sptSet[]) {
  // Initialize min value
  int min = INT_MAX, min_index;
  for (int v = 0; v < isGraph->GetNodes().size(); v++)
    if (sptSet[v] == false && dist[v] <= min)
      min = dist[v], min_index = v;

  return min_index;
}

// A utility function to print the constructed distance array
void Drone::printSolution(entity_project::IGraph* isGraph, int dist[]) {
  printf("Vertex \t\t Distance from Source\n");
  for (int i = 0; i < isGraph->GetNodes().size(); i++)
    printf("%d \t\t %d\n", i, dist[i]);
}
// TODO::To understand what is src value and what is getposition

void Drone::dijkstra(entity_project::IGraph* isGraph, int src) {
	int V = isGraph->GetNodes().size();
	int dist[V];
	bool vertexSet[V];
	for (int i = 0; i < isGraph->GetNodes().size(); i++)
		dist[i] = INT_MAX, vertexSet[i] = false;
	dist[src] = 0;
	for (int count = 0; count < isGraph->GetNodes().size() - 1; count++) {
		int u = minDistance(isGraph, dist, vertexSet);
		vertexSet[u] = true;
		for (int v = 0; v < isGraph->GetNodes().size(); v++)
			if (!vertexSet[v] && isGraph[u][v] && dist[u] != INT_MAX &&
					dist[u] + isGraph[u][v] < dist[v])
				dist[v] = dist[u] + isGraph[u][v];
	}
	printSolution(isGraph, dist);
}
*/

}  // namespace csci3081
