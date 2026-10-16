#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

// 탐색할 유저 그래프, 시작 유저, 목적지 유저, 각 유저까지의 최단 거리 저장, 최단 경로에서 이전 유저 저장(부모 노드)
int Dijkstra(Graph* graph, int startUser, int endUser, long long* distance, int* prev);

#endif // DIJKSTRA_H