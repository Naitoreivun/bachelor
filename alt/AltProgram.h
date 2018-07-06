#ifndef BACHELOR_ALTPROGRAM_H
#define BACHELOR_ALTPROGRAM_H

#include <string>
#include <fstream>
#include <iostream>
#include "AltVertex.h"
#include "alt.h"

using namespace std;

struct AltProgram {
    int n, m;
    vector<AltVertex *> graph;
    const int DEFAULT_LANDMARKS_COUNT = 16;
    int landmarksCount;
    Alt *alt;

    explicit AltProgram(const string &pathToGraph) {
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
            graph.push_back(new AltVertex(i));
        }

        int source, dest;
        LL weight;
        for (int i = 0; i < m; ++i) {
            file >> source >> dest >> weight;
            if (source != dest) {
                graph[source - 1]->link(graph[dest - 1], weight);
            }
        }
        file.close();

        landmarksCount = min(DEFAULT_LANDMARKS_COUNT, n / 4);
    }

    virtual ~AltProgram() {
        delete alt;
    }

    void preprocessing() {
        alt = new Alt(graph, landmarksCount);
    }

    const vector<LL> processQueries(vector<pair<int, int>> &queries) {
        vector<LL> results;
        for (pair<int, int> &query: queries) {
            results.push_back(alt->altDijkstra(graph[query.first - 1], graph[query.second - 1]));
        }
        return results;
    }

    vector<LL> processQueriesBidirectional(vector<pair<int, int>> &queries) {
        vector<LL> results;
        for (pair<int, int> &query: queries) {
            results.push_back(alt->bidirectionalAltDijkstra(graph[query.first - 1], graph[query.second - 1]));
        }
        return results;
    }

};

#endif //BACHELOR_ALTPROGRAM_H
