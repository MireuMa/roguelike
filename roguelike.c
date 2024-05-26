#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define NUM_MAPS 10
#define STAGES 3
#define INITIAL_HEALTH 10
#define TOXIC_GAS_DURATION 5

typedef struct {
    char* map;
    int HEIGHT;
    int WIDTH;
} Map;

void printMap(Map* map, int playerX, int playerY);
int playGame(Map* map);
void handleMovement(Map* map, int* x, int* y, char move);
void toxicGasEffect(int* toxicGasTimer);
void clearScreen();
void clearUI();
void clearMap();
void hideCursor();
void setCursorPosition(int x, int y);
void checkCell(Map* map, int x, int y, int* health, int* coins, int* toxicGasTimer, DWORD* lastUpdate, DWORD* messageLastUpdate, DWORD* currentTime, char* statusMessage);
void printUI(int health, int coins, int toxicGasTimer, char* statusMessage);
void gameOverScreen(int coins);
void gameStartScreen();
void gameCompleteScreen(int coins);
void resetToxicGasTimerAndRestoreHealth(int* health, int* toxicGasTimer);
void initializeMaps();
void freeMaps();

const char* map1 =
    "WWWGWWWWWWW"
    "S     T  CS"
    "S & C     S"
    "SWWWWWW+  S"
    "S   & C  +S"
    "S    WWWWWS"
    "S  T    C S"
    "WWWWWWWWEWW";

const char* map2 =
    "WWWWWWWW"
    "S  TC  G"
    "S &  WWS"
    "SWW+ T S"
    "E SC   S"
    "S S &  S"
    "SC   C S"
    "WWWWWWWW";

const char* map3 =
    "WWWGWWW"
    "S+  T S"
    "S  C  S"
    "ST WWWS"
    "S & +CS"
    "S T   S"
    "SWWWW+S"
    "S  C  S"
    "WWWEWWW";

const char* map4 =
    "WWWWWWWWWW"
    "S  C  S+ S"
    "S  S  ST E"
    "S  S TC  S"
    "S CS  S& S"
    "S  S& S TS"
    "ST  +    S"
    "WWWWWGWWWW";

const char* map5 =
    "WWWWWWWWWW"
    "S   T    G"
    "S+   S C S"
    "S   &WWWWS"
    "S  C T   S"
    "S &   +  S"
    "S WWWWWW S"
    "S TC  +  S"
    "S      C S"
    "WWEWWWWWWW";

const char* map6 =
    "WWWWWWWWWWW"
    "S +    C  S"
    "S  TT     S"
    "S  + S &  S"
    "SWWW S  T S"
    "S &  S    E"
    "S   WWWWWWS"
    "S T   +  CS"
    "S    C    S"
    "WWWGWWWWWWW";

const char* map7 =
    "WWGWWWW"
    "S  C  S"
    "SWW+T S"
    "E S+ &S"
    "S S T S"
    "S+SW  S"
    "S & C+S"
    "WWWWWWW";

const char* map8 =
    "WWWWWWWWWW"
    "S C& +   S"
    "S   S   CS"
    "ST TS  T S"
    "E   S &  S"
    "S  CS C  S"
    "WWWWWWWGWW";

const char* map9 =
    "WWWWWWW"
    "S C   S"
    "S  &+ S"
    "S     E"
    "S& WWWS"
    "S  C  S"
    "SWWWW S"
    "G & T S"
    "S +   S"
    "S  C  S"
    "WWWWWWW";

const char* map10 =
    "WWWEWWWWWW"
    "S C  &  CS"
    "SW TWWW +S"
    "S   &  & S"
    "SC+  SWWWS"
    "S   TS   G"
    "S  C ST  S"
    "S TC +   S"
    "WWWWWWWWWW";

Map maps[NUM_MAPS];

void initializeMaps() {
    const char* staticMaps[NUM_MAPS] = {map1, map2, map3, map4, map5, map6, map7, map8, map9, map10};
    int heights[NUM_MAPS] = {8, 8, 9, 8, 10, 10, 8, 7, 11, 9};
    int widths[NUM_MAPS] = {11, 8, 7, 10, 10, 11, 7, 10, 8, 10};

    for (int i = 0; i < NUM_MAPS; i++) {
        maps[i].HEIGHT = heights[i];
        maps[i].WIDTH = widths[i];
        maps[i].map = (char*)malloc((heights[i] * widths[i] + 1) * sizeof(char)); // +1 for null terminator
        strcpy(maps[i].map, staticMaps[i]);
    }
}

void freeMaps() {
    for (int i = 0; i < NUM_MAPS; i++) {
        free(maps[i].map);
    }
}

int main() {
    srand(time(NULL));

    hideCursor(); // 커서 숨김
    gameStartScreen(); // 게임 시작 화면을 출력

    initializeMaps(); // 맵 초기화

    int gameWon = 1; // 게임 승리 여부를 나타내는 변수
    int totalCoins = 0; // 총 획득한 동전 수

    // 맵 중복 선택 방지를 위한 배열
    int usedMaps[NUM_MAPS] = {0};

    for (int stage = 0; stage < STAGES; ++stage) {
        int mapIndex;
        do {
            mapIndex = rand() % NUM_MAPS;
        } while (usedMaps[mapIndex] != 0); // 이미 사용된 맵은 다시 선택하지 않음

        usedMaps[mapIndex] = 1; // 선택한 맵을 사용된 것으로 표시

        printf("Stage %d\n", stage + 1);
        int coins = playGame(&maps[mapIndex]);
        if (coins < 0) {
            gameWon = 0;
            totalCoins += (-coins);
            break; // 플레이어가 죽었으므로 게임 종료
        } else {
            totalCoins += coins;
        }
        printf("다음 단계로 진행 중...\n");
        Sleep(2000); // 다음 단계로 넘어가기 전에 잠시 대기
    }

    if (gameWon) {
        gameCompleteScreen(totalCoins); // 게임이 완료되었음을 알리는 화면 출력
    }

    freeMaps(); // 동적 메모리 해제

    return 0;
}

// 플레이어의 이동 처리
void handleMovement(Map* map, int* x, int* y, char move) {
    int newX = *x;
    int newY = *y;

    switch (move) {
        case 'w': newY--; break; // 위로 이동
        case 's': newY++; break; // 아래로 이동
        case 'a': newX--; break; // 왼쪽으로 이동
        case 'd': newX++; break; // 오른쪽으로 이동
        default: break;
    }

    // 맵 경계와 충돌 체크
    if (newX >= 0 && newX < map->WIDTH && newY >= 0 && newY < map->HEIGHT && map->map[newY * map->WIDTH + newX] != 'W' && map->map[newY * map->WIDTH + newX] != 'S') {
        *x = newX;
        *y = newY;
    }
}

// 셀 체크 및 효과 적용
void checkCell(Map* map, int x, int y, int* health, int* coins, int* toxicGasTimer, DWORD* lastUpdate, DWORD* messageLastUpdate, DWORD* currentTime, char* statusMessage) {
    switch (map->map[y * map->WIDTH + x]) {
        case 'T':
            toxicGasEffect(toxicGasTimer); // 유독 가스 효과 적용
            snprintf(statusMessage, 256, "유독 가스를 만났습니다!");
            if (*toxicGasTimer == 0) {
                *lastUpdate = *currentTime;
            }
            *messageLastUpdate = *currentTime;
            break;
        case 'C':
            snprintf(statusMessage, 256, "동전을 획득했습니다!");
            (*coins)++;
            map->map[y * map->WIDTH + x] = ' ';
            *messageLastUpdate = *currentTime;
            break;
        case '&':
            snprintf(statusMessage, 256, "적을 만났습니다!");
            (*health)--;
            *messageLastUpdate = *currentTime;
            break;
        case '+':
            resetToxicGasTimerAndRestoreHealth(health, toxicGasTimer); // 체력 회복 및 유독 가스 제거
            snprintf(statusMessage, 256, "체력이 회복되고 유독 가스가 제거되었습니다!");
            map->map[y * map->WIDTH + x] = ' ';
            *messageLastUpdate = *currentTime;
            break;
        default:
            break;
    }
}

void resetToxicGasTimerAndRestoreHealth(int* health, int* toxicGasTimer) {
    *health = INITIAL_HEALTH;
    *toxicGasTimer = 0;
    printf("\033[0m"); // 텍스트 색상 재설정
}

void toxicGasEffect(int* toxicGasTimer) {
    printf("\033[32m"); // 텍스트 색상을 초록색으로 설정
    *toxicGasTimer = TOXIC_GAS_DURATION;
}

void clearScreen() {
    printf("\033[H\033[J");
}

void clearUI() {
    printf("\033[1H\033[K");
    printf("\033[2H\033[K");
    printf("\033[3H\033[K");
}

void clearMap() {
    printf("\033[4H\033[J");
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = {0,};
    cursorInfo.bVisible = 0;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 맵 출력
void printMap(Map* map, int playerX, int playerY) {
    for (int row = 0; row < map->HEIGHT; row++) {
        for (int column = 0; column < map->WIDTH; column++) {
            if (row == playerY && column == playerX) {
                printf(" P"); // 플레이어 위치 출력
            } else {
                // 맵 요소에 따라 다른 문자를 출력
                char cell = map->map[row * map->WIDTH + column];
                if (cell == 'W')
                    printf(" ─");
                else if (cell == 'S')
                    printf(" |");
                else if (cell == ' ')
                    printf("  "); // 공백으로 업데이트
                else if (cell == 'E')
                    printf(" E");
                else if (cell == 'G')
                    printf(" G");
                else if (cell == 'T')
                    printf(" T");
                else if (cell == 'C')
                    printf(" C");
                else if (cell == '&')
                    printf(" &");
                else if (cell == '+')
                    printf(" +");
            }
        }
        printf("\n");
    }
}

// UI 출력
void printUI(int health, int coins, int toxicGasTimer, char* statusMessage) {
    printf("\033[1H");
    printf("체력: %d | 동전: %d | 유독 가스 타이머: %d\n", health, coins, toxicGasTimer);
    printf("========================================\n");
    printf("%s\n", statusMessage);
}

// 게임 오버 화면 출력
void gameOverScreen(int coins) {
    clearScreen();
    printf("========================================\n");
    printf("               게임 오버               \n");
    printf("========================================\n");
    printf("당신은 죽었습니다!\n");
    printf("획득한 동전: %d\n", coins);
    printf("========================================\n");
    printf("나가려면 아무 키나 누르세요...\n");
    _getch();
    exit(0);
}

// 게임 완료 화면 출력
void gameCompleteScreen(int coins) {
    clearScreen();
    printf("========================================\n");
    printf("             게임 완료             \n");
    printf("========================================\n");
    printf("축하합니다! 모든 단계를 완료했습니다.\n");

    setCursorPosition(17, 11); // (18, 12) in 1-based index
    printf(" P");
    setCursorPosition(17, 6);  // (18, 7) in 1-based index
    printf(" A");

    fflush(stdout);
    Sleep(1000); // 1초 대기

    // 반복문을 사용하여 A를 아래로 이동
    for (int i = 6; i < 11; i++) {
        setCursorPosition(17, i);  // 이전 위치를 지움
        printf("  ");
        setCursorPosition(17, i + 1);  // 새로운 위치에 A를 출력
        printf(" A");
        fflush(stdout);
        Sleep(1000); // 1초 대기
    }

    setCursorPosition(0, 13); // (1, 14) in 1-based index
    printf("당신은 족보를 얻었습니다. 시험 힘내세요!\n");
    printf("========================================\n");
    printf("나가려면 아무 키나 누르세요...\n");

    fflush(stdout);
    _getch(); // 키 입력 대기
    exit(0);
}

// 게임 시작 화면 출력
void gameStartScreen() {
    clearScreen();
    printf("=============================================================================\n");
    printf("=(상명 다이캇코오 재수강 순방전 파이널 키라키라 섬머 페스타) 족보 찾기 대작전\n");
    printf("=============================================================================\n");
    printf("설명서:\n");
    printf("이동: W, A, S, D를 사용하세요.\n");
    printf("동전을 수집하고, 적을 피하며, 출구로 이동하세요!\n");
    printf("'+'를 터치하면 체력이 회복되고 유독 가스가 제거됩니다.\n");
    printf("=============================================================================\n");
    printf("시작하려면 아무 키나 누르세요...\n");
    _getch();
}

// 게임 플레이 함수
int playGame(Map* map) {
    int x = -1, y = -1;
    int health = INITIAL_HEALTH;
    int coins = 0;
    int toxicGasTimer = 0;
    char move;
    char statusMessage[256] = "";

    // 'G'로 표시된 시작 위치 찾기
    for (int row = 0; row < map->HEIGHT; row++) {
        for (int col = 0; col < map->WIDTH; col++) {
            if (map->map[row * map->WIDTH + col] == 'G') {
                x = col;
                y = row;
                break;
            }
        }
        if (x != -1 && y != -1) break;
    }

    if (x == -1 || y == -1) {
        printf("오류: 맵에서 시작 위치 'G'를 찾을 수 없습니다!\n");
        return -1; // 잘못된 맵에 대한 오류 코드
    }

    DWORD lastUpdate = GetTickCount();
    DWORD messageLastUpdate = GetTickCount();

    clearScreen();
    printUI(health, coins, toxicGasTimer, statusMessage);
    printMap(map, x, y);

    while (1) {
        DWORD currentTime = GetTickCount();

        // 플레이어가 출구에 도달했는지 확인
        if (map->map[y * map->WIDTH + x] == 'E') {
            printf("\033[0m"); // 텍스트 색상 재설정
            printf("\n출구에 도착했습니다!\n");
            return coins;
        }

        // 사용자 입력 처리
        if (_kbhit()) {
            move = _getch();
            handleMovement(map, &x, &y, move);

            checkCell(map, x, y, &health, &coins, &toxicGasTimer, &lastUpdate, &messageLastUpdate, &currentTime, statusMessage);

            clearScreen();
            printUI(health, coins, toxicGasTimer, statusMessage);
            printMap(map, x, y);
        }

        // 유독 가스 타이머 업데이트
        if (currentTime - lastUpdate >= 1000) { // 매 초마다 업데이트
            lastUpdate = currentTime;
            if (toxicGasTimer > 0) {
                toxicGasTimer--;
                clearUI();
                printUI(health, coins, toxicGasTimer, statusMessage);
                if (toxicGasTimer == 0) {
                    health = 0;
                    break;
                }
            }
        }

        // 상태 메시지를 2초 후에 지움
        if (currentTime - messageLastUpdate >= 2000) {
            messageLastUpdate = currentTime;
            statusMessage[0] = '\0'; // 상태 메시지 지움
            clearUI();
            printUI(health, coins, toxicGasTimer, statusMessage);
        }

        // 플레이어의 체력이 0인지 확인
        if (health <= 0) {
            break;
        }

        Sleep(0);
    }

    gameOverScreen(coins);
    return coins;
}
