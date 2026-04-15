#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetrimino.h"

//テトリミノを定義
char tetriminos[7][2][4] = {
    {{0, 1, 1, 0},
     {1, 1, 0, 0}},
    {{2, 2, 0, 0},
     {0, 2, 2, 0}},
    {{3, 0, 0, 0},
     {3, 3, 3, 0}},
    {{0, 0, 4, 0},
     {4, 4, 4, 0}},
    {{0, 5, 0, 0},
     {5, 5, 5, 0}},
    {{6, 6, 0, 0},
     {6, 6, 0, 0}},
    {{0, 0, 0, 0},
     {7, 7, 7, 7}}
};

//テトリミノの構造体
struct tetrimino {
    int x; //x座標
    int y; //y座標
    char type; //テトリミノの種類
    char **mino; //テトリミノを格納する配列
};

Tetrimino* gen_tetrimino() {
    Tetrimino *t = (Tetrimino*)malloc(sizeof(Tetrimino));
    t->mino = (char**)malloc(4 * sizeof(char*));
    for (int i = 0; i < 4; ++i) {
        t->mino[i] = (char*)malloc(4 * sizeof(char));
    }
    return t;
}

int get_x(Tetrimino *t) {
    return t -> x;
}

int get_y(Tetrimino *t) {
    return t -> y;
}

char get_type(Tetrimino *t) {
    return t -> type;
}

char **get_mino(Tetrimino *t) {
    return t -> mino;
}

void copy_tetrimino(Tetrimino *t_original, Tetrimino *t_copy) {

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            t_copy->mino[y][x] = t_original -> mino[y][x];
        }
    }
    t_copy->x = t_original-> x;
    t_copy->y = t_original -> y;
    t_copy->type = t_original -> type;
}

char **get_tetrimino(char type) {
    char **tetrimino = (char**)malloc(2*sizeof(char*));
    for (int i = 0; i < 2; i++) {
        tetrimino[i] = (char*)malloc(4*sizeof(char));
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            tetrimino[i][j]  = tetriminos[(int)type][i][j];
        }
    }
    return tetrimino;
}

//テトリミノの初期化
void init_tetrimino(Tetrimino *t, char *next_mino, char *nnext_mino) {
    t -> x = 0;
    t -> y = 0;//座標を０に戻す
    t -> type = *next_mino; 
    *next_mino = *nnext_mino; //次のテトリミノ
    *nnext_mino = rand()%7; //次の次のテトリミノ
    //同じテトリミノが3回以上連続しない
    while (*nnext_mino == *next_mino && *next_mino == t -> type) {
        *nnext_mino = rand()%7;
    }
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x  < 4; x++) {
            t -> mino[y][x] = tetriminos[(int)t -> type][y][x];             
        }
    }
    for (int y = 2; y < 4; y++) {
        for (int x = 0; x  < 4; x++) {
            t -> mino[y][x] = 0;             
        }
    }   
}

//下に１マス落ちられるか判定
int is_valid_move_y(Tetrimino *t, char **board, const char Height, const char Width) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (t->mino[y][x] > 0) {
                if (y+t->y+1 == Height || board[y+t->y+1][x+t->x] > 0 ) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

//下に１マス移動
void move_y(Tetrimino *t) {
    t -> y ++;
}

//左移動
void move_left(Tetrimino *t, char **board, const char Height, const char Width) {
    int is_valid_move = 1;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if(t->mino[y][x] > 0) {
                if (x+t->x == 0 || board[y+t->y][x+t->x-1] > 0){
                    is_valid_move = 0;
                }
            }
        }
    }
    if (is_valid_move == 1) {
        t->x--;
    }
}

//右移動
void move_right(Tetrimino *t, char **board, const char Height, const char Width) {
    int is_valid_move = 1;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if(t->mino[y][x] > 0) {
                if (x+t->x+1 == Width || board[y+t->y][x+t->x+1] > 0){
                    is_valid_move = 0;
                }
            }
        }
    }
    if (is_valid_move == 1) {
        t->x++;
    }
}

//時計回りに回転
void rotate_clockwise(Tetrimino *t, char **board, const char Height, const char Width) {
    if (t->type ==5) {
        return;
    }
    else {
        int valid_rotation = 1;//回転可能か判定
        int mino_copy[4][4]={0};
        if (t->type == 6) {
            if (t->mino[0][1] == 0) {
                for (int y = 0; y < 4; y++) {
                    mino_copy[y][1] = t->mino[1][1];
                }
            }
            else {
                for (int x = 0; x < 4; x++) {
                    mino_copy[1][x] = t->mino[1][1];
                }                
            }
        }
        else {
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x ++) {
                    mino_copy[y][x] = t->mino[x][2-y];
                }
            }
        }

        for (int y = 0; y < 4; y ++) {
            for (int x = 0; x < 4; x++) {
                if(mino_copy[y][x] > 0) {
                    if (t->x+x >= Width || t->y+y >= Height) {
                        valid_rotation = 0;
                    }
                    else if (board[t->y+y][t->x+x] > 0) {
                        valid_rotation = 0;
                    }
                    else if (t->y+y < 1) {
                        valid_rotation = 0;
                    }                    
                }       
            }
        }
        if (valid_rotation == 1) {
            for (int y = 0; y < 4; y ++) {
                for (int x = 0; x < 4; x++) {
                    t->mino[y][x] = mino_copy[y][x];     
                }
            }        
        }

    }
}

//反時計回りに回転
void rotate_counter_clockwise(Tetrimino *t, char **board, const char Height, const char Width) {
    if (t->type ==5) {
        return;
    }
    else {
        int valid_rotation = 1;//回転可能か判定
        int mino_copy[4][4]={0};
        if (t->type == 6) {
            if (t->mino[0][1] == 0) {
                for (int y = 0; y < 4; y++) {
                    mino_copy[y][1] = t->mino[1][1];
                }
            }
            else {
                for (int x = 0; x < 4; x++) {
                    mino_copy[1][x] = t->mino[1][1];
                }                
            }
        }
        else {
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x ++) {
                    mino_copy[x][2-y] = t->mino[y][x];
                }
            }
        }

        for (int y = 0; y < 4; y ++) {
            for (int x = 0; x < 4; x++) {
                if(mino_copy[y][x] > 0) {
                    if (t->x+x >= Width || t->y+y >= Height) {
                        valid_rotation = 0;
                    }
                    else if (board[t->y+y][t->x+x] > 0) {
                        valid_rotation = 0;
                    }
                    else if (t->y+y < 1) {
                        valid_rotation = 0;
                    }
                }       
            }
        }
        if (valid_rotation == 1) {
            for (int y = 0; y < 4; y ++) {
                for (int x = 0; x < 4; x++) {
                    t->mino[y][x] = mino_copy[y][x];     
                }
            }        
        }

    }
}

//一番下まで落下
void hard_drop(Tetrimino *t, char **board, const char Height, const char Width) {
    while(is_valid_move_y(t, board, Height, Width) == 1) {
        t -> y ++;
    }
}

//一マス落下
void soft_drop(Tetrimino *t, char **board, const char Height, const char Width) {
    if (is_valid_move_y(t, board, Height, Width) == 1) {
        t -> y++;
    }
}

//列消去
int delete_line(char **board, const char Height, const char Width) {
    int count_deleted_lines = 0; //消去された列の数
    for (int y = 2 ; y < Height; y++) {
        int count = 0;
        for (int x = 0; x < Width; x++) {
            if (board[y][x] > 0) {
                count++;
            }
        }
        if (count == Width) {
            for (int j = y; j > 0; j--) {
                for (int i = 0; i < Width; i++) {
                    board[j][i] = board[j-1][i];
                }
            }
            for (int x = 0; x < Width; x ++) {
                board[0][x] = 0;
            }
            count_deleted_lines++;
        }
    }
    return count_deleted_lines;
}

void free_tetrimino(Tetrimino *t) {
    for (int i = 0; i < 4; i++) {
        free(t->mino[i]);
    }
    free(t->mino);
    free(t);
}