#ifndef BACHELOR_BIDI_VERTEX_H
#define BACHELOR_BIDI_VERTEX_H

#include "../imports_and_configs.h"

struct BidiVertex {
    const int id;
    bool visited[2];
    LL dist[2];
    BidiVertex *parent[2];
    unordered_map<BidiVertex *, LL> edges[2];

    explicit BidiVertex(int id);

    inline void link(BidiVertex *dest, LL weight) {
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

struct BidiVertexDijkstraForwardComparator {
    bool operator()(const BidiVertex *v1, const BidiVertex *v2) const;
};

struct BidiVertexDijkstraBackwardComparator {
    bool operator()(const BidiVertex *v1, const BidiVertex *v2) const;
};

#endif //BACHELOR_BIDI_VERTEX_H
