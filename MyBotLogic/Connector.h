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
	const Node * GetBeginNode() const noexcept {
		return beginNode;
	}
	const Node * GetEndNode() const noexcept {
		return endNode;
	}
	Tile::ETilePosition GetDirection() const noexcept {
		return direction;
	}
};