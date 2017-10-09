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
	const Node * getBeginNodeC() const noexcept {
		return beginNode;
	}
	Node * getBeginNode() const noexcept {
		return beginNode;
	}
	const Node * getEndNodeC() const noexcept {
		return endNode;
	}
	Node * getEndNode() const noexcept {
		return endNode;
	}
	Tile::ETilePosition getDirection() const noexcept {
		return direction;
	}
	Tile::ETilePosition getInvertDirection() const noexcept {
		return (Tile::ETilePosition) ((direction + 3) % 6) ;
	}
};