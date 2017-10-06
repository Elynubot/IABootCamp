#include "Connector.h"
#include "Node.h"

Connector::Connector(Node * beg, Node * e, Tile::ETilePosition dir)
{
	beginNode = beg;
	endNode = e;
	direction = dir;
}
