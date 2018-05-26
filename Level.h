#ifndef BACHELOR_LEVEL_H
#define BACHELOR_LEVEL_H

#include "Vertex.h"
#include <set>
#include <queue>

typedef unordered_set<Vertex *, VertexHasher, VertexComparator> VertexSet;

struct Level {
    const int value;
//    unordered_set<Vertex *> selectedVertices;
    VertexSet selectedVertices;

    Level(int value, VertexSet selectedVertices);

    Level(const VertexSet &selectedVertices, const Level &prevLevel);

private:
    void calculateShortestPathTree(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices);

    void dijkstra(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices) const;

    void pickEdgesForNewLevel(Vertex *prevLevelVertex);

    void addChildrenToQueue(Vertex *parent, queue<Vertex *> &Q);
};


#endif //BACHELOR_LEVEL_H
