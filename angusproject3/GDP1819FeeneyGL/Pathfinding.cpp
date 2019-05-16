#include "globalStuff.h"
#include <iostream>
#include <math.h>
#include <algorithm>

std::string getObjNodeKey(cMeshObject* obj)
{
	int scale = mapNodes[0][0]->thisNodeObj->nonUniformScale.x;

	int xRemainder = (int)(obj->position.x) % (10);
	int zRemainder = (int)(obj->position.z) % (10);

	int posX = (int)obj->position.x;
	int posZ = (int)obj->position.z;

	if (xRemainder < 5)
	{
		posX = posX - xRemainder;
	}
	else
	{
		posX = posX - xRemainder + (5);
	}

	if (zRemainder < 5)
	{
		posZ = posZ - zRemainder;
	}
	else
	{
		posZ = posZ - zRemainder + (5);
	}

	std::string key = std::to_string(posZ) + std::to_string(posX);

	//std::cout << "The Key: " << key << std::endl;

	return key;
}

std::vector<cMapNode*> Dijkstra(cMeshObject* obj)
{
	//Final vector to be returned
	std::vector<cMapNode*> finalPath;


	//Get the key for the current object
	std::string key = getObjNodeKey(obj);

	//Based on the key, get the object's current node
	cMapNode* nodeWithKey = positionNodeMap[key];
	std::cout << "Node: " << nodeWithKey->thisNodeObj->friendlyName << std::endl;


	//Map holds pointers to previous nodes, using the key for that node
	std::map<std::string, cMapNode*> path;

	std::vector<std::vector<cMapNode*>> nodesInGraph;

	//Set up initial distances from one node to the next
	//std::vector<std::vector<int>> distances;
	int nodeVecNum = mapNodes.size();
	for (int i = 0; i < nodeVecNum; i++)
	{
		//std::vector<int> vecDists;
		//distances.push_back(vecDists);

		std::vector<cMapNode*> vecNodes;
		nodesInGraph.push_back(vecNodes);
		
		for (int j = 0; j < mapNodes[i].size(); j++)
		{
			//Add current node to list of nodes in graph
			mapNodes[i][j]->visited = false;

			//If starting position
			if (mapNodes[i][j]->key == key)
			{
				mapNodes[i][j]->distance = 0;
				nodesInGraph[i].push_back(mapNodes[i][j]);
				//distances[i].push_back(0);
				//Insert NULL at this node's key on the path, indicating it's at the end of the path
				//path.insert(std::pair<std::string, cMapNode*>(key, NULL));
			}
			else
			{
				if (mapNodes[i][j]->nodeColour == '_')
				{
					mapNodes[i][j]->distance = INT_MAX;
					//distances[i].push_back(INT_MAX); //can't traverse these nodes
				}
				//if (mapNodes[i][j]->nodeColour == 'r')
				//{
				//	distances[i].push_back(1);
				//}
				else
				{
					mapNodes[i][j]->distance = INT_MAX;
					nodesInGraph[i].push_back(mapNodes[i][j]);
					//distances[i].push_back(1); //other nodes have a distance of 1
				}
			}			
		}
	}

	int altDist = 0;
	cMapNode * targetNode = NULL;

	while (!nodesInGraph.empty())
	{
		int minDist = INT_MAX;
		int nodeI = -1;
		int nodeJ = -1;

		for (int i = 0; i < nodesInGraph.size(); i++)
		{
			for (int j = 0; j < nodesInGraph[i].size(); j++)
			{
				//if (nodesInGraph[i][j]->nodeColour != '_')
				//{
					int newMin = nodesInGraph[i][j]->distance;
					if (newMin < minDist)
					{
						minDist = newMin;
						nodeI = i;
						nodeJ = j;
					}
				//}
			}
		}
		////Get shortest distance
		//for (int i = 0; i < distances.size(); i++)
		//{
		//	for (int j = 0; j < distances[i].size(); j++)
		//	{

		//	}
		//}

		//Point to node
		cMapNode* node = nodesInGraph[nodeI][nodeJ];
		node->visited = true;
		//Remove node from graph
		nodesInGraph[nodeI].erase(nodesInGraph[nodeI].begin() + nodeJ);

		//If we found a resource tile with a resource, exit loop
		if (node->nodeColour == 'r' && node->hasResource)
		{
			targetNode = node;
			break;
		}

		//Calculate path costs for NW
		if(node->neighbourNW != NULL)
		{
			if (!node->neighbourNW->visited && node->neighbourNW->nodeColour != '_')
			{
				if (node->neighbourNN == NULL || node->neighbourWW == NULL)
				{
					//this should never happen - if there is a NW neighbour, then there is a North and West neighbour as well
				}
				else if ((node->neighbourNN->nodeColour == '_') || (node->neighbourWW->nodeColour == '_'))
				{
					//this might happen - if it does, do nothing
				}
				else
				{
					altDist = node->distance + 3;
					if (altDist < node->neighbourNW->distance)
					{
						node->neighbourNW->distance = altDist;
						path.insert(std::pair<std::string, cMapNode*>(node->neighbourNW->key, node));
					}
				}
			}
		}

		//Calculate path costs for NE
		if (node->neighbourNE != NULL)
		{
			if (!node->neighbourNE->visited && node->neighbourNE->nodeColour != '_')
			{
				if (node->neighbourNN == NULL || node->neighbourEE == NULL)
				{
					//this should never happen - if there is a NE neighbour, then there is a North and East neighbour as well
				}
				else if ((node->neighbourNN->nodeColour == '_') || (node->neighbourEE->nodeColour == '_'))
				{
					//this might happen - if it does, do nothing
				}
				else
				{
					altDist = node->distance + 3;
					if (altDist < node->neighbourNE->distance)
					{
						node->neighbourNE->distance = altDist;
						path.insert(std::pair<std::string, cMapNode*>(node->neighbourNE->key, node));
					}
				}
			}
		}

		//Calculate path costs for SW
		if (node->neighbourSW != NULL)
		{
			if (!node->neighbourSW->visited && node->neighbourSW->nodeColour != '_')
			{
				if (node->neighbourSS == NULL || node->neighbourWW == NULL)
				{
					//this should never happen - if there is a SW neighbour, then there is a South and West neighbour as well
				}
				else if ((node->neighbourSS->nodeColour == '_') || (node->neighbourWW->nodeColour == '_'))
				{
					//this might happen - if it does, do nothing
				}
				else
				{
					altDist = node->distance + 3;
					if (altDist < node->neighbourSW->distance)
					{
						node->neighbourSW->distance = altDist;
						path.insert(std::pair<std::string, cMapNode*>(node->neighbourSW->key, node));
					}
				}
			}
		}

		//Calculate path costs for SE
		if (node->neighbourSE != NULL)
		{
			if (node->neighbourSE->nodeColour != '_')
			{
				if (!node->neighbourSE->visited && node->neighbourSE->nodeColour != '_')
				{
					if (node->neighbourSS == NULL || node->neighbourEE == NULL)
					{
						//this should never happen - if there is a SE neighbour, then there is a South and East neighbour as well
					}
					else if ((node->neighbourSS->nodeColour == '_') || (node->neighbourEE->nodeColour == '_'))
					{
						//this might happen - if it does, do nothing
					}
					else
					{
						altDist = node->distance + 3;
						if (altDist < node->neighbourSE->distance)
						{
							node->neighbourSE->distance = altDist;
							path.insert(std::pair<std::string, cMapNode*>(node->neighbourSE->key, node));
						}
					}
				}
			}
		}

		//Calculate path costs for NN
		if (node->neighbourNN != NULL)
		{
			if (!node->neighbourNN->visited && node->neighbourNN->nodeColour != '_')
			{
				altDist = node->distance + 2;
				if (altDist < node->neighbourNN->distance)
				{
					node->neighbourNN->distance = altDist;
					path.insert(std::pair<std::string, cMapNode*>(node->neighbourNN->key, node));
				}
			}
		}

		//Calculate path costs for SS
		if (node->neighbourSS != NULL)
		{
			if (!node->neighbourSS->visited && node->neighbourSS->nodeColour != '_')
			{
				altDist = node->distance + 2;
				if (altDist < node->neighbourSS->distance)
				{
					node->neighbourSS->distance = altDist;
					path.insert(std::pair<std::string, cMapNode*>(node->neighbourSS->key, node));
				}
			}
		}

		//Calculate path costs for WW
		if (node->neighbourWW != NULL)
		{
			if (!node->neighbourWW->visited && node->neighbourWW->nodeColour != '_')
			{
				altDist = node->distance + 2;
				if (altDist < node->neighbourWW->distance)
				{
					node->neighbourWW->distance = altDist;
					path.insert(std::pair<std::string, cMapNode*>(node->neighbourWW->key, node));
				}
			}
		}

		//Calculate path costs for EE
		if (node->neighbourEE != NULL)
		{
			if (!node->neighbourEE->visited && node->neighbourEE->nodeColour != '_')
			{
				altDist = node->distance + 2;
				if (altDist < node->neighbourEE->distance)
				{
					node->neighbourEE->distance = altDist;
					path.insert(std::pair<std::string, cMapNode*>(node->neighbourEE->key, node));
				}
			}
		}
		//distances[nodeI][nodeJ] = INT_MAX;
	}

	//Create the final path
	finalPath.push_back(targetNode);
	cMapNode* currentNode = targetNode;
	while (path[currentNode->key] != NULL)
	{
		finalPath.push_back(path[currentNode->key]);

		printf("%c\n", currentNode->nodeColour);
		std::cout << "test: " << std::to_string(currentNode->nodeColour) << std::endl;

		currentNode = path[currentNode->key];
	}
	std::reverse(finalPath.begin(), finalPath.end());

	cMeshObject* gatherer = findObjectByFriendlyName("Gatherer");
	gatherer->needPath = false;
	gatherer->atTarget = true;

	return finalPath;
}

// Function to be called after Dijkstra or A* path has been generated
void GathererPath(std::vector<cMapNode*> * path)
{
	cMeshObject* gatherer = findObjectByFriendlyName("Gatherer");

	if ((gatherer->aiType == cMeshObject::aiBehaviour::GATHER) || (gatherer->aiType == cMeshObject::aiBehaviour::RETURN))
	{
		if (gatherer->atTarget == true && path->size() > 1)
		{
			std::cout << path->size() << std::endl;
			gatherer->targetPos = (*path)[1]->thisNodeObj->position;
			gatherer->targetPos.y = gatherer->position.y;
			gatherer->atTarget = false;

			glm::vec3 dir = glm::normalize(gatherer->targetPos - gatherer->position);
			glm::vec3 vel = dir * 2.0f;
			gatherer->velocity = vel; //set gatherer velocity
			path->erase(path->begin()+1); //remove first element of path
			std::cout << path->size() << std::endl;
		}
		else if (gatherer->atTarget == false && path->size() == 1)
		{
			if (glm::abs(glm::distance(gatherer->targetPos, gatherer->position)) < 0.5f)
			{
				gatherer->position = gatherer->targetPos;
				gatherer->velocity = glm::vec3(0.0f); //set velocity to 0
				gatherer->atTarget = true;
				gatherer->needPath = true;
				gatherer->timer = 0.0f;

				if (gatherer->aiType == cMeshObject::aiBehaviour::GATHER)
				{
					idleInPlace(gatherer, 0.0f, 7.0f);

					//gatherer->aiType = cMeshObject::aiBehaviour::RETURN;
					//gatherer->position = gatherer->targetPos;
					//gatherer->velocity = glm::vec3(0.0f); //set velocity to 0
					//gatherer->atTarget = true;
				}
				else if (gatherer->aiType == cMeshObject::aiBehaviour::RETURN)
				{
					idleInPlace(gatherer, 0.0f, 2.0f);
					//gatherer->aiType = cMeshObject::aiBehaviour::GATHER;
					//gatherer->position = gatherer->targetPos;
					//gatherer->velocity = glm::vec3(0.0f); //set velocity to 0
					//gatherer->atTarget = true;
				}

				path->erase(path->begin());
			}
		}
		//else if (gatherer->needPath == true)
		//{

		//}
	}
}

float calculateHeuristic(cMapNode* startNode, cMapNode* endNode)
{
	int dx = (int)glm::abs(endNode->thisNodeObj->position.x - startNode->thisNodeObj->position.x);
	int dz = (int)glm::abs(endNode->thisNodeObj->position.z - startNode->thisNodeObj->position.y);

	return (dx + dz) + (-1) * std::min(dx, dz);
}

//Function for AStar Algorithm
std::vector<cMapNode*> AStar(cMeshObject* obj, cMapNode* endNode)
{

	//Get the key for the current object
	std::string key = getObjNodeKey(obj);

	//Based on the key, get the object's current node
	cMapNode* startNode = positionNodeMap[key];

	//std::vector<cMapNode*> closedSet; //set of nodes already dealt with
	std::map<std::string, cMapNode*> closedSet;

	//std::vector<cMapNode*> openSet; //set of nodes to be evaluated
	//openSet.push_back(startNode);
	std::map<std::string, cMapNode*> openSet;
	openSet.insert(std::pair<std::string, cMapNode*>(startNode->key, startNode));

	//path map holds pointers to previous nodes, using the key for that node
	std::map<std::string, cMapNode*> path;

	//gCosts map holds the costs to reach each node, using the key for that node
	std::map<std::string, int> gCosts;
	for (int i = 0; i < mapNodes.size(); i++)
	{
		for (int j = 0; j < mapNodes[i].size(); j++)
		{
			//Set initial costs to "infinity"
			gCosts.insert(std::pair<std::string, int>(mapNodes[i][j]->key, INT_MAX));
		}
	}

	//fCosts map holds the total costs to reach goal through that node, using the key for that node
	std::map<std::string, int> fCosts = gCosts; //assign infinity to all values

	gCosts[startNode->key] = 0; //set initial cost for starting node to be 0
	fCosts[startNode->key] = calculateHeuristic(startNode, endNode); //set initial total cost from start to goal as the distance between them

	//while openSet is not empty
	//	current : = the node in openSet having the lowest fScore[] value
	//	if current = goal
	//		return reconstruct_path(cameFrom, current)

	//		openSet.Remove(current)
	//		closedSet.Add(current)

	cMapNode* currentNode = NULL;

	//loop until open set is empty
	while (!openSet.empty())
	{
		std::string lowestKey;
		int lowestCost = INT_MAX;

		std::map<std::string, int>::iterator it;
		for (it = fCosts.begin(); it != fCosts.end(); it++)
		{
			if (it->second < lowestCost)
			{
				if (openSet.find(it->first) != openSet.end())
				{
					lowestKey = it->first;
					lowestCost = it->second;
				}
			}
		}

		currentNode = openSet[lowestKey];
		openSet.erase(lowestKey);
		closedSet.insert(std::pair<std::string, cMapNode*>(lowestKey, currentNode));

		if (currentNode == endNode)
		{
			break; //exit loop
		}

		//Check NW neighbour of current node
		if (currentNode->neighbourNW != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourNW->key) == closedSet.end())
			{
				if (currentNode->neighbourNN == NULL || currentNode->neighbourWW == NULL)
				{
					//this should never happen - if there is a NW neighbour, then there is a North and West neighbour as well
					bestPath = false;
				}
				else if ((currentNode->neighbourNN->nodeColour == '_') || (currentNode->neighbourWW->nodeColour == '_'))
				{
					//this might happen - if it does, do nothing
					bestPath = false;
				}
				else
				{
					//if ((gCosts[currentNode->key] < INT_MAX) && (currentNode->neighbourNW->nodeColour != '_'))
					if ((gCosts[currentNode->key] < INT_MAX))
					{
						tempCost = gCosts[currentNode->key] + 3;
					}

					//if not in open set, add it
					if (openSet.find(currentNode->neighbourNW->key) == openSet.end())
					{
						openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourNW->key, currentNode->neighbourNW));
					}
					//otherwise 
					else if (tempCost >= gCosts[currentNode->neighbourNW->key])
					{
						bestPath = false;
					}
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourNW->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourNW->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourNW->key] = currentNode;
				}
				gCosts[currentNode->neighbourNW->key] = tempCost;
				if (gCosts[currentNode->neighbourNW->key] < INT_MAX && (currentNode->neighbourNW->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourNW->key] = gCosts[currentNode->neighbourNW->key] + calculateHeuristic(currentNode->neighbourNW, endNode);
				}
			}
		}

		//Check NE neighbour of current node
		if (currentNode->neighbourNE != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourNE->key) == closedSet.end())
			{
				if (currentNode->neighbourNN == NULL || currentNode->neighbourEE == NULL)
				{
					//this should never happen - if there is a NW neighbour, then there is a North and West neighbour as well
					bestPath = false;
				}
				else if ((currentNode->neighbourNN->nodeColour == '_') || (currentNode->neighbourEE->nodeColour == '_'))
				{
					//this might happen - if it does, do nothing
					bestPath = false;
				}
				else
				{
					//if ((gCosts[currentNode->key] < INT_MAX) && (currentNode->neighbourNE->nodeColour != '_'))
					if (gCosts[currentNode->key] < INT_MAX)
					{
						tempCost = gCosts[currentNode->key] + 3;
					}

					//if not in open set, add it
					if (openSet.find(currentNode->neighbourNE->key) == openSet.end())
					{
						openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourNE->key, currentNode->neighbourNE));
					}
					//otherwise 
					else if (tempCost >= gCosts[currentNode->neighbourNE->key])
					{
						bestPath = false;
					}
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourNE->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourNE->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourNE->key] = currentNode;
				}
				gCosts[currentNode->neighbourNE->key] = tempCost;
				if (gCosts[currentNode->neighbourNE->key] < INT_MAX && (currentNode->neighbourNE->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourNE->key] = gCosts[currentNode->neighbourNE->key] + calculateHeuristic(currentNode->neighbourNE, endNode);
				}
			}
		}

		//Check SW neighbour of current node
		if (currentNode->neighbourSW != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourSW->key) == closedSet.end())
			{
				if (currentNode->neighbourSS == NULL || currentNode->neighbourWW == NULL)
				{
					//this should never happen - if there is a NW neighbour, then there is a North and West neighbour as well
					bestPath = false;
				}
				else if ((currentNode->neighbourSS->nodeColour == '_') || (currentNode->neighbourWW->nodeColour == '_'))
				{
					//this might happen - if it does, do nothing
					bestPath = false;
				}
				else
				{
					//if ((gCosts[currentNode->key] < INT_MAX) && (currentNode->neighbourSW->nodeColour != '_'))
					if (gCosts[currentNode->key] < INT_MAX)
					{
						tempCost = gCosts[currentNode->key] + 3;
					}

					//if not in open set, add it
					if (openSet.find(currentNode->neighbourSW->key) == openSet.end())
					{
						openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourSW->key, currentNode->neighbourSW));
					}
					//otherwise 
					else if (tempCost >= gCosts[currentNode->neighbourSW->key])
					{
						bestPath = false;
					}
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourSW->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourSW->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourSW->key] = currentNode;
				}
				gCosts[currentNode->neighbourSW->key] = tempCost;
				if (gCosts[currentNode->neighbourSW->key] < INT_MAX && (currentNode->neighbourSW->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourSW->key] = gCosts[currentNode->neighbourSW->key] + calculateHeuristic(currentNode->neighbourSW, endNode);
				}
			}
		}

		//Check SE neighbour of current node
		if (currentNode->neighbourSE != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourSE->key) == closedSet.end())
			{
				if (currentNode->neighbourSS == NULL || currentNode->neighbourEE == NULL)
				{
					//this should never happen - if there is a SE neighbour, then there is a South and East neighbour as well
					bestPath = false;
				}
				else if ((currentNode->neighbourSS->nodeColour == '_') || (currentNode->neighbourEE->nodeColour == '_'))
				{
					//this might happen - if it does, do nothing
					bestPath = false;
				}
				else
				{
					if (gCosts[currentNode->key] < INT_MAX)
					{
						tempCost = gCosts[currentNode->key] + 3;
					}

					//if not in open set, add it
					if (openSet.find(currentNode->neighbourSE->key) == openSet.end())
					{
						openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourSE->key, currentNode->neighbourSE));
					}
					//otherwise 
					else if (tempCost >= gCosts[currentNode->neighbourSE->key])
					{
						bestPath = false;
					}
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourSE->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourSE->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourSE->key] = currentNode;
				}
				gCosts[currentNode->neighbourSE->key] = tempCost;
				if (gCosts[currentNode->neighbourSE->key] < INT_MAX && (currentNode->neighbourSE->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourSE->key] = gCosts[currentNode->neighbourSE->key] + calculateHeuristic(currentNode->neighbourSE, endNode);
				}
			}
		}

		//Check NN neighbour of current node
		if (currentNode->neighbourNN != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourNN->key) == closedSet.end())
			{
				//if ((gCosts[currentNode->key] < INT_MAX) && (currentNode->neighbourNN->nodeColour != '_'))
				if (gCosts[currentNode->key] < INT_MAX)
				{
					tempCost = gCosts[currentNode->key] + 2;
				}

				//if not in open set, add it
				if (openSet.find(currentNode->neighbourNN->key) == openSet.end())
				{
					openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourNN->key, currentNode->neighbourNN));
				}
				//otherwise 
				else if (tempCost >= gCosts[currentNode->neighbourNN->key])
				{
					bestPath = false;
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourNN->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourNN->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourNN->key] = currentNode;
				}
				gCosts[currentNode->neighbourNN->key] = tempCost;
				if (gCosts[currentNode->neighbourNN->key] < INT_MAX && (currentNode->neighbourNN->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourNN->key] = gCosts[currentNode->neighbourNN->key] + calculateHeuristic(currentNode->neighbourNN, endNode);
				}
			}
		}

		//Check SS neighbour of current node
		if (currentNode->neighbourSS != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourSS->key) == closedSet.end())
			{
				//if ((gCosts[currentNode->key] < INT_MAX) && (currentNode->neighbourSS->nodeColour != '_'))
				if (gCosts[currentNode->key] < INT_MAX)
				{
					tempCost = gCosts[currentNode->key] + 2;
				}

				//if not in open set, add it
				if (openSet.find(currentNode->neighbourSS->key) == openSet.end())
				{
					openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourSS->key, currentNode->neighbourSS));
				}
				//otherwise 
				else if (tempCost >= gCosts[currentNode->neighbourSS->key])
				{
					bestPath = false;
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourSS->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourSS->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourSS->key] = currentNode;
				}
				gCosts[currentNode->neighbourSS->key] = tempCost;
				if (gCosts[currentNode->neighbourSS->key] < INT_MAX && (currentNode->neighbourSS->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourSS->key] = gCosts[currentNode->neighbourSS->key] + calculateHeuristic(currentNode->neighbourSS, endNode);
				}
			}
		}

		//Check WW neighbour of current node
		if (currentNode->neighbourWW != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourWW->key) == closedSet.end())
			{
				//if ((gCosts[currentNode->key] < INT_MAX) && (currentNode->neighbourWW->nodeColour != '_'))
				if (gCosts[currentNode->key] < INT_MAX)
				{
					tempCost = gCosts[currentNode->key] + 2;
				}

				//if not in open set, add it
				if (openSet.find(currentNode->neighbourWW->key) == openSet.end())
				{
					openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourWW->key, currentNode->neighbourWW));
				}
				//otherwise 
				else if (tempCost >= gCosts[currentNode->neighbourWW->key])
				{
					bestPath = false;
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourWW->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourWW->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourWW->key] = currentNode;
				}
				gCosts[currentNode->neighbourWW->key] = tempCost;
				if (gCosts[currentNode->neighbourWW->key] < INT_MAX && (currentNode->neighbourWW->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourWW->key] = gCosts[currentNode->neighbourWW->key] + calculateHeuristic(currentNode->neighbourWW, endNode);
				}
			}
		}

		//Check EE neighbour of current node
		if (currentNode->neighbourEE != NULL)
		{
			bool bestPath = true;
			int tempCost = INT_MAX;
			if (closedSet.find(currentNode->neighbourEE->key) == closedSet.end())
			{
				//if ((gCosts[currentNode->key] < INT_MAX) && (currentNode->neighbourEE->nodeColour != '_'))
				if (gCosts[currentNode->key] < INT_MAX)
				{
					tempCost = gCosts[currentNode->key] + 2;
				}

				//if not in open set, add it
				if (openSet.find(currentNode->neighbourEE->key) == openSet.end())
				{
					openSet.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourEE->key, currentNode->neighbourEE));
				}
				//otherwise 
				else if (tempCost >= gCosts[currentNode->neighbourEE->key])
				{
					bestPath = false;
				}
			}
			else
			{
				bestPath = false;
			}
			//If this is the best path
			if (bestPath)
			{
				if (path.find(currentNode->neighbourEE->key) == path.end())
				{
					path.insert(std::pair<std::string, cMapNode*>(currentNode->neighbourEE->key, currentNode));
				}
				else
				{
					path[currentNode->neighbourEE->key] = currentNode;
				}
				gCosts[currentNode->neighbourEE->key] = tempCost;
				if (gCosts[currentNode->neighbourEE->key] < INT_MAX && (currentNode->neighbourEE->nodeColour != '_'))
				{
					fCosts[currentNode->neighbourEE->key] = gCosts[currentNode->neighbourEE->key] + calculateHeuristic(currentNode->neighbourEE, endNode);
				}
			}
		}

		//end of while loop
	}


	//Create the final path
	std::vector<cMapNode*> finalPath;

	finalPath.push_back(currentNode);
	while (path[currentNode->key] != NULL)
	{
		finalPath.push_back(path[currentNode->key]);

		printf("%c\n", currentNode->nodeColour);
		std::cout << "test: " << std::to_string(currentNode->nodeColour) << std::endl;

		currentNode = path[currentNode->key];
	}
	std::reverse(finalPath.begin(), finalPath.end());
	//finalPath.erase(finalPath.begin());

	//cMeshObject* gatherer = findObjectByFriendlyName("Gatherer");
	obj->needPath = false;
	obj->atTarget = true;

	return finalPath;
}

void idleInPlace(cMeshObject* obj, float dt, float maxTime)
{
	obj->aiType = cMeshObject::IDLE;
	obj->timer += dt;
	obj->maxTime = maxTime;

	if (obj->timer > obj->maxTime)
	{
		std::string nodeKey = getObjNodeKey(obj);
		cMapNode* currentNode = positionNodeMap[nodeKey];

		if (currentNode->nodeColour == 'r')
		{
			std::string nodeKey = getObjNodeKey(obj);
			cMapNode* resNode = positionNodeMap[nodeKey];
			resNode->hasResource = false;
			obj->linkedObjects.push_back(resourceMap[nodeKey]);
			resourceMap.erase(nodeKey); //remove node from map

			obj->aiType = cMeshObject::aiBehaviour::RETURN;

		}
		else
		{
			obj->aiType = cMeshObject::aiBehaviour::GATHER;
			for (int i = 0; i < obj->linkedObjects.size(); i++)
			{
				if (obj->linkedObjects[i]->objectClass == cMeshObject::objClass::RESOURCE)
				{
					obj->linkedObjects.erase(obj->linkedObjects.begin() + i);
				}
			}
		}
	}
}