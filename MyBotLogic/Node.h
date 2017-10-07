#pragma once

#include "TileInfo.h"
#include "Connector.h"
#include <vector>

using namespace std;

class Node
{
public:
	using value_type = int;
private:
	//Coordonnée XY
	value_type x;
	value_type y;

	value_type id;
	Tile::ETileType type;
	vector<Connector*> connectors;
	
public:
	Node(const TileInfo&, int colCount) noexcept;
	~Node();

	value_type getX() const noexcept {
		return x;
	}
	value_type getY() const noexcept {
		return y;
	}

	value_type getId() const noexcept {
		return id;
	}
	Tile::ETileType getType() const noexcept {
		return type;
	}
	const vector<Connector *> getConnectors() const noexcept {
		return connectors;
	}
	void addConnector(Tile::ETilePosition dir, Node * obj);
};

