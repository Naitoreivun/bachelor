#ifndef BACHELOR_EDGE_H
#define BACHELOR_EDGE_H

#include "imports_and_configs.h"
#include "Vertex.h"
struct Vertex;

struct Edge {
    Vertex *const dest;
    const LL weight;

    Edge(Vertex *dest, LL weight);
};


#endif //BACHELOR_EDGE_H
