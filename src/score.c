#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "score.h"

int update_highscore(int score) {
    int high_score = 0;
    FILE *fp = fopen("data/high_score", "rb+");
    if (fp == NULL) {
        fp = fopen("data/high_score", "wb+");
        if (fp == NULL) {
            fprintf(stderr, "Failed to load or create high score file.\n");
            return 0;
        }
    }
    
    int buf[1];
    size_t n = fread(buf, sizeof(int), 1, fp);
    if (n == 1) {
        high_score = buf[0];
    }

    if (high_score < score) {
        high_score = score;
        buf[0] = high_score;
        rewind(fp);
        n = fwrite(buf, sizeof(int), 1, fp);
        if (n != 1) {
            fprintf(stderr, "Failed to update high score.\n");
            fclose(fp);
            return 0;
        }
    }
    
    fclose(fp);
    return high_score;
}
