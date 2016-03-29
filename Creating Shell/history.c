#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LEN  128
#define HISTORY_COUNT 20

int history(char *hist[], int current)
{
        int i = current;
        int hist_num = 1;
       // char command[MAX_CMD_LEN] = cmd;
        //printf("%sThis was the command sent to me ",command);

        do {
                if (hist[i]) {
                        printf("%4d  %s\n", hist_num, hist[i]);
                        hist_num++;
                }

                i = (i + 1) % HISTORY_COUNT;

        } while (i != current);

        return 0;
}

int clear_history(char *hist[])
{
        int i;

        for (i = 0; i < HISTORY_COUNT; i++) {
                free(hist[i]);
                hist[i] = NULL;
        }

        return 0;
}

int main()
{
        char cmd[MAX_CMD_LEN];
        char *hist[HISTORY_COUNT];
        int i, current = 0;

        for (i = 0; i < HISTORY_COUNT; i++)
                hist[i] = NULL;

        while (1) {
                printf("user@shell # ");

                fgets(cmd, MAX_CMD_LEN, stdin);
                printf("can backspace be found%c",cmd[strlen(cmd)-1]);

                /* get rid of the '\n' from fgets */
                if (cmd[strlen(cmd) - 1] == '\n')
                        cmd[strlen(cmd) - 1] = '\0';

                /* NULL to `free` is a NO-OP */
                free(hist[current]);

                hist[current] = strdup(cmd);

                current = (current + 1) % HISTORY_COUNT;

                if (strcmp(cmd, "history") > 0)
                        history(hist, current);
                else if (strcmp(cmd, "hc") == 0)
                        clear_history(hist);
                else if (strcmp(cmd, "quit") == 0)
                        break;
        }

        clear_history(hist);

        return 0;
}