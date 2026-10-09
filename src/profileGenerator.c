#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CONFIG_FILE "data/generatorConfig.txt"
#define PROFILE_FILE "data/profiles.txt"

#define REGION_TYPE 8  // 뭐 이건 나중에 늘리던가 해야지
#define INTEREST_TYPE 8
#define STATUS_TYPE 3 // 온라인, 오프라인, 취침

// 간선 생성기꺼 재탕
static long long RandomID(long long range){
    unsigned int random1 = rand() & 0x7FFF;
    unsigned int random2 = rand() & 0x7FFF;
    unsigned int result = (random1 << 15) | random2;

    return (long long)(result % (unsigned long long)range);
}

int GeneratorProfile(){
    const char* region[REGION_TYPE] = {
        "Korea", "Japan", "China", "USA", "Canada", "Germany", "France", "UK"
    };
    const char* interest[INTEREST_TYPE] = {
        "Game", "Music", "Movie", "Sports", "Study", "Travel", "Art", "Book"
    };
    const char* status[STATUS_TYPE] = {
        "Online", "Offline", "Away"
    };

    long long user, edge, hashTag, post, check; // check는 사실상 안쓸거 같긴 한데 일단 2차때 검증용으로 사용

    FILE* configFile = fopen(CONFIG_FILE, "r");
    if(configFile == NULL){
        printf("Failed to open file. (%s)\n", CONFIG_FILE);
        return 1;
    }

    // 식별용 문자열
    char header[100];

    if(fgets(header, sizeof(header), configFile) == NULL){
        printf("Failed to read file. (%s)\n", CONFIG_FILE);
        fclose(configFile);
        return 1;
    }
    if(strcmp(header, "DATA_STRUCTURE_PROJECT_CONFIG\n") != 0){
        printf("Invaild config file\n");
        fclose(configFile);
        return 1;
    }

    // 파일 읽어오기
    if(fscanf(configFile, "USER=%lld\nEDGE=%lld\nHASHTAG=%lld\nPOST=%lld\nCHECK=%lld\n", &user, &edge, &hashTag, &post, &check) != 5){
        printf("Failed to read config data\n");
        fclose(configFile);
        return 1;
    }

    FILE* profileFile = fopen(PROFILE_FILE, "w");
    if(profileFile == NULL){
        printf("Failed to create profile file. (%s)\n", PROFILE_FILE);
        return 1;
    }

    srand((unsigned int)time(NULL));
    // 프로필은 유저수와 동일
    for(long long i = 1; i <= user; i++){
        int age = (int)RandomID(76) + 15; // 15 ~ 100세
        int regionIndex = (int)RandomID(REGION_TYPE);
        int interestIndex = (int)RandomID(INTEREST_TYPE);
        int statusIndex = (int)RandomID(STATUS_TYPE);
        // userID userAge 지역(국가) 관심사 상태
        fprintf(profileFile, "%lld %d %s %s %s\n", i, age, region[regionIndex], interest[interestIndex], status[statusIndex]);
    }
    fclose(profileFile);

    printf("%lld profiles generated successfully.\n", user);
    return 0;
}