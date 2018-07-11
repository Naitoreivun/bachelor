# bachelor

The code is wrapped in Celero benchmark tool. Path to code:
`celero/experiments/BachelorAlgorithmsBenchmark`

In order to build and run the benchmark test:
1. Open terminal in root directory (that consists `tests` and `celero` directories)
2. Type `mkdir build && cd build`
3. Type `cmake ..` to build makefile
4. Type `make` to build program
5. Type `cd BachelorAlgorithmsBenchmark`
6. Type `./celeroBachelorAlgorithmsBenchmark` to run benchmark (it can last even few hours)
7. You will see the table with results:
    - `Group` column denotes test type:
        - Execution - measures algorithm execution time
        - Preprocessing - measures algorithm preprocessing time
    - `Experiment` column denotes particular algorithm:
        - `RegularDijkstra` - Dijkstra
        - `BidirectionalDijkstra` - Bidirectional Dijkstra
        - `ALT_1` - Unidirectional ALT
        - `ALT_2` - Bidirectional ALT
        - `MultiLevelGraph` - algorithm that uses Multi-level Graph structure
    - `Prob. Space` column denotes the number of test case:
        - `0` - US Airports
        - `1` - Oldenburg
        - `2` - San Francisco
        - `3` - High Energy Physics - Phenomenology collaboration network
    - `Samples`, `Iterations` and `Baseline` columns are well described [here](https://github.com/DigitalInBlue/Celero/#using-the-code); in our case one iteration is the set of tests for particular test case
    - `us/Iteration` column denotes the time (in microseconds) spent on processing one iteration
    - `Iterations/sec` column denotes processed iteration count per second


---

**benchmark tool** - https://github.com/DigitalInBlue/Celero/

### algorithm sources:
**Dijkstra** - https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

**MLD** - https://i11www.iti.kit.edu/extra/publications/swz-umlgt-02.pdf

**BIDI** - https://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf

**ALT** - https://www.microsoft.com/en-us/research/wp-content/uploads/2004/07/tr-2004-24.pdf

### test sources:
https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm

https://toreopsahl.com/datasets/#usairports

https://snap.stanford.edu/data/ca-HepPh.html
