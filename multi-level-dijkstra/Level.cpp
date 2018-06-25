#include "Level.h"
#include <c++/iostream>

Level::Level(const int value, VertexSet selectedVertices) : value(value),
                                                            selectedVertices(std::move(selectedVertices)) {
}

Level::Level(const VertexSet &selectedVertices, const Level &prevLevel) : Level(prevLevel.value + 1, selectedVertices) {

    const VertexSet &prevSelectedVertices = prevLevel.selectedVertices;

    for (Vertex *prevLevelVertex: prevSelectedVertices) {
        calculateShortestPathTree(prevLevelVertex, prevSelectedVertices);
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

    dijkstra(prevLevelVertex, prevSelectedVertices);

//    if (value == 2 && prevLevelVertex->id == 18) {
//        Vertex *const &v4 = *prevSelectedVertices.find(new Vertex(4));
//        Vertex *const &v254 = *prevSelectedVertices.find(new Vertex(254));
//        cout << endl << endl;
//        printPathFromSourceToTarget(prevLevelVertex, v4);
//        cout << endl << endl;
//        printPathFromSourceToTarget(prevLevelVertex, v254);
//        cout << endl << endl;
//    }
//
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

        for (auto edge: u->levelEdges) {
            Vertex *const dest = edge.first;
            if (dest->visited) {
                continue;
            }

            const ULL newDist = u->dist + edge.second;
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
//                v->addParentAdjVertexForEveryAdjCC(*selectedVertexIt);
            }
        }
        else if (isDestSelectedVertex) {
            prevLevelVertex->linkUp(*destSelectedVertexIt, v->dist); // upward edge
//            prevLevelVertex->addParentAdjVertexForEveryAdjCC(*destSelectedVertexIt);
        }

        if (!isDestSelectedVertex) {
            addChildrenToQueue(v, Q);
        }
    }
}

void Level::addChildrenToQueue(Vertex *parent, std::queue<Vertex *> &Q) {
    for (auto edge: parent->levelEdges) {
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
