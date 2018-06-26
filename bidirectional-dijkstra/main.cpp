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

using namespace std::chrono;

int main() {
    cout << "start" << endl;
    init();
    cout << "original graph loaded" << endl;

//    calculate();

    benchmark();

    cout << "finish" << endl;
    return 0;
}

void calculate() {
    bool ok = true;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            ok &= calculateDistance(i, j);
        }
        cout << i << " -> " << (ok ? "\t\t JAK NA RAZIE SPOKO" : "\t\t COS SIE ZEPSULO :(") << "\n";
    }
    cout << "\n" << (ok ? "Tests passed" : "Some tests failed") << endl;
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
        ULL weight;
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
    const int iEnd = n / 5;

    cout << "reg start\n";
    milliseconds startReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (int i = 1; i <= iEnd; ++i) {
        for (int j = 1; j <= n; ++j) {
            regularDijkstra(graph, graph[i - 1], graph[j - 1]);
        }
        cout << i << " ";
    }
    milliseconds stopReg = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nreg stop:\n\t" << (stopReg.count() - startReg.count()) << "\n\n";

    cout << "bidi start\n";
    milliseconds startBidi = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for (int i = 1; i <= iEnd; ++i) {
        for (int j = 1; j <= n; ++j) {
            bidirectionalDijkstra(graph, graph[i - 1], graph[j - 1]);
        }
        cout << i << " ";
    }
    milliseconds stopBidi = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "\nbidi stop:\n\t" << (stopBidi.count() - startBidi.count()) << "\n\n";

}