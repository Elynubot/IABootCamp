#include "Node.h"

Node::Node(unsigned int givenId, unsigned int givenX, unsigned int givenY, Tile::ETileType givenType)
{
	id = givenId;
	x = givenX;
	y = givenY;
	type = givenType;
	cX = x - floor((1.0*y) / 2.0);
	cY = y;
	cZ = -x - ceil((1.0*y) / 2.0);
}

void Node::addConnector(Tile::ETilePosition dir, Node * obj)
{
	if (obj->getType() != Tile::TileAttribute_Forbidden) {
		connectors.push_back(new Connector(this, obj, dir));
	}
}
