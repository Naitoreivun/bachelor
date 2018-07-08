#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "alt/AltProgram.h"
#include "bidirectional-dijkstra/BidiProgram.h"
#include "dijkstra/DijkstraProgram.h"
#include "multi-level-dijkstra/MLDProgram.h"

using namespace std;
using namespace std::chrono;

#define getTime duration_cast<milliseconds>(system_clock::now().time_since_epoch())

void runBenchmark(const string &filename);

void loadQueries(const string &pathToQueries);

void runAlts(const string &pathToGraph);

void runRegularDijkstra(const string &pathToGraph);

const vector<LL> runBidirectionalAlt(AltProgram &altProgram);

const vector<LL> runAlt(AltProgram &altProgram);

void runBidirectionalDijkstra(const string &pathToGraph);

void runMultiLevelDijkstra(const string &pathToGraph);

int main() {
    runBenchmark("test");
//    runBenchmark("san-francisco");
//    runBenchmark("oldenburg");
    runBenchmark("us-airport");
//    runBenchmark("cop-ph");
    return 0;
}

void runBenchmark(const string &filename) {
    cout << filename << " benchmark:\n\n";

    const string pathToGraph = "../tests/" + filename + ".in";
    const string pathToQueries = "../tests/" + filename + "_queries.in";

    loadQueries(pathToQueries);
    runRegularDijkstra(pathToGraph);
    runAlts(pathToGraph);
    runBidirectionalDijkstra(pathToGraph);
    runMultiLevelDijkstra(pathToGraph);

    cout << endl << endl;
}

vector<pair<int, int>> queries;
vector<LL> dijkstraResults;

void loadQueries(const string &pathToQueries) {
    fstream file(pathToQueries);

    if (!file.good()) {
        cout << "\tCannot open file: '" << pathToQueries << "'." << endl;
        return;
    }

    queries.clear();
    int count, from, to;
    file >> count;
    for (int i = 0; i < count; ++i) {
        file >> from >> to;
        queries.emplace_back(from, to);
    }

    cout << "\tqueries loaded" << endl;

    file.close();
}

void runRegularDijkstra(const string &pathToGraph) {
    DijkstraProgram dijkstraProgram(pathToGraph);
    milliseconds start = getTime;
    dijkstraResults = dijkstraProgram.processQueries(queries);
    milliseconds stop = getTime;
    cout << "\tregular dijkstra time: " << (stop.count() - start.count()) << endl << endl;
}

void runAlts(const string &pathToGraph) {
    AltProgram altProgram(pathToGraph);

    milliseconds start = getTime;
    altProgram.preprocessing();
    milliseconds stop = getTime;
    cout << "\talt preprocessing time: " << (stop.count() - start.count()) << endl;

    const vector<LL> &&bidiAltResults = runBidirectionalAlt(altProgram);
    const vector<LL> &&altResults = runAlt(altProgram);

    cout << "\tbidi alt ok? " << (bidiAltResults == dijkstraResults) << endl;
    cout << "\talt ok? " << (altResults == dijkstraResults) << endl << endl;
}

const vector<LL> runBidirectionalAlt(AltProgram &altProgram) {
    milliseconds start = getTime;
    const vector<LL> &&bidiAltResults = altProgram.processQueriesBidirectional(queries);
    milliseconds stop = getTime;
    cout << "\tbidirectional alt execution time: " << (stop.count() - start.count()) << endl;

    return bidiAltResults;
}

const vector<LL> runAlt(AltProgram &altProgram) {
    milliseconds start = getTime;
    const vector<LL> &&altResults = altProgram.processQueries(queries);
    milliseconds stop = getTime;
    cout << "\talt execution time: " << (stop.count() - start.count()) << endl;
    return altResults;
}

void runBidirectionalDijkstra(const string &pathToGraph) {
    BidiProgram bidiProgram(pathToGraph);

    milliseconds start = getTime;
    const vector<LL> &&bidiResults = bidiProgram.processQueries(queries);
    milliseconds stop = getTime;
    cout << "\tbidirectional dijkstra execution time: " << (stop.count() - start.count()) << endl;

    cout << "\tbidi ok? " << (bidiResults == dijkstraResults) << endl << endl;
}

void runMultiLevelDijkstra(const string &pathToGraph) {
    MLDProgram mldProgram(pathToGraph);

    milliseconds start = getTime;
    mldProgram.preprocessing();
    milliseconds stop = getTime;
    cout << "\tmld preprocessing time: " << (stop.count() - start.count()) << endl;

    start = getTime;
    const vector<LL> &&mldResults = mldProgram.processQueries(queries);
    stop = getTime;
    cout << "\tmld execution time: " << (stop.count() - start.count()) << endl;

    cout << "\tmld ok? " << (mldResults == dijkstraResults) << endl << endl;
}