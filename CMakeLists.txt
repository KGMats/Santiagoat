cmake_minimum_required(VERSION 4.0)
project(Grafos C)

set(CMAKE_C_STANDARD 11)

add_executable(Grafos main.c
        Graph.c
        Graph.h
        Algorithms.c
        Algorithms.h
        Solutions.h
        Stack.h
        Stack.c
        LinkedList.h
        Utils.c
        Utils.h
        Tarjan.c
        Tarjan.h
        Heuristics/MaximumDegreeHeuristic.c
        Heuristics/MaximumDegreeHeuristic.h
        Heuristics/bridgeHeuristic.c
        Heuristics/bridgeHeuristic.h
        Heuristics/hillClimbLocalSearch.c
        Heuristics/hillClimbLocalSearch.h
)
