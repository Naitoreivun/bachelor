#ifndef BACHELOR_MULTILEVELGRAPH_H
#define BACHELOR_MULTILEVELGRAPH_H

#include "imports_and_configs.h"
#include "Level.h"

struct MultilevelGraph {
    vector<Level> levels;

    explicit MultilevelGraph(const vector<Vertex *> &originalVertices);
    void addLevel(const vector<Vertex *> &selectedVertices);
};

#endif //BACHELOR_MULTILEVELGRAPH_H
