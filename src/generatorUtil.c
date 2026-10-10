#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generatorUtil.h"

#define CONFIG_FILE "data/generatorConfig.txt"

int LoadGeneratorConfig(GeneratorConfig* config){
    FILE* configFile = fopen(CONFIG_FILE, "r");
    char header[100];

    if (configFile == NULL){
        printf("Failed to open data size configuration file.(%s)\n", CONFIG_FILE);
        printf("Please run the program from /DataStructureProject/.\n");
        printf("Please run generatorManager.c first.\n");
        return 1;
    }

    // 파일 읽기 실패시
    if(fgets(header, sizeof(header), configFile) == NULL){
        printf("Failed to read configuration file. \n");
        fclose(configFile);
        return 1;
    }
    // 식별 문자열이 맞지 않을 시
    if(strcmp(header, "DATA_STRUCTURE_PROJECT_CONFIG\n") != 0){
        printf("Invaild configuration file. (Header mismatch)\n");
        fclose(configFile);
        return 1;
    }
    // 파일 내부 데이터 읽어오고 목록 숫자 안맞으면 fail
    if(fscanf(configFile, "USER=%lld\nEDGE=%lld\nHASHTAG=%lld\nPOST=%lld\nCHECK=%lld\n", &config->user, &config->edge, &config->hashTag, &config->post, &config->check) != 5){
        printf("Failed to read configuration data.\n");
        fclose(configFile);
        return 1;
    }
    fclose(configFile);
    return 0;
}

long long RandomID(long long range){
    unsigned int random1 = rand() & 0x7FFF;
    unsigned int random2 = rand() & 0x7FFF;
    unsigned int result = (random1 << 15) | random2;
    return (long long)(result % (unsigned long long)range);
}