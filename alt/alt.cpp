#include <set>
#include <algorithm>
#include <chrono>
#include "alt.h"


Alt::Alt(vector<Vertex *> &graph, const int landmarksCount) : graph(&graph) {
    // dummy picking
    selectLandmarks(landmarksCount);

    for (Vertex *landmark: landmarks) {
        calculateLandmarkDistances(landmark, FROM);
        calculateLandmarkDistances(landmark, TO);
    }

    for (Vertex *v: graph) {
        v->fullReset();
        v->reversedEdges.clear(); // no more needed, release memory
    }
}

void Alt::selectLandmarks(const int landmarksCount) {
    unordered_set<Vertex *> dummyLandmarkSingleton;
    dummyLandmarkSingleton.insert(graph[0][0]);
    findNewFarthestLandmark(dummyLandmarkSingleton);
    for (int i = 1; i < landmarksCount; ++i) {
        findNewFarthestLandmark(landmarks);
    }
}

void Alt::findNewFarthestLandmark(const unordered_set<Vertex *> &currentLandmarks) {
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

            const ULL newDist = u->dist + edge.second;
            if (newDist < dest->dist) {
                Q.erase(dest);
                dest->dist = newDist;
                Q.insert(dest);
            }
        }
    }

    Vertex *landmarkCandidate = nullptr;
    ULL currentMaxDist = 0ull;

    for (Vertex *v: *graph) {
        if (v->dist > currentMaxDist && landmarks.find(v) == landmarks.end()) {
            landmarkCandidate = v;
            currentMaxDist = v->dist;
        }
        v->reset();
    }
    landmarks.insert(landmarkCandidate);
}

void Alt::calculateLandmarkDistances(Vertex *landmark, const int direction) {
    for (Vertex *vertex: *graph) {
        vertex->landmarkDist[direction][landmark] = INF;
        vertex->visited = false;
    }
    landmark->landmarkDist[direction][landmark] = 0;

    auto comp = [&landmark, &direction](Vertex *v1, Vertex *v2) -> bool {
        const ULL dist1 = v1->landmarkDist[direction][landmark];
        const ULL dist2 = v2->landmarkDist[direction][landmark];
        return dist1 == dist2 ? v1->id < v2->id : dist1 < dist2;
    };
    auto Q = set<Vertex *, decltype(comp)>(comp);

    Q.insert(landmark);

    while (!Q.empty()) {
        Vertex *u = *Q.begin();
        Q.erase(u);
        if (u->visited) {
            continue;
        }
        u->visited = true;

        const unordered_map<Vertex *, ULL> &edges = direction == FROM ? u->edges : u->reversedEdges;
        for (auto edge: edges) {
            Vertex *const dest = edge.first;
            if (dest->visited) {
                continue;
            }

            const ULL newDist = u->landmarkDist[direction][landmark] + edge.second;
            if (newDist < dest->landmarkDist[direction][landmark]) {
                Q.erase(dest);
                dest->landmarkDist[direction][landmark] = newDist;
                Q.insert(dest);
            }
        }
    }
}


ULL Alt::altDijkstra(Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ull;
    }

//    vector<Vertex *> activeLandmarks = selectActiveLandmarks(source, target);

    source->dist = 0;
    source->f = heuristic(source, target);
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

            const ULL tentativeDist = u->dist + edge.second;
            if (tentativeDist < dest->dist) {
                Q.erase(dest);
                dest->parent = u;
                dest->dist = tentativeDist;
                dest->f = dest->dist + heuristic(dest, target);
                Q.insert(dest);
            }
        }
    }

    const ULL result = target->dist;
    for (Vertex *v : affectedVertices) {
        v->fullReset();
    }
    while (!Q.empty()) { // todo speedup
        Vertex *u = *Q.begin();
        Q.erase(Q.begin());
        u->fullReset();
    }

    return result;
}

ULL Alt::heuristic(Vertex *v, Vertex *t) {
    if (v == t) {
        return 0ull;
    }

    if (t->landmarkDist[FROM].find(v) != t->landmarkDist[FROM].end()) { //landmarks contains todo
        return t->landmarkDist[FROM][v];
    }

    if (v->landmarkDist[TO].find(t) != v->landmarkDist[TO].end()) { //landmarks contains todo
        return v->landmarkDist[TO][t];
    }

    ULL result = 0ull;
    for (Vertex *landmark: landmarks) {
        const ULL fromDiff = t->landmarkDist[FROM][landmark] > v->landmarkDist[FROM][landmark]
                             ? t->landmarkDist[FROM][landmark] - v->landmarkDist[FROM][landmark]
                             : 0;
        const ULL toDiff = v->landmarkDist[TO][landmark] > t->landmarkDist[TO][landmark]
                           ? v->landmarkDist[TO][landmark] - t->landmarkDist[TO][landmark]
                           : 0;

        result = max(result, max(
                fromDiff,
                toDiff
        ));
    }
    return result;
}

ULL Alt::regularDijkstra(Vertex *source, Vertex *target) {
    if (source == target) {
        return 0ull;
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

            const ULL newDist = u->dist + edge.second;
            if (newDist < dest->dist) {
                Q.erase(dest);
                dest->dist = newDist;
                dest->parent = u;
                Q.insert(dest);
            }
        }
    }

    const ULL result = target->dist;
    for (Vertex *v : affectedVertices) {
        v->reset();
    }
    while (!Q.empty()) { // todo speedup
        Vertex *u = *Q.begin();
        Q.erase(Q.begin());
        u->reset();
    }

    return result;
}
