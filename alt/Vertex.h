#ifndef BACHELOR_VERTEX_H
#define BACHELOR_VERTEX_H

#include "../imports_and_configs.h"

const int FROM = 0;
const int TO = 1;

const int FORWARD = 0;
const int BACKWARD = 1;
// todo data struct
struct Vertex {
    const int id;
    vector<LL> landmarkDist[2]; // FROM / TO

    // FORWARD / BACKWARD (for bidirectional version)
    bool visited[2];
    LL dist[2];
    LL f[2];
    Vertex *parent[2];
    unordered_map<Vertex *, LL> edges[2];

    explicit Vertex(int id);

    inline void link(Vertex *dest, LL weight) {
        edges[FORWARD][dest] = weight;
        dest->edges[BACKWARD][this] = weight;
    }
    
    inline void fullReset() {
        fullReset(FORWARD);
        fullReset(BACKWARD);
    }
    
    inline void fullReset(const int direction) {
        f[direction] = INF;
        reset(direction);
    }

    inline void reset(const int direction) {
        parent[direction] = nullptr;
        dist[direction] = INF;
        visited[direction] = false;
    }
};

struct VertexDijkstraDefaultComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

struct VertexDijkstraFComparatorForward {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

struct VertexDijkstraFComparatorBackward {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

#endif //BACHELOR_VERTEX_H
