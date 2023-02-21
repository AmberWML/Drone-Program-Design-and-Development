#include <EntityProject/graph.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <src/dijkstra_route.h>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::cout;
using std::endl;

namespace entity_project {
  DijkstraRoute::DijkstraRoute() {}

  void DijkstraRoute::setGraph(const IGraph *igraph) {
    my_dis.clear();
    graph = const_cast<IGraph *>(igraph);
    vector<IGraphNode *> my_node = igraph->GetNodes();
    for (int i = 0; i < my_node.size(); ++i) {
      string my_node_name = my_node[i]->GetName();
      vector<IGraphNode *> near_node = my_node[i]->GetNeighbors();
      for (int j = 0; j < near_node.size(); ++j) {
        string my_dest_name = near_node[j]->GetName();
        float dis = calculate_dis(my_node[i], near_node[j]);
        my_dis[my_node_name][my_dest_name] = dis;
      }
    }
  }

  const IGraphNode * DijkstraRoute::getGraphNode(const std::vector<float> &points) {
    vector<IGraphNode *> my_node = graph->GetNodes();
    for (auto node : my_node) {
      vector<float> position = node->GetPosition();
      if (position[0] == points[0] && position[1] == points[1] && position[2] == points[2])
        return node;
    }
    return nullptr;
  }

  const IGraphNode * DijkstraRoute::getGraphNode(const float *points) {
    std::vector<float> temp_Node{points[0], points[1], points[2]};
    return getGraphNode(temp_Node);
  }

  float DijkstraRoute::calculate_dis(const IGraphNode *src, const IGraphNode *dest) const {
    vector<float> my_node_point = src->GetPosition();
    vector<float> my_dest_points = dest->GetPosition();
    float x = my_node_point[0] - my_dest_points[0];
    float y = my_node_point[1] - my_dest_points[1];
    float z = my_node_point[2] - my_dest_points[2];
    float dis = static_cast<float>(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
    return dis;
  }
}  // namespace entity_project
