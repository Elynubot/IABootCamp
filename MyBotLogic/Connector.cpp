#include "Connector.h"
#include "Node.h"

void Connector::init(Node * _beginNode, Node * _endNode, Tile::ETilePosition _direction) 
{
	beginNode = _beginNode;
	endNode = _endNode;
	direction = _direction;
	isToDestroy = false;
}
