#ifndef _MAP_NODE_HG_
#define _MAP_NODE_HG_

#include "cMeshObject.h"

class cMapNode
{

public:
	cMapNode();
	~cMapNode();

	//The color of this node
	char nodeColour;

	//The cMeshObject of this node, including info such as position
	cMeshObject * thisNodeObj;

	//Pointers to the neighbouring nodes
	cMapNode * neighbourNW;
	cMapNode * neighbourNE;
	cMapNode * neighbourSW;
	cMapNode * neighbourSE;
	cMapNode * neighbourNN;
	cMapNode * neighbourSS;
	cMapNode * neighbourWW;
	cMapNode * neighbourEE;

	std::string key;
	bool hasResource = false;
	bool visited = false;
	int distance = INT_MAX;
};

#endif // !_MAP_NODE_HG_
