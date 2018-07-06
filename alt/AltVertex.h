#ifndef BACHELOR_ALT_VERTEX_H
#define BACHELOR_ALT_VERTEX_H

#include "../imports_and_configs.h"

struct AltVertex {
    const int id;
    vector<LL> landmarkDist[2]; // FROM / TO

    // FORWARD / BACKWARD (for bidirectional version)
    bool visited[2];
    LL dist[2];
    LL f[2];
    AltVertex *parent[2];
    unordered_map<AltVertex *, LL> edges[2];

    explicit AltVertex(int id);

    inline void link(AltVertex *dest, LL weight) {
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

struct AltVertexDijkstraDefaultComparator {
    bool operator()(const AltVertex *v1, const AltVertex *v2) const;
};

struct AltVertexDijkstraFComparatorForward {
    bool operator()(const AltVertex *v1, const AltVertex *v2) const;
};

struct AltVertexDijkstraFComparatorBackward {
    bool operator()(const AltVertex *v1, const AltVertex *v2) const;
};

#endif //BACHELOR_ALT_VERTEX_H
