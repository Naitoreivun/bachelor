#include <c++/iostream>
#include "MultilevelGraph.h"

MultilevelGraph::MultilevelGraph(const vector<Vertex *> &originalVertices) {
    VertexSet vertexSet;
    for (Vertex *vertex: originalVertices) {
        vertexSet.insert(vertex);
    }
//    cout << vertexSet.size();
    levels.emplace_back(0, vertexSet);
}

void MultilevelGraph::addLevel(const vector<Vertex *> &selectedVertices) {
//    cout << "\nLEVEL " << levels.size() << "(" << selectedVertices.size() << ")" << endl;

    VertexSet vertexSet;
    for (Vertex *vertex: selectedVertices) {
        Vertex *newVertex = new Vertex(vertex->id);
//        cout << newVertex->id << "(" << newVertex << ")" << endl;
        vertexSet.insert(newVertex);
    }

    levels.emplace_back(vertexSet, levels.back());
}

void MultilevelGraph::printAll() {
    for (Level &level: levels) {
        cout << "LEVEL " << level.value << ":\n";
        for (Vertex *u: level.selectedVertices) {
//            cout << u->id << ":\n";
            cout << u->id << "(" << u << "):\n";
            cout << "\tupward:";
            for (auto edge: u->upwardEdges) {
                cout << " " << edge.first->id << "(" << edge.second << ")";
            }
            cout << "\n\tlevel:";
            for (auto edge: u->levelEdges) {
                cout << " " << edge.first->id << "(" << edge.second << ")";
            }
            cout << "\n\tdownward:";
            for (auto edge: u->downwardEdges) {
                cout << " " << edge.first->id << "(" << edge.second << ")";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    cout << endl;
}

void MultilevelGraph::printConnectedComponents() {
    cout << "Connected components:\n";
    for (Level &level: levels) {
        cout << "LEVEL " << level.value << " (size: " << level.connectedComponents.size() << "):\n";
        for (auto cc: level.connectedComponents) {
            cc->print();
        }
        cout << endl;
    }
}

void MultilevelGraph::createConnectedComponents() {
    createConnectedComponentsForLevelZero();
    for (int i = 1; i < levels.size(); ++i) {
        createConnectedComponentsForLevel(i);
    }
}

void MultilevelGraph::createConnectedComponentsForLevelZero() {
    Level &level0 = levels.front();
    for (Vertex *v: level0.selectedVertices) {
        auto *cc = new ConnectedComponent();
        cc->adjVertices.insert(v);
        v->adjCC.insert(cc);
        level0.connectedComponents.insert(cc);
    }
}

void MultilevelGraph::createConnectedComponentsForLevel(int levelValue) {
    Level &levelZero = levels.front();
    Level &currentLevel = levels[levelValue];
    Level &prevLevel = levels[levelValue - 1];

    levelZero.resetVertices();

    for (Vertex *vertex: levelZero.selectedVertices) {
        if (!vertex->visited && !currentLevel.isSelectedVertex(vertex)) {
            createConnectedComponent(vertex, prevLevel, currentLevel);
        }
    }

    duplicateConnectedComponentsWithNoParent(prevLevel, currentLevel);
}

void MultilevelGraph::createConnectedComponent(Vertex *vertex, Level &prevLevel, Level &currentLevel) {
    queue<Vertex *> Q;
    vertex->visited = true;
    Q.push(vertex);
    ConnectedComponent *cc = new ConnectedComponent();

    while (!Q.empty()) {
        Vertex *u = Q.front();
        Q.pop();

        if (prevLevel.isSelectedVertex(u)) { // todo speedup
            Vertex *prevU = *prevLevel.selectedVertices.find(u);
            for (ConnectedComponent *prevCC: prevU->adjCC) {
                prevCC->parent = cc;
            }
        }

        for (auto edge: u->levelEdges) {
            processAdjVertexInBfs(currentLevel, Q, cc, edge.first);
        }
        for (Vertex *v: u->reversedLevelEdges) {
            processAdjVertexInBfs(currentLevel, Q, cc, v);
        }
    }

    currentLevel.connectedComponents.insert(cc);
}

inline void MultilevelGraph::processAdjVertexInBfs(Level &currentLevel,
                                                   queue<Vertex *> &Q,
                                                   ConnectedComponent *cc,
                                                   Vertex *v) {
    if (currentLevel.isSelectedVertex(v)) { //todo speedup
        currentLevel.bindCCWithSelectedVertex(cc, v);
    }
    else if (!v->visited) {
        Q.push(v);
        v->visited = true;
    }
}

void MultilevelGraph::duplicateConnectedComponentsWithNoParent(Level &prevLevel, Level &currentLevel) {
    for (ConnectedComponent *prevCC: prevLevel.connectedComponents) {
        if (prevCC->parent == ConnectedComponent::ROOT) {
            ConnectedComponent *cc = new ConnectedComponent();
            prevCC->parent = cc;
            for (Vertex *prevAdjVertex: prevCC->adjVertices) {
                currentLevel.bindCCWithSelectedVertex(cc, prevAdjVertex);
            }
            currentLevel.connectedComponents.insert(cc);
        }
    }
}

void MultilevelGraph::prepareVerticesForQueries() {
    for (Level &level: levels) {
        level.resetVertices();
    }
    for (int i = 1; i < levels.size(); ++i) {
        for (Vertex *v: levels[i].selectedVertices) {
            Vertex *lowerV = *levels[i - 1].selectedVertices.find(v);
            v->lower = lowerV;
            lowerV->upper = v;
        }
    }
}

ULL MultilevelGraph::calculateDistance(Vertex *source, Vertex *target) {
    if ((*source->adjCC.begin())->parent == (*target->adjCC.begin())->parent) {
        return regularDijkstra(source, target);
    }

    vector<ConnectedComponent *> upwardCCSourcePath = getUpwardCCPath(source);
    vector<ConnectedComponent *> upwardCCTargetPath = getUpwardCCPath(target);
    popEveryCommonCCAncestors(upwardCCSourcePath, upwardCCTargetPath);
    const unsigned int lastCommonLevel = upwardCCSourcePath.size() - 1;

//    printCCPaths(upwardCCSourcePath, upwardCCTargetPath);

    vector<Vertex *> affectedVertices;
    set<VertexAndLevel, VertexAndLevelDijkstraComparator> Q;
    source->dist = 0;
    Q.insert({source, 0});

    while (!Q.empty()) {
        const VertexAndLevel triple = *Q.begin();
        Q.erase(triple);
        Vertex *const u = triple.vertex;
        const unsigned int uLevel = triple.level;
        affectedVertices.push_back(u);
        if (u == target) {
            break;
        }
        if (u->visited) {
            continue;
        }
        u->visited = true;

        if (uLevel < lastCommonLevel) { // i can go up
            if (upwardCCSourcePath[uLevel]->adjVertices.find(u) != upwardCCSourcePath[uLevel]->adjVertices.end()) {
                upwardEdgesDijkstra(triple, upwardCCSourcePath, Q);
            }
            Vertex *upper = u->upper;
            if (upper != nullptr && !upper->visited && u->dist < upper->dist) { // todo extract
                const VertexAndLevel upperTriple{upper, uLevel + 1u};
                Q.erase(upperTriple);
                upper->dist = u->dist;
                upper->parent = u;
                Q.insert(upperTriple);
            }
        }
        if (uLevel > 0) { // i can go down
            if (upwardCCTargetPath[uLevel]->adjVertices.find(u) != upwardCCTargetPath[uLevel]->adjVertices.end()) {
                downwardEdgesDijkstra(triple, upwardCCTargetPath, Q);
            }
            Vertex *lower = u->lower;
            if (lower != nullptr && !lower->visited && u->dist < lower->dist) { // todo extract
                const VertexAndLevel lowerTriple{lower, uLevel - 1u};
                Q.erase(lowerTriple);
                lower->dist = u->dist;
                lower->parent = u;
                Q.insert(lowerTriple);
            }
        }

        if (uLevel == lastCommonLevel) { // i can spread
//            if (sourceTopCCAdjVertices->find(triple.vertex) != sourceTopCCAdjVertices->end()) {
            levelEdgesDijkstra(triple, upwardCCTargetPath, Q);
//            }
        }
    }

//    printPathFromSourceToTarget(source, target);

    const ULL result = target->dist;

    for (Vertex *v : affectedVertices) {
        v->reset();
    }
    for (const VertexAndLevel &pair: Q) {
        pair.vertex->reset();
    }

//    for (ConnectedComponent *cc: upwardCCSourcePath) {
//        for (Vertex *v: *cc->adjVertices) {
//            v->reset();
//        }
//    }
//    for (ConnectedComponent *cc: upwardCCTargetPath) {
//        for (Vertex *v: *cc->adjVertices) {
//            v->reset();
//        }
//    }

    return result;
}

void MultilevelGraph::printCCPaths(const vector<ConnectedComponent *> &upwardCCSourcePath,
                                   const vector<ConnectedComponent *> &upwardCCTargetPath) const {
    cout << "Source CC path:\n";
    for (ConnectedComponent *cc: upwardCCSourcePath) {
        cc->print();
    }
    cout << "Target CC path:\n";
    for (ConnectedComponent *cc: upwardCCTargetPath) {
        cc->print();
    }
    cout << endl;
}

vector<ConnectedComponent *> MultilevelGraph::getUpwardCCPath(Vertex *vertex0lvl) {
    vector<ConnectedComponent *> V;
    ConnectedComponent *cc = (*vertex0lvl->adjCC.begin());
    while (cc != ConnectedComponent::NULL_OBJECT) {
        V.push_back(cc);
        cc = cc->parent;
    }

    return std::move(V);
}

void MultilevelGraph::popEveryCommonCCAncestors(vector<ConnectedComponent *> &v1, vector<ConnectedComponent *> &v2) {
    while (v1.back() == v2.back()) {
        v1.pop_back();
        v2.pop_back();
    }
}

void MultilevelGraph::levelEdgesDijkstra(const VertexAndLevel &triple,
                                         vector<ConnectedComponent *> &upwardCCTargetPath,
                                         set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    Vertex *const u = triple.vertex;
//    for (Vertex *v: *upwardCCTargetPath.back()->adjVertices) { // upwardCCTargetPath[triple.level + 0]
    for (auto e: u->levelEdges) {
        Vertex *v = e.first;
        const unordered_map<Vertex *, ULL>::const_iterator &destIt = u->levelEdges.find(v);
        if (v->visited || destIt == u->levelEdges.end()) {
            continue;
        }

        const ULL newDist = u->dist + destIt->second;
        if (newDist < v->dist) {
            const VertexAndLevel destTriple{v, triple.level}; //todo extract
            Q.erase(destTriple);
            v->dist = newDist;
            v->parent = u;
            Q.insert(destTriple);
        }
    }
}

void MultilevelGraph::upwardEdgesDijkstra(const VertexAndLevel &triple,
                                          const vector<ConnectedComponent *> &upwardCCSourcePath,
                                          set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    const unsigned int nextLevel = triple.level + 1u;
    Vertex *const u = triple.vertex;
    for (Vertex *v: upwardCCSourcePath[nextLevel]->adjVertices) {
        const unordered_map<Vertex *, ULL>::const_iterator &destIt = u->upwardEdges.find(v);
        if (v->visited || destIt == u->upwardEdges.end()) {
            continue;
        }

        const ULL newDist = u->dist + destIt->second;
        if (newDist < v->dist) {
            const VertexAndLevel destTriple{v, nextLevel}; // todo extract
            Q.erase(destTriple);
            v->dist = newDist;
            v->parent = u;
            Q.insert(destTriple);
        }
    }
}

void MultilevelGraph::downwardEdgesDijkstra(const VertexAndLevel &newTriple,
                                            const vector<ConnectedComponent *> &upwardCCTargetPath,
                                            set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    const unsigned int prevLvl = newTriple.level - 1u;
    Vertex *const u = newTriple.vertex;
    for (Vertex *v: upwardCCTargetPath[prevLvl]->adjVertices) {
        const unordered_map<Vertex *, ULL>::const_iterator &destIt = u->downwardEdges.find(v);
        if (v->visited || destIt == u->downwardEdges.end()) {
            continue;
        }

        const ULL newDist = u->dist + destIt->second;
        if (newDist < v->dist) {
            const VertexAndLevel destTriple{v, prevLvl};
            Q.erase(destTriple);
            v->dist = newDist;
            v->parent = u;
            Q.insert(destTriple);
        }
    }
}

ULL MultilevelGraph::regularDijkstra(Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ull;
    }

    set<Vertex *, VertexDijkstraComparator> Q;
    vector<Vertex *> affectedVertices;

    source->dist = 0;
    Q.insert(source);

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
        Q.erase(u);
        affectedVertices.push_back(u);
        if (u == target) {
            break;
        }
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
            if (newDist < dest->dist) {
                Q.erase(dest);
                dest->dist = newDist;
                dest->parent = u;
                Q.insert(dest);
            }
        }
    }

//    printPathFromSourceToTarget(source, target);

    const ULL result = target->dist;
    for (Vertex *v : affectedVertices) {
        v->reset();
    }
    while (!Q.empty()) { // todo speedup
        Vertex *u = *Q.begin();
        Q.erase(u);
        u->reset();
    }

    return result;
}

void MultilevelGraph::printPathFromSourceToTarget(Vertex *source, Vertex *target) {
    if (source != target) {
        printPathFromSourceToTarget(source, target->parent);
    }
    else {
        cout << endl;
    }
    cout << " -> " << target->id << "(" << target << ", " << target->dist << ")";
}
