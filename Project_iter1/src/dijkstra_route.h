#ifndef DIJKSTRA_ROUTE_H_
#define DIJKSTRA_ROUTE_H_

#include <EntityProject/graph.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <utility>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::cout;
using std::endl;

namespace entity_project {

/// Class in charge of returning dijkstra routes for prototyping.
class DijkstraRoute {
 public:
  /// Default Constructor
  DijkstraRoute();

  /// Constructer used to initialize node in manager
  explicit DijkstraRoute(const IGraph *igraph) : graph(const_cast<IGraph *>(igraph)) {
    vector<IGraphNode *> my_node = igraph->GetNodes();
    cout << "Source\tDestination\tDistance" << endl;
    for (int i = 0; i < my_node.size(); ++i) {
      string my_node_name = my_node[i]->GetName();
      cout << my_node_name << "\t";
      vector<IGraphNode *> near_node = my_node[i]->GetNeighbors();
      for (int j = 0; j < near_node.size(); ++j) {
        string my_dest_name = near_node[j]->GetName();
        cout << my_dest_name << "\t";
        float dis = calculate_dis(my_node[i], near_node[j]);
        cout << dis <<endl;
        my_dis[my_node_name][my_dest_name] = dis;
      }
    }
  }

  void setGraph(const IGraph *igraph);

  /// Returns the best route match between two points.
  std::vector<IGraphNode *> GetRoute(const IGraphNode *src, const IGraphNode *dest) const {
    // If the point is in the set, it means that it has been visited
    unordered_set<string> sptSet;

    // dis from node to src
    unordered_map<string, int> dis;

    // parent node, for return
    unordered_map<string, string> parent;

    // The dis is initialized to -1, which means infinity
    vector<IGraphNode *> my_node = graph->GetNodes();
    string my_node_name = src->GetName();
    for (auto node : my_node) {
      dis[node->GetName()] = -1;
    }
    dis[my_node_name] = 0;
    parent[my_node_name] = "null";

    // run n-1 times
    for (int i = 0; i < my_node.size() - 1; ++i) {
      // Pick the minimum dis vertex from the set of vertices not
      // yet processed.
      string temp_node;
      float min_dis = static_cast<float>(INT32_MAX);
      for (auto node : my_node) {
        string node_name = node->GetName();
        if (!sptSet.count(node_name) && dis[node_name] != -1 &&
            my_dis[my_node_name][node_name] < min_dis) {
          min_dis = my_dis[my_node_name][node_name];
          temp_node = std::move(node_name);
        }
      }

      // Mark the picked vertex as processed
      sptSet.insert(temp_node);

      for (auto node : my_node) {
        string node_name = node->GetName();
        if (!sptSet.count(node_name) && dis[temp_node] != -1 &&
          my_dis[temp_node][node_name] != 0) {
          if (dis[node_name] == -1 ||
              dis[temp_node] + my_dis[temp_node][node_name] < dis[node_name]) {
            dis[node_name] = dis[temp_node] + my_dis[temp_node][node_name];
            parent[node_name] = temp_node;
          }
        }
      }
    }

    // get result_node
    vector<IGraphNode *> result_node;
    string node_name = dest->GetName();
    while (true) {
      result_node.push_back(const_cast<IGraphNode *>(graph->GetNode(node_name)));
      string parent_name = parent[node_name];
      if (parent_name == "null") {
        // src point is not in route
        result_node.pop_back();
        std::reverse(result_node.begin(), result_node.end());
        // dest point is not in route
        result_node.pop_back();
        break;
      }
      node_name = std::move(parent_name);
    }
  }

  const IGraphNode *getGraphNode(const std::vector<float> &points);

  const IGraphNode *getGraphNode(const float *points);

 private:
  float calculate_dis(const IGraphNode *src, const IGraphNode *dest) const;
  IGraph *graph;
  mutable unordered_map<string, unordered_map<string, float>> my_dis;
};
}  // namespace entity_project
#endif
