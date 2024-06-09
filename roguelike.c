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
int playGame(Map* map, int* health, int* totalCoins);
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
void printStory();
void gameHiddenendingScreen();

enum ColorType { // 색깔 열거형 표시
    BLACK,       //0
    DarkBLUE,    //1
    DarkGreen,   //2
    DarkSkyBlue, //3
    DarkRed,     //4
    DarkPurple,  //5
    DarkYellow,  //6
    GRAY,        //7
    DarkGray,    //8
    BLUE,        //9
    GREEN,       //10
    SkyBlue,     //11
    RED,         //12
    PURPLE,      //13
    YELLOW,      //14
    WHITE        //15
} COLOR;

void textcolor(unsigned short text, unsigned short back) { // \033[n;nm이 불편해서 인터넷에서 검색한 텍스트 색 변경 함수
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}

//맵 10개
const char* map1 =
"MWWGWWWWWWxWWWWWWWWN"
"SC  & T  CS  &  C  S"
"SWN+ SC &   S  +MWTS"
"S VWWzWWxWWWB SCSC S"
"S C T   S & C S VWWs"
"S MWWxW C  TS  C  +S"
"S S CS&  MWWB &MWWWs"
"S &  S C S C  TS C S"
"SWW WB S+S& WWWB STS"
"S  C   S C     & S S"
"SWN WWWBT WWWN SC  S"
"S S+ & C   & S VW Ws"
"S  T S S S  + T  C S"
"STWxWB S VWWN VxWW+S"
"S  S T S  C    S   S"
"S WzWW VWWW SW&VWWWs"
"S& C   & + CS    C S"
"SWW MWW WWWWzWWN WWs"
"SC& S  C  C& TCS+  S"
"VWWWzWWWEWWWWWWzWWWB";

const char* map2 =
"MWWWWWWxWWWWWWWWWN"
"SC T  +S &  + C  S"
"S WWN&  CS TMWWN S"
"S& CSWN  SWWBC STS"
"SWWWB ST ST   &S G"
"S&  CTS  S CS  SCS"
"S +S& S WzWWB +VWs"
"SC S   &  C   T  S"
"S TVWWWWxWWW&WWWWs"
"S &  &+CS  T   & S"
"S C S WxB MWWWWN S"
"SWWWB TSC S& +CSCS"
"S CT C&S &S  WWzWs"
"SWN  S S+ C T  C S"
"E SCWs S&MW&WxW  S"
"S S &S C SC CS & S"
"SC + ST  S+  CT  S"
"VWWWWWWWWWWWWWWWWB";

const char* map3 =
"MWWWWWWWGWWWWWWWN"
"SC   C &   C   &S"
"S TS  S  TS  S CS"
"S Ws WzxWxzW&SW S"
"S  S &CSCS+  ST S"
"S& VWxWB VWxWB +S"
"SC  CS  C &STC CS"
"S T&+S &  CS  & S"
"S MWWB S S VWWN S"
"SCS & +STSC   SCS"
"S  C  MBCVN T & S"
"S& TMWB   VWNC  S"
"SWWWBC  &C  VWWWS"
"ST C  MWWWNC  C S"
"S MWWWB  +S WN& S"
"S&SC + TSCS CS +S"
"S VW S& VWzWWB TS"
"SC  TS C + T C &S"
"VWWWWzWWEWWWWWWWB";

const char* map4 =
"MWWWWWWWWWWWWWWWWWWN"
"SCT  C    + C    & S"
"S MWN MWW WWWWWNC CE"
"SCSCS&SC  C  & VWWWs"
"S S+S STWN T  +  C S"
"S+C SCSC+SWWWWN MWTS"
"SWWWB VWWs  + SCS CS"
"SC T  C +ST S S&VWWs"
"S &C   & S MB    C S"
"SWWWWWNC S SC TMWN S"
"S& C +S TS VWWWB+VWs"
"S  ST C &S+& C    CS"
"S TVWWWN VWWWWWWW& S"
"S+C & CSC+ C  C   TS"
"SWWC TWzWxW &WWWWWWs"
"S    C  CSC  + C   S"
"SCTWWN& WzWWWW TMW+S"
"S C+ SC  T C   &SC S"
"VWWWWzWWWWGWWWWWzWWB";

const char* map5 =
"MWWxWWWWWWWWWWWWWN"
"S+CSC T &   C + TS"
"S &VN C   WWNC&  S"
"SC  VxWW& + S SC S"
"SWNC SC  S TS S &S"
"S+S  S T SC  CS CS"
"S S&  C WzWWN ST S"
"SCS  S&  C TS+VN S"
"S TC SWWWN  S &S S"
"SC  &S+C S CSC  TS"
"SWW WzNT S& VN&C S"
"S+ C TS CS  +VWN S"
"SWWN  S  C &  TS+S"
"SC+S&   &MWWN  S S"
"E CVN SC S& S MB S"
"S&  S S  S  S+S CS"
"SC TSC +  TMzWB& S"
"S WWzWWWW &SC T  S"
"S C +   C  S C  TS"
"VWWWWWWWWWWzWGWWWB";

const char* map6 =
"MWWWWWWWWxWWWWWWWWWWN"
"SC   C T SC&   C + CS"
"S  &C +  S  C  &  & S"
"S& WWWN CVWWWNC TS  S"
"S  C &S  &  +S& MzWCS"
"S T C S& C T S  S   S"
"SWWN& VWxWW CS &S CTS"
"S& VNC  SC &   CS&  S"
"S  &S  TVN   CT S  &S"
"S C  C  CVWNCMWWsC  S"
"S S T & +  VxB CVN+ S"
"S S+ S C & CS+   S CS"
"SCVxWB WWNT S C& S& S"
"S  SC  & S  VWN  S CS"
"S TVWN C S & CS TS &S"
"S   CSWWWsC  &S  S  E"
"S& +MBC &VW WWBC ST S"
"S  MB T   C   +  S CS"
"SC&SC   &C  TC  &S+ S"
"VWWzWWGWWWWWWWWWWzWWB";

const char* map7 =
"MWWWWEWWWWWWWWWWN"
"SC  C  T   C   +S"
"SWN+ &  CS& WxW S"
"SCVWWWWWxB C S CS"
"S  T C &S  MWB TS"
"SCMWWWN S TS C+ S"
"S S C+S S+ S MWWs"
"S+VW MB VWC  S CS"
"S  & SC  & MWsT S"
"SWWN VWWWWWBCS  S"
"SC&S& C  T  +S &S"
"G  S + S  C& S+ S"
"ST STSCSWWWN VNCS"
"S+CS S S&  SC S S"
"S MBCS SCS+S & CS"
"SCS  S VWB VNC  S"
"S  C&S C T  S T S"
"VWWWWzWWWWWWzWWWB";

const char* map8 =
"MWWWWWWWWWWWWWWWWxWN"
"S T  C  +  CT   CS+S"
"SC  S MWWW&  C&  S G"
"S MWB S+ C  MWWN C S"
"S+S C SWWxWWB  VxWWs"
"S S T SC+SC  &  S+CS"
"S SC WB  S  WxW S TS"
"SCVN&  T VW TSC S CS"
"S  S +   C   S &VW S"
"S& SCWWWxW & S  T& S"
"S CS  C S +  VN+ C S"
"S WB & MB WN  VWWWWs"
"S    MWsT CST C   CS"
"STCMWB SC& SWWWN& Ws"
"SWWBC  VWWWBC  VW  S"
"SC   &C  +   TC   CS"
"VWWEWWWWWWWWWWWWWWWB";

const char* map9 =
"MWWWWWxWWWWWWWWWN"
"S  C &SC  T  & CS"
"SCS S+  WW MWW+ S"
"SWB S ST C S  & E"
"S   S VWWxWzN WWs"
"STS+S&   S+CST  S"
"S SWWWWCWs MzWWWs"
"SCS T +  SCS&  CS"
"SWB WWN& S S MW S"
"S& C  S C  & S+ S"
"S WxW VWWN  C T S"
"S  S+  T S &WWWWs"
"SW S WWN SC  &  S"
"SC SC  VWzxWW S S"
"SWWzWN  C S  CS S"
"ST +CS& S S+S  TS"
"G  MWB TS S SWWWs"
"S&CS C  SC &S& CS"
"S  VWW& VWWWB S S"
"S C  T  + C  C  S"
"VWWWWWWWWWWWWWWWB";

const char* map10 =
"MWWxWGWWWWWxWWWWWWWN"
"S+ S&  C  TS +  C  S"
"S WB  MWW &   S  S S"
"S C  TS  C  MWzWWB S"
"SWWWWWzWW   SC T  CS"
"S  C + C  TMzW  CWWs"
"SWW TWWWWWWsC +& C S"
"S&   C T   S S   & S"
"S  WWWN CS S STS  TS"
"S C + S MB SC  S WWs"
"SW MWWB S CVWxWBC +S"
"S TS C T & + SC  T S"
"S& S  MWWWW  VWWWWWs"
"S  S& S  C  & +  C S"
"SC+   S S T  C  MWWs"
"S S&T C S  MWWWWsC E"
"S VW WWWzWWBT C VW S"
"SC   C   TC   &   +S"
"VWWWWWWWWWWWWWWWWWWB";

Map maps[NUM_MAPS];


// 맵 초기화 
void initializeMaps() {
    const char* staticMaps[NUM_MAPS] = { map1, map2, map3, map4, map5, map6, map7, map8, map9, map10 };
    int heights[NUM_MAPS] = { 20, 19, 19, 19, 20, 20, 18, 17, 21, 19 };
    int widths[NUM_MAPS] = { 20, 18, 17, 20, 18, 21, 17, 20, 17, 20 };

    for (int i = 0; i < NUM_MAPS; i++) {
        maps[i].HEIGHT = heights[i];
        maps[i].WIDTH = widths[i];
        maps[i].map = (char*)malloc((heights[i] * widths[i] + 1) * sizeof(char)); // +1한 이유 null 위한 공간
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
    printStory(); // 게임 스토리 출력

    initializeMaps(); // 맵 초기화

    int gameWon = 1; // 게임 승리 여부를 나타내는 변수
    int totalCoins = 0; // 총 획득한 동전 수
    int health = INITIAL_HEALTH; // 플레이어의 초기 체력

    // 맵 중복 선택 방지를 위한 배열
    int usedMaps[NUM_MAPS] = { 0 };

    for (int stage = 0; stage < STAGES; ++stage) {
        int mapIndex;
        do {
            mapIndex = rand() % NUM_MAPS;
        } while (usedMaps[mapIndex] != 0); // 이미 사용된 맵은 다시 선택하지 않음

        usedMaps[mapIndex] = 1; // 선택한 맵을 사용된 것으로 표시

        printf("Stage %d\n", stage + 1);
        int result = playGame(&maps[mapIndex], &health, &totalCoins);
        if (result < 0) {
            gameWon = 0;
            break; // 플레이어가 죽었으므로 게임 종료
        }
        printf("다음 단계로 진행 중...\n");
        Sleep(2000); // 다음 단계로 넘어가기 전에 잠시 대기
    }

    if (gameWon) {
        if (totalCoins > 80) { // 코인 80개 이상일 시 히든 엔딩 출력
            gameHiddenendingScreen(totalCoins);
        }
        else {
            gameCompleteScreen(totalCoins); // 게임이 완료되었음을 알리는 화면 출력
        }
    }

    freeMaps(); // 동적 메모리 해제

    return 0;
}

// 플레이어의 이동 처리
void handleMovement(Map* map, int* x, int* y, char move) {
    int newX = *x; //현재 X 저장
    int newY = *y;//현재 Y 저장

    switch (move) {
    case 'w': newY--; break; // 위로 이동
    case 's': newY++; break; // 아래로 이동
    case 'a': newX--; break; // 왼쪽으로 이동
    case 'd': newX++; break; // 오른쪽으로 이동
    default: break;
    }

    // 맵 경계와 충돌 체크
    if (newX >= 0 && newX < map->WIDTH && newY >= 0 && newY < map->HEIGHT && map->map[newY * map->WIDTH + newX] != 'W' && map->map[newY * map->WIDTH + newX] != 'S' && map->map[newY * map->WIDTH + newX] != 'M' && map->map[newY * map->WIDTH + newX] != 'N' && map->map[newY * map->WIDTH + newX] != 'B' && map->map[newY * map->WIDTH + newX] != 'V' && map->map[newY * map->WIDTH + newX] != 's' && map->map[newY * map->WIDTH + newX] != 'z' && map->map[newY * map->WIDTH + newX] != 'x') {
        *x = newX; //충돌 없으면 바뀐 X 반영
        *y = newY;//충돌 없으면 바뀐 Y 반영
    }
}

// 셀 체크 및 효과 적용
void checkCell(Map* map, int x, int y, int* health, int* coins, int* toxicGasTimer, DWORD* lastUpdate, DWORD* messageLastUpdate, DWORD* currentTime, char* statusMessage) {
    switch (map->map[y * map->WIDTH + x]) {//맵구조체 형식의 맵 포인터를 받고 거기 안에서 캐릭터 배열을 가리키는 맵 포인터에서 2차원 인덱슬르 1차원 인덱스로 변경
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
        map->map[y * map->WIDTH + x] = ' '; //맵 배열에서 동전 위치 비움
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
        if (*health < 10) {
            (*health)++;
        }
        map->map[y * map->WIDTH + x] = ' '; //맵 배열에서 체력키트 위치 비움
        *messageLastUpdate = *currentTime;
        break;
    default:
        break;
    }
}

void resetToxicGasTimerAndRestoreHealth(int* health, int* toxicGasTimer) {
    *toxicGasTimer = 0;
    textcolor(15, 0); // 텍스트 색상 재설정
    clearScreen();
}

void toxicGasEffect(int* toxicGasTimer) {
    *toxicGasTimer = TOXIC_GAS_DURATION;
    textcolor(2, 0); // 텍스트 색상을 초록색으로 설정
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
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
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
void printMap(Map* map, int playerX, int playerY, int toxicGasTimer) {
    if (toxicGasTimer != 0) { //독가스 먹었을 때 맵 출력
        for (int row = 0; row < map->HEIGHT; row++) {
            for (int column = 0; column < map->WIDTH; column++) {
                if (row == playerY && column == playerX) {
                    printf(" P"); // 플레이어 위치 출력
                }
                else {
                    // 맵 요소에 따라 다른 문자를 출력
                    char cell = map->map[row * map->WIDTH + column];
                    if (cell == 'W') {
                        textcolor(2, 0);
                        printf("──");
                    }
                    else if (cell == 'S') {
                        textcolor(2, 0);
                        printf(" |");
                    }
                    else if (cell == 's') {
                        textcolor(2, 0);
                        printf("─|");
                    }
                    else if (cell == 'M') {
                        textcolor(2, 0);
                        printf(" ┌");
                    }
                    else if (cell == 'N') {
                        textcolor(2, 0);
                        printf("─┐");
                    }
                    else if (cell == 'B') {
                        textcolor(2, 0);
                        printf("─┘");
                    }
                    else if (cell == 'V') {
                        textcolor(2, 0);
                        printf(" └");
                    }
                    else if (cell == 'z') {
                        textcolor(2, 0);
                        printf("─┴");
                    }
                    else if (cell == 'x') {
                        textcolor(2, 0);
                        printf("─┬");
                    }
                    else if (cell == ' ')
                        printf("  ");
                    else if (cell == 'E') {
                        textcolor(2, 0);
                        printf(" E");
                    }
                    else if (cell == 'G') {
                        textcolor(2, 0);
                        printf(" G");
                    }
                    else if (cell == 'T') {
                        textcolor(2, 0);
                        printf(" T");
                    }
                    else if (cell == 'C') {
                        textcolor(2, 0);
                        printf(" C");
                    }
                    else if (cell == '&') {
                        textcolor(2, 0);
                        printf(" &");
                    }
                    else if (cell == '+') {
                        textcolor(2, 0);
                        printf(" +");
                    }
                }
            }
            printf("\n");
        }
    }

    else { // 기본 맵 출력
        for (int row = 0; row < map->HEIGHT; row++) {
            for (int column = 0; column < map->WIDTH; column++) {
                if (row == playerY && column == playerX) {
                    printf(" P"); // 플레이어 위치 출력
                }
                else {
                    // 맵 요소에 따라 다른 문자를 출력
                    char cell = map->map[row * map->WIDTH + column];
                    if (cell == 'W') {
                        printf("──");
                    }
                    else if (cell == 'S') {
                        printf(" |");
                    }
                    else if (cell == 's') {
                        printf("─|");
                    }
                    else if (cell == 'M') {
                        printf(" ┌");
                    }
                    else if (cell == 'N') {
                        printf("─┐");
                    }
                    else if (cell == 'B') {
                        printf("─┘");
                    }
                    else if (cell == 'V') {
                        printf(" └");
                    }
                    else if (cell == 'z') {
                        printf("─┴");
                    }
                    else if (cell == 'x') {
                        printf("─┬");
                    }
                    else if (cell == ' ')
                        printf("  "); // 공백으로 업데이트
                    else if (cell == 'E')
                        printf(" E");
                    else if (cell == 'G')
                        printf(" G");
                    else if (cell == 'T') {
                        textcolor(2, 0);
                        printf(" T");
                        textcolor(15, 0);
                    }
                    else if (cell == 'C') {
                        textcolor(14, 0);
                        printf(" C");
                        textcolor(15, 0);
                    }
                    else if (cell == '&') {
                        textcolor(12, 0);
                        printf(" &");
                        textcolor(15, 0);
                    }
                    else if (cell == '+') {
                        textcolor(3, 0);
                        printf(" +");
                        textcolor(15, 0);
                    }
                }
            }
            printf("\n");
        }
    }
}

// UI 출력
void printUI(int health, int coins, int toxicGasTimer, char* statusMessage) {
    printf("\033[1H");//커서 1H(Home)으로 이동
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

// 게임 히든 엔딩 출력
void gameHiddenendingScreen() {
    clearScreen();
    printf("==============================================\n");
    printf("                    게임 완료                 \n");
    printf("==============================================\n");
    printf("이정도면 회사 안 다녀도 되겠는데? \n");
    textcolor(14, 0);
    printf("           ⓒ     ⓒ                 ⓒ       \n");
    printf("  ⓒ                    ⓒ                  ⓒ\n");
    printf("         ⓒ     ⓒ               ⓒ      ⓒ   \n");
    printf("      ⓒ           ⓒ      ⓒ                 \n");
    printf("ⓒ                              ⓒ      ⓒ    \n");
    printf("             ⓒ        ⓒ           ⓒ        \n");
    textcolor(15, 0);
    printf("                        P                     \n");
    printf("==============================================\n");
    printf("나가려면 아무 키나 누르세요...\n");
}

// 게임 완료 화면 출력
void gameCompleteScreen() {
    clearScreen();
    printf("========================================\n");
    printf("               게임 완료             \n");
    printf("========================================\n");
    printf("축하합니다! 모든 단계를 완료했습니다.\n");

    setCursorPosition(17, 11); // (18, 12)에서 1부터 시작하는 인덱스
    printf(" P");
    setCursorPosition(17, 6);  // (18, 7)에서 1부터 시작하는 인덱스
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

    setCursorPosition(0, 13); // (1, 14)
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
    printf("==============================================================================\n");
    printf(" (상명 다이캇코오 재수강 순방전 파이널 키라키라 섬머 페스타) 족보 찾기 대작전\n");
    printf("==============================================================================\n");
    printf("설명서:\n");
    printf("이동: W, A, S, D를 사용하세요.\n");
    printf("동전을 수집하고, 적을 피하며, 출구로 이동하세요!\n");
    printf("'+'를 터치하면 체력이 회복되고 유독 가스가 제거됩니다.\n");
    printf("==============================================================================\n");
    printf("시작하려면 아무 키나 누르세요...\n");
    _getch();
}

// 글 한 글자씩 출력
void printOneLetter(const char* story) {
    for (int i = 0; story[i] != '\0'; i++) {
        printf("%c", story[i]);
        fflush(stdout);
        if (story[i] != ' ') {
            Sleep(100); //0.1초 대기
        }
    }
}

// 스토리 출력
void printStory() {
    clearScreen();

    const char* story[] = {
        "                                2621년..",
        "       교수님의 C뿌리기로 인해, A를 받은 학생이 벌써 400년 전이다..",
        "     좋은 회사에 취업하기 위해선 무조건 이 강의에서 A를 받아야 한다..",
        "                     우리 학과에는 전설이 하나있다..",
        "                 2024년 학교 지하 어딘가에 족보를 숨겼다는..!",
        "                               나는 오늘..!",
        "                 족보를 찾아 이 강의에서 A를 받고야 말겠다!!"
    };

    int storyLines = sizeof(story) / sizeof(story[0]);

    for (int i = 0; i < storyLines; i++) {
        printf("\033[2K"); // 줄 클리어

        printf("\033[6;1H");
        printOneLetter(story[i]);
        Sleep(100);
    }

    printf("\n\n\n\n\n\n시작하려면 아무 키나 누르세요...");
    _getch();
}

// 게임 플레이 함수
int playGame(Map* map, int* health, int* totalCoins) {
    int x = -1, y = -1;
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
    printUI(*health, *totalCoins, toxicGasTimer, statusMessage);
    printMap(map, x, y, toxicGasTimer);

    while (1) {
        DWORD currentTime = GetTickCount();

        // 플레이어가 출구에 도달했는지 확인
        if (map->map[y * map->WIDTH + x] == 'E') {
            printf("\033[0m"); // 텍스트 색상 재설정
            printf("\n출구에 도착했습니다!\n");
            return *totalCoins;
        }

        // 사용자 입력 처리
        if (_kbhit()) {
            move = _getch();
            handleMovement(map, &x, &y, move);

            checkCell(map, x, y, health, totalCoins, &toxicGasTimer, &lastUpdate, &messageLastUpdate, &currentTime, statusMessage);

            clearScreen();
            printUI(*health, *totalCoins, toxicGasTimer, statusMessage);
            printMap(map, x, y, toxicGasTimer);
        }

        // 유독 가스 타이머 업데이트
        if (currentTime - lastUpdate >= 1000) { // 매 초마다 업데이트
            lastUpdate = currentTime;
            if (toxicGasTimer > 0) {
                toxicGasTimer--;
                clearUI();
                printUI(*health, *totalCoins, toxicGasTimer, statusMessage);
                if (toxicGasTimer == 0) {
                    *health = 0;
                    break;
                }
            }
        }

        // 상태 메시지를 2초 후에 지움
        if (currentTime - messageLastUpdate >= 2000) {
            messageLastUpdate = currentTime;
            statusMessage[0] = '\0'; // 상태 메시지 지움
            clearUI();
            printUI(*health, *totalCoins, toxicGasTimer, statusMessage);
        }

        // 플레이어의 체력이 0인지 확인
        if (*health <= 0) {
            break;
        }
        Sleep(0);
    }

    gameOverScreen(*totalCoins);
    return *totalCoins;
}
