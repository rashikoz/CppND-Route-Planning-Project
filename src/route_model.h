#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>
using std::vector;
using std::unordered_map;
using std::string;
using std::sqrt;
using std::pow;


class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:
        // Add public Node variables and methods here.
        Node * parent = nullptr; 
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0;
        bool visited = false;
        vector<Node *> neighbors;      
        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
      float distance(Node otherNode) const {
      	return sqrt(pow((x - otherNode.x), 2) + pow((y -otherNode.y), 2));
      }
      void FindNeighbors(void);
      private:
        // Add private Node variables and methods here.
        int index;
        RouteModel * parent_model = nullptr;
      	Node* FindNeighbor(vector<int> node_indices);
    };
    
    // Add public RouteModel variables and methods here.
    RouteModel(const vector<std::byte> &xml);  
    auto &SNodes() {return m_Nodes;}
    vector<Node> path; // This variable will eventually store the path that is found by the A* search.
  	auto &GetNodeToRoadMap() {return node_to_road;}
	  Node &FindClosestNode(float x, float y);
  
  private:
    // Add private RouteModel variables and methods here.
    vector<Node> m_Nodes;
  	void CreateNodeToRoadHashmap(void);
  	unordered_map <int, vector<const Model::Road*>> node_to_road;
};

#endif