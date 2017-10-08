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
	const Node * cGetBeginNode() const noexcept {
		return beginNode;
	}
	Node * GetBeginNode() const noexcept {
		return beginNode;
	}
	const Node * cGetEndNode() const noexcept {
		return endNode;
	}
	Node * GetEndNode() const noexcept {
		return endNode;
	}
	Tile::ETilePosition GetDirection() const noexcept {
		return direction;
	}
	Tile::ETilePosition GetInvertDirection() const noexcept {
		return (Tile::ETilePosition) ((direction + 3) % 6) ;
	}
};