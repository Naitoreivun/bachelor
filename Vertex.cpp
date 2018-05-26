#include "Vertex.h"

Vertex::Vertex(int id) : id(id), visited(false), dist(0ull), parent(nullptr) {}

void Vertex::link(Vertex *dest, ULL weight, unordered_set<Edge *> &edges) {
    edges.insert(new Edge(dest, weight));
}

size_t VertexHasher::operator()(const Vertex *v) const {
    return hash<int>()(v->id);
}

bool VertexComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->id == v2->id;
}
