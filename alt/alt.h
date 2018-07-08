#ifndef BACHELOR_ALT_H
#define BACHELOR_ALT_H

#include "../imports_and_configs.h"
#include "AltVertex.h"

struct Alt {
    vector<AltVertex *> landmarks;
    unordered_map<AltVertex *, int> landmarkIds;
    vector<AltVertex *> *graph;

    Alt(vector<AltVertex *> &graph, int landmarksCount);

    LL altDijkstra(AltVertex *source, AltVertex *target);

    LL bidirectionalAltDijkstra(AltVertex *source, AltVertex *target);

private:
    void selectLandmarks(int landmarksCount);

    void findNewFarthestLandmark(const vector<AltVertex *> &currentLandmarks);

    void calculateLandmarkDistances(int landmarkId, int direction);

    inline vector<int> selectActiveLandmarks(AltVertex *source, AltVertex *target) {
        vector<int> result = {0, 0};

        for (unsigned landmarkId = 1; landmarkId < landmarks.size(); ++landmarkId) {
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

    inline LL heuristic(AltVertex *s, AltVertex *t, const vector<int> &activeLandmarkIds) {
        if (s == t) {
            return 0ll;
        }

        LL result = 0ll;
        for (int landmarkId: activeLandmarkIds) {
            const LL est = max(t->landmarkDist[FROM][landmarkId] - s->landmarkDist[FROM][landmarkId],
                               s->landmarkDist[TO][landmarkId] - t->landmarkDist[TO][landmarkId]);

            if (est > result && est != INF) {
                result = est;
            }
        }

        return result;
    }
};


#endif //BACHELOR_ALT_H
