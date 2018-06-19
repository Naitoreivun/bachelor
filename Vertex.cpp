#include "Vertex.h"

Vertex::Vertex(int id) : id(id), visited(false), dist(0ull), parent(nullptr), upper(nullptr), lower(nullptr) {}

void Vertex::link(Vertex *dest, ULL weight, unordered_map<Vertex *, ULL> &edges) {
    edges[dest] = weight;
}

void Vertex::addParentAdjVertexForEveryAdjCC(Vertex *const vertex) {
    for (ConnectedComponent *cc: adjCC) {
        cc->parentAdjVertices->insert(vertex);
    }
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

size_t SetHasher::operator()(const unordered_set<Vertex *> *us) const {
    size_t seed = us->size();
    for (Vertex *v : *us) {
//        seed ^= hash<Vertex *>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // fixme
        seed += hash<Vertex *>()(v);
    }
    return seed;
}

bool SetComparator::operator()(const unordered_set<Vertex *> *s1, const unordered_set<Vertex *> *s2) const {
    return *s1 == *s2;
}

bool VertexDijkstraComparator::operator()(const Vertex *v1, const Vertex *v2) const {
    return *v1 < *v2;
}
