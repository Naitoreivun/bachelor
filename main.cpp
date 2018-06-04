#include <iostream>
#include <fstream>
#include <algorithm>
#include "MultilevelGraph.h"

int n, m;
vector<Vertex *> originalVertices;
vector<int> degrees;
vector<int> levelSizes;

void init();

vector<Vertex *> getVerticesSortedByDegree();

int main() {
    cout << "start" << endl;
    init();
    cout << "original graph loaded" << endl;

    auto verticesSortedByDegree = getVerticesSortedByDegree();

    MultilevelGraph M(originalVertices);
    for (int i = 1; i < levelSizes.size(); ++i) {
        M.addLevel(vector<Vertex *>(verticesSortedByDegree.begin(), verticesSortedByDegree.begin() + levelSizes[i]));
    }
    M.prepareVerticesForQueries();

    M.printAll();
    M.printConnectedComponents();

    cout << M.calculateDistance(originalVertices[0], originalVertices[10]) << endl; // 1 -> 11
    cout << M.calculateDistance(originalVertices[10], originalVertices[0]) << endl; // 11 -> 1
    cout << M.calculateDistance(originalVertices[6], originalVertices[10]) << endl; // 7 -> 11
    cout << M.calculateDistance(originalVertices[3], originalVertices[10]) << endl; // 4 -> 11
    cout << M.calculateDistance(originalVertices[2], originalVertices[9]) << endl; // 3 -> 10
    cout << M.calculateDistance(originalVertices[6], originalVertices[4]) << endl; // 7 -> 5

    cout << "finish";
    return 0;
}

void init() {
//    const char *const filename = "../tests/USairport500.in";
    const char *const filename = "../tests/test.in";
//    const char *const filename = "../tests/test2.in";
    fstream file(filename);

    if (file.good()) {
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
            originalVertices.push_back(new Vertex(i));
            degrees.push_back(0);
        }

        int source, dest;
        ULL weight;
        for (int i = 0; i < m; ++i) {
            file >> source >> dest >> weight;
            if (source != dest) {
                originalVertices[source - 1]->link(originalVertices[dest - 1], weight);
                ++degrees[source];
                ++degrees[dest];
            }
        }
        file.close();
    }
    else {
        cout << "Cannot open file: '" << filename << "'." << endl;
    }
}

vector<Vertex *> getVerticesSortedByDegree() {
    vector<Vertex *> result = originalVertices;
    sort(result.begin(), result.end(), [](Vertex *a, Vertex *b) -> bool { return degrees[a->id] > degrees[b->id]; });
//    for (Vertex *v: result) {
//        cout << v->id << ": " << degrees[v->id] << endl;
//    }
    return result;
}