#ifndef BACHELOR_ALT_H
#define BACHELOR_ALT_H

#include "../imports_and_configs.h"
#include "Vertex.h"

struct Alt {
    unordered_set<Vertex *> landmarks;

    Alt(vector<Vertex *> &graph, int landmarksCount);

    void calculateLandmarkDistances(vector<Vertex *> &graph, Vertex *landmark, int direction);

    ULL altDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target);

    ULL regularDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target);

private:
    ULL heuristic(Vertex *v, Vertex *t);
};


#endif //BACHELOR_ALT_H
