#ifndef BACHELOR_BIDIRECTIONALDIJKSTRA_H
#define BACHELOR_BIDIRECTIONALDIJKSTRA_H

#include "../imports_and_configs.h"
#include "Vertex.h"

LL bidirectionalDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target);
LL regularDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target);

#endif //BACHELOR_BIDIRECTIONALDIJKSTRA_H
