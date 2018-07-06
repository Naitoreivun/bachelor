#include <set>
#include <algorithm>
#include "alt.h"


Alt::Alt(vector<AltVertex *> &graph, const int landmarksCount) : graph(&graph) {
    selectLandmarks(landmarksCount);

    for (int i = 0; i < landmarks.size(); ++i) {
        calculateLandmarkDistances(i, FROM);
        calculateLandmarkDistances(i, TO);
    }

    for (AltVertex *v: graph) {
        v->fullReset();
    }
}

void Alt::selectLandmarks(const int landmarksCount) {
    vector<AltVertex *> dummyLandmarkSingleton;
    dummyLandmarkSingleton.push_back(graph[0][0]);
    findNewFarthestLandmark(dummyLandmarkSingleton);
    for (int i = 1; i < landmarksCount; ++i) {
        findNewFarthestLandmark(landmarks);
    }
    for (int i = 0; i < landmarks.size(); ++i) {
        landmarkIds[landmarks[i]] = i;
    }
}

void Alt::findNewFarthestLandmark(const vector<AltVertex *> &currentLandmarks) {
    set<AltVertex *, AltVertexDijkstraDefaultComparator> Q;

    for (AltVertex *landmark: currentLandmarks) {
        landmark->dist[FORWARD] = 0;
        Q.insert(landmark);
    }

    while (!Q.empty()) {
        AltVertex *u = *Q.begin();
        Q.erase(u);
        if (u->visited[FORWARD]) {
            continue;
        }
        u->visited[FORWARD] = true;

        for (auto edge: u->edges[FORWARD]) {
            AltVertex *const dest = edge.first;
            if (dest->visited[FORWARD]) {
                continue;
            }

            const LL newDist = u->dist[FORWARD] + edge.second;
            if (newDist < dest->dist[FORWARD]) {
                Q.erase(dest);
                dest->dist[FORWARD] = newDist;
                Q.insert(dest);
            }
        }
    }

    AltVertex *landmarkCandidate = nullptr;
    LL currentMaxDist = 0ll;

    for (AltVertex *v: *graph) {
        if (v->dist[FORWARD] > currentMaxDist && find(landmarks.begin(), landmarks.end(), v) == landmarks.end()) {
            landmarkCandidate = v;
            currentMaxDist = v->dist[FORWARD];
        }
        v->reset(FORWARD);
    }
    landmarks.push_back(landmarkCandidate);
}

void Alt::calculateLandmarkDistances(const int landmarkId, const int direction) {
    for (AltVertex *vertex: *graph) {
        vertex->landmarkDist[direction].push_back(INF);
        vertex->visited[FORWARD] = false;
    }
    landmarks[landmarkId]->landmarkDist[direction][landmarkId] = 0;

    auto comp = [&landmarkId, &direction](AltVertex *v1, AltVertex *v2) -> bool {
        const LL dist1 = v1->landmarkDist[direction][landmarkId];
        const LL dist2 = v2->landmarkDist[direction][landmarkId];
        return dist1 == dist2 ? v1->id < v2->id : dist1 < dist2;
    };
    auto Q = set<AltVertex *, decltype(comp)>(comp);

    Q.insert(landmarks[landmarkId]);

    while (!Q.empty()) {
        AltVertex *u = *Q.begin();
        Q.erase(u);
        if (u->visited[FORWARD]) {
            continue;
        }
        u->visited[FORWARD] = true;

        for (auto edge: u->edges[direction]) {
            AltVertex *const dest = edge.first;
            if (dest->visited[FORWARD]) {
                continue;
            }

            const LL newDist = u->landmarkDist[direction][landmarkId] + edge.second;
            if (newDist < dest->landmarkDist[direction][landmarkId]) {
                Q.erase(dest);
                dest->landmarkDist[direction][landmarkId] = newDist;
                Q.insert(dest);
            }
        }
    }
}


LL Alt::altDijkstra(AltVertex *source, AltVertex *target) {
    if (source == target) {
        return 0ll;
    }

    const vector<int> &&activeLandmarkIds = selectActiveLandmarks(source, target);

    source->dist[FORWARD] = 0;
    source->f[FORWARD] = heuristic(source, target, activeLandmarkIds);
    set<AltVertex *, AltVertexDijkstraFComparatorForward> Q;
    vector<AltVertex *> affectedVertices;
    Q.insert(source);

    while (!Q.empty()) {
        AltVertex *u = *Q.begin();
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
            AltVertex *const dest = edge.first;
            if (dest->visited[FORWARD]) {
                continue;
            }

            const LL tentativeDist = u->dist[FORWARD] + edge.second;
            if (tentativeDist < dest->dist[FORWARD]) {
                Q.erase(dest);
                dest->parent[FORWARD] = u;
                dest->dist[FORWARD] = tentativeDist;
                dest->f[FORWARD] = dest->dist[FORWARD] + heuristic(dest, target, activeLandmarkIds);
                Q.insert(dest);
            }
        }
    }

    const LL result = target->dist[FORWARD];
    for (AltVertex *v : affectedVertices) {
        v->fullReset(FORWARD);
    }
    for (AltVertex *v : Q) {
        v->fullReset(FORWARD);
    }

    return result;
}

LL Alt::bidirectionalAltDijkstra(AltVertex *source, AltVertex *target) {
    if (source == target) {
        return 0ll;
    }
    const vector<int> &&activeLandmarkIds = selectActiveLandmarks(source, target);

    set<AltVertex *, AltVertexDijkstraFComparatorForward> forwardQueue;
    set<AltVertex *, AltVertexDijkstraFComparatorBackward> backwardQueue;
    vector<AltVertex *> affectedVertices;

    source->dist[FORWARD] = target->dist[BACKWARD] = 0;
    source->f[FORWARD] = target->f[BACKWARD] = heuristic(source, target, activeLandmarkIds);
    forwardQueue.insert(source);
    backwardQueue.insert(target);

    int direction;
    AltVertex *u;
    LL result = INF;

    while (!forwardQueue.empty() && !backwardQueue.empty()) {
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
        if (u->f[direction] >= result) {
            break;
        }
        if (u->visited[direction]) {
            continue;
        }
        u->visited[direction] = true;

        for (auto edge: u->edges[direction]) {
            AltVertex *const dest = edge.first;
            if (dest->visited[direction]) {
                continue;
            }
            const LL tentativeDist = u->dist[direction] + edge.second;

            if (dest->visited[!direction]) {
                result = min(result, tentativeDist + dest->dist[!direction]);
            }
            else if (tentativeDist < dest->dist[direction]) {
                direction == FORWARD ? forwardQueue.erase(dest) : backwardQueue.erase(dest);
                dest->dist[direction] = tentativeDist;
                dest->parent[direction] = u;
                if (direction == FORWARD) {
                    dest->f[direction] = dest->dist[direction] + heuristic(dest, target, activeLandmarkIds);
                    forwardQueue.insert(dest);
                }
                else {
                    dest->f[direction] = dest->dist[direction] + heuristic(source, dest, activeLandmarkIds);
                    backwardQueue.insert(dest);
                }
            }
        }
    }

    for (AltVertex *v: forwardQueue) {
        v->fullReset(FORWARD);
    }
    for (AltVertex *v: backwardQueue) {
        v->fullReset(BACKWARD);
    }
    for (AltVertex *v : affectedVertices) {
        v->fullReset();
    }

    return result;
}
