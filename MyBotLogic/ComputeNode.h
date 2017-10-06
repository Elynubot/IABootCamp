#pragma once
#include "Node.h"

class ComputeNode {
private:
	Node * node;
	float estimation;
	int cout;
	Connector * entry;
	Connector * exit;

public:
	Node * getNode() {
		return node;
	}
	float getEstim() {
		return estimation;
	}
	void setEstim(float estim) {
		estimation = estim;
	}
	int getCout() {
		return cout;
	}
	void setCout(int nCout) {
		cout = nCout;
	}
	float calcEstim(Node * dest) {
		estimation = (abs(node->getCX()*1.0f - dest->getCX()*1.0f) + abs(node->getCY()*1.0f - dest->getCY()*1.0f) + abs(node->getCZ()*1.0f - dest->getCZ())*1.0f) / 2.0f;
		return estimation;
	}
	void setEntry(Connector * con) {
		entry = con;
	}
	Connector * getEntry() {
		return entry;
	}
	void setExit(Connector * con) {
		exit = con;
	}
	Connector * getExit() {
		return exit;
	}
	ComputeNode(Node * node);
	ComputeNode(Node * node, Node * dest);
};