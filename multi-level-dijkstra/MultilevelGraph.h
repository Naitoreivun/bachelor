#ifndef BACHELOR_MULTILEVELGRAPH_H
#define BACHELOR_MULTILEVELGRAPH_H

#include "../imports_and_configs.h"
#include "Level.h"

struct VertexAndLevel {
    Vertex *vertex;
    unsigned int level;
public:
    VertexAndLevel(Vertex *vertex, unsigned int level) : vertex(vertex), level(level) {}
};

struct VertexAndLevelDijkstraComparator {
    bool operator()(const VertexAndLevel &t1, const VertexAndLevel &t2) const {
        Vertex *const v1 = t1.vertex;
        Vertex *const v2 = t2.vertex;
        return v1->dist == v2->dist
               ? (v1->id == v2->id
                  ? t1.level < t2.level //todo speedup
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

    void createConnectedComponents();

private:
    vector<ConnectedComponent *> getUpwardCCPath(Vertex *vertex0lvl);

    void popEveryCommonCCAncestors(vector<ConnectedComponent *> &v1, vector<ConnectedComponent *> &v2);

    void levelEdgesDijkstra(const VertexAndLevel &triple,
                            vector<ConnectedComponent *> &upwardCCTargetPath,
                            set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    void upwardEdgesDijkstra(const VertexAndLevel &triple,
                             const vector<ConnectedComponent *> &upwardCCSourcePath,
                             set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    void downwardEdgesDijkstra(const VertexAndLevel &newTriple,
                               const vector<ConnectedComponent *> &upwardCCTargetPath,
                               set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    void printCCPaths(const vector<ConnectedComponent *> &upwardCCSourcePath,
                      const vector<ConnectedComponent *> &upwardCCTargetPath) const;

    void printPathFromSourceToTarget(Vertex *source, Vertex *target);

    void createConnectedComponentsForLevelZero();

    void createConnectedComponentsForLevel(int levelValue);

    void createConnectedComponent(Vertex *vertex, Level &prevLevel, Level &currentLevel);

    void processAdjVertexInBfs(Level &currentLevel, queue<Vertex *> &Q, ConnectedComponent *cc, Vertex *v);

    void duplicateConnectedComponentsWithNoParent(Level &prevLevel, Level &currentLevel);
};

#endif //BACHELOR_MULTILEVELGRAPH_H
