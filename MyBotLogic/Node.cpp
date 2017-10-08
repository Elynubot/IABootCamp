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

void Node::UpdateNode(const TileInfo& tileInfo) noexcept {
	type = tileInfo.tileType;
}

Node::~Node() {
	//Free memory
	std::for_each(connectors.begin(), connectors.end(), [](Connector * connector) { 
		delete(connector); 
	});
}

void Node::AddConnector(Tile::ETilePosition dir, Node * obj) {
	//Allocate memory
	connectors.push_back(new Connector(this, obj, dir));
}
