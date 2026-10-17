#ifndef RANKING_H
#define RANKING_H

#include "graph.h"

typedef struct {
    int userID;
    int friendCount;
} UserRanking;

// 그래프에서 각 유저의 친구 수를 계산해서 배열 생성함
UserRanking* CreateFriendRanking(Graph* graph);
// ranking : 전체 랭킹 배열, left : 현재 합칠 범위의 시작 위치, mid : 왼쪽 절반의 마지막 위치, right : 현재 합칠 범위의 마지막 위치
void MergeRanking(UserRanking* ranking, int left, int mid, int right); // MergeSortRanking 내부에서 사용 (쪼갠거 다시 합치기)
// 친구 수 기준 내림차순 Merge Sort (나누는 기준은 그냥 1/2 때려서 mid 정함)
void MergeSortRanking(UserRanking* ranking, int left, int right); // 쪼갠거 재귀정렬
// 정렬된 랭킹 상위 count명 출력
void PrintTopRanking(UserRanking* ranking);

#endif // RANKING_H