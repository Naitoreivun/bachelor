#ifndef BACHELOR_MULTILEVELGRAPH_H
#define BACHELOR_MULTILEVELGRAPH_H

#include "imports_and_configs.h"
#include "Level.h"

enum Direction {
    UP = 0, DOWN = 1
};

struct Triple {
    Vertex *vertex;
    Direction direction;
    unsigned int cc;
public:
    Triple(Vertex *vertex, Direction direction, unsigned int cc) : vertex(vertex), direction(direction), cc(cc) {}
};

struct TripleDijkstraComparator {
    bool operator()(const Triple &t1, const Triple &t2) const {
        Vertex *const v1 = t1.vertex;
        Vertex *const v2 = t2.vertex;
        return v1->dist == v2->dist
               ? (v1->id == v2->id
                  ? t1.cc < t2.cc
                  : v1->id < v2->id)
               : v1->dist < v2->dist;
    }
};

struct MultilevelGraph {
    vector<Level> levels;

    explicit MultilevelGraph(const vector<Vertex *> &originalVertices);

    void addLevel(const vector<Vertex *> &selectedVertices);

    void printAll();

    void printConnectedComponents();

    void prepareVerticesForQueries();

    ULL calculateDistance(Vertex *source, Vertex *target);

    ULL regularDijkstra(Vertex *source, Vertex *target);

private:
    vector<ConnectedComponent *> getUpwardCCPath(Vertex *vertex0lvl);

    void popEveryCommonCCAncestors(vector<ConnectedComponent *> &v1, vector<ConnectedComponent *> &v2);

    void levelEdgesDijkstra(const Triple &triple,
                            vector<ConnectedComponent *> &upwardCCTargetPath,
                            set<Triple, TripleDijkstraComparator> &Q) const;

    void upwardEdgesDijkstra(const Triple &triple,
                             const vector<ConnectedComponent *> &upwardCCSourcePath,
                             set<Triple, TripleDijkstraComparator> &Q) const;

    void downwardEdgesDijkstra(const Triple &newTriple,
                               const vector<ConnectedComponent *> &upwardCCTargetPath,
                               set<Triple, TripleDijkstraComparator> &Q) const;
};

#endif //BACHELOR_MULTILEVELGRAPH_H
