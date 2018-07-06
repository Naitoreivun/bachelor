#ifndef BACHELOR_DIJKSTRAPROGRAM_H
#define BACHELOR_DIJKSTRAPROGRAM_H

#include <string>
#include <fstream>
#include <iostream>
#include "RegularVertex.h"
#include "Dijkstra.h"

using namespace std;

struct DijkstraProgram {
    int n, m;
    vector<RegularVertex *> graph;

    explicit DijkstraProgram(const string &pathToGraph) {
        fstream file(pathToGraph);

        if (!file.good()) {
            cout << "Cannot open file: '" << pathToGraph << "'." << endl;
            return;
        }

        file >> n >> m;

        int unusedLevelsCount;
        file >> unusedLevelsCount;
        for (int i = 1, unusedLevelSize; i <= unusedLevelsCount; ++i) {
            file >> unusedLevelSize;
        }

        for (int i = 1; i <= n; ++i) {
            graph.push_back(new RegularVertex(i));
        }

        int source, dest;
        LL weight;
        for (int i = 0; i < m; ++i) {
            file >> source >> dest >> weight;
            if (source != dest) {
                graph[source - 1]->edges[graph[dest - 1]] = weight;
            }
        }
        file.close();
    }

    const vector<LL> processQueries(vector<pair<int, int>> &queries) {
        vector<LL> results;
        for (pair<int, int> &query: queries) {
            results.push_back(dijkstra(graph, graph[query.first - 1], graph[query.second - 1]));
        }
        return results;
    }
};

#endif //BACHELOR_DIJKSTRAPROGRAM_H
