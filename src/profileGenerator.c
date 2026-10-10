#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "generatorUtil.h"

#define PROFILE_FILE "data/profiles.txt"

#define REGION_TYPE 8  // 뭐 이건 나중에 늘리던가 해야지
#define INTEREST_TYPE 8
#define STATUS_TYPE 3 // 온라인, 오프라인, 취침


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

    GeneratorConfig config;
    if(LoadGeneratorConfig(&config) != 0){
        return 1;
    }

    FILE* profileFile = fopen(PROFILE_FILE, "w");
    if(profileFile == NULL){
        printf("Failed to create profile file. (%s)\n", PROFILE_FILE);
        return 1;
    }

    // 프로필은 유저수와 동일
    for(long long i = 1; i <= config.user; i++){
        int age = (int)RandomID(76) + 15; // 15 ~ 100세
        int regionIndex = (int)RandomID(REGION_TYPE);
        int interestIndex = (int)RandomID(INTEREST_TYPE);
        int statusIndex = (int)RandomID(STATUS_TYPE);
        // userID userAge 지역(국가) 관심사 상태
        fprintf(profileFile, "%lld %d %s %s %s\n", i, age, region[regionIndex], interest[interestIndex], status[statusIndex]);
    }
    fclose(profileFile);

    printf("%lld profiles generated successfully.\n", config.user);
    return 0;
}