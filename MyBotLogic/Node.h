#pragma once

#include "Globals.h"
#include "Connector.h"
#include <vector>

using namespace std;

class Node
{
private:
	int id;
	int x;
	int y;
	//autre syteme de coordonnees base sur http://www-cs-students.stanford.edu/~amitp/Articles/Hexagon2.html (simplifie les calculs de distance)
	int cX;
	int cY;
	int cZ;
	Tile::ETileType type;
	vector<Connector *> connectors;
	
public:
	Node(unsigned int id, unsigned int x, unsigned int y, Tile::ETileType type);
	int getId() {
		return id;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	int getCX() {
		return cX;
	}
	int getCY() {
		return cY;
	}
	int getCZ() {
		return cZ;
	}
	Tile::ETileType getType() {
		return type;
	}
	vector<Connector *> getConnectors() {
		return connectors;
	}
	void addConnector(Tile::ETilePosition dir, Node * obj);
};

