#include "Vertex.h"

Vertex::Vertex(int id) : id(id), visited(false), dist(0ll), parent(nullptr), upper(nullptr), lower(nullptr) {}

void Vertex::link(Vertex *dest, LL weight, unordered_map<Vertex *, LL> &edges) {
    edges[dest] = weight;
}

bool Vertex::operator<(const Vertex &rhs) const {
    return dist == rhs.dist
           ? id < rhs.id
           : dist < rhs.dist;
}

size_t VertexHasher::operator()(const Vertex *v) const {
    return hash<int>()(v->id);
}

bool VertexComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->id == v2->id;
}

bool VertexDijkstraComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return *v1 < *v2;
}
