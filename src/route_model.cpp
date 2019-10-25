#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml)
{
	int iNode=0;
  	for(Model::Node currentNode:this->Nodes())
    {
      	m_Nodes.push_back(Node(iNode, this, currentNode));
      	iNode++;                          
    }
  	CreateNodeToRoadHashmap();
}

void RouteModel::CreateNodeToRoadHashmap(void)
{
  	for(const Model::Road &currentRoad: Roads())
    {
      	if(currentRoad.type != Model::Road::Type::Footway)
        {
         	for(int nodeId : Ways()[currentRoad.way].nodes)
            {
              	if(node_to_road.find(nodeId) == node_to_road.end())
                {
                 	node_to_road[nodeId] = std::vector<const Model::Road *> ();
                }
              	node_to_road[nodeId].push_back(&currentRoad);
            }
        }
      
    }
}

RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices)
{
	Node *closestNode = nullptr;
  	Node curNode;
  	for (int nodeIds : node_indices)
    {
    	curNode = parent_model->SNodes()[nodeIds];
      	if(this->distance(curNode) != 0 && !curNode.visited)
        {
          	if(closestNode == nullptr || this->distance(curNode) < this->distance(*closestNode))
            {
              	closestNode = &parent_model->SNodes()[nodeIds];
            }
        }
    }
  	return closestNode;
}
void RouteModel::Node::FindNeighbors(void)
{
	for(auto & road : parent_model->node_to_road[this->index])
    {
      	RouteModel::Node *newNeighbour = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
      	if(newNeighbour)
        {
          	this->neighbors.emplace_back(newNeighbour);
        }
    }
}
RouteModel::Node &RouteModel::FindClosestNode(float x, float y)
{
  	Node input;
  	input.x = x;
  	input.y = y;
  	float minDistance = std::numeric_limits<float>::max();
    float distance;
  	int  closestId;
  	for(const Model::Road &currentRoad: Roads())
    {
      	if(currentRoad.type != Model::Road::Type::Footway)
        {
         	for(int nodeId : Ways()[currentRoad.way].nodes)
            {
              	distance = input.distance(SNodes()[nodeId]);
                if (distance < minDistance)
                {
                  	closestId = nodeId;
                  	minDistance = distance;
                }
            }
        }      
    }
  	return SNodes()[closestId];  
}