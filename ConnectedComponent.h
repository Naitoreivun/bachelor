#ifndef BACHELOR_CONNECTEDCOMPONENT_H
#define BACHELOR_CONNECTEDCOMPONENT_H

#include "imports_and_configs.h"
#include "Vertex.h"

struct Vertex;

struct ConnectedComponent {
    static ConnectedComponent* const root;
    static int idCounter;
    const int id;
    unordered_set<Vertex *> *adjVertices = new unordered_set<Vertex *>{};
    unordered_set<Vertex *> *parentAdjVertices = new unordered_set<Vertex *>{};
    ConnectedComponent *parent = root;

    ConnectedComponent();

    explicit ConnectedComponent(int id);

    virtual ~ConnectedComponent();

    void print();
};

#endif //BACHELOR_CONNECTEDCOMPONENT_H
