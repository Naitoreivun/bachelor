#ifndef BACHELOR_MLDPROGRAM_H
#define BACHELOR_MLDPROGRAM_H

#include <string>
#include <fstream>
#include <iostream>
#include "Vertex.h"
#include "MultilevelGraph.h"

using namespace std;

struct MLDProgram {
    int n, m;
    vector<Vertex *> graph;
    vector<int> degrees;
    vector<int> levelSizes;
    MultilevelGraph *M;

    explicit MLDProgram(const string &pathToGraph) {
        fstream file(pathToGraph);

        if (!file.good()) {
            cout << "Cannot open file: '" << pathToGraph << "'." << endl;
            return;
        }

        file >> n >> m;

        int levelsCount;
        file >> levelsCount;
        levelSizes.push_back(n);
        for (int i = 1, levelSize; i <= levelsCount; ++i) {
            file >> levelSize;
            levelSizes.push_back(levelSize);
        }

        degrees.push_back(0);
        for (int i = 1; i <= n; ++i) {
            graph.push_back(new Vertex(i));
            degrees.push_back(0);
        }

        int source, dest;
        LL weight;
        for (int i = 0; i < m; ++i) {
            file >> source >> dest >> weight;
            if (source != dest) {
                graph[source - 1]->linkBackAndForth(graph[dest - 1], weight);
                ++degrees[source];
                ++degrees[dest];
            }
        }
        file.close();
    }

    virtual ~MLDProgram() {
        delete M;
    }

    void preprocessing() {
        auto verticesSortedByDegree = getVerticesSortedByDegree();
        M = new MultilevelGraph(graph);
        for (int i = 1; i < levelSizes.size(); ++i) {
            M->addLevel(
                    vector<Vertex *>(verticesSortedByDegree.begin(), verticesSortedByDegree.begin() + levelSizes[i]));
        }
        M->createConnectedComponents();
        M->prepareVerticesForQueries();
    }

    vector<Vertex *> getVerticesSortedByDegree() {
        vector<Vertex *> result;
        auto comp = [this](Vertex *a, Vertex *b) -> bool {
            return degrees[a->id] == degrees[b->id]
                   ? a->id < b->id
                   : degrees[a->id] > degrees[b->id];
        };
        auto Q = set<Vertex *, decltype(comp)>(comp);
        for (Vertex *v:graph) {
            Q.insert(v);
        }

        while (!Q.empty()) {
            Vertex *top = *Q.begin();
            result.push_back(top);
            Q.erase(Q.begin());

            for (auto edge: top->levelEdges) {
                Vertex *v = edge.first;
                if (Q.find(v) != Q.end()) {
                    Q.erase(v);
                    --degrees[v->id];
                    Q.insert(v);
                }
            }
            for (auto edge: top->reversedLevelEdges) {
                Vertex *v = edge.first;
                if (Q.find(v) != Q.end()) {
                    Q.erase(v);
                    --degrees[v->id];
                    Q.insert(v);
                }
            }
        }

        return result;
    }


    const vector<LL> processQueries(vector<pair<int, int>> &queries) {
        vector<LL> results;
        for (pair<int, int> &query: queries) {
            results.push_back(M->calculateDistance(graph[query.first - 1], graph[query.second - 1]));
        }
        return results;
    }
};

#endif //BACHELOR_MLDPROGRAM_H
