#ifndef BACHELOR_ALT_H
#define BACHELOR_ALT_H

#include "../imports_and_configs.h"
#include "Vertex.h"

struct Alt {
    vector<Vertex *> landmarks;
    unordered_map<Vertex *, int> landmarkIds;
    vector<Vertex *> *graph;

    Alt(vector<Vertex *> &graph, int landmarksCount);

    LL altDijkstra(Vertex *source, Vertex *target);

    LL regularDijkstra(Vertex *source, Vertex *target);

private:
    void selectLandmarks(int landmarksCount);

    void findNewFarthestLandmark(const vector<Vertex *> &currentLandmarks);

    void calculateLandmarkDistances(int landmarkId, int direction);

    inline vector<int> selectActiveLandmarks(Vertex *source, Vertex *target) {
        vector<int> result = {0, 0};

        for (int landmarkId = 1; landmarkId < landmarks.size(); ++landmarkId) {
            if (target->landmarkDist[FROM][landmarkId] - source->landmarkDist[FROM][landmarkId] >
                target->landmarkDist[FROM][result[FROM]] - source->landmarkDist[FROM][result[FROM]]) {
                result[FROM] = landmarkId;
            }
            if (source->landmarkDist[TO][landmarkId] - target->landmarkDist[TO][landmarkId] >
                source->landmarkDist[TO][result[TO]] - target->landmarkDist[TO][result[TO]]) {
                result[TO] = landmarkId;
            }
        }

        return result;
    }

    inline LL heuristic(Vertex *v, Vertex *t, const vector<int> &activeLandmarkIds) {
        if (v == t) {
            return 0ll;
        }

//    auto vLandmarkIt = landmarkIds.find(v);
//    if (vLandmarkIt != landmarkIds.end()) {
//        return t->landmarkDist[FROM][vLandmarkIt->second];
//    }
//
//    auto tLandmarkIt = landmarkIds.find(t);
//    if (tLandmarkIt != landmarkIds.end()) {
//        return v->landmarkDist[TO][tLandmarkIt->second];
//    }

        LL result = 0ll;
        for (int landmarkId: activeLandmarkIds) {
            const LL fromDiff = t->landmarkDist[FROM][landmarkId] - v->landmarkDist[FROM][landmarkId];
            const LL toDiff = v->landmarkDist[TO][landmarkId] - t->landmarkDist[TO][landmarkId];

            if (fromDiff > toDiff) {
                if (fromDiff > result) {
                    result = fromDiff;
                }
            }
            else if (toDiff > result) {
                result = toDiff;
            }
        }

        return result;
    }
};


#endif //BACHELOR_ALT_H
