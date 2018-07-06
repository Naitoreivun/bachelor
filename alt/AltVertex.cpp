#include "AltVertex.h"

AltVertex::AltVertex(const int id) : id(id) {
    fullReset();
}

bool AltVertexDijkstraDefaultComparator::operator()(const AltVertex *v1, const AltVertex *v2) const {
    return v1->dist[FORWARD] == v2->dist[FORWARD]
           ? v1->id < v2->id
           : v1->dist[FORWARD] < v2->dist[FORWARD];
}


bool AltVertexDijkstraFComparatorForward::operator()(const AltVertex *v1, const AltVertex *v2) const {
    return v1->f[FORWARD] == v2->f[FORWARD]
           ? v1->id < v2->id
           : v1->f[FORWARD] < v2->f[FORWARD];
}

bool AltVertexDijkstraFComparatorBackward::operator()(const AltVertex *v1, const AltVertex *v2) const {
    return v1->f[BACKWARD] == v2->f[BACKWARD]
           ? v1->id < v2->id
           : v1->f[BACKWARD] < v2->f[BACKWARD];
}

