#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CONFIG_FILE "data/generatorConfig.txt"
#define HASHTAG_FILE "data/hashTags.txt"
#define HASHTAG_TYPE 11 // 해시태그 목록 : 11개 

// 간선 생성기꺼 재탕 
// rand() 두개 써서 한개는 15비트 왼쪽으로 옮기고 나머지 합쳐서 30비트 난수 하나 만들기
static long long RandomID(long long range){
    unsigned int random1 = rand() & 0x7FFF;
    unsigned int random2 = rand() & 0x7FFF;

    unsigned int result = (random1 << 15) | random2;

    return (long long)(result % (unsigned long long)range);
}
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

    long long user, edge, hashTag, post, check; // 유저수, 간선수, 해시태그 수(종류 아님), 게시물 수, 파일 수정 여부 확인

    FILE* configFile = fopen(CONFIG_FILE, "r");
    if(configFile == NULL){
        printf("Fail to open file. (%s)\n", CONFIG_FILE); // 한글 인코딩 문제 해결용 영어로만 출력
        return 1;
    }
    
    // 식별용 문자열
    char header[100];
    if(fgets(header, sizeof(header), configFile) == NULL){
        printf("Failed to open file. %s\n", CONFIG_FILE);
        fclose(configFile);
        return 1;
    }

    if (strcmp(header, "DATA_STRUCTURE_PROJECT_CONFIG\n") != 0){
        printf("Invaild config file.\n");
        fclose(configFile);
        return 1;
    }
    // 파일 읽어와서 5종류의 데이터가 읽혀 오면 통과 아니면 예외처리 문구 출력
    if(fscanf(configFile, "USER=%lld\nEDGE=%lld\nHASHTAG=%lld\nPOST=%lld\nCHECK=%lld\n", &user, &edge, &hashTag, &post, &check) != 5){
        printf("Failed to read config data.\n");
        fclose(configFile);
        return 1;
    }
    fclose(configFile);

    // 현재 설계에선 게시물 하나당 해시태그 1개가 최대 (최소는 0개, 나중에 피드백 받고 이렇게 하면 안됀다 하시면 변경예정)
    if (hashTag > post){
        printf("ERROR : hashtag count cannot exceed post count.\n");
        return 1;
    }

    // 게시물 ID 생성 (postID)
    long long* postID = (long long*)malloc(sizeof(long long) * post);
    // 할당 실패
    if (postID == NULL){
        printf("Failed to allocate \n");
        return 1;
    }
    // 게시물 번호 = 1번 부터 시작 (이거 게시물 ID랑 엮어서 저장해야해서 쩔 수 없음)
    for(long long i = 0; i < post; i++){
        postID[i] = i + 1;
    }

    // 난수 초기화
    srand((unsigned int)time(NULL));

    // 필요한 부분만 섞기 (딱 해시태그 갯수만큼만!)
    // postID[0] ~ postID[hashTag - 1] 중복 안되게끔함 (게시물당 해시태그 최대치가 1개임)
    for(long long i = 0; i < hashTag; i++){
        long long randomIndex = i + RandomID(post - i);
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
    for (long long i = 0; i < hashTag; i++){
        long long postIDs = postID[i]; // 이거 나중가면 이름 헷갈릴거 같으니까 나중에 문장형으로 바꾸던가 해서 안헷갈리게 하기
        int hashTagID = (int)RandomID(HASHTAG_TYPE) + 1;
        fprintf(hashTagFile, "%lld %d %s\n", postIDs, hashTagID, hashTagName[hashTagID - 1]);
    }
    fclose(hashTagFile);
    free(postID);

    printf("%lld hashtag data generated successfully.\n", hashTag);

    return 0;
}