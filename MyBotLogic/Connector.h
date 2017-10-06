#pragma once
#include "Globals.h"

class Node;
class Connector {
public:
	Node * beginNode;
	Node * endNode;
	Tile::ETilePosition direction;
	Connector(Node * begin, Node * end, Tile::ETilePosition dir);
};