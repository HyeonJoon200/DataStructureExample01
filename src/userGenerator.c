#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CONFIG_FILE "data/generatorConfig.txt"
#define USER_FILE "data/users.txt"

int main(void) {
    // 코드 실행 시간 측정용 
    clock_t start = clock();

    long long user;
    FILE* configFile = fopen(CONFIG_FILE, "r");

    if (configFile == NULL){
        printf("Failed to open configuration file. (%s)\n", CONFIG_FILE);
        return 1;
    }

    // 파일 식별용 문자열 
    char header[100];
    if(fgets(header, sizeof(header), configFile) == NULL){
        printf("Failed to read configuration file.\n");
        fclose(configFile);
        return 1;
    }

    // 식별용 문자열이 맞는지 확인
    if(strcmp(header, "DATA_STRUCTURE_PROJECT_CONFIG\n") != 0){
        printf("Invaild configuration file. (Header mismatch)\n");
        fclose(configFile);
        return 1;
    }

    // USER 값 읽어 오기
    if (fscanf(configFile, "USER=%lld\n", &user) != 1){
        printf("Failed to read USER data.\n");
        fclose(configFile);
        return 1;
    }

    // 파일 닫기
    fclose(configFile);

    printf("Number of users to generate : %lld\n", user);
    FILE* userFile = fopen(USER_FILE, "w");

    if(userFile == NULL){
        printf("Failed to create user file. (%s)\n", USER_FILE);
        return 1;
    }

    // 실질적인 유저 ID 및 유저명 생성 
    for(long long i = 1; i<=user; i++){
        fprintf(userFile, "%lld User%lld\n", i, i);
    }
    fclose(userFile);

    printf("User count : %lld\n", user);

    // 코드 종료 시간 측정
    clock_t end = clock();

    // 실행시간 계산
    double time = (double)(end-start) / CLOCKS_PER_SEC;
    printf("Execution time : %f s\n", time);
    printf("Execution time : %f ms\n", time * 1000.0);
    
    return 0;
}