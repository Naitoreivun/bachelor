#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "MultilevelGraph.h"
#include "Level.h"
#include "Vertex.h"
#include "Edge.h"

int n, m;
vector<Vertex *> originalVertices;
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

    return 0;
}

void init() {
    const char *const filename = "../USairport500.txt";
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

        for (int i = 0; i <= n; ++i) {
            originalVertices.push_back(new Vertex(i)); // 0 is dummy
        }

        int source, dest;
        LL weight;
        for (int i = 0; i < m; ++i) {
            file >> source >> dest >> weight;
            originalVertices[source]->link(originalVertices[dest], weight);
        }
        file.close();
    }
    else {
        cout << "Cannot open file: '" << filename << "'." << endl;
    }
}

vector<Vertex *> getVerticesSortedByDegree() {
    auto result = originalVertices;
    sort(result.begin(), result.end(), [](Vertex *a, Vertex *b) -> bool { return a->degree() > b->degree(); });
//    for (Vertex *v: result) {
//        cout << v->id << ": " << v->degree() << endl;
//    }
    return result;
}