#ifndef BACHELOR_ALT_H
#define BACHELOR_ALT_H

#include "../imports_and_configs.h"
#include "Vertex.h"

struct Alt {
    unordered_set<Vertex *> landmarks;
    vector<Vertex*> *graph;

    Alt(vector<Vertex *> &graph, int landmarksCount);

    ULL altDijkstra(Vertex *source, Vertex *target);

    ULL regularDijkstra(Vertex *source, Vertex *target);

private:
    void selectLandmarks( int landmarksCount);

    void findNewFarthestLandmark(const unordered_set<Vertex *> &currentLandmarks);

    void calculateLandmarkDistances(Vertex *landmark,  int direction);

    ULL heuristic(Vertex *v, Vertex *t);

    inline vector<Vertex *> selectActiveLandmarks(Vertex *source, Vertex *target) {
//        vector<Vertex *> result = {*landmarks.begin(), *landmarks.begin()};
//
//        for (Vertex *landmark: landmarks) {
//            if ()
//        }
//
//        return move(result);
    }
};


#endif //BACHELOR_ALT_H
