#ifndef BACHELOR_VERTEX_H
#define BACHELOR_VERTEX_H

#include "../imports_and_configs.h"

struct ConnectedComponent;

struct Vertex {
    static const int FORWARD = 0;
    static const int BACKWARD = 1;
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

    inline bool operator<(const Vertex &rhs) const {
        return dist == rhs.dist
               ? id < rhs.id
               : dist < rhs.dist;
    };

    inline void reset() {
        parent[FORWARD] = nullptr;
        dist[FORWARD] = INF;
        visited[FORWARD] = false;

        parent[BACKWARD] = nullptr;
        dist[BACKWARD] = INF;
        visited[BACKWARD] = false;
    }
};

struct VertexDijkstraComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

#endif //BACHELOR_VERTEX_H
