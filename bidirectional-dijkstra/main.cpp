#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "Vertex.h"
#include "BidirectionalDijkstra.h"

int n, m;
vector<Vertex *> graph;

void init();

bool calculateDistance(int v1, int v2);

void calculate();

void benchmark();

void loadQueries();

using namespace std::chrono;

vector<pair<int, int>> queries;

int main() {
    cout << "start" << endl;
    init();
    cout << "original graph loaded" << endl;

    loadQueries();

//    calculate();
    benchmark();

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

    int unusedLevelsCount;
    file >> unusedLevelsCount;
    for (int i = 1, unusedLevelSize; i <= unusedLevelsCount; ++i) {
        file >> unusedLevelSize;
    }

    for (int i = 1; i <= n; ++i) {
        graph.push_back(new Vertex(i));
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

void calculate() {
    bool ok = true;
    for (pair<int, int> &query: queries) {
        ok &= calculateDistance(query.first, query.second);
    }
    cout << "\n" << (ok ? "Tests passed" : "Some tests failed") << endl;
}

bool calculateDistance(int v1, int v2) {
    const auto source = graph[v1 - 1];
    const auto target = graph[v2 - 1];

    const auto bidi = bidirectionalDijkstra(graph, source, target);
    const auto reg = regularDijkstra(graph, source, target);

//    if (bidi != reg) {
//        cout << v1 << " -> " << v2 << " --- ERROR:\n\tBIDI -> " << bidi << "\n\tREG -> " << reg << "\n";
//        return false;
//    }
//    else {
//        cout << v1 << " -> " << v2 << " --- OK ( " << reg << " )\n";
//        return true;
//    }
    return bidi == reg;
}


void benchmark() {
//    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//    cout << (ms.count()) << endl;

    cout << "bidi start\n";
    milliseconds startBidi = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (pair<int, int> &query: queries) {
        bidirectionalDijkstra(graph, graph[query.first - 1], graph[query.second - 1]);
    }
    milliseconds stopBidi = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nbidi stop:\n\t" << (stopBidi.count() - startBidi.count()) << "\n\n";

    cout << "reg start\n";
    milliseconds startReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (pair<int, int> &query: queries) {
        regularDijkstra(graph, graph[query.first - 1], graph[query.second - 1]);
    }
    milliseconds stopReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nreg stop:\n\t" << (stopReg.count() - startReg.count()) << "\n\n";
}