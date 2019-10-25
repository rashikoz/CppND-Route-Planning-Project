#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, 	  float end_x, float end_y);
  	float GetDistance(){ return distance;}
  	void AStarSearch(void);
    // Add public variables or methods declarations here.

  private:
    // Add private variables or methods declarations here.
    RouteModel &m_Model;
  	float distance;
  	RouteModel::Node *start_node, *end_node;
  	std::vector<RouteModel::Node *> open_list;
  	std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *current_node);
  	float CalculateHValue(RouteModel::Node *theNode);
  	RouteModel::Node *NextNode();
  	void AddNeighbors(RouteModel::Node *theNode);
  
};

#endif