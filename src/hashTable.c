#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

// User HashTable

// 해시 함수 (유저)
unsigned int HashUserID(long long userID, int tableSize){
    return (unsigned int)(userID % tableSize);
}

// 테이블 생성
UserHashTable* CreateUserHashTable(int tableSize){
    UserHashTable* table = (UserHashTable*)malloc(sizeof(UserHashTable)); // 동적 할당
 
    if(table == NULL){ // 동적할당 실패시 예외처리
        return NULL; 
    }

    table->tableSize = tableSize;
    table->bucket = (UserHashNode**)malloc(sizeof(UserHashNode*) * tableSize); // 테이블 사이즈 만큼 버킷 생성
    if(table->bucket == NULL){
        free(table);
        return NULL;
    }
    for(int i=0; i<tableSize; i++){
        table->bucket[i] = NULL; // 버킷 내부 초기화
    }
    return table;
}

// 해시테이블에 유저 프로필 삽입
int InsertProfile(UserHashTable* table, UserProfile profile){
    unsigned int index = HashUserID(profile.userID, table->tableSize); // 테이블 반환 
    UserHashNode* newNode = (UserHashNode*)malloc(sizeof(UserHashNode)); // 동적할당

    if(newNode == NULL){
        return 1;
    }
    newNode->key = profile.userID;
    newNode->profile = profile;

    // 분리연결법 (체이닝)
    newNode->next = table->bucket[index]; // 해시로 반환된 테이블 크기 만큼 배열 생성
    table->bucket[index] = newNode;

    return 0;
}

// Search User HashTable
UserProfile* SearchProfile(UserHashTable* table, long long userID){
    unsigned int index = HashUserID(userID, table->tableSize);
    UserHashNode* current = table->bucket[index];

    while(current!=NULL){
        if(current->key == userID){ // 해당 버킷에 키값이 탐색중인 유저ID와 일치하면 해당 버킷의 유저 프로필 반환
            return &current->profile;
        }
        current = current->next;
    }
    return NULL; // 모든 버킷 돌아봤는데 없으면 없는거임
}

// 할당 해제
void FreeUserHashTable(UserHashTable* table){
    if(table == NULL){
        return;
    }
    for(int i=0; i<table->tableSize; i++){
        UserHashNode* current = table->bucket[i];
        while(current != NULL){
            UserHashNode* next = current->next; // 삭제하기 전에 다음거 주소 저장해놓기
            free(current);
            current = next;
        }
    }
    free(table->bucket);
    free(table);
}