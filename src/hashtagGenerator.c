#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define HASHTAG_COUNT 50000 // 프로필 및 해시태그 

int main(){
    // 해시태그 목록 (임시로 게임 이름으로 떼움)
    const char* hashTag[] = {
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

    // 해시태그 가짓수 (해시태그 배열 전체 크기 / 해시태그 배열 한칸 크기 = 해시태그 배열 인덱스 갯수)
    int hashTagCount = sizeof(hashTag) / sizeof(hashTag[0]);

    // srand 로 난수 초기화 (시간 기준으로 시드 뽑기) (rand()이게 한계점이 있음)
    srand((unsigned int)time(NULL));

    // 프로필 저장할 파일 생성 (생성이니까 당연히 쓰기 모드로 열어야함)
    FILE* file = fopen("data/hashtag.txt", "w"); // 교수님 파일 경로를 모르니 상대 경로로 하는게 편할듯함
    if(file == NULL){
        printf("hashtag.txt 파일 생성 실패\n");
        return 1;
    }

    // 해시태그 데이터 생성 (테스트 용은 상수로 데이터 개수 세팅함 50000개)
    for(int i=1; i<= HASHTAG_COUNT; i++){
        int userID = i;
        int hashTagIndex = rand() % hashTagCount;

        fprintf(file, "%d %s\n", userID, hashTag[hashTagIndex]);
    }
    fclose(file);
    printf("해시태그 데이터 %d개 생성 완료\n", HASHTAG_COUNT);
    
    return 0;
}