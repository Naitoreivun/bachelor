#ifndef BACHELOR_BIDIRECTIONALDIJKSTRA_H
#define BACHELOR_BIDIRECTIONALDIJKSTRA_H

#include "../imports_and_configs.h"
#include "Vertex.h"

ULL bidirectionalDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target);
ULL regularDijkstra(vector<Vertex *> &graph, Vertex *source, Vertex *target);

#endif //BACHELOR_BIDIRECTIONALDIJKSTRA_H
