#include "Vertex.h"

Vertex::Vertex(const int id) : id(id) {
    reset();
}

bool VertexDijkstraComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return *v1 < *v2;
}
