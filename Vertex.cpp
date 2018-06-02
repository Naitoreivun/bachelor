#include "Vertex.h"

Vertex::Vertex(int id) : id(id), visited(false), dist(0ull), parent(nullptr) {}

void Vertex::link(Vertex *dest, ULL weight, unordered_set<Edge *> &edges) {
    edges.insert(new Edge(dest, weight));
}

void Vertex::addParentAdjVertexForEveryAdjCC(Vertex *const vertex) {
    for (ConnectedComponent *cc: adjCC) {
        cc->parentAdjVertices->insert(vertex);
    }
}

size_t VertexHasher::operator()(const Vertex *v) const {
    return hash<int>()(v->id);
}

bool VertexComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return v1->id == v2->id;
}

size_t SetHasher::operator()(const unordered_set<Vertex *> *us) const {
    size_t seed = us->size();
    for (Vertex *v : *us) {
        seed ^= hash<Vertex *>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

bool SetComparator::operator()(const unordered_set<Vertex *> *s1, const unordered_set<Vertex *> *s2) const {
    return *s1 == *s2;
}
