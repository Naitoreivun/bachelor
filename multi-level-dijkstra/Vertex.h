#ifndef BACHELOR_VERTEX_H
#define BACHELOR_VERTEX_H

#include "../imports_and_configs.h"
#include "ConnectedComponent.h"

struct ConnectedComponent;

struct Vertex {
    const int id;
    bool visited;
    LL dist;
    Vertex *parent;
    unordered_map<Vertex *, LL> levelEdges;
    unordered_set<Vertex *> reversedLevelEdges; // useful for finding connected components
    unordered_map<Vertex *, LL> upwardEdges;
    unordered_map<Vertex *, LL> downwardEdges;
    unordered_set<ConnectedComponent *> adjCC;
    Vertex *upper;
    Vertex *lower;

    explicit Vertex(int id);

    inline void link(Vertex *dest, LL weight) {
        link(dest, weight, levelEdges);
    }

    inline void linkBackAndForth(Vertex *dest, LL weight) {
        link(dest, weight, levelEdges);
        dest->reversedLevelEdges.insert(this);
    }

    inline void linkUp(Vertex *dest, LL weight) {
        link(dest, weight, upwardEdges);
    }

    inline void linkDown(Vertex *dest, LL weight) {
        link(dest, weight, downwardEdges);
    }

//    void addParentAdjVertexForEveryAdjCC(Vertex *vertex);

    bool operator<(const Vertex &rhs) const;

    inline void reset() {
        parent = nullptr;
        dist = INF;
        visited = false;
    }

private:
    void link(Vertex *dest, LL weight, unordered_map<Vertex *, LL> &edges);
};

// http://thispointer.com/how-to-use-unordered_set-with-user-defined-classes-tutorial-example/
struct VertexHasher {
    size_t operator()(const Vertex *v) const;
};

struct VertexComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

// https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/27216842#27216842
// https://stackoverflow.com/a/27216842
//struct SetHasher {
//    size_t operator()(const unordered_set<Vertex *> *us) const;
//};
//
//struct SetComparator {
//    bool operator()(const unordered_set<Vertex *> *s1, const unordered_set<Vertex *> *s2) const;
//};

struct VertexDijkstraComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

#endif //BACHELOR_VERTEX_H
