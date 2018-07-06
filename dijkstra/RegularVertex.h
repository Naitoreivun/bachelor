#ifndef BACHELOR_REGULARVERTEX_H
#define BACHELOR_REGULARVERTEX_H

#include "../imports_and_configs.h"

struct RegularVertex {
    const int id;
    bool visited;
    LL dist;
    RegularVertex *parent;
    unordered_map<RegularVertex *, LL> edges;

    explicit RegularVertex(int id) : id(id) {
        reset();
    }

    inline void reset() {
        parent = nullptr;
        dist = INF;
        visited = false;
    }
};

struct RegularVertexDijkstraComparator {
    bool operator()(const RegularVertex *v1, const RegularVertex *v2) const {
        return v1->dist == v2->dist
               ? v1->id < v2->id
               : v1->dist < v2->dist;
    }
};

#endif //BACHELOR_REGULARVERTEX_H
