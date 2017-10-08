#include "Node.h"
#include <algorithm>

Node::Node(const TileInfo& tileInfo, int colCount) noexcept
	: x{}
	, y{ tileInfo.tileID / colCount }
	, id{ tileInfo.tileID }
	, type{ tileInfo.tileType }
	, connectors{}
{
	x = 2*(tileInfo.tileID % colCount) + y % 2;
}

void Node::AddConnector(Tile::ETilePosition dir, Node * obj) {
	//Add target obj in connector
	if ((obj->GetType() != Tile::TileAttribute_Forbidden)&&(GetType() != Tile::TileAttribute_Forbidden)) {
		connectors.push_back(Connector(this, obj, dir));
	}
}

void Node::PopConnector(Node * obj) {
	//Pop target obj in connectors
	int i{};
	while (i < connectors.size()) {
		if (connectors[i].cGetEndNode == obj) {
			connectors.erase(connectors.begin() + i);
			break;
		}
		++i;
	}
}