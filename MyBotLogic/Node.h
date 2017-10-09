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
	vector<Connector> connectors; //Only connectors without any forbidden type on both end are stored
	
public:
	Node(const TileInfo& tileInfo, int colCount) noexcept;

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

	const vector<Connector>* GetConnectors() const noexcept {
		return &connectors;
	}
	void ClearConnectors() noexcept {
		connectors.clear();
	}
	bool IsGoal() const noexcept {
		return GetType() == Tile::TileAttribute_Goal;
	}

	void AddConnector(Tile::ETilePosition dir, Node * obj);
	void PopConnector(Node * obj);

	friend bool operator==(const Node& l, const Node& r) noexcept;
	friend bool operator!=(const Node& l, const Node& r) noexcept;
};

bool operator==(const Node& l, const Node& r) noexcept
{
	return (l.GetId() == r.GetId());
}

bool operator!=(const Node& l, const Node& r) noexcept
{
	return (l.GetId() != r.GetId());
}