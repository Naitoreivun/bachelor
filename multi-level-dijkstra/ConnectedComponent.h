#ifndef BACHELOR_CONNECTEDCOMPONENT_H
#define BACHELOR_CONNECTEDCOMPONENT_H

#include "../imports_and_configs.h"
#include "Vertex.h"

struct Vertex;

struct ConnectedComponent {
    static ConnectedComponent* const ROOT;
    static ConnectedComponent* const NULL_OBJECT;
    static int idCounter;
    const int id;
    unordered_set<Vertex *> adjVertices;
    ConnectedComponent *parent = ROOT;

    ConnectedComponent();

    void print();

private:
    ConnectedComponent(int id, ConnectedComponent *parent);
};

#endif //BACHELOR_CONNECTEDCOMPONENT_H
