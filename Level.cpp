#include "Level.h"
#include <c++/iostream>

Level::Level(const int value, VertexSet selectedVertices) : value(value),
                                                            selectedVertices(std::move(selectedVertices)) {
    if (value == 0) {
        for (Vertex *v: this->selectedVertices) {
            auto *cc = new ConnectedComponent();
            cc->adjVertices->insert(v);
            v->adjCC.insert(cc);
            connectedComponents.insert(cc);
        }
    }
}

Level::Level(const VertexSet &selectedVertices, const Level &prevLevel) : Level(prevLevel.value + 1, selectedVertices) {

    const VertexSet &prevSelectedVertices = prevLevel.selectedVertices;

    for (Vertex *prevLevelVertex: prevSelectedVertices) {
        calculateShortestPathTree(prevLevelVertex, prevSelectedVertices);
    }
    createConnectedComponents(prevLevel.connectedComponents);
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
                v->addParentAdjVertexForEveryAdjCC(*selectedVertexIt);
            }
        }
        else if (isDestSelectedVertex) {
            prevLevelVertex->linkUp(*destSelectedVertexIt, v->dist); // upward edge
            prevLevelVertex->addParentAdjVertexForEveryAdjCC(*destSelectedVertexIt);
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

void Level::createConnectedComponents(const unordered_set<ConnectedComponent *> &prevConnectedComponents) {
    unordered_map<unordered_set<Vertex *> *, ConnectedComponent *, SetHasher, SetComparator> M;

    for (ConnectedComponent *cc: prevConnectedComponents) {
        if (cc->parentAdjVertices->empty()) {
//            ConnectedComponent *parentCC = new ConnectedComponent();
            for (Vertex *vertex: *cc->adjVertices) {
                Vertex *upperVertex = *selectedVertices.find(vertex);
                // tricky part, when upperVertex knows about new adj CC, but the CC does not know about the upperVertex,
                // so the upperVertex will be able to tell its CC that it should be merged with other CCs when the time comes,
                // and there is no additional fake upward or downward edge between cc and upperVertex :)
                upperVertex->adjCC.insert(cc);
//                upperVertex->adjCC.insert(parentCC);
            }
            connectedComponents.insert(cc); // be careful when deleting (duplications in prev levels)
//            connectedComponents.insert(parentCC);
//            cc->parent = parentCC;
        }
        else {
            ConnectedComponent *parentCC = M[cc->parentAdjVertices];
            if (parentCC == nullptr) {
                parentCC = new ConnectedComponent();

                for (Vertex *upperVertex: *cc->parentAdjVertices) {
                    upperVertex->adjCC.insert(parentCC);
                }
                *parentCC->adjVertices = *cc->parentAdjVertices;

                connectedComponents.insert(parentCC);
                M[cc->parentAdjVertices] = parentCC;
            }
            cc->parent = parentCC;
        }
    }
}
