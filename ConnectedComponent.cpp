#include <c++/iostream>
#include "ConnectedComponent.h"

int ConnectedComponent::idCounter = 0;
ConnectedComponent *const ConnectedComponent::NULL_OBJECT = new ConnectedComponent(-2, nullptr);
ConnectedComponent *const ConnectedComponent::ROOT = new ConnectedComponent(-1, NULL_OBJECT);

ConnectedComponent::~ConnectedComponent() {
    delete adjVertices;
    delete parentAdjVertices;
}

ConnectedComponent::ConnectedComponent() : ConnectedComponent(++idCounter, ROOT) {}

ConnectedComponent::ConnectedComponent(const int id, ConnectedComponent *parent) : id(id), parent(parent) {}

void ConnectedComponent::print() {
    cout << id << " (p: " << parent->id << ", " << SetHasher{}(adjVertices) << "):";
    for (Vertex *v: *adjVertices) {
        cout << " " << v->id << "(" << v << ")";
    }
    cout << endl;
}
