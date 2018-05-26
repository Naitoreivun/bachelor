#include "Level.h"
#include <c++/iostream>

Level::Level(const int value, VertexSet selectedVertices) : value(value),
                                                            selectedVertices(std::move(selectedVertices)) {}

Level::Level(const VertexSet &selectedVertices, const Level &prevLevel) : Level(prevLevel.value + 1, selectedVertices) {

    const VertexSet &prevSelectedVertices = prevLevel.selectedVertices;

    for (Vertex *prevLevelVertex: prevSelectedVertices) {
        calculateShortestPathTree(prevLevelVertex, prevSelectedVertices);
    }
}

struct VertexDijkstraComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const {
        return v1->dist == v2->dist
               ? v1->id < v2->id
               : v1->dist < v2->dist;
    }
};

void Level::calculateShortestPathTree(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices) {

    dijkstra(prevLevelVertex, prevSelectedVertices);
    pickEdgesForNewLevel(prevLevelVertex);

//    cout << endl << prevLevelVertex->id << ":" << endl;
//    for (Vertex *v: prevSelectedVertices) {
//        cout << "\t" << v->id << " (" << v->dist << "):";
//        Vertex *current = v;
//        while (current != prevLevelVertex) {
//            cout << " -> " << current->id;
//            current = current->parent;
//        }
//        cout << endl;
//    }
//    cout << endl;
}

void Level::dijkstra(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices) const {
    for (Vertex *v: prevSelectedVertices) {
        v->dist = INF;
        v->visited = false;
        v->parent = nullptr;
    }

    prevLevelVertex->dist = 0;
    set<Vertex *, VertexDijkstraComparator> Q;
    Q.insert(prevLevelVertex);

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
        Q.erase(u);
        if (u->visited) {
            continue;
        }
        u->visited = true;

        for (Edge *edge: u->levelEdges) {
            Vertex *const dest = edge->dest;
            if (dest->visited) {
                continue;
            }

            const ULL newDist = u->dist + edge->weight;
            if (newDist < dest->dist) { // todo: consider edges count from source when ==
                Q.erase(dest);
                dest->dist = newDist;
                dest->parent = u;
                Q.insert(dest);
            }
        }
    }
}

void Level::pickEdgesForNewLevel(Vertex *prevLevelVertex) {
    queue<Vertex *> Q;
    addChildrenToQueue(prevLevelVertex, Q);

    auto selectedVertexIt = selectedVertices.find(prevLevelVertex);
    const bool isVertexSelectedVertex = selectedVertexIt != selectedVertices.end();
    
    while (!Q.empty()) {
        Vertex *v = Q.front();
        Q.pop();
        auto destSelectedVertexIt = selectedVertices.find(v);
        const bool isDestSelectedVertex = destSelectedVertexIt != selectedVertices.end();

        if (isVertexSelectedVertex) {
            if (isDestSelectedVertex) {
                (*selectedVertexIt)->link(*destSelectedVertexIt, v->dist); // level edge
            }
            else {
                (*selectedVertexIt)->linkDown(v, v->dist); // downward edge
            }
        }
        else if (isDestSelectedVertex) {
            prevLevelVertex->linkUp(*destSelectedVertexIt, v->dist); // upward edge
        }

        if (!isDestSelectedVertex) {
            addChildrenToQueue(v, Q);
        }
    }
}

void Level::addChildrenToQueue(Vertex *parent, std::queue<Vertex *> &Q) {
    for (Edge *edge: parent->levelEdges) {
        if (edge->dest->parent == parent) {
            Q.push(edge->dest);
        }
    }
}
