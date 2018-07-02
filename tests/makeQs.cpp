#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <set>
#include <random>

using namespace std;

set<pair<int, int>> queries;

int main() {
    fstream output;
    output.open("../tests/us-airport_queries.in", ios_base::out | ios_base::trunc);
    if (!output.good()) {
        cout << "output is bad" << endl;
        return 1;
    }

    int n = 500;
    int count = 100000;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(1.0, n + 1.0);

    while (queries.size() < count) {
        queries.insert(make_pair((int) dist(mt), (int) dist(mt)));
    }

    output << count << endl;
    for (auto q: queries) {
        output << q.first << " " << q.second << endl;
    }

    output.close();
    return 0;
}