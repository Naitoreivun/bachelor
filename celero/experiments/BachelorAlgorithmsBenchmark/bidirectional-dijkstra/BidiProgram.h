#ifndef BACHELOR_BIDIPROGRAM_H
#define BACHELOR_BIDIPROGRAM_H

#include <string>
#include <fstream>
#include "BidiVertex.h"
#include "BidirectionalDijkstra.h"

using namespace std;

struct BidiProgram {
    int n, m;
    vector<BidiVertex *> graph;

    virtual ~BidiProgram() {
        for (BidiVertex *v: graph) {
            delete v;
        }
        graph.clear();
    }

    explicit BidiProgram(const string &pathToGraph) {
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
            graph.push_back(new BidiVertex(i));
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
    }

    const vector<LL> processQueries(vector<pair<int, int>> &queries) {
        vector<LL> results;
        for (pair<int, int> &query: queries) {
            results.push_back(bidirectionalDijkstra(graph, graph[query.first - 1], graph[query.second - 1]));
        }
        return results;
    }
};

#endif //BACHELOR_BIDIPROGRAM_H
