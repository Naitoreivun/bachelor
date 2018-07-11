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

class BenchmarkBaseFixture: public celero::TestFixture {
public:
    const vector<string> filenames = {"us-airport", "oldenburg", "san-francisco", "cop-ph"};
    vector<pair<int, int>> queries;
    string pathToGraph;

    BenchmarkBaseFixture() {

    }

    virtual std::vector<celero::TestFixture::ExperimentValue> getExperimentValues() const override {
        std::vector<celero::TestFixture::ExperimentValue> filenameNumber;
        for (unsigned i = 0; i < filenames.size(); ++i) {
            filenameNumber.emplace_back(i);
        }
        return filenameNumber;
    }

    virtual void setUp(const celero::TestFixture::ExperimentValue &experimentValue) override {
        const string filename = filenames[experimentValue.Value];
        const string pathToQueries = "../../tests/" + filename + "_queries.in";
        loadQueries(pathToQueries);
        pathToGraph = "../../tests/" + filename + ".in";
    }


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

        file.close();
    }
};

class RegularDijkstraFixture : public BenchmarkBaseFixture {
public:
    DijkstraProgram *dijkstraProgram;

    virtual void setUp(const celero::TestFixture::ExperimentValue &experimentValue) override {
        BenchmarkBaseFixture::setUp(experimentValue);
        dijkstraProgram = new DijkstraProgram(pathToGraph);
    }

    virtual void tearDown() override {
        delete dijkstraProgram;
    }
};

class BidirectionalDijkstraFixture : public BenchmarkBaseFixture {
public:
    BidiProgram *bidiProgram;

    virtual void setUp(const celero::TestFixture::ExperimentValue &experimentValue) override {
        BenchmarkBaseFixture::setUp(experimentValue);
        bidiProgram = new BidiProgram(pathToGraph);
    }

    virtual void tearDown() override {
        delete bidiProgram;
    }
};

class AltsFixture : public BenchmarkBaseFixture {
public:
    AltProgram *altProgram;

    virtual void setUp(const celero::TestFixture::ExperimentValue &experimentValue) override {
        BenchmarkBaseFixture::setUp(experimentValue);
        altProgram = new AltProgram(pathToGraph);
        altProgram->preprocessing();
    }

    virtual void tearDown() override {
        delete altProgram;
    }
};

class MLDFixture : public BenchmarkBaseFixture {
public:
    MLDProgram *mldProgram;

    virtual void setUp(const celero::TestFixture::ExperimentValue &experimentValue) override {
        BenchmarkBaseFixture::setUp(experimentValue);
        mldProgram = new MLDProgram(pathToGraph);
        mldProgram->preprocessing();
    }

    virtual void tearDown() override {
        delete mldProgram;
    }
};

static const int SamplesCount = 1;
static const int IterationsCount = 1;

BASELINE_F(Execution, RegularDijkstra, RegularDijkstraFixture, SamplesCount, IterationsCount) {
    dijkstraProgram->processQueries(queries);
}

BENCHMARK_F(Execution, BidirectionalDijkstra, BidirectionalDijkstraFixture, SamplesCount, IterationsCount) {
    bidiProgram->processQueries(queries);
}

BENCHMARK_F(Execution, ALT_1, AltsFixture, SamplesCount, IterationsCount) {
    altProgram->processQueries(queries);
}

BENCHMARK_F(Execution, ALT_2, AltsFixture, SamplesCount, IterationsCount) {
    altProgram->processQueriesBidirectional(queries);
}

BENCHMARK_F(Execution, MultiLevelGraph, MLDFixture, SamplesCount, IterationsCount) {
    mldProgram->processQueries(queries);
}

class ALTPreprocessingFixture : public BenchmarkBaseFixture {
public:
    AltProgram *altProgram = nullptr;
};

class MLDPreprocessingFixture : public BenchmarkBaseFixture {
public:
    MLDProgram *mldProgram = nullptr;
};

BASELINE_F(Preprocessing, ALT, ALTPreprocessingFixture, 1, 1) {
    altProgram = new AltProgram(pathToGraph);
    altProgram->preprocessing();
    delete altProgram;
}

BENCHMARK_F(Preprocessing, MultiLevelGraph, MLDPreprocessingFixture, 1, 1) {
    mldProgram = new MLDProgram(pathToGraph);
    mldProgram->preprocessing();
    delete mldProgram;
}