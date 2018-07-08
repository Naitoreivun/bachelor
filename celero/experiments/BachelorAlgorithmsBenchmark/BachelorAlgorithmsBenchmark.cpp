#include <celero/Celero.h>
#include <algorithm>
#include "alt/AltProgram.h"
#include "bidirectional-dijkstra/BidiProgram.h"
#include "dijkstra/DijkstraProgram.h"
#include "multi-level-dijkstra/MLDProgram.h"

#ifndef WIN32
#include <cmath>
#include <cstdlib>
#endif

CELERO_MAIN

class DijkstraExecutionFixture : public celero::TestFixture {
public:
    DijkstraProgram *dijkstraProgram;
    BidiProgram *bidiProgram;
    AltProgram *altProgram;
    MLDProgram *mldProgram;
//    const vector<string> filenames = {"us-airport", "oldenburg", "san-francisco", "cop-ph"};
//    const vector<string> filenames = {"test", "us-airport"};
    const vector<string> filenames = {"us-airport"};
    vector<pair<int, int>> queries;

    DijkstraExecutionFixture() {

    }

    virtual std::vector<celero::TestFixture::ExperimentValue> getExperimentValues() const override {
        std::vector<celero::TestFixture::ExperimentValue> filenameNumber;
        for (unsigned i = 0; i < filenames.size(); ++i) {
            filenameNumber.emplace_back(i);
        }
        return filenameNumber;
    }

    /// Before each sample
    virtual void setUp(const celero::TestFixture::ExperimentValue &experimentValue) override {
        const string filename = filenames[experimentValue.Value];
        const string pathToQueries = "../tests/" + filename + "_queries.in";
        loadQueries(pathToQueries);

        const string pathToGraph = "../tests/" + filename + ".in";
        dijkstraProgram = new DijkstraProgram(pathToGraph);
        bidiProgram = new BidiProgram(pathToGraph);
        altProgram = new AltProgram(pathToGraph);
        mldProgram = new MLDProgram(pathToGraph);

        altProgram->preprocessing();
        mldProgram->preprocessing();
    }

    /// Before each iteration
    virtual void onExperimentStart(const celero::TestFixture::ExperimentValue &) override {
    }

    /// After each iteration
    virtual void onExperimentEnd() override {
    }

    /// After each sample
    virtual void tearDown() override {
        delete dijkstraProgram;
        delete bidiProgram;
        delete altProgram;
        delete mldProgram;
    }

private:
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

//        cout << "\tqueries loaded" << endl;

        file.close();
    }
};

static const int SamplesCount = 1;
static const int IterationsCount = 1;

BASELINE_F(Dijkstras, RegularDijkstra, DijkstraExecutionFixture, SamplesCount, IterationsCount) {
    dijkstraProgram->processQueries(queries);
}

BENCHMARK_F(Dijkstras, BidirectionalDijkstra, DijkstraExecutionFixture, SamplesCount, IterationsCount) {
    bidiProgram->processQueries(queries);
}

BENCHMARK_F(Dijkstras, ALT, DijkstraExecutionFixture, SamplesCount, IterationsCount) {
    altProgram->processQueries(queries);
}

BENCHMARK_F(Dijkstras, BidirectionalALT, DijkstraExecutionFixture, SamplesCount, IterationsCount) {
    altProgram->processQueriesBidirectional(queries);
}

BENCHMARK_F(Dijkstras, MultiLevelGraph, DijkstraExecutionFixture, SamplesCount, IterationsCount) {
    mldProgram->processQueries(queries);
}
