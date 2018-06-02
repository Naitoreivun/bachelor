#include <c++/iostream>
#include "ConnectedComponent.h"

int ConnectedComponent::idCounter = 0;
ConnectedComponent *const ConnectedComponent::root = new ConnectedComponent(-1);

ConnectedComponent::~ConnectedComponent() {
    delete adjVertices;
    delete parentAdjVertices;
}

ConnectedComponent::ConnectedComponent() : ConnectedComponent(++idCounter) {}

ConnectedComponent::ConnectedComponent(const int id) : id(id) {}

void ConnectedComponent::print() {
    cout << id << " (p: " << parent->id << "):";
    for (Vertex *v: *adjVertices) {
        cout << " " << v->id;
    }
    cout << endl;
}
