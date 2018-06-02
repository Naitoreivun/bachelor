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
            for (Edge *edge: u->upwardEdges) {
                cout << " " << edge->dest->id << "(" << edge->weight << ")";
            }
            cout << "\n\tlevel:";
            for (Edge *edge: u->levelEdges) {
                cout << " " << edge->dest->id << "(" << edge->weight << ")";
            }
            cout << "\n\tdownward:";
            for (Edge *edge: u->downwardEdges) {
                cout << " " << edge->dest->id << "(" << edge->weight << ")";
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
