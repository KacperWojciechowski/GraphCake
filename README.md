# GraphCake 

## Overview

A lightweight graph library started off as a university project. The aim of this project is to provide a scallable, open source library which would allow users to utilize an arrangement of algorithms in their own projects, or extend them with new ones, using the GraphCake as a framework. 

## Functionalities

GraphCase so far provides two basic graph representations - **adjacency matrix** and **adjacency list**. They utilize a common interface, which allows any of the representations to be used with implemented algorithms. 

Algorithms currently available in the library:
- Greedy coloring
- LF coloring
- SL coloring

Additional currently available functionalities:
- Benchmarking algorithms
- Serialization and deserialization of following formats:
    - .mat (according to [House of Graphs](https://houseofgraphs.org/));
    - .lst (according to [House of Graphs](https://houseofgraphs.org/));
    - basic .graphml (according to [GraphML Primer](http://graphml.graphdrawing.org/primer/graphml-primer.html)):
        - nodes
        - edges
        - edge weights
        - graph directionality

## Development

Any user who would like to contribute to the library is free to do so **using a fork and posting a merge request** with new functionalities. In the nearby future, I will try to create a docker configuration which will allow contributors to set up the entire environment in a ready-to-use state.

If you have ideas about functionalities and fixes you would like to see in the project, **feel free to open an issue**!

Releases are created on stable version of the project, aggregating a set of new functionalities and fixes since previous release. However, it's a hobbyistic project, and as such, there is no definite period between releases, and there is no warranty for the fixes to be implemented, and how quickly will that happen. 

I am putting my best effort to keep the main branch with the complete versions of software, however bugs may still happen, and as such, using versions **other** than the official releases is **at the user's own risk**.
