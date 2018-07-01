#include "BidirectionalDijkstra.h"
#include <set>

LL bidirectionalDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ll;
    }

    set<Vertex *, VertexDijkstraForwardComparator> forwardQueue;
    set<Vertex *, VertexDijkstraBackwardComparator> backwardQueue;
    vector<Vertex *> affectedVertices;

    source->dist[FORWARD] = 0;
    target->dist[BACKWARD] = 0;

    forwardQueue.insert(source);
    backwardQueue.insert(target);

    int direction;
    Vertex *u;
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
        if (u->visited[direction]) { // todo try swap
            continue;
        }
//        if (u->visited[!direction]) {
//            break;
//        }
        u->visited[direction] = true;

        for (auto edge: u->edges[direction]) {
            Vertex *const dest = edge.first;
            if (dest->visited[direction]) {
                continue;
            }

            const LL newDist = u->dist[direction] + edge.second;
            if (newDist < dest->dist[direction]) {
                direction == FORWARD ? forwardQueue.erase(dest) : backwardQueue.erase(dest);
                dest->dist[direction] = newDist;
                dest->parent[direction] = u;
                direction == FORWARD ? forwardQueue.insert(dest) : backwardQueue.insert(dest);
            }
            if (dest->visited[!direction]) {
                result = min(result, newDist + dest->dist[!direction]); // todo at the beginning
            }
        }
    }

    while (!forwardQueue.empty()) { // todo speedup
        Vertex *u = *forwardQueue.begin();
        forwardQueue.erase(forwardQueue.begin());
        u->reset(FORWARD);
    }
    while (!backwardQueue.empty()) { // todo speedup
        Vertex *u = *backwardQueue.begin();
        backwardQueue.erase(backwardQueue.begin());
        u->reset(BACKWARD);
    }
    for (Vertex *v : affectedVertices) {
        v->reset();
    }

    return result;
}

LL regularDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ll;
    }

    set<Vertex *, VertexDijkstraForwardComparator> Q;
    vector<Vertex *> affectedVertices;

    source->dist[FORWARD] = 0;
    Q.insert(source);

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
        Q.erase(u);
        affectedVertices.push_back(u);
        if (u == target) {
            break;
        }
        if (u->visited[FORWARD]) {
            continue;
        }
        u->visited[FORWARD] = true;

        for (auto edge: u->edges[FORWARD]) {
            Vertex *const dest = edge.first;
            if (dest->visited[FORWARD]) {
                continue;
            }

            const LL newDist = u->dist[FORWARD] + edge.second;
            if (newDist < dest->dist[FORWARD]) {
                Q.erase(dest);
                dest->dist[FORWARD] = newDist;
                dest->parent[FORWARD] = u;
                Q.insert(dest);
            }
        }
    }

    const LL result = target->dist[FORWARD];
    for (Vertex *v : affectedVertices) {
        v->reset(FORWARD);
    }
    while (!Q.empty()) { // todo speedup
        Vertex *u = *Q.begin();
        Q.erase(Q.begin());
        u->reset();
    }

    return result;
}
