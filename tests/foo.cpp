#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

int main() {
    fstream input;
    input.open("../tests/x.in", ios_base::in);

    if (!input.good()) {
        cout << "input is bad" << endl;
        return 1;
    }

    fstream output;
    output.open("../tests/xxx.in", ios_base::out | ios_base::trunc);
    if (!output.good()) {
        input.close();
        cout << "output is bad" << endl;
        return 1;
    }

    int ignoredEdgeId, from, to, maxVertexId = 0, edgesCount = 0;
    double dist;
    while (input >> ignoredEdgeId) {
        input >> from >> to >> dist;
        output << from + 1 << " " << to + 1 << " " << (long long)(round(1000000ll * dist)) << endl;
        maxVertexId = max(maxVertexId, max(from, to));
        ++edgesCount;
    }

    input.close();
    output.close();

    cout << "done" << endl;
    cout << "max vertex id: " << maxVertexId + 1 << endl;
    cout << "edges count: " << edgesCount << endl;
    return 0;
}