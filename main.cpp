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

bool calculateDistance(int v1, int v2, MultilevelGraph &M);

void calculate(MultilevelGraph &M);

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
//    calculateDistance(1, 11, M);
//    calculateDistance(11, 1, M);
//    calculateDistance(7, 11, M);
//    calculateDistance(4, 11, M);
//    calculateDistance(3, 10, M);
//    calculateDistance(7, 5, M);
//    calculateDistance(5, 7, M);
//    calculateDistance(1, 500, M);
//
//    calculate(M);

    calculateDistance(4, 5, M); // USairport500.in airportDebug2
//    calculateDistance(4, 3, M); // airportDebug2
//    calculateDistance(4, 254, M); // USairport500.in
//    calculateDistance(5, 11, M);

    cout << "finish" << endl;
    return 0;
}

void calculate(MultilevelGraph &M) {
    bool ok = true;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            ok &= calculateDistance(i, j, M);
        }
        cout << i << " -> " << ok << "\n";
    }
    cout << "\n" << (ok ? "Tests passed" : "Some tests failed") << endl;
}

void init() {
//    const char *const filename = "../tests/airportDebug.in";
//    const char *const filename = "../tests/airportDebug2.in";
    const char *const filename = "../tests/airportDebug3.in";
//    const char *const filename = "../tests/USairport500.in";
//    const char *const filename = "../tests/test.in";
//    const char *const filename = "../tests/test2.in";
//    const char *const filename = "../tests/test3.in";
//    const char *const filename = "../tests/test4.in";
//    const char *const filename = "../tests/pile.in";
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

bool calculateDistance(int v1, int v2, MultilevelGraph &M) {
    const auto source = originalVertices[v1 - 1];
    const auto target = originalVertices[v2 - 1];
    const auto multi = M.calculateDistance(source, target);
    const auto reg = M.regularDijkstra(source, target);

    if (multi != reg) {
        cout << v1 << " -> " << v2 << " --- ERROR:\n\tMLD -> "
             << multi << "\n\tREG -> " << reg << "\n";
        return false;
    }
    else {
        cout << v1 << " -> " << v2 << " --- OK ( " << reg << " )\n";
        return true;
    }
}