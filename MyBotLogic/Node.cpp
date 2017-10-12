#include "Node.h"
#include <algorithm>

Node::Node(unsigned int tileId, int colCount) noexcept
	: x{}
	, y{ (int)tileId / colCount  }
	, id{ (int)tileId }
	, type{ Tile::TileAttribute_Default }
	, connectors{}
{
	x = 2*(static_cast<int>(tileId) % colCount) + y % 2;
}

void Node::addConnector(Tile::ETilePosition dir, Node * obj) noexcept {
	//Add target obj in connector
	if ((obj->getType() != Tile::TileAttribute_Forbidden)&&(getType() != Tile::TileAttribute_Forbidden)) {
		connectors.push_back(Connector(this, obj, dir));
	}
}

void Node::popConnector(Node * obj) noexcept {
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

Connector* Node::getConnector(Node * obj) noexcept {
	int i{};
	while (i < connectors.size()) {
		if (connectors[i].getEndNodeC() == obj) {
			return &connectors[i];
		}
	}
	return nullptr;
}

Connector* Node::getConnector(Tile::ETilePosition dir) noexcept {
	//Pop target obj in connectors
	int i{};
	while (i < connectors.size()) {
		if (connectors[i].getDirection() == dir) {
			return &connectors[i];
		}
	}
	return nullptr;
}