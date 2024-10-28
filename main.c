#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL 0
#define PATH 1
#define START 5
#define END 9
#define VISITED 2

// 좌표를 나타내는 구조체
typedef struct {
    int x;
    int y;
} Position;

// 스택 구조체
typedef struct {
    Position* data;
    int top;
} Stack;

// 스택 초기화
void initStack(Stack* stack, int size) {
    stack->data = (Position*)malloc(size * sizeof(Position));
    stack->top = -1;
}

// 스택에 위치 추가
void push(Stack* stack, Position pos) {
    stack->data[++stack->top] = pos;
}

// 스택에서 위치 꺼내기
Position pop(Stack* stack) {
    return stack->data[stack->top--];
}

// 스택이 비어있는지 확인
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// 스택 메모리 해제
void freeStack(Stack* stack) {
    free(stack->data);
}

// 미로 출력
void printMaze(int** maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (maze[i][j] == WALL)
                printf("# ");
            else if (maze[i][j] == PATH)
                printf("  ");
            else if (maze[i][j] == START)
                printf("S ");
            else if (maze[i][j] == END)
                printf("E ");
            else
                printf("2 ");
        }
        printf("\n");
    }
}

// 미로 초기화 및 생성
void initializeMaze(int** maze, int width, int height) {
    // 모든 위치를 벽으로 초기화
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze[i][j] = WALL;
        }
    }

    Stack stack;
    initStack(&stack, width * height);

    // 미로 생성 시작점
    int x = 1, y = 1;
    maze[y][x] = PATH;
    push(&stack, (Position){ x, y });

    // 방향 배열 (상하좌우)
    Position directions[] = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };
    srand(time(NULL));

    // 미로 생성 알고리즘 (DFS)
    while (!isEmpty(&stack)) {
        Position current = pop(&stack);
        int dirIndices[] = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; i++) {
            int r = rand() % 4;
            int temp = dirIndices[i];
            dirIndices[i] = dirIndices[r];
            dirIndices[r] = temp;
        }

        for (int i = 0; i < 4; i++) {
            int dir = dirIndices[i];
            int newX = current.x + directions[dir].x;
            int newY = current.y + directions[dir].y;
            if (newX > 0 && newX < width - 1 && newY > 0 && newY < height - 1 && maze[newY][newX] == WALL) {
                maze[newY][newX] = PATH;
                maze[current.y + directions[dir].y / 2][current.x + directions[dir].x / 2] = PATH;
                push(&stack, (Position){ newX, newY });
            }
        }
    }

    freeStack(&stack);

    // 입구와 출구 설정
    maze[1][0] = START;              // 입구 위치
    maze[height - 2][width - 2] = END; // 출구 위치
    maze[height - 2][width - 3] = PATH; // 출구 왼쪽을 길로 설정
    maze[height - 3][width - 2] = PATH; // 출구 위를 길로 설정

    // 하단과 우측 벽 설정
    for (int i = 0; i < height; i++) {
        maze[i][width - 1] = WALL;
    }
    for (int j = 0; j < width; j++) {
        maze[height - 1][j] = WALL;
    }
}

// 깊이 우선 탐색 (DFS) 알고리즘
void depthFirstSearch(int** maze, int width, int height) {
    Stack stack;
    initStack(&stack, width * height);
    push(&stack, (Position){ 1, 0 });
    maze[1][0] = VISITED;

    while (!isEmpty(&stack)) {
        Position current = pop(&stack);
        if (maze[current.y][current.x] == END) {
            printf("출구를 찾았습니다!\n");
            break;
        }

        // 상하좌우 탐색
        Position directions[] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i].x;
            int newY = current.y + directions[i].y;
            if (newX >= 0 && newX < width && newY >= 0 && newY < height &&
                (maze[newY][newX] == PATH || maze[newY][newX] == END)) {
                push(&stack, (Position){ newX, newY });
                if (maze[newY][newX] != END)
                    maze[newY][newX] = VISITED;
            }
        }

        printMaze(maze, width, height);
        printf("계속하려면 엔터를 누르세요... \n");
        getchar();
    }

    freeStack(&stack);
}

int main() {
    int width, height;
    printf("미로의 가로 길이를 입력하세요: ");
    scanf("%d", &width);
    printf("미로의 세로 길이를 입력하세요: ");
    scanf("%d", &height);

    if (width < 3 || height < 3) {
        printf("미로의 크기는 최소 3x3 이상이어야 합니다.\n");
        return 1;
    }

    // 미로 메모리 할당
    int** maze = (int**)malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        maze[i] = (int*)malloc(width * sizeof(int));
    }

    initializeMaze(maze, width, height);
    depthFirstSearch(maze, width, height);

    // 미로 메모리 해제
    for (int i = 0; i < height; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}
