#include <stdio.h>
#include <stdlib.h>
#include "ranking.h"

// 친구 수 계산
UserRanking* CreateFriendRanking(Graph* graph){
    if(graph == NULL){
        return NULL;
    }
    // 사용자 수만큼 배열 동적 할당
    UserRanking* ranking = (UserRanking*)malloc(sizeof(UserRanking) * graph->userCount);
    if(ranking == NULL){
        return NULL;
    }
    
    for(int i = 1; i <=graph->userCount; i++){
        int friendCount = 0;
        
        // 인접리스트
        ListNode* current = graph->list[i];
        // 인접리스트 순회
        while(current != NULL){
            friendCount++;
            current = current->next;
        }
        // rankint[i-1] 로 한 사유 : 그래프는 1번 부터인데 배열은 0번부터라 한칸씩 땡겨야함
        ranking[i-1].userID = i;
        ranking[i-1].friendCount = friendCount;
    }
    return ranking;
}

// 정렬된 왼쪽 구간과 오른쪽 구간을 하나의 정렬된 구간으로 합치는 함수(여기서도 비교해서 합침)
int MergeRanking(UserRanking* ranking, int left, int mid, int right){
    // 현재 병합될 전체 구간의 크기
    int size = right - left + 1;
    
    // 병합 결과를 임시 저장할 배열(동적 할당)
    UserRanking* temp = (UserRanking*)malloc(sizeof(UserRanking) * size);
    if(temp == NULL){
        return 1;
    }

    int i = left; // i : 왼쪽 배열에서 현재 보고 있는 위치
    int j = mid + 1; // j : 오른쪽 배열에서 현재 보고 있는 위치 (mid : 왼쪽 배열의 마지막 위치)
    int k = 0; // temp 배열에 다음으로 넣을 위치

    // 왼쪽과 오른쪽에 비교할 데이터가 남아 있는 동안
    while(i <= mid && j <= right){
        // 친구 수가 더 많은 사용자를 먼저 저장 + 그거 저장하고 그쪽배열에서 다음 칸으로 넘김
        // 왼쪽 사용자의 친구 수가 더 많은 경우
        if(ranking[i].friendCount > ranking[j].friendCount){
            temp[k++] = ranking[i++];
        // 오른쪽 사용자의 친구수가 더 많은 경우
        } else if(ranking[i].friendCount < ranking[j].friendCount){
            temp[k++] = ranking[j++];
        } else {
            // 친구 수가 같으면 userID가 작은쪽이 우선됨
            if(ranking[i].userID < ranking[j].userID){
                temp[k++] = ranking[i++];
            }else {
                temp[k++] = ranking[j++];
            }
        }
    }
    // 오른쪽 구간이 먼저 끝났을 경우
    // 왼쪽에 남아 있는거 그대로 temp에 저장
    while(i <= mid){
        temp[k++] = ranking[i++];
    }

    // 왼쪽 구간이 먼저 끝났을 경우
    // 오른쪽 구간에 남아있는 데이터 그대로 temp에 저장
    while(j <= right){
        temp[k++] = ranking[j++];
    }

    // 정렬된 결과를 원래 ranking 배열에 다시 복사
    for(int index = 0; index < size; index++){
        ranking[left + index] = temp[index];
    }
    free(temp);
    return 0;
}

// Merge Sort를 이용해서 ranking 배열을 정렬
int MergeSortRanking(UserRanking* ranking, int left, int right){
    if(ranking == NULL){
        return 1;
    }

    // 원소가 하나 -> 이미 정렬됨
    // 시작점과 끝점이 같으면 원소가 하나임
    if(left == right){
        return 0;
    }

    // 현재 구간을 절반으로 쪼갬 
    // (userCount를 거의 최대 1억으로 잡은 상태라서 여기서 뭘해도 오버플로는 안남)
    int mid = (left + right) / 2;

    // 왼쪽 절반 정렬(재귀)
    if(MergeSortRanking(ranking, left, mid) != 0){
        return 1;
    }
    // 오른쪽 절반 정렬(재귀)
    if(MergeSortRanking(ranking, mid + 1, right) != 0){
        return 1;
    }

    // 정렬된 두쪽 합치기
    return MergeRanking(ranking, left, mid, right);
}

// 정렬된 랭킹 상위 count명 출력
void PrintTopRanking(UserRanking* ranking){
    if(ranking == NULL){
        return;
    }

    printf("\n============== FRIEND RANKING TOP 50 ================\n");
    
    // 순위 50까지 출력
    for(int i=0; i < 50; i++){
        printf("%d. User %d | Friends : %d\n", i + 1, ranking[i].userID, ranking[i].friendCount);
    }
}