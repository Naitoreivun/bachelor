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

ULL MultilevelGraph::calculateDistance(Vertex *source, Vertex *target) {
    cout << (*source->adjCC.begin())->id << endl;
    cout << (*target->adjCC.begin())->id << endl;
    return 0;
}
