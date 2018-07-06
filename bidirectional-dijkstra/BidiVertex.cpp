#include "BidiVertex.h"

BidiVertex::BidiVertex(const int id) : id(id) {
    reset();
}

bool BidiVertexDijkstraForwardComparator::operator()(const BidiVertex *v1, const BidiVertex *v2) const {
    return v1->dist[FORWARD] == v2->dist[FORWARD]
           ? v1->id < v2->id
           : v1->dist[FORWARD] < v2->dist[FORWARD];
}

bool BidiVertexDijkstraBackwardComparator::operator()(const BidiVertex *v1, const BidiVertex *v2) const {
    return v1->dist[BACKWARD] == v2->dist[BACKWARD]
           ? v1->id < v2->id
           : v1->dist[BACKWARD] < v2->dist[BACKWARD];
}
