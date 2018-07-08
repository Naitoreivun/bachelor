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
                  ? t1.level < t2.level
                  : v1->id < v2->id)
               : v1->dist < v2->dist;
    }
};

struct MultilevelGraph {
    vector<Level*> levels;

    explicit MultilevelGraph(const vector<Vertex *> &originalVertices);

    virtual ~MultilevelGraph();

    void addLevel(const vector<Vertex *> &selectedVertices);

    void prepareVerticesForQueries();

    LL calculateDistance(Vertex *source, Vertex *target);

    LL regularDijkstra(Vertex *source, Vertex *target);

    void createConnectedComponents();

private:
    inline vector<ConnectedComponent *> getUpwardCCPath(Vertex *vertex0lvl);

    inline void popEveryCommonCCAncestors(vector<ConnectedComponent *> &v1, vector<ConnectedComponent *> &v2);

    inline void relaxUpperOrLowerVertex(Vertex *u, Vertex *anotherVertex, unsigned int anotherLevel,
                                        set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    inline void levelEdgesDijkstra(const VertexAndLevel &triple,
                                   vector<ConnectedComponent *> &upwardCCTargetPath,
                                   set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    inline void upwardEdgesDijkstra(const VertexAndLevel &triple,
                                    const vector<ConnectedComponent *> &upwardCCSourcePath,
                                    set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    inline void downwardEdgesDijkstra(const VertexAndLevel &newTriple,
                                      const vector<ConnectedComponent *> &upwardCCTargetPath,
                                      set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    inline void upwardOrDownwardEdgesDijkstra(Vertex *u,
                                              const unordered_map<Vertex *, LL> &uEdges,
                                              unsigned int anotherLevel,
                                              const vector<ConnectedComponent *> &upwardCCPath,
                                              set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    inline void relax(Vertex *u, Vertex *v, LL uvDist, unsigned int anotherLevel,
                      set<VertexAndLevel, VertexAndLevelDijkstraComparator> &Q) const;

    void createConnectedComponentsForLevelZero();

    void createConnectedComponentsForLevel(int levelValue);

    void createConnectedComponent(Vertex *vertex, Level *prevLevel, Level *currentLevel);

    void processAdjVertexInBfs(Level *currentLevel, queue<Vertex *> &Q, ConnectedComponent *cc, Vertex *v);

    void duplicateConnectedComponentsWithNoParent(Level *prevLevel, Level *currentLevel);
};

#endif //BACHELOR_MULTILEVELGRAPH_H
