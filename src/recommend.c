#include <stdio.h>
#include <stdlib.h>

#include "recommend.h"

int RecommendFriends(Graph* graph, int userID){
    if(graph == NULL){
        return -1;
    }
    if(userID < 1 || userID > graph->userCount){
        return -1;
    }

    // state 0 : 미확인 유저. 1 : 자기 자신 or 인접리스트. 2 : 추천 친구 (count++)
    unsigned char* state = (unsigned char*)calloc(graph->userCount + 1, sizeof(unsigned char)); // calloc : 메모리 할당 + 값 0으로 초기화 (원소 개수, 원소 하나의 사이즈)

    if(state == NULL){ // 할당 실패
        return -1;
    }

    // 자기 자신은 추천대상이 될 수 없음 (뜨면 뜨는대로 웃기긴 하겠네)
    state[userID] = 1;

    // 유저의 인접리스트 먼저 표시 (친구)
    ListNode* current = graph->list[userID];

    while(current != NULL){
        int friendID = current->userID;
        state[friendID] = 1;
        current = current->next;
    }

    // 유저의 인접리스트 순회
    current = graph->list[userID];
    int recommendCount = 0;
    printf("Recommended friends for User %d : \n", userID);

    while(current != NULL){
        int friendID = current->userID; 
        // 유저의 인접리스트의 인접리스트 (친구의 친구)
        ListNode* candidateCurrent = graph->list[friendID];

        while(candidateCurrent != NULL){
            int candidateID = candidateCurrent->userID;
            
            // state가 0인 경우만 추천 (본인 = 1, 직접적인 친구 = 1, 이미 추천한 유저 = 2 -> 순회에서 읽히는 유저중에 state가 0이면 추천친구로 넣으면 됨)
            if(state[candidateID] == 0){ // 중복 제거 역할도 함 (의도한건 아니었다만은)
                state[candidateID] = 2;
                printf("%d ", candidateID); // 추천 친구 목록 출력
                recommendCount++;
            }
            candidateCurrent = candidateCurrent->next;
        }
        current = current->next;
    }
    printf("\n");
    free(state);
    return recommendCount;
}