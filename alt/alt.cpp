#include <set>
#include <algorithm>
#include <chrono>
#include "alt.h"


Alt::Alt(vector<Vertex *> &graph, const int landmarksCount) : graph(&graph) {
    selectLandmarks(landmarksCount);

    for (int i = 0; i < landmarks.size(); ++i) {
        calculateLandmarkDistances(i, FROM);
        calculateLandmarkDistances(i, TO);
    }

    for (Vertex *v: graph) {
        v->fullReset();
        v->reversedEdges.clear(); // no more needed, release memory
    }
}

void Alt::selectLandmarks(const int landmarksCount) {
    vector<Vertex *> dummyLandmarkSingleton;
    dummyLandmarkSingleton.push_back(graph[0][0]);
    findNewFarthestLandmark(dummyLandmarkSingleton);
    for (int i = 1; i < landmarksCount; ++i) {
        findNewFarthestLandmark(landmarks);
    }
    for (int i = 0; i < landmarks.size(); ++i) {
        landmarkIds[landmarks[i]] = i;
    }
}

void Alt::findNewFarthestLandmark(const vector<Vertex *> &currentLandmarks) {
    set<Vertex *, VertexDijkstraDefaultComparator> Q;

    for (Vertex *landmark: currentLandmarks) {
        landmark->dist = 0;
        Q.insert(landmark);
    }

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
        Q.erase(u);
        if (u->visited) {
            continue;
        }
        u->visited = true;

        for (auto edge: u->edges) {
            Vertex *const dest = edge.first;
            if (dest->visited) {
                continue;
            }

            const LL newDist = u->dist + edge.second;
            if (newDist < dest->dist) {
                Q.erase(dest);
                dest->dist = newDist;
                Q.insert(dest);
            }
        }
    }

    Vertex *landmarkCandidate = nullptr;
    LL currentMaxDist = 0ll;

    for (Vertex *v: *graph) {
        if (v->dist > currentMaxDist && find(landmarks.begin(), landmarks.end(), v) == landmarks.end()) {
            landmarkCandidate = v;
            currentMaxDist = v->dist;
        }
        v->reset();
    }
    landmarks.push_back(landmarkCandidate);
}

void Alt::calculateLandmarkDistances(const int landmarkId, const int direction) {
    for (Vertex *vertex: *graph) {
        vertex->landmarkDist[direction].push_back(INF);
        vertex->visited = false;
    }
    landmarks[landmarkId]->landmarkDist[direction][landmarkId] = 0;

    auto comp = [&landmarkId, &direction](Vertex *v1, Vertex *v2) -> bool {
        const LL dist1 = v1->landmarkDist[direction][landmarkId];
        const LL dist2 = v2->landmarkDist[direction][landmarkId];
        return dist1 == dist2 ? v1->id < v2->id : dist1 < dist2;
    };
    auto Q = set<Vertex *, decltype(comp)>(comp);

    Q.insert(landmarks[landmarkId]);

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
        Q.erase(u);
        if (u->visited) {
            continue;
        }
        u->visited = true;

        const unordered_map<Vertex *, LL> &edges = direction == FROM ? u->edges : u->reversedEdges;
        for (auto edge: edges) {
            Vertex *const dest = edge.first;
            if (dest->visited) {
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


LL Alt::altDijkstra(Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ll;
    }

    const vector<int> &&activeLandmarkIds = selectActiveLandmarks(source, target);

    source->dist = 0;
    source->f = heuristic(source, target, activeLandmarkIds);
    set<Vertex *, VertexDijkstraFComparator> Q;
    vector<Vertex *> affectedVertices;
    Q.insert(source);

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
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
            Vertex *const dest = edge.first;
            if (dest->visited) {
                continue;
            }

            const LL tentativeDist = u->dist + edge.second;
            if (tentativeDist < dest->dist) {
                Q.erase(dest);
                dest->parent = u;
                dest->dist = tentativeDist;
                dest->f = dest->dist + heuristic(dest, target, activeLandmarkIds);
                Q.insert(dest);
            }
        }
    }

    const LL result = target->dist;
    for (Vertex *v : affectedVertices) {
        v->fullReset();
    }
    for (Vertex *v : Q) {
        v->fullReset();
    }

    return result;
}

LL Alt::regularDijkstra(Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ll;
    }

    set<Vertex *, VertexDijkstraDefaultComparator> Q;
    vector<Vertex *> affectedVertices;

    source->dist = 0;
    Q.insert(source);

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
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
            Vertex *const dest = edge.first;
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
    for (Vertex *v : affectedVertices) {
        v->reset();
    }
    for (Vertex *v : Q) {
        v->reset();
    }

    return result;
}
