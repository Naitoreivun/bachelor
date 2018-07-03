#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "Vertex.h"
#include "alt.h"

int n, m;
vector<Vertex *> graph;
const int DEFAULT_LANDMARKS_COUNT = 16;

void init();

bool calculateDistance(Alt &alt, int v1, int v2);

void calculate(Alt &alt);

void benchmark(Alt &alt);

void printLandmarkDistances();

void loadQueries();

using namespace std::chrono;

vector<pair<int, int>> queries;

int main() {
    cout << "start" << endl;
    init();
    cout << "original graph loaded" << endl;

    const int landmarksCount = min(DEFAULT_LANDMARKS_COUNT, n / 4);

    milliseconds startPre = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    Alt alt(graph, landmarksCount);

    milliseconds stopPre = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\npreprocessing time:\n\t" << (stopPre.count() - startPre.count()) << "\n\n";

//    printLandmarkDistances();

    loadQueries();

//    calculate(alt);
    benchmark(alt);

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

void calculate(Alt &alt) {
    bool ok = true;
    for (pair<int, int> &query: queries) {
        ok &= calculateDistance(alt, query.first, query.second);
    }
    cout << "\n" << (ok ? "Tests passed" : "Some tests failed") << endl;
}

bool calculateDistance(Alt &alt, int v1, int v2) {
    const auto source = graph[v1 - 1];
    const auto target = graph[v2 - 1];

    const LL altDist = alt.altDijkstra(source, target);
    const LL regDist = alt.regularDijkstra(source, target);

//    if (altDist != regDist) {
//        cout << v1 << " -> " << v2 << " --- ERROR:\n\tALT -> " << altDist << "\n\tREG -> " << regDist << "\n";
//        return false;
//    }
//    else {
//        cout << v1 << " -> " << v2 << " --- OK ( " << regDist << " )\n";
//        return true;
//    }

    return altDist == regDist;
}


void benchmark(Alt &alt) {
//    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//    cout << (ms.count()) << endl;

    cout << "alt start\n";
    milliseconds startAlt = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (pair<int, int> &query: queries) {
        alt.altDijkstra(graph[query.first - 1], graph[query.second - 1]);
    }
    milliseconds stopAlt = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nalt stop:\n\t" << (stopAlt.count() - startAlt.count()) << "\n\n";

    cout << "reg start\n";
    milliseconds startReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (pair<int, int> &query: queries) {
        alt.regularDijkstra(graph[query.first - 1], graph[query.second - 1]);
    }
    milliseconds stopReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nreg stop:\n\t" << (stopReg.count() - startReg.count()) << "\n\n";

}

void printLandmarkDistances() {
    for (Vertex *v: graph) {
        cout << v->id << ":\n";
        cout << "FROM:\n";
        int i = 0;
        for (LL dist: v->landmarkDist[FROM]) {
            cout << "\t" << i++ << " -> " << dist << "\n";
        }
        cout << "TO:\n";
        i = 0;
        for (LL dist: v->landmarkDist[TO]) {
            cout << "\t" << i++ << " -> " << dist << "\n";
        }
        cout << endl;
    }
    cout << endl;
}