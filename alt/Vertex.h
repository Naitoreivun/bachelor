#ifndef BACHELOR_VERTEX_H
#define BACHELOR_VERTEX_H

#include "../imports_and_configs.h"

const int FROM = 0;
const int TO = 1;

struct Vertex {
    const int id;
    bool visited;
    ULL dist;
    ULL f;
    Vertex *parent;
    unordered_map<Vertex *, ULL> edges;
    unordered_map<Vertex *, ULL> reversedEdges;
    unordered_map<Vertex *, ULL> landmarkDist[2];

    explicit Vertex(int id);

    inline void link(Vertex *dest, ULL weight) {
        edges[dest] = weight;
        dest->reversedEdges[this] = weight;
    }

    inline void fullReset() {
        f = INF;
        reset();
    }

    inline void reset() {
        parent = nullptr;
        dist = INF;
        visited = false;
    }
};

struct VertexDijkstraDefaultComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

struct VertexDijkstraFComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

#endif //BACHELOR_VERTEX_H
