#include "Node.h"
#include <algorithm>

Node::Node(const TileInfo& tileInfo, int colCount) noexcept
	: x{}
	, y{ (int)tileInfo.tileID / colCount  }
	, id{ (int)tileInfo.tileID }
	, type{ tileInfo.tileType }
	, connectors{}
{
	x = 2*(static_cast<int>(tileInfo.tileID) % colCount) + y % 2;
}

void Node::addConnector(Tile::ETilePosition dir, Node * obj) {
	//Add target obj in connector
	if ((obj->getType() != Tile::TileAttribute_Forbidden)&&(getType() != Tile::TileAttribute_Forbidden)) {
		connectors.push_back(Connector(this, obj, dir));
	}
}

void Node::popConnector(Node * obj) {
	//Pop target obj in connectors
	int i{};
	while (i < connectors.size()) {
		if (connectors[i].getEndNodeC() == obj) {
			connectors.erase(connectors.begin() + i);
			break;
		}
		++i;
	}
}