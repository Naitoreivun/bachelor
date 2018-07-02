#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "MultilevelGraph.h"

int n, m;
vector<Vertex *> originalVertices;
vector<int> degrees;
vector<int> levelSizes;

void init();

vector<Vertex *> getVerticesSortedByDegree();

bool calculateDistance(int v1, int v2, MultilevelGraph &M);

void calculate(MultilevelGraph &M);

void benchmark(MultilevelGraph &M);

void loadQueries();

using namespace std::chrono;

vector<pair<int, int>> queries;

int main() {
    cout << "start" << endl;
    init();
    cout << "original graph loaded" << endl;

    milliseconds startPre = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    auto verticesSortedByDegree = getVerticesSortedByDegree();
    MultilevelGraph M(originalVertices);
    for (int i = 1; i < levelSizes.size(); ++i) {
        M.addLevel(vector<Vertex *>(verticesSortedByDegree.begin(), verticesSortedByDegree.begin() + levelSizes[i]));
    }
    M.createConnectedComponents();
    M.prepareVerticesForQueries();

    milliseconds stopPre = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\npreprocessing time:\n\t" << (stopPre.count() - startPre.count()) << "\n\n";

//    M.printAll();
//    M.printConnectedComponents();

    loadQueries();

//    calculate(M);
    benchmark(M);

    cout << "finish" << endl;
    return 0;
}

void init() {
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
        originalVertices.push_back(new Vertex(i));
        degrees.push_back(0);
    }

    int source, dest;
    LL weight;
    for (int i = 0; i < m; ++i) {
        file >> source >> dest >> weight;
        if (source != dest) {
            originalVertices[source - 1]->linkBackAndForth(originalVertices[dest - 1], weight);
            ++degrees[source];
            ++degrees[dest];
        }
    }
    file.close();
}

void loadQueries() {
    fstream file(pathToQueries);

    if (!file.good()) {
        cout << "Cannot open file: '" << pathToQueries << "'." << endl;
        return;
    }

    int count, from, to;
    file >> count;
    for (int i = 0; i < count; ++i) {
        file >> from >> to;
        queries.emplace_back(from, to);
    }

    cout << "queries loaded" << endl;

    file.close();
}

void calculate(MultilevelGraph &M) {
    bool ok = true;
    for (pair<int, int> &query: queries) {
        ok &= calculateDistance(query.first, query.second, M);
//        cout << i << " -> " << (ok ? "\t\t JAK NA RAZIE SPOKO" : "\t\t COS SIE ZEPSULO :(") << "\n";
    }
    cout << "\n" << (ok ? "Tests passed" : "Some tests failed") << endl;
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

//    if (multi != reg) {
//        cout << v1 << " -> " << v2 << " --- ERROR:\n\tMLD -> " << multi << "\n\tREG -> " << reg << "\n";
//        return false;
//    }
//    else {
//        cout << v1 << " -> " << v2 << " --- OK ( " << reg << " )\n";
//        return true;
//    }
    return multi == reg;
}


void benchmark(MultilevelGraph &M) {
//    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//    cout << (ms.count()) << endl;

    cout << "mul start\n";
    milliseconds startMul = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (pair<int, int> &query: queries) {
        M.calculateDistance(originalVertices[query.first - 1], originalVertices[query.second - 1]);
    }
    milliseconds stopMul = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nmul stop:\n\t" << (stopMul.count() - startMul.count()) << "\n\n";

    cout << "reg start\n";
    milliseconds startReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (pair<int, int> &query: queries) {
        M.regularDijkstra(originalVertices[query.first - 1], originalVertices[query.second - 1]);
    }
    milliseconds stopReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nreg stop:\n\t" << (stopReg.count() - startReg.count()) << "\n\n";
}