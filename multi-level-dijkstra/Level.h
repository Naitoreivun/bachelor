#ifndef BACHELOR_LEVEL_H
#define BACHELOR_LEVEL_H

#include "../imports_and_configs.h"
#include "Vertex.h"
#include "ConnectedComponent.h"
#include <set>
#include <queue>


typedef unordered_set<Vertex *, VertexHasher, VertexComparator> VertexSet;

struct Level {
    const int value;
//    unordered_set<Vertex *> selectedVertices;
    VertexSet selectedVertices;
    unordered_set<ConnectedComponent *> connectedComponents;

    Level(int value, VertexSet selectedVertices);

    Level(const VertexSet &selectedVertices, const Level &prevLevel);

    inline void resetVertices() {
        for (Vertex *v: selectedVertices) {
            v->reset();
        }
    }

    inline bool isSelectedVertex(Vertex *vertex) {
        return selectedVertices.find(vertex) != selectedVertices.end();
    }

    void bindCCWithSelectedVertex(ConnectedComponent *cc, Vertex *prevVertex) const;

private:
    void calculateShortestPathTree(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices);

    void dijkstra(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices) const;

    void pickEdgesForNewLevel(Vertex *prevLevelVertex);

    void addChildrenToQueue(Vertex *parent, queue<Vertex *> &Q);
};

#endif //BACHELOR_LEVEL_H
