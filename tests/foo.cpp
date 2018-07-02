#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <unordered_map>

using namespace std;

unordered_map<int, int> vertexToId;
int vertexCounter = 0;

int getVertexId(int vertex);

int main() {
    fstream input;
    input.open("../tests/ph.in", ios_base::in);

    if (!input.good()) {
        cout << "input is bad" << endl;
        return 1;
    }

    fstream output;
    output.open("../tests/cop-ph.in", ios_base::out | ios_base::trunc);
    if (!output.good()) {
        input.close();
        cout << "output is bad" << endl;
        return 1;
    }

    int from, to, maxVertexId = 0, edgesCount = 0;
    while (input >> from) {
        input >> to;
        from = getVertexId(from);
        to = getVertexId(to);
        output << from << " " << to << " " << 1 << endl;
        maxVertexId = max(maxVertexId, max(from, to));
        ++edgesCount;
    }

    input.close();
    output.close();

    cout << "done" << endl;
    cout << "max vertex id: " << maxVertexId << endl;
    cout << "edges count: " << edgesCount << endl;
    return 0;
}

inline int getVertexId(int vertex) {
    auto it = vertexToId.find(vertex);
    if (it != vertexToId.end()) {
        return it->second;
    }
    vertexToId[vertex] = ++vertexCounter;
    return vertexCounter;
}