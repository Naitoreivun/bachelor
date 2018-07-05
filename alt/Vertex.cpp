#include "Vertex.h"

Vertex::Vertex(const int id) : id(id) {
    fullReset();
}

bool VertexDijkstraDefaultComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->dist[FORWARD] == v2->dist[FORWARD]
           ? v1->id < v2->id
           : v1->dist[FORWARD] < v2->dist[FORWARD];
}


bool VertexDijkstraFComparatorForward::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->f[FORWARD] == v2->f[FORWARD]
           ? v1->id < v2->id
           : v1->f[FORWARD] < v2->f[FORWARD];
}

bool VertexDijkstraFComparatorBackward::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->f[BACKWARD] == v2->f[BACKWARD]
           ? v1->id < v2->id
           : v1->f[BACKWARD] < v2->f[BACKWARD];
}

