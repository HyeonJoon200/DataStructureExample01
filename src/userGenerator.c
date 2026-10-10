#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "generatorUtil.h"

#define USER_FILE "data/users.txt"

int GeneratorUser(void) {
    // 코드 실행 시간 측정용 
    clock_t start = clock();

    GeneratorConfig config;
    if(LoadGeneratorConfig(&config) != 0){
        return 1;
    }

    printf("Number of users to generate : %lld\n", config.user);
    FILE* userFile = fopen(USER_FILE, "w");

    if(userFile == NULL){
        printf("Failed to create user file. (%s)\n", USER_FILE);
        return 1;
    }

    // 실질적인 유저 ID 및 유저명 생성 
    for(long long i = 1; i<=config.user; i++){
        fprintf(userFile, "%lld User%lld\n", i, i);
    }
    fclose(userFile);

    printf("User count : %lld\n", config.user);

    // 코드 종료 시간 측정
    clock_t end = clock();

    // 실행시간 계산
    double time = (double)(end-start) / CLOCKS_PER_SEC;
    printf("Execution time : %f s\n", time);
    printf("Execution time : %f ms\n", time * 1000.0);
    
    return 0;
}