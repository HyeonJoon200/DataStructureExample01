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
    if(tableSize <= 0){
        return NULL;
    }

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

    /*
    // 충돌나는거 확인용 임시 코드
    if(table->bucket[index] != NULL){
        printf("Hash collision : userID %lld -> bucket %u\n", profile.userID, index);
    }
    */
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


// 해시태그 해시테이블

// 해시 함수 (djb2 쓰는 사람 많다길래 31 기반 polynomial hash 사용)
unsigned int HashTag(const char* hashTagName, int tableSize){ // const char* hashTagName : 해당 문자열의 첫글자 그래서 \0로 구분, const : 해당 문자열에서 내용 변경 안함
    unsigned long hash = 0;

    while (*hashTagName != '\0'){
        hash = hash * 31 + (unsigned char)(*hashTagName); // 각 문자의 아스키값 누적 + 기존 해시값에 31곱해서 문자 순서에 따른 차이 반영
        hashTagName++; // 문자열에서 가리키는 포인터 변경
    }
    return (unsigned int)(hash % tableSize);
}

// 테이블 생성
HashTagHashTable* CreateHashTagHashTable(int tableSize){
    if(tableSize <= 0){
        return NULL;
    }

    HashTagHashTable* table = (HashTagHashTable*)malloc(sizeof(HashTagHashTable));

    if(table==NULL){ // 동적할당 실패시 예외처리
        return NULL; 
    }

    table->tableSize = tableSize;
    table->bucket = (HashTagHashNode**)malloc(sizeof(HashTagHashNode*)*tableSize); // 테이블 사이즈 만큼 버킷 생성
    if(table->bucket == NULL){
        free(table);
        return NULL;
    }

    for(int i=0; i<tableSize; i++){
        table->bucket[i] = NULL; // 버킷 내부 초기화
    }
    return table;
} 

// 테이블 삽입 
int InsertHashTag(HashTagHashTable* table, const char* hashTagName, long long postID){
    // 문자열 해시 함수 이용 -> hashTagName 들어갈 버킷 위치 계산
    unsigned int index = HashTag(hashTagName, table->tableSize); 
    // 버킷의 첫번째 해시태그 노드부터 탐색 -> 같은 index에 여러 키값이 들어갈 수 있어서 분리연결법으로 확인해야함
    HashTagHashNode* current = table->bucket[index]; 

    while(current != NULL) { // 버킷 안에 같은 해시태그가 있는지 확인
        // 문자열 비교
        if(strcmp(current->hashTagName, hashTagName) == 0){
            // 같은 해시태그가 존재하면 그 해시태그에 연결될 postID 노드만 생성
            PostIDNode* newPost = (PostIDNode*)malloc(sizeof(PostIDNode));

            if(newPost == NULL){
                return 1;
            }

            // 새 게시물ID 저장함
            newPost->postID = postID;
            // 기존 postList 맨앞에 postID 노드 삽입 (push-front)
            newPost->next = current->postList;
            current->postList = newPost;

            return 0;
        }
        // 노드의 키값이 다르면 해시충돌 체이닝 다음 hashTagHashNode로 이동
        current = current->next;
    }

    // 같은 버킷 안에 같은 해시 태그가 없으면 여기에서 삽입함
    
    // 신규 해시태그라서 새 HashTagHashNode 생성 (종류 11개라서 사실상 11번하고 나머지는 저 위에 while에서 처리함)
    HashTagHashNode* newNode = (HashTagHashNode*)malloc(sizeof(HashTagHashNode));
    if(newNode == NULL){
        return 1;
    }

    // 해시태그 문자열 저장
    strcpy(newNode->hashTagName, hashTagName);
    newNode->postList = NULL; // 연결된 postID가 아직은 없으니 NULL로 초기화

    // 신규 해시태그의 첫 postID 노드 생성
    PostIDNode* newPost = (PostIDNode*)malloc(sizeof(PostIDNode));

    if(newPost == NULL){
        free(newNode);
        return 1;
    }

    newPost->postID = postID;
    newPost->next = NULL;

    // 새로운 해시태그가 첫번째 postID를 가리키게 연결
    newNode->postList = newPost;
    // 해시충돌은 새 해시태그 노드 삽입 -> 버킷의 기존 대가리 앞에 삽입
    newNode->next = table->bucket[index];
    table->bucket[index] = newNode;

    return 0;
}

// 테이블 검색
HashTagHashNode* SearchHashTag(HashTagHashTable* table, const char* hashTagName){
    // 검색할 해시태그 버킷 위치 계산 (해시 계산 -> 인덱스)
    unsigned int index = HashTag(hashTagName, table->tableSize);

    // 해당 버킷의 첫노드부터 탐색하기
    HashTagHashNode* current = table->bucket[index];

    // 해시충돌 체이닝 순회 탐색
    while(current != NULL){
        // 문자열이 같으면 성공
        if(strcmp(current->hashTagName, hashTagName) == 0){
            return current;
        }
        // 실패시 다음꺼 탐색
        current = current->next;
    }
    // 없음, 탐색 실패
    return NULL;
}

// 할당 해제
void FreeHashTagHashTable(HashTagHashTable* table){
    if(table==NULL){
        return;
    }

    // 모든 버킷 순회
    for(int i=0; i<table->tableSize; i++){
        HashTagHashNode* current = table->bucket[i];

        // 버킷 내부 해시충돌 체이닝 순회 
        while(current != NULL){
            // 할당 해제하기 전에 다음꺼 저장
            HashTagHashNode* nextHashTag = current->next;
            // 해시태그 내부 postID 목록 해제
            PostIDNode* postCurrent = current->postList;
            while(postCurrent != NULL){
                // 이거도 다음거 날리기 전에 다음꺼 저장
                PostIDNode* nextPost = postCurrent->next;
                free(postCurrent);
                postCurrent = nextPost;
            }

            // postID 해제 후 해시태그 자체 날림
            free(current); // 여기서 날린건 해시태그 노드
            current = nextHashTag;
        }
    }
    // table, table->bucket 날리기 (create에서 할당한거 날리기)
    free(table->bucket); // 여기서 날린건 포인터 배열
    free(table);
}