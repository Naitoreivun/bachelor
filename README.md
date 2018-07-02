# bachelor
:(

### algorithm sources:
**Dijkstra** - https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

**MLD** - https://i11www.iti.kit.edu/extra/publications/swz-umlgt-02.pdf

**BIDI** - https://www.cs.princeton.edu/courses/archive/spr06/cos423/Handouts/EPP%20shortest%20path%20algorithms.pdf

**ALT (unidirectional version)** - https://www.microsoft.com/en-us/research/wp-content/uploads/2004/07/tr-2004-24.pdf

### test sources:
https://www.cs.utah.edu/~lifeifei/SpatialDataset.htm

https://toreopsahl.com/datasets/#usairports

## results:

*ALGORITHM (preprocessing time (ms), execution time (ms)) - additional settings*

###### san francisco: (500k queries)
+ **MLD** (1274111, 263847) - 5 2000 400 150 50 20
+ **MLD** (1096424, 274832) - 4 2000 200 70 20
+ **ALT** (1681, 317788)
+ **BIDI** (0, 112903)
+ **REG** (0, 245527)

###### us-airport: (100k queries)
+ **MLD** (752, 11567) - 3 60 20 10
+ **ALT** (131, 60205)
+ **BIDI** (0, 64931)
+ **REG** (0, 90891)

###### oldenburg: (500k queries)
+ **MLD** (1127, 6322) - 3 60 20 10
+ **ALT** (121, 11665)
+ **BIDI** (0, 8266)
+ **REG** (0, 10059)
