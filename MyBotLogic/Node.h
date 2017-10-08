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
	//XY Coordinates
	value_type x;
	value_type y;

	value_type id;
	Tile::ETileType type;
	vector<Connector*> connectors; //Only connectors without any forbidden type on both end are stored
	
public:
	Node(const TileInfo&, int colCount) noexcept;
	~Node();

	void UpdateNode(const TileInfo&) noexcept;

	value_type GetX() const noexcept {
		return x;
	}
	value_type GetY() const noexcept {
		return y;
	}

	value_type GetId() const noexcept {
		return id;
	}
	Tile::ETileType GetType() const noexcept {
		return type;
	}
	void SetType(Tile::ETileType _type) noexcept {
		type = _type;
	}

	const vector<Connector *> GetConnectors() const noexcept {
		return connectors;
	}
	void AddConnector(Tile::ETilePosition dir, Node * obj);
};

