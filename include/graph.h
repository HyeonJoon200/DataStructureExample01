// 표기법 정리 
// 변수 : 카멜(camel) 표기 (userName)
// 그외 : 파스칼(pascal) 표기 (UserName)

#ifndef GRAPH_H
#define GRAPH_H
typedef struct ListNode { // 인접 리스트 
    int userID; // 정점 (유저 번호) (유저ID는 1~100,000) 카멜 표기에 안맞긴 한데 ID가 보기 좋아서..
    // 가중치는 어차피 전부 1이라 생략함 (이럼 다익스트라 쓸 이유가 있나.. BFS 쓰고 싶다)
    struct ListNode* next; // 다음 인접 노드를 가리키는 포인터
    
} ListNode;
typedef struct Graph { // 그래프
    int userCount; // 정점의 총합 개수 (100,000개)
    struct ListNode** list; // 인접 리스트
} Graph;


ListNode* CreateListNode(int userID);
Graph* CreateGraph(int nodeCount); // nodeCount = 노드 갯수 (100,000개)
int IsConnected(Graph* graph, int user1, int user2); // user1,2 : 두 정점 (start, end로 써도 될듯)
void AddEdge(Graph* graph, int user1, int user2);
void FreeGraph(Graph* graph);

#endif