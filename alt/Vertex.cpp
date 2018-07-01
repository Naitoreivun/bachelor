#include "Vertex.h"

Vertex::Vertex(const int id) : id(id) {
    fullReset();
}

bool VertexDijkstraDefaultComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->dist == v2->dist
           ? v1->id < v2->id
           : v1->dist < v2->dist;
}


bool VertexDijkstraFComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->f == v2->f
           ? v1->id < v2->id
           : v1->f < v2->f;
}

