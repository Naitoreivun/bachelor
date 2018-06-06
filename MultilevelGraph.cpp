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
//    for (Vertex *v: selectedVertices) {
//        cout << v->id << "(" << v << ")" << endl;
//    }

    VertexSet vertexSet;
    for (Vertex *vertex: selectedVertices) {
        vertexSet.insert(new Vertex(vertex->id));
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

void MultilevelGraph::prepareVerticesForQueries() {
    for (Level &level: levels) {
        for (Vertex *v: level.selectedVertices) {
            v->reset();
        }
    }
    for (int i = 1; i < levels.size(); ++i) {
        for (Vertex *v: levels[i].selectedVertices) {
            Vertex *lowerV = *levels[i - 1].selectedVertices.find(v);
            v->downwardEdges[lowerV] = 0ull;
            lowerV->upwardEdges[v] = 0ull;
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
    auto sourceTopCCAdjVertices = upwardCCSourcePath.back()->adjVertices;
    auto targetTopCCAdjVertices = upwardCCTargetPath.back()->adjVertices;

    cout << "Source CC path:\n";
    for (ConnectedComponent *cc: upwardCCSourcePath) {
        cc->print();
    }
    cout << "Target CC path:\n";
    for (ConnectedComponent *cc: upwardCCTargetPath) {
        cc->print();
    }
    cout << endl;

    set<Triple, TripleDijkstraComparator> Q;
    source->dist = 0;
    Q.insert({source, UP, 1});


    while (!Q.empty()) {
        const Triple triple = *Q.begin();
        Q.erase(triple);
        if (triple.vertex == target) {
            break;
        }
        if (triple.vertex->visited) {
            continue;
        }
        triple.vertex->visited = true;

        if (triple.direction == UP) {
            if (triple.cc == upwardCCSourcePath.size()) {
                if (sourceTopCCAdjVertices->find(triple.vertex) != sourceTopCCAdjVertices->end()) {
                    levelEdgesDijkstra(triple, upwardCCTargetPath, Q);
                }
                if (targetTopCCAdjVertices->find(triple.vertex) != targetTopCCAdjVertices->end()) {
                    const Triple newTriple{triple.vertex, DOWN, upwardCCTargetPath.size() - 2u};
                    downwardEdgesDijkstra(newTriple, upwardCCTargetPath, Q);
                }
            }
            else {
                upwardEdgesDijkstra(triple, upwardCCSourcePath, Q);
            }
        }
        else { // triple.direction == DOWN
            downwardEdgesDijkstra(triple, upwardCCTargetPath, Q);
        }
    }

    const ULL result = target->dist;

    for (ConnectedComponent *cc: upwardCCSourcePath) {
        for (Vertex *v: *cc->adjVertices) {
            v->reset();
        }
    }
    for (ConnectedComponent *cc: upwardCCTargetPath) {
        for (Vertex *v: *cc->adjVertices) {
            v->reset();
        }
    }

    return result;
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

void MultilevelGraph::levelEdgesDijkstra(const Triple &triple,
                                         vector<ConnectedComponent *> &upwardCCTargetPath,
                                         set<Triple, TripleDijkstraComparator> &Q) const {
    Vertex *const u = triple.vertex;
    for (Vertex *v: *upwardCCTargetPath.back()->adjVertices) {
        auto destIt = u->levelEdges.find(v);
        if (v->visited || destIt == u->levelEdges.end()) {
            continue;
        }

        const ULL newDist = u->dist + destIt->second;
        if (newDist < v->dist) {
            const Triple destTriple{v, UP, triple.cc};
            Q.erase(destTriple);
            v->dist = newDist;
            v->parent = u;
            Q.insert(destTriple);
        }
    }
}

void MultilevelGraph::upwardEdgesDijkstra(const Triple &triple,
                                          const vector<ConnectedComponent *> &upwardCCSourcePath,
                                          set<Triple, TripleDijkstraComparator> &Q) const {
    Vertex *const u = triple.vertex;
    for (Vertex *v: *upwardCCSourcePath[triple.cc]->adjVertices) {
        auto destIt = u->upwardEdges.find(v);
        if (v->visited || destIt == u->upwardEdges.end()) {
            continue;
        }

        const ULL newDist = u->dist + destIt->second;
        if (newDist < v->dist) {
            const Triple destTriple{v, UP, triple.cc + 1u};
            Q.erase(destTriple);
            v->dist = newDist;
            v->parent = u;
            Q.insert(destTriple);
        }
    }
}

void MultilevelGraph::downwardEdgesDijkstra(const Triple &newTriple,
                                            const vector<ConnectedComponent *> &upwardCCTargetPath,
                                            set<Triple, TripleDijkstraComparator> &Q) const {
    Vertex *const u = newTriple.vertex;
    for (Vertex *v: *upwardCCTargetPath[newTriple.cc]->adjVertices) {
        auto destIt = u->downwardEdges.find(v);
        if (v->visited || destIt == u->downwardEdges.end()) {
            continue;
        }

        const ULL newDist = u->dist + destIt->second;
        if (newDist < v->dist) {
            const Triple destTriple{v, DOWN, newTriple.cc - 1u};
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

    const ULL result = target->dist;
    for (Vertex *v : affectedVertices) {
        v->reset();
    }
    while (!Q.empty()) {
        Vertex *u = *Q.begin();
        Q.erase(u);
        u->reset();
    }

    return result;
}
