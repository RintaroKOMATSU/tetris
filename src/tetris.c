#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <fcntl.h>
#include "tetrimino.h"
#include "score.h"

#define Height 22//ボードの高さ
#define Width 10//ボードの幅
#define FPS 15 //frames per second
#define LEVEL 4

//キー入力があるかどうか判定
int key_down() {
    fd_set set;
    struct timeval timeout;

    // ファイルディスクリプタの監視対象を設定
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    // タイムアウトを設定
    timeout.tv_sec = 0;
    timeout.tv_usec = 0; 
    int ready = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
    return ready;
}

//画面の描画
void print_board(char** board, char **board_copy, int status, Tetrimino* t, int score, char next_type, char nnext_type) {
    Tetrimino *t_place = gen_tetrimino();//落下地点の表示用
    int mino_x = get_x(t);
    int mino_y = get_y(t);
    char** mino = get_mino(t);
    for (int y = 0; y < Height; y++) {
        for (int x = 0; x < Width; x++) {
            board_copy[y][x] = board[y][x];
        }
    }
    copy_tetrimino(t, t_place);
    hard_drop(t_place, board, Height, Width);
    int t_place_x = get_x(t_place);
    int t_place_y = get_y(t_place);
    char** t_place_mino = get_mino(t_place);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (t_place_mino[y][x] > 0) {
                board_copy[t_place_y+y][t_place_x+x] = 8;
            }

        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (mino[y][x] > 0) {
                board_copy[mino_y+y][mino_x+x] = mino[y][x];
            }

        }
    }
    for (int y = 0; y < Height; y++) {
        printf("\e[47m  \e[0m");
        for (int x = 0; x < Width; x++) {
            if (y == 1) {

                switch(board_copy[y][x]) {
                case 0: printf("\e[40m__\e[0m"); break;
                case 1: printf("\e[41m__\e[0m"); break;
                case 2: printf("\e[42m__\e[0m"); break;
                case 3: printf("\e[43m__\e[0m"); break;
                case 4: printf("\e[44m__\e[0m"); break;
                case 5: printf("\e[45m__\e[0m"); break;
                case 6: printf("\e[46m__\e[0m"); break;
                case 7: printf("\e[46m__\e[0m"); break;
                }              
            }
            else {
                switch(board_copy[y][x]) {
                case 0: printf("\e[40m  \e[0m"); break;
                case 1: printf("\e[41m  \e[0m"); break;
                case 2: printf("\e[42m  \e[0m"); break;
                case 3: printf("\e[43m  \e[0m"); break;
                case 4: printf("\e[44m  \e[0m"); break;
                case 5: printf("\e[45m  \e[0m"); break;
                case 6: printf("\e[46m  \e[0m"); break;
                case 7: printf("\e[46m  \e[0m"); break;
                case 8: printf("\e[40m[]\e[0m"); break;
                }
            }
        }
        printf("\e[47m  \e[0m\r\n");
    }
    printf("\e[47m                        \e[0m\r\n");
    printf("\e[%dA\e[%dC Score : %d\r\n\n", Height-1, 2*Width+8, score);
    printf("\e[%dC next\r\n\n", 2*Width+10);
    printf("\e[%dC", 2*Width+10);
    char **tetrimino_next = get_tetrimino(next_type);
    for (int y = 0; y < 2; y++ ) {
        for (int x = 0; x < 4; x ++) {
            switch(tetrimino_next[y][x]) {
            case 0: printf("\e[0m  "); break;  
            case 1: printf("\e[41m  \e[0m"); break;
            case 2: printf("\e[42m  \e[0m"); break;
            case 3: printf("\e[43m  \e[0m"); break;
            case 4: printf("\e[44m  \e[0m"); break;
            case 5: printf("\e[45m  \e[0m"); break;
            case 6: printf("\e[46m  \e[0m"); break;
            case 7: printf("\e[46m  \e[0m"); break;
            }
        }
        printf("\r\n\e[%dC", 2*Width+10);
    }
    printf("\r\n\e[%dC", 2*Width+10);
    char **tetrimino_nnext = get_tetrimino(nnext_type);
    for (int y = 0; y < 2; y++ ) {
        for (int x = 0; x < 4; x ++) {
            switch(tetrimino_nnext[y][x]) {
            case 0: printf("\e[0m  "); break;
            case 1: printf("\e[41m  \e[0m"); break;
            case 2: printf("\e[42m  \e[0m"); break;
            case 3: printf("\e[43m  \e[0m"); break;
            case 4: printf("\e[44m  \e[0m"); break;
            case 5: printf("\e[45m  \e[0m"); break;
            case 6: printf("\e[46m  \e[0m"); break;
            case 7: printf("\e[46m  \e[0m"); break;
            }
        }
        printf("\r\n\e[%dC", 2*Width+10);
    }
    printf("\r\n\e[%dB\r\n", Height-11);
    fflush(stdout);
    free_tetrimino(t_place);
    for (int i = 0; i < 2; i++) {
        free(tetrimino_next[i]);
        free(tetrimino_nnext[i]);
    }
    free(tetrimino_next);
    free(tetrimino_nnext);
}

//終了処理
void GAME_OVER(int score) {
    int high_score = update_highscore(score);
    printf("\e[%dF", Height+2);
    for (int i = 0; i < Height+1; i++) {
        printf("\e[%dC               \n", 2*Width+5);
    }
    printf("    GAME OVER\nScore : %d   High Score : %d\n", score, high_score);
    printf("w:replay");
    printf("   q:quit\n");
    system("/bin/stty raw onlcr");
    while(1) {
        char c = getchar();
        if (c == 'w') {
            system("/bin/stty cooked");
            system("bin/tetris");
            exit(0);
        }
        else if (c == 'q') {
            system("/bin/stty cooked");
            system("clear");
            exit(0);
        }
    }
}

//メイン関数
int main() {
    char** board = (char**)malloc(sizeof(char*)*Height); //盤面の定義
    for (int i = 0; i < Height; i++) {
        board[i] = (char*)malloc(sizeof(char)*Width);
    }
    //盤面の初期化
    for (int y = 0; y < Height; y++) {
        for (int x = 0; x < Width; x++) {
            board[y][x] = 0;
        }
    };
    char** board_copy = (char**)malloc(sizeof(char*)*Height); //盤面の定義
    for (int i = 0; i < Height; i++) {
        board_copy[i] = (char*)malloc(sizeof(char)*Width);
    }

    //入力を非同期モードに変更
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    srand((unsigned int) time(NULL)); //シードの初期化
    int status = 1; //ゲーム状態
    int clock = (int) 1000000/FPS; //ゲーム更新速度
    int count = 0;
    int level = LEVEL; //難易度
    int game_over = 0;
    int score = 0; //スコア
    char next_mino = rand()%7;
    char nnext_mino = rand()%7;
    Tetrimino *t = gen_tetrimino(); 
    init_tetrimino(t, &next_mino, &nnext_mino); 
    usleep(clock);
    printf("\e[2J\e[H");
    print_board(board, board_copy, status, t, score, next_mino, nnext_mino);
    system("/bin/stty raw onlcr");  //enterなしで入力を受け付ける
    //メインループ
    while(game_over == 0) {
        //復帰
        if (status == 0) { 
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    char ** t_mino = get_mino(t);
                    if (t_mino[y][x] > 0) {
                        board[get_y(t)+y][get_x(t)+x] = t_mino[y][x];
                    }

                }
            }

            int count_deleted_lines = delete_line(board, Height, Width); //ラインの消去
            switch (count_deleted_lines) {
                case 1 : score += 100; break;
                case 2 : score += 300; break;
                case 3 : score += 500; break;
                case 4 : score += 800; break;
            }

            //点線の上にテトリミノがあればゲームオーバー
            for (int x = 0; x < Width; x++) {
                if (board[1][x] > 0) {
                    game_over = 1; 
                    break;
                }
            }
            if (game_over == 1) {
                break;
            }

            //テトリミノの復帰
            init_tetrimino(t, &next_mino, &nnext_mino); 
            status = 1;

            //動かせなかったらゲームオーバー
            if (is_valid_move_y(t, board, Height, Width) == 0) {
                game_over = 1;
            }
        }

        //落下、移動
        else if (status == 1) {

            //キー入力
            if (key_down() > 0) {  
                char input = getchar();

                //下についても移動ができるようにする
                if (is_valid_move_y(t, board, Height, Width) != 1) {
                    count = 0;
                }
            
                switch(input) {
                    case 's' : hard_drop(t, board, Height, Width);count = 0;break; //落下
                    case 'x' : soft_drop(t, board, Height, Width);break;
                    case 'a' : move_left(t, board, Height, Width);break; //左移動
                    case 'd' : move_right(t, board, Height, Width);break; //右移動
                    case 'q' : rotate_clockwise(t, board, Height, Width);break; //時計回り回転
                    case 'e' : rotate_counter_clockwise(t, board, Height, Width);break; //反時計回りに回転
                    case 'z' : game_over = 1;break;
                }
            }
            //下に1落ちる
            if (is_valid_move_y(t, board, Height, Width) == 1) {
                if (key_down() == 0) {
                    if (count == level ) {
                        move_y(t);
                        count = 0;
                    }
                    else {
                        count ++;
                    }
                }
            }
            else {
                if (count == level) {
                    status = 0;
                    count = 0;
                }
                else {
                    count++;
                }     
            }

        }
        printf("\e[H");
        print_board(board, board_copy, status, t, score, next_mino, nnext_mino);
        usleep(clock);
    }
    system("/bin/stty cooked");
    GAME_OVER(score);

    return 0;
}
