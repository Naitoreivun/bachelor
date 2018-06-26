#include "Vertex.h"

Vertex::Vertex(const int id) : id(id) {
    reset();
}

bool VertexDijkstraForwardComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->dist[FORWARD] == v2->dist[FORWARD]
           ? v1->id < v2->id
           : v1->dist[FORWARD] < v2->dist[FORWARD];
}

bool VertexDijkstraBackwardComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->dist[BACKWARD] == v2->dist[BACKWARD]
           ? v1->id < v2->id
           : v1->dist[BACKWARD] < v2->dist[BACKWARD];
}
