#ifndef BACHELOR_DIJKSTRA_H
#define BACHELOR_DIJKSTRA_H

#include "../imports_and_configs.h"
#include "RegularVertex.h"
#include <set>

LL dijkstra(vector<RegularVertex *> &graph, RegularVertex *source, RegularVertex *target) {
    if (source == target) {
        return 0ll;
    }

    set<RegularVertex *, RegularVertexDijkstraComparator> Q;
    vector<RegularVertex *> affectedVertices;

    source->dist = 0;
    Q.insert(source);

    while (!Q.empty()) {
        RegularVertex *u = *Q.begin();
        Q.erase(u);
        affectedVertices.push_back(u);
        if (u == target) {
            break;
        }
        if (u->visited) {
            continue;
        }
        u->visited = true;

        for (auto edge: u->edges) {
            RegularVertex *const dest = edge.first;
            if (dest->visited) {
                continue;
            }

            const LL newDist = u->dist + edge.second;
            if (newDist < dest->dist) {
                Q.erase(dest);
                dest->dist = newDist;
                dest->parent = u;
                Q.insert(dest);
            }
        }
    }

    const LL result = target->dist;
    for (RegularVertex *v : affectedVertices) {
        v->reset();
    }
    for (RegularVertex *v : Q) {
        v->reset();
    }

    return result;
}


#endif //BACHELOR_DIJKSTRA_H
