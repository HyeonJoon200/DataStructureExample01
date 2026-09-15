#include <stdio.h> // 입출력하려면 필수
#include <stdlib.h> // 뭐 언젠간 쓰겠지
#include <time.h> // 나중에 코드 실행시간 측정용으로 넣음
// 모든 변수명은 camel 표기법으로 작성 (한단어면 그냥 소문자로 고정)
int main(){
    // 파일 열기 (수정하면 안돼니까 읽기모드로 열기)
    FILE* file = fopen("graph.txt", "r");
    if (file == NULL){
        printf("파일을 찾을 수 없습니다.\n");
        return 1; // 파일 못찾으면 바로 종료 (실질적으로 나올일 없는데 보험용)
    }

    int user1, user2;
    int edgeCount = 0;

    // EOF : 더이상 읽을거 없을때 c언어가 보내는 신호 => 더이상 읽을게 없을때 까지 반복
    while(fscanf(file, "%d %d", &user1, &user2) != EOF) {
        // 인접리스트 코드 작성 (나중에 교수님이 견본 코드 주시지 않을까...)
        edgeCount++; // 일단 읽어왔으니까 간선 갯수 추가
    }
    printf("총 %d개의 간선 데이터를 읽음 \n", edgeCount);

    // malloc처럼 다 썼으면 해제해줘야함
    fclose(file);
    return 0;
}