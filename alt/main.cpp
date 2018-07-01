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

using namespace std::chrono;

int main() {
    cout << "start" << endl;
    init();
    cout << "original graph loaded" << endl;

    const int landmarksCount = min(DEFAULT_LANDMARKS_COUNT, n / 4);
    Alt alt(graph, landmarksCount);

//    printLandmarkDistances();

//    calculate(alt);

    benchmark(alt);

    cout << "finish" << endl;
    return 0;
}

void init() {
//    const char *const filename = "../../tests/test.in";
//    const char *const filename = "../../tests/test2.in";
//    const char *const filename = "../../tests/test3.in";
//    const char *const filename = "../../tests/test4.in";
//    const char *const filename = "../../tests/pile.in";
//    const char *const filename = "../../tests/airportDebug.in";
//    const char *const filename = "../../tests/airportDebug2.in";
//    const char *const filename = "../../tests/airportDebug3.in";
    const char *const filename = "../../tests/USairport500.in";
//    const char *const filename = "../../tests/california.in";
    fstream file(filename);

    if (file.good()) {
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
    else {
        cout << "Cannot open file: '" << filename << "'." << endl;
    }
}

void calculate(Alt &alt) {
    bool ok = true;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            ok &= calculateDistance(alt, i, j);
        }
        cout << i << " -> " << (ok ? "\t\t JAK NA RAZIE SPOKO" : "\t\t COS SIE ZEPSULO :(") << "\n";
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
    const int iEnd = n / 5;
//    const int iEnd = n / 200;

    cout << "alt start\n";
    milliseconds startAlt = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (int i = 1; i <= iEnd; ++i) {
        for (int j = 1; j <= n; ++j) {
            alt.altDijkstra(graph[i - 1], graph[j - 1]);
        }
        cout << i << " ";
    }
    milliseconds stopAlt = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nalt stop:\n\t" << (stopAlt.count() - startAlt.count()) << "\n\n";

    cout << "reg start\n";
    milliseconds startReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (int i = 1; i <= iEnd; ++i) {
        for (int j = 1; j <= n; ++j) {
            alt.regularDijkstra(graph[i - 1], graph[j - 1]);
        }
        cout << i << " ";
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