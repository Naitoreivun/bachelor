#ifndef BACHELOR_LEVEL_H
#define BACHELOR_LEVEL_H

#include "Vertex.h"

typedef unordered_set<Vertex *, VertexHasher, VertexComparator> VertexSet;

struct Level {
    const short lvl;
//    unordered_set<Vertex *> selectedVertices;
     VertexSet selectedVertices;

    Level(short lvl, const VertexSet &selectedVertices);
};


#endif //BACHELOR_LEVEL_H
