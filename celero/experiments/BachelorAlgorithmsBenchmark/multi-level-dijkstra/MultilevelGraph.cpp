#include <iostream>
#include "MultilevelGraph.h"

MultilevelGraph::MultilevelGraph(const vector<Vertex *> &originalVertices) {
    VertexSet vertexSet;
    for (Vertex *vertex: originalVertices) {
        vertexSet.insert(vertex);
    }
    levels.push_back(new Level(0, vertexSet));
}

void MultilevelGraph::addLevel(const vector<Vertex *> &selectedVertices) {
    VertexSet vertexSet;
    for (Vertex *vertex: selectedVertices) {
        vertexSet.insert(new Vertex(vertex->id));
    }

    levels.push_back(new Level(vertexSet, levels.back()));
}

void MultilevelGraph::createConnectedComponents() {
    createConnectedComponentsForLevelZero();
    for (unsigned i = 1; i < levels.size(); ++i) {
        createConnectedComponentsForLevel(i);
    }
}

void MultilevelGraph::createConnectedComponentsForLevelZero() {
    Level *level0 = levels.front();
    for (Vertex *v: level0->selectedVertices) {
        auto *cc = new ConnectedComponent();
        cc->adjVertices.insert(v);
        v->adjCC.insert(cc);
        level0->connectedComponents.insert(cc);
    }
}

void MultilevelGraph::createConnectedComponentsForLevel(int levelValue) {
    Level *levelZero = levels.front();
    Level *currentLevel = levels[levelValue];
    Level *prevLevel = levels[levelValue - 1];

    levelZero->resetVertices();

    for (Vertex *vertex: levelZero->selectedVertices) {
        if (!vertex->visited && !currentLevel->isSelectedVertex(vertex)) {
            createConnectedComponent(vertex, prevLevel, currentLevel);
        }
    }

    duplicateConnectedComponentsWithNoParent(prevLevel, currentLevel);
}

void MultilevelGraph::createConnectedComponent(Vertex *vertex, Level *prevLevel, Level *currentLevel) {
    queue<Vertex *> Q;
    vertex->visited = true;
    Q.push(vertex);
    ConnectedComponent *cc = new ConnectedComponent();

    while (!Q.empty()) {
        Vertex *u = Q.front();
        Q.pop();

        auto prevUIt = prevLevel->selectedVertices.find(u);
        if (prevUIt != prevLevel->selectedVertices.end()) {
            for (ConnectedComponent *prevCC: (*prevUIt)->adjCC) {
                prevCC->parent = cc;
            }
        }

        for (auto edge: u->levelEdges) {
            processAdjVertexInBfs(currentLevel, Q, cc, edge.first);
        }
        for (auto edge: u->reversedLevelEdges) {
            processAdjVertexInBfs(currentLevel, Q, cc, edge.first);
        }
    }

    currentLevel->connectedComponents.insert(cc);
}

inline void MultilevelGraph::processAdjVertexInBfs(Level *currentLevel,
                                                   queue<Vertex *> &Q,
                                                   ConnectedComponent *cc,
                                                   Vertex *v) {
    if (currentLevel->isSelectedVertex(v)) {
        currentLevel->bindCCWithSelectedVertex(cc, v);
    }
    else if (!v->visited) {
        Q.push(v);
        v->visited = true;
    }
}

void MultilevelGraph::duplicateConnectedComponentsWithNoParent(Level *prevLevel, Level *currentLevel) {
    for (ConnectedComponent *prevCC: prevLevel->connectedComponents) {
        if (prevCC->parent == ConnectedComponent::ROOT) {
            ConnectedComponent *cc = new ConnectedComponent();
            prevCC->parent = cc;
            for (Vertex *prevAdjVertex: prevCC->adjVertices) {
                currentLevel->bindCCWithSelectedVertex(cc, prevAdjVertex);
            }
            currentLevel->connectedComponents.insert(cc);
        }
    }
}

void MultilevelGraph::prepareVerticesForQueries() {
    for (Level *level: levels) {
        level->resetVertices();
    }
    for (unsigned i = 1; i < levels.size(); ++i) {
        for (Vertex *v: levels[i]->selectedVertices) {
            Vertex *lowerV = *levels[i - 1]->selectedVertices.find(v);
            v->lower = lowerV;
            lowerV->upper = v;
        }
    }
}

LL MultilevelGraph::calculateDistance(Vertex *source, Vertex *target) {
    if ((*source->adjCC.begin())->parent == (*target->adjCC.begin())->parent) {
        return regularDijkstra(source, target);
    }

    vector<ConnectedComponent *> upwardCCSourcePath = getUpwardCCPath(source);
    vector<ConnectedComponent *> upwardCCTargetPath = getUpwardCCPath(target);
    popEveryCommonCCAncestors(upwardCCSourcePath, upwardCCTargetPath);
    const unsigned int lastCommonLevel = upwardCCSourcePath.size() - 1;

    vector<Vertex *> affectedVertices;
    set<VertexAndLevel, VertexAndLevelDijkstraComparator> Q;
    source->dist = 0;
    Q.insert({source, 0});

    while (!Q.empty()) {
        const VertexAndLevel vl = *Q.begin();
        Q.erase(vl);
        Vertex *const u = vl.vertex;
        const unsigned int uLevel = vl.level;
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
                upwardEdgesDijkstra(vl, upwardCCSourcePath, Q);
            }
            relaxUpperOrLowerVertex(u, u->upper, uLevel + 1u, Q);
        }
        if (uLevel > 0) { // i can go down
            if (upwardCCTargetPath[uLevel]->adjVertices.find(u) != upwardCCTargetPath[uLevel]->adjVertices.end()) {
                downwardEdgesDijkstra(vl, upwardCCTargetPath, Q);
            }
            relaxUpperOrLowerVertex(u, u->lower, uLevel - 1u, Q);
        }

        if (uLevel == lastCommonLevel) { // i can spread
            levelEdgesDijkstra(vl, upwardCCTargetPath, Q);
        }
    }

    const LL result = target->dist;

    for (Vertex *v : affectedVertices) {
        v->reset();
    }
    for (const VertexAndLevel &pair: Q) {
        pair.vertex->reset();
    }

    return result;
}

inline void MultilevelGraph::relaxUpperOrLowerVertex(Vertex *const u,
                                                     Vertex *anotherVertex,
                                                     const unsigned int anotherLevel,
                                                     set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    if (anotherVertex != nullptr && !anotherVertex->visited && u->dist < anotherVertex->dist) {
        const VertexAndLevel lowerVL{anotherVertex, anotherLevel};
        Q.erase(lowerVL);
        anotherVertex->dist = u->dist;
        anotherVertex->parent = u;
        Q.insert(lowerVL);
    }
}

inline vector<ConnectedComponent *> MultilevelGraph::getUpwardCCPath(Vertex *vertex0lvl) {
    vector<ConnectedComponent *> V;
    ConnectedComponent *cc = (*vertex0lvl->adjCC.begin());
    while (cc != ConnectedComponent::NULL_OBJECT) {
        V.push_back(cc);
        cc = cc->parent;
    }

    return std::move(V);
}

inline void MultilevelGraph::popEveryCommonCCAncestors(vector<ConnectedComponent *> &v1,
                                                       vector<ConnectedComponent *> &v2) {
    while (v1.back() == v2.back()) {
        v1.pop_back();
        v2.pop_back();
    }
}

inline void MultilevelGraph::upwardEdgesDijkstra(const VertexAndLevel &vl,
                                                 const vector<ConnectedComponent *> &upwardCCSourcePath,
                                                 set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    upwardOrDownwardEdgesDijkstra(vl.vertex, vl.vertex->upwardEdges, vl.level + 1u, upwardCCSourcePath, Q);
}

inline void MultilevelGraph::downwardEdgesDijkstra(const VertexAndLevel &vl,
                                                   const vector<ConnectedComponent *> &upwardCCTargetPath,
                                                   set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    upwardOrDownwardEdgesDijkstra(vl.vertex, vl.vertex->downwardEdges, vl.level - 1u, upwardCCTargetPath, Q);
}

inline void MultilevelGraph::levelEdgesDijkstra(const VertexAndLevel &vl,
                                                vector<ConnectedComponent *> &upwardCCTargetPath,
                                                set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    for (auto edge: vl.vertex->levelEdges) {
        if (edge.first->visited) {
            continue;
        }
        relax(vl.vertex, edge.first, edge.second, vl.level, Q);
    }
}

inline void MultilevelGraph::upwardOrDownwardEdgesDijkstra(Vertex *u,
                                                           const unordered_map<Vertex *, LL> &uEdges,
                                                           const unsigned int anotherLevel,
                                                           const vector<ConnectedComponent *> &upwardCCPath,
                                                           set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    for (Vertex *v: upwardCCPath[anotherLevel]->adjVertices) {
        const unordered_map<Vertex *, LL>::const_iterator &destIt = uEdges.find(v);
        if (v->visited || destIt == uEdges.end()) {
            continue;
        }
        relax(u, v, destIt->second, anotherLevel, Q);
    }
}

inline void MultilevelGraph::relax(Vertex *u, Vertex *v, const LL uvDist, unsigned int anotherLevel,
                                   set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const {
    const LL newDist = u->dist + uvDist;
    if (newDist < v->dist) {
        const VertexAndLevel destVL{v, anotherLevel};
        Q.erase(destVL);
        v->dist = newDist;
        v->parent = u;
        Q.insert(destVL);
    }
}

LL MultilevelGraph::regularDijkstra(Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ll;
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

            const LL newDist = u->dist + edge.second;
            if (newDist < dest->dist) {
                Q.erase(dest);
                dest->dist = newDist;
                dest->parent = u;
                Q.insert(dest);
            }
        }
    }

    const LL result = target->dist;
    for (Vertex *v : affectedVertices) {
        v->reset();
    }
    for (Vertex *v : Q) {
        v->reset();
    }

    return result;
}

MultilevelGraph::~MultilevelGraph() {
    for (Level *l: levels) {
        delete l;
    }
}
