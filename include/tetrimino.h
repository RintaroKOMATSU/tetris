//テトリミノの構造体
typedef struct tetrimino Tetrimino;


Tetrimino* gen_tetrimino();
int get_x(Tetrimino *t);
int get_y(Tetrimino *t);
char get_type(Tetrimino *t);
char **get_mino(Tetrimino *t);
void copy_tetrimino(Tetrimino *t_original, Tetrimino *t_copy);
char **get_tetrimino(char type);


//テトリミノの初期化
void init_tetrimino(Tetrimino *t, char *next_mino, char *nnext_mino);
//下に１マス落ちられるか判定
int is_valid_move_y(Tetrimino *t, char **board, const char Height, const char Width);
//下に１マス移動
void move_y(Tetrimino *t);
//左移動
void move_left(Tetrimino *t, char **board, const char Height, const char Width);
//右移動
void move_right(Tetrimino *t, char **board, const char Height, const char Width);
//時計回りに回転
void rotate_clockwise(Tetrimino *t, char **board, const char Height, const char Width);
//反時計回りに回転
void rotate_counter_clockwise(Tetrimino *t, char **board, const char Height, const char Width);
//１番下まで落下
void hard_drop(Tetrimino *t, char **board, const char Height, const char Width);
//１マス落下
void soft_drop(Tetrimino *t, char **board, const char Height, const char Width);
//列消去
int delete_line(char **board, const char Height, const char Width);

void free_tetrimino(Tetrimino *t);