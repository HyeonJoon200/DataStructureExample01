#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "generatorUtil.h"

#define HASHTAG_FILE "data/hashTags.txt"
#define HASHTAG_TYPE 11 // 해시태그 목록 : 11개 


int GeneratorHashtag(){
    // 해시태그 목록 (임시로 게임 이름으로 떼움)
    const char* hashTagName[] = {
        "Destiny2",
        "KingdomCome2",
        "WarHammer40K",
        "EldenRing",
        "Concord",
        "Marathon", 
        "EscapeFromTarkov",
        "SlayTheSpire2",
        "BaldursGate3",
        "ForHonor",
        "HellDivers2"
    };

    GeneratorConfig config;
    if(LoadGeneratorConfig(&config) != 0){
        return 1;
    }

    // 현재 설계에선 게시물 하나당 해시태그 1개가 최대 (최소는 0개, 나중에 피드백 받고 이렇게 하면 안됀다 하시면 변경예정)
    if (config.hashTag > config.post){
        printf("ERROR : hashtag count cannot exceed post count.\n");
        return 1;
    }

    // 게시물 ID 생성 (postID)
    long long* postID = (long long*)malloc(sizeof(long long) * config.post);
    // 할당 실패
    if (postID == NULL){
        printf("Failed to allocate \n");
        return 1;
    }
    // 게시물 번호 = 1번 부터 시작 (이거 게시물 ID랑 엮어서 저장해야해서 쩔 수 없음)
    for(long long i = 0; i < config.post; i++){
        postID[i] = i + 1;
    }

    // 필요한 부분만 섞기 (딱 해시태그 갯수만큼만!)
    // postID[0] ~ postID[hashTag - 1] 중복 안되게끔함 (게시물당 해시태그 최대치가 1개임)
    for(long long i = 0; i < config.hashTag; i++){
        long long randomIndex = i + RandomID(config.post - i);
        long long temp = postID[i];
        postID[i] = postID[randomIndex];
        postID[randomIndex] = temp;
    }

    FILE* hashTagFile = fopen(HASHTAG_FILE, "w");

    if (hashTagFile == NULL){
        printf("Failed to create hashtag file (%s)\n", HASHTAG_FILE);
        free(postID);
        return 1;
    }
    // 해시태그 랜덤 배정
    for (long long i = 0; i < config.hashTag; i++){
        long long postIDs = postID[i]; // 이거 나중가면 이름 헷갈릴거 같으니까 나중에 문장형으로 바꾸던가 해서 안헷갈리게 하기
        int hashTagID = (int)RandomID(HASHTAG_TYPE) + 1;
        fprintf(hashTagFile, "%lld %d %s\n", postIDs, hashTagID, hashTagName[hashTagID - 1]);
    }
    fclose(hashTagFile);
    free(postID);

    printf("%lld hashtag data generated successfully.\n", config.hashTag);

    return 0;
}