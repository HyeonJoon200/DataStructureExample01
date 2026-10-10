#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "generatorUtil.h"

#define HASHTAG_FILE "data/hashTags.txt"
#define POST_FILE "data/posts.txt"



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
    GeneratorConfig config;
    if (LoadGeneratorConfig(&config) != 0){
        return 1;
    }

    // posts.txt 에 저장되는 내용 : postID, userID, title, content, createdTime, likes
    // 해시태그에 대한 데이터는 hashtags.txt에서 postID, hashTagID, hashTagName에 저장되어있는 게시물ID로 나중에 연결해서 읽으면될듯함
    FILE* postFile = fopen(POST_FILE, "w");
    if (postFile == NULL){
        printf("Failed to create post file. (%s)\n", POST_FILE);
        return 1;
    }

    long long currentTime = (long long)time(NULL); // 현재 시간
    for(long long postID = 1; postID <= config.post; postID++){
        long long userID = RandomID(config.user) + 1; // 작성자 ID
        // 현재 시간 기준으로 게시 시간 설정 (과거)
        long long passedTime = RandomPassedTime();
        long long createdTime = currentTime - passedTime;

        // 좋아요수 0~10000
        long long likes = RandomID(10001);

        fprintf(postFile, "%lld %lld Title%lld Content%lld %lld %lld\n", postID, userID, postID, postID, createdTime, likes);
    }
    fclose(postFile);
    printf("%lld posts generated successfully\n", config.post);
    return 0;
}