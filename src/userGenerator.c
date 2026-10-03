#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NODE 100000

int main(void) {
    // 코드 실행 시간 측정용 
    clock_t start = clock();

    srand((unsigned int)time(NULL));

    // 쓰기(write) 모드로 파일 열기 (안만들어 놨는데 뭐지)
    FILE *file = fopen("data/user.txt", "w");
    if (file == NULL) {
        printf("파일 생성 실패 (user.txt)\n");
        return 1;
    }

    printf("데이터 생성 시작 (NODE: %d)\n", NODE);
    int count;
    for(count=1; count<=NODE; count++){
        fprintf(file, "%d User%d\n", count, count);
        if(count==NODE)
            fprintf(file, "%d명 유저 생성\n", count);
    }

    fclose(file);
    printf("데이터 생성 완료 (NODE: %d)\n", count-1);

    // 코드 종료 시간 측정
    clock_t end = clock();

    // 실행시간 계산
    double time = (double)(end-start) / CLOCKS_PER_SEC;
    printf("실행시간 : %f s\n", time);
    printf("실행시간 : %f ms\n", time * 1000.0);
    
    return 0;
}