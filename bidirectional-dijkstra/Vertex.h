#ifndef BACHELOR_VERTEX_H
#define BACHELOR_VERTEX_H

#include "../imports_and_configs.h"

const int FORWARD = 0;
const int BACKWARD = 1;

struct Vertex {
    const int id;
    bool visited[2];
    ULL dist[2];
    Vertex *parent[2];
    unordered_map<Vertex *, ULL> edges[2];

    explicit Vertex(int id);

    inline void link(Vertex *dest, ULL weight) {
        edges[FORWARD][dest] = weight;
        dest->edges[BACKWARD][this] = weight;
    }

    inline void reset() {
        reset(FORWARD);
        reset(BACKWARD);
    }

    inline void reset(const int direction) {
        parent[direction] = nullptr;
        dist[direction] = INF;
        visited[direction] = false;
    }
};

struct VertexDijkstraForwardComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

struct VertexDijkstraBackwardComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

#endif //BACHELOR_VERTEX_H
