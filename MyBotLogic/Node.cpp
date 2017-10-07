#include "Node.h"
#include <algorithm>

Node::Node(const TileInfo& tileInfo, int colCount) noexcept
	: x{ tileInfo.tileID % colCount }
	, y{ tileInfo.tileID / colCount }
	, id{ tileInfo.tileID }
	, type{ tileInfo.tileType }
	, connectors{}
{
}

Node::~Node() {
	//Free memory
	std::for_each(connectors.begin(), connectors.end(), [](Connector * connector) { 
		delete(connector); 
	});
}

void Node::addConnector(Tile::ETilePosition dir, Node * obj)
{
	//Allocate memory
	if (obj->getType() != Tile::TileAttribute_Forbidden) {
		connectors.push_back(new Connector(this, obj, dir));
	}
}
