#include<stdio.h>
#include<stdlib.h>

#define MIN_USER 100000LL
#define MAX_USER 100000000LL
#define MIN_EDGE 500000LL
#define MIN_HASHTAG 50000LL
#define MIN_POST 200000LL

#define CONFIG_FILE "data/generatorConfig.txt" // 생성할 파일 상대 경로 (괜히 오타낼까봐)

// 해당 프로그램 역할 : 교수님께서 데이터 셋 변경해서 테스트 하신다고 하셔서 그거 대비용으로 
//                    데이터 셋 생성기들에 데이터 량을 입력받고 요구조건 이하 혹은 초과 시의 예외처리 수행
// 입력 버퍼 지우기 (잘못된 입력시의 예외처리용)
void ClearBuffer(){
    int index;
    while((index = getchar()) != '\n' && index != EOF){
        // 남아 있는 입력 날리기
    }
}

// 데이터 입력 함수 (앞부분에 출력될 텍스트, 최소값, 최대값)
long long InputData(const char* message, long long min, long long max){
    long long value;

    while(1){
        // 출력되는건 여기서 따로 작성하기엔 
        // 뭐 받는지 체크 할게 필요해져서 매개변수로 받아오게 설정함
        printf("%s", message); 
        // 문자 입력시
        if(scanf("%lld", &value) != 1){
            printf("잘못된 입력입니다. 정수로만 입력해주세요.\n");
            // 버퍼 삭제
            ClearBuffer();
            continue;
        }
        ClearBuffer();
        
        // 최소값 미만으로 입력시
        if(value < min){
            printf("잘못된 입력입니다. 최소값은 %lld입니다.\n", min);
            continue;
        }
        
        // 최대값 초과로 입력시
        if(max != -1 && value > max){
            printf("잘못된 입력입니다. 최대값은 %lld입니다.\n", max);
            continue;
        }

        return value;
    }
}

// 파일 임의로 변경 확인용 (이건 굳이 함수로 뺄 필요는 없는데 각 생성기에서 다시 만들자니 귀찮아서)
// 그리고 각 값들에 곱해놓은건 내 생일임 일단 곱해서 합치면 뭔값인지 한번에 알기 어려우니 뭐 안건들거 같은데 확인용임
long long CheckSum(long long user, long long edge, long long hashTag, long long post){
    return user*0 + edge*1 + hashTag*2 + post*9; 
}


int main(){
    long long user, edge, hashTag, profile, post; // 유저, 간선, 해시태그, 프로필, 게시물 
    // edge는 최소 500,000 이상 + (user*(user-1))/2 보다 작아야함 (무방향 그래프라서)
    long long maxEdge;

    printf("================================================\n");
    printf("            데이터 규모 설정 프로그램             \n"); // 오.. 거의 딱 맞춤
    printf("================================================\n");

    // user 수
    user = InputData("사용자 수 입력 (100,000 ~ 100,000,000) : ", MIN_USER, MAX_USER);

    // edge 수 : 무방향 그래프에서 중복 제거한 최대 = user*(user-1)/2
    maxEdge = user*(user-1)/2;
    char message[100];
    // 문자열 안에 간선 최대치 삽입
    snprintf(message, sizeof(message), "간선 수 입력 (500,000 ~ %lld) : ", maxEdge);
    edge = InputData(message, MIN_EDGE, maxEdge);

    // hashTag 수 (최대치 제한이 없긴 한데 데이터 생성하는거 보고 유저랑 동일하게 하던가 할거임)
    hashTag = InputData("해시태그 수 입력 (최소 50,000) : ", MIN_HASHTAG, -1);

    // post 수
    post = InputData("게시물 수 입력 (최소 200,000) : ", MIN_POST, -1);

    // 파일 임의 변경 확인용 텍스트
    long long check = CheckSum(user, edge, hashTag, post);

    printf("================================================\n");
    printf("               입력된 데이터셋 규모               \n"); 
    printf("================================================\n");

    printf("USER : %lld\n", user);
    printf("EDGE : %lld\n", edge);
    printf("HASHTAG : %lld\n", hashTag);
    printf("POST : %lld\n", post);
    printf("================================================\n");

    FILE* file = fopen(CONFIG_FILE, "w");
    if (file == NULL){
        printf("파일 생성 실패 (%s)\n", CONFIG_FILE);
        return 1;
    }

    fprintf(file, "DATA_STRUCTURE_PROJECT_CONFIG\n"); // 파일 식별용 문자열
    fprintf(file, "USER=%lld\nEDGE=%lld\nHASHTAG=%lld\nPOST=%lld\nCHECK=%lld\n", user, edge, hashTag, post, check);

    fclose(file);

    return 0;
}