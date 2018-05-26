#ifndef BACHELOR_VERTEX_H
#define BACHELOR_VERTEX_H

#include "imports_and_configs.h"
#include "Edge.h"

struct Edge;

struct Vertex {
    const int id;
    bool visited;
    ULL dist;
    Vertex *parent;
    unordered_set<Edge *> levelEdges;
    unordered_set<Edge *> upwardEdges;
    unordered_set<Edge *> downwardEdges;

    explicit Vertex(int id);

    inline void link(Vertex *dest, ULL weight) {
        link(dest, weight, levelEdges);
    }

    inline void linkUp(Vertex *dest, ULL weight) {
        link(dest, weight, upwardEdges);
    }

    inline void linkDown(Vertex *dest, ULL weight) {
        link(dest, weight, downwardEdges);
    }

private:
    void link(Vertex *dest, ULL weight, unordered_set<Edge *> &edges);
};

// http://thispointer.com/how-to-use-unordered_set-with-user-defined-classes-tutorial-example/
struct VertexHasher {
    size_t operator()(const Vertex *v) const;
};

struct VertexComparator {
    bool operator()(const Vertex *v1, const Vertex *v2) const;
};

#endif //BACHELOR_VERTEX_H
