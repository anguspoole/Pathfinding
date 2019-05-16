#include "cMapNode.h"

cMapNode::cMapNode()
{
	this->nodeColour = NULL;
	this->thisNodeObj = NULL;
	this->neighbourNW = NULL;
	this->neighbourNN = NULL;
	this->neighbourNE = NULL;
	this->neighbourSW = NULL;
	this->neighbourSS = NULL;
	this->neighbourSE = NULL;
	this->neighbourWW = NULL;
	this->neighbourEE = NULL;
}

cMapNode::~cMapNode()
{
	
}