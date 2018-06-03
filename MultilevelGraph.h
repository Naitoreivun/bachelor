#ifndef BACHELOR_MULTILEVELGRAPH_H
#define BACHELOR_MULTILEVELGRAPH_H

#include "imports_and_configs.h"
#include "Level.h"

struct MultilevelGraph {
    vector<Level> levels;

    explicit MultilevelGraph(const vector<Vertex *> &originalVertices);
    void addLevel(const vector<Vertex *> &selectedVertices);
    void printAll();
    void printConnectedComponents();

    ULL calculateDistance(Vertex *source, Vertex *target);
};

#endif //BACHELOR_MULTILEVELGRAPH_H
