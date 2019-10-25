#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) 
{
	start_x *= 0.01;
  	start_y *= 0.01;
  	end_x *= 0.01;
  	end_y *= 0.01;
  	start_node = &model.FindClosestNode(start_x, start_y);
	end_node = &model.FindClosestNode(end_x, end_y);  
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node)
{
  std::vector<RouteModel::Node>  path_found;
  distance = 0.0f;
  while (current_node->parent != nullptr)
  {
    	path_found.push_back(*current_node);
    	distance += current_node->distance(*(current_node->parent));
    	current_node = current_node->parent;   
  }
  path_found.push_back(*current_node);
  distance *= m_Model.MetricScale();
  return path_found;  
}

void RoutePlanner::AStarSearch(void)
{
  	RouteModel::Node *current_node = nullptr;
	start_node->visited = true;
  	open_list.push_back(start_node);
  	while (open_list.size() > 0)
    {
      	current_node = NextNode();
      	if (current_node->distance(*end_node) == 0)
        {
          	m_Model.path = ConstructFinalPath(current_node);
          	return;
        }
  		else
        {
          	AddNeighbors(current_node);
        }
    }
 }

float RoutePlanner::CalculateHValue(RouteModel::Node *theNode)
{
  	return theNode->distance(*end_node) ;
}
RouteModel::Node *RoutePlanner::NextNode()
{
  	std::sort(open_list.begin(), open_list.end(), [](auto theNode, auto otherNode) {
        return theNode->h_value + theNode->g_value < otherNode->h_value + otherNode->g_value;   
    });
  	RouteModel::Node *lowestFValueNode;
  	lowestFValueNode = open_list.front();
  	open_list.erase(open_list.begin());
  	return lowestFValueNode;
}
void RoutePlanner::AddNeighbors(RouteModel::Node *theNode)
{
  	theNode->FindNeighbors();
  	for(auto eachNeighbour:theNode->neighbors)
    {
      	eachNeighbour->parent = theNode;
      	eachNeighbour->g_value = theNode->g_value + theNode->distance(*eachNeighbour);
      	eachNeighbour->h_value = CalculateHValue(eachNeighbour);
      	open_list.push_back(eachNeighbour);
      	eachNeighbour->visited = true;         
    }
}