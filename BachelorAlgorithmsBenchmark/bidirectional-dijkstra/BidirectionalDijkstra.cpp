#include "BidirectionalDijkstra.h"
#include <set>

LL bidirectionalDijkstra(vector<BidiVertex *> &graph, BidiVertex *source, BidiVertex *target) {
    if (source == target) {
        return 0ll;
    }

    set<BidiVertex *, BidiVertexDijkstraForwardComparator> forwardQueue;
    set<BidiVertex *, BidiVertexDijkstraBackwardComparator> backwardQueue;
    vector<BidiVertex *> affectedVertices;

    source->dist[FORWARD] = 0;
    target->dist[BACKWARD] = 0;

    forwardQueue.insert(source);
    backwardQueue.insert(target);

    int direction;
    BidiVertex *u;
    LL result = INF;

    while (!forwardQueue.empty() && !backwardQueue.empty()) {
        if ((*forwardQueue.begin())->dist[FORWARD] + (*backwardQueue.begin())->dist[BACKWARD] >= result) {
            break;
        }
        if ((*forwardQueue.begin())->dist[FORWARD] < (*backwardQueue.begin())->dist[BACKWARD]) {
            u = *forwardQueue.begin();
            direction = FORWARD;
            forwardQueue.erase(forwardQueue.begin());
        }
        else {
            u = *backwardQueue.begin();
            direction = BACKWARD;
            backwardQueue.erase(backwardQueue.begin());
        }
        affectedVertices.push_back(u);
        if (u->visited[direction]) {
            continue;
        }
        u->visited[direction] = true;

        for (auto edge: u->edges[direction]) {
            BidiVertex *const dest = edge.first;
            if (dest->visited[direction]) {
                continue;
            }
            const LL newDist = u->dist[direction] + edge.second;
            if (dest->visited[!direction]) {
                result = min(result, newDist + dest->dist[!direction]);
            }
            else if (newDist < dest->dist[direction]) {
                direction == FORWARD ? forwardQueue.erase(dest) : backwardQueue.erase(dest);
                dest->dist[direction] = newDist;
                dest->parent[direction] = u;
                direction == FORWARD ? forwardQueue.insert(dest) : backwardQueue.insert(dest);
            }
        }
    }

    for (BidiVertex *v: forwardQueue) {
        v->reset(FORWARD);
    }
    for (BidiVertex *v: backwardQueue) {
        v->reset(BACKWARD);
    }
    for (BidiVertex *v : affectedVertices) {
        v->reset();
    }

    return result;
}
