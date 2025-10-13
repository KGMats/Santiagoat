cmake_minimum_required(VERSION 4.0)
project(SemNome C)

set(CMAKE_C_STANDARD 11)

add_executable(SemNome main.c
        Graph.c
        Graph.h
        Algorithms.c
        Algorithms.h
        Solutions.c
        Solutions.h
        Stack.h
        Stack.c
        LinkedList.h)
