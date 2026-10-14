#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// UserID HashTable Search

typedef struct {
    long long userID;
    int age;
    char region[20];
    char interest[20];
    char status[20];
} UserProfile;

typedef struct UserHashNode {
    long long key; // 탐색용 키값 hash 함수로 지정할 듯?
    UserProfile profile; // 해시테이블에 프로필 저장해야함..
    struct UserHashNode* next; 
} UserHashNode;

typedef struct {
    int tableSize;
    UserHashNode** bucket;
} UserHashTable;

unsigned int HashUserID(long long userID, int tableSize);
UserHashTable* CreateUserHashTable(int tableSize);
int InsertProfile(UserHashTable* table, UserProfile profile);
UserProfile* SearchProfile(UserHashTable* table, long long userID);
void FreeUserHashTable(UserHashTable* table);


// HashTag HashTable Search

typedef struct PostIDNode{ // 하나의 해시태그에 연결된 게시물의 목록 
    long long postID;
    struct PostIDNode* next;
} PostIDNode;

typedef struct HashTagHashNode { // 하나의 해시태그 키(key)
    char hashTagName[50];
    PostIDNode* postList;
    // 해시 충돌 해결용 체이닝
    struct HashTagHashNode* next;
} HashTagHashNode;

typedef struct {
    int tableSize;
    HashTagHashNode** bucket;
}HashTagHashTable;

unsigned int HashTag(const char* hashTagName, int tableSize);
HashTagHashTable* CreateHashTagHashTable(int tableSize);
int InsertHashTag(HashTagHashTable* table, const char* hashTagName, long long postID);
HashTagHashNode* SearchHashTag(HashTagHashTable* table, const char* hashTagName);
void FreeHashTagHashTable(HashTagHashTable* table);

#endif 