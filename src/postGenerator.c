#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CONFIG_FILE "data/generatorConfig.txt"
#define HASHTAG_FILE "data/hashTags.txt"
#define POST_FILE "data/posts.txt"

// 간선 생성기꺼 재탕
long long RandomID(long long range){
    unsigned int random1 = rand() & 0x7FFF;
    unsigned int random2 = rand() & 0x7FFF;
    unsigned int result = (random1 << 15) | random2;

    return (long long)(result % (unsigned long long)range);
}


// 게시물 작성후 경과 시간 생성기 (데이터 생성할때 시간 기준으로 하면 그게 그거라서 공식쓰기 귀찮아짐)
static long long RandomPassedTime(){
    int a = (int)RandomID(5); // 이름 뭐할까 하다 함수 안이니까 대충 씀
    switch(a){
        case 0:
            // 0~1시간
            return RandomID(60*60); 
        case 1:
            // 1~24시간
            return (60*60) + RandomID(23*60*60);
        case 2:
            // 24시간 ~ 7일
            return (24*60*60) + RandomID(6*24*60*60);
        case 3:
            // 7일 ~ 30일
            return (7*24*60*60) + RandomID(23*24*60*60);
        default:
            // 30일 ~ 60일
            return (30*24*60*60) + RandomID(30*24*60*60);
    }
}

int GeneratorPost(){
    long long user, edge, hashTag, post, check;

    FILE* configFile = fopen(CONFIG_FILE, "r");
    if(configFile==NULL){
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
        printf("Invaild config file.\n");
        fclose(configFile);
        return 1;
    }

    if(fscanf(configFile, "USER=%lld\nEDGE=%lld\nHASHTAG=%lld\nPOST=%lld\nCHECK=%lld\n", &user, &edge, &hashTag, &post, &check) != 5){
        printf("Failed to read config file data\n");
        fclose(configFile);
        return 1;
    }
    fclose(configFile); // 여기 까지는 해시태그, 프로필 생성기랑 거의 동일 (반복해서 쓰면서 그냥 함수로 만들었어야 했나 싶긴 한데)

    // posts.txt 에 저장되는 내용 : postID, userID, title, content, createdTime, likes
    // 해시태그에 대한 데이터는 hashtags.txt에서 postID, hashTagID, hashTagName에 저장되어있는 게시물ID로 나중에 연결해서 읽으면될듯함
    FILE* postFile = fopen(POST_FILE, "w");
    if (postFile == NULL){
        printf("Failed to create post file. (%s)\n", POST_FILE);
        return 1;
    }

    srand((unsigned int)time(NULL));

    long long currentTime = (long long)time(NULL); // 현재 시간
    for(long long postID = 1; postID <= post; postID++){
        long long userID = RandomID(user) + 1; // 작성자 ID
        // 현재 시간 기준으로 게시 시간 설정 (과거)
        long long passedTime = RandomPassedTime();
        long long createdTime = currentTime - passedTime;

        // 좋아요수 0~10000
        long long likes = RandomID(10001);

        fprintf(postFile, "%lld %lld Title%lld Content%lld %lld %lld\n", postID, userID, postID, postID, createdTime, likes);
    }
    fclose(postFile);
    printf("%lld posts generated successfully\n", post);
    return 0;
}