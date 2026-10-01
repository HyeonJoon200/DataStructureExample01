#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// 해당 프로그램 역할 : 교수님께서 데이터 셋 변경해서 테스트 하신다고 하셔서 그거 대비용으로 
//                    데이터 셋 생성기들에 데이터 량을 입력받고 요구조건 이하 혹은 초과 시의 예외처리 수행

int main(){
    // 데이터 생성기들이 읽을 데이터량을 넣어둘 파일 생성
    FILE* file = fopen("data/dataManager.txt", "w");
    long long user, edge, hashTag, profile, post; // 유저, 간선, 해시태그, 프로필, 게시물 
    // user는 최소 100,000 이상 + 양의 정수 데이터만 받게 예외처리
    // edge는 최소 500,000 이상 + (user*(user-1))/2 보다 작게 + 양의 정수 데이터만 받게 예외처리
    // hashTag, profile은 최소 50,000 이상 + 양의 정수 데이터만 받게 예외처리
    // post는 최소 200,000 이상 + 양의 정수 데이터만 받게 예외처리


    // 유저 수 입력 받고 예외 처리 이거 좀더 깎을 여지가 보이는데 일단 여기서 중단.
    while(1){
        printf("유저수를 입력하세요 : ");
        if (scanf("%lld", &user) != 1 || user < 100000) {
            printf("잘못된 입력입니다. 100,000이상의 정수만 입력해주세요.\n");
            while(getchar() != '\n'); // 버퍼 날리기
            continue;
        }
        break;
    }

}