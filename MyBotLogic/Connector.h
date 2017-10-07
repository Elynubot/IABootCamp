#pragma once
#include "Globals.h"

class Node;

class Connector {
private:
	Node * beginNode;
	Node * endNode;
	Tile::ETilePosition direction;
public:
	Connector(Node * begin, Node * end, Tile::ETilePosition direction);
	const Node * getBeginNode() const noexcept {
		return beginNode;
	}
	const Node * getEndNode() const noexcept {
		return endNode;
	}
	Tile::ETilePosition getDirection() const noexcept {
		return direction;
	}
};