#include "Level.h"
#include <c++/iostream>

Level::Level(const int value, VertexSet selectedVertices) : value(value),
                                                            selectedVertices(std::move(selectedVertices)) {
}

Level::Level(const VertexSet &selectedVertices, const Level &prevLevel) : Level(prevLevel.value + 1, selectedVertices) {

    const VertexSet &prevSelectedVertices = prevLevel.selectedVertices;

    for (Vertex *vertex: this->selectedVertices) {
        calculateShortestPathTree(*prevSelectedVertices.find(vertex), prevSelectedVertices);
    }
}

void printPathFromSourceToTarget(Vertex *source, Vertex *target) {
    if (source != target) {
        printPathFromSourceToTarget(source, target->parent);
    }
    else {
        cout << endl;
    }
    cout << " -> " << target->id << "(" << target << ", " << target->dist << ")";
}

void Level::calculateShortestPathTree(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices) {
    dijkstra(prevLevelVertex, prevSelectedVertices, false);
    pickEdgesForNewLevel(prevLevelVertex, false); // level and upward
    dijkstra(prevLevelVertex, prevSelectedVertices, true);
    pickEdgesForNewLevel(prevLevelVertex, true); // downward
}

void Level::dijkstra(Vertex *prevLevelVertex, const VertexSet &prevSelectedVertices, const bool reversed) const {
    for (Vertex *v: prevSelectedVertices) {
        v->reset();
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

        const unordered_map<Vertex *, LL> &edges = reversed ? u->reversedLevelEdges : u->levelEdges;
        for (auto edge: edges) {
            Vertex *const dest = edge.first;
            if (dest->visited) {
                continue;
            }

            const LL newDist = u->dist + edge.second;
            if (newDist < dest->dist) {
                Q.erase(dest);
                dest->dist = newDist;
                dest->parent = u;
                Q.insert(dest);
            }
        }
    }
}

void Level::pickEdgesForNewLevel(Vertex *prevLevelVertex, const bool reversed) {
    queue<Vertex *> Q;
    addChildrenToQueue(prevLevelVertex, Q, reversed);

    Vertex *selectedVertex = *selectedVertices.find(prevLevelVertex);

    while (!Q.empty()) {
        Vertex *dest = Q.front();
        Q.pop();
        auto destSelectedVertexIt = selectedVertices.find(dest);
        const bool isDestSelectedVertex = destSelectedVertexIt != selectedVertices.end();

        if (reversed) {
            if (!isDestSelectedVertex) {
                dest->linkUp(selectedVertex, dest->dist); // upward edge
            }
        }
        else {
            if (isDestSelectedVertex) {
                selectedVertex->linkBackAndForth(*destSelectedVertexIt, dest->dist); // level edge + reversed
            }
            else {
                selectedVertex->linkDown(dest, dest->dist); // downward edge
            }
        }

        if (!isDestSelectedVertex) {
            addChildrenToQueue(dest, Q, reversed);
        }
    }
}

void Level::addChildrenToQueue(Vertex *parent, std::queue<Vertex *> &Q, const bool reversed) {
    const unordered_map<Vertex *, LL> &edges = reversed ? parent->reversedLevelEdges : parent->levelEdges;
    for (auto edge: edges) {
        if (edge.first->parent == parent) {
            Q.push(edge.first);
        }
    }
}

void Level::bindCCWithSelectedVertex(ConnectedComponent *cc, Vertex *prevVertex) const {
    Vertex *selectedVertex = *selectedVertices.find(prevVertex);
    cc->adjVertices.insert(selectedVertex);
    selectedVertex->adjCC.insert(cc);
}
