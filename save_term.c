#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>

int main(int argc, char **argv)
{
    // FILE *fpb = fopen(argv[1], "w");
    // time_t timer = time(NULL);
    // fprintf(fpb, "%s", ctime(&timer));
    // char cwd[PATH_MAX];
    // if (getcwd(cwd, sizeof(cwd)) != NULL)
    // {
    //     fprintf(fpb, "%s $ ", cwd);
    //     for (int j = 2; j < argc; j++)
    //     {
    //         fprintf(fpb, "%s ", argv[j]);
    //     }
    //     fprintf(fpb, "\n\n");
    // }
    // else
    // {
    //     perror("getcwd() error");
    //     return 1;
    // }
    // fclose(fpb);

    int status;

    int direct = fork();
    if (direct == 0)
    {
        if (strcmp(argv[1], "-i") == 0)
        {
            FILE *fpb = fopen(argv[3], "w");
            time_t timer = time(NULL);
            fprintf(fpb, "%s", ctime(&timer));
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                fprintf(fpb, "%s $ ", cwd);
                for (int j = 2; j < argc; j++)
                {
                    fprintf(fpb, "%s ", argv[j]);
                }
                fprintf(fpb, "\n\n");
            }
            else
            {
                perror("getcwd() error");
                return 1;
            }
            fclose(fpb);
            int input_fds = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
            int inp = open(argv[2], O_RDONLY);
            char **arg = (char **)malloc((argc - 3) * sizeof(char *));
            for (int i = 4; i <= argc; i++)
            {
                arg[i - 4] = argv[i];
            }
            arg[argc - 2] = NULL;
            dup2(inp, 0);
            dup2(input_fds, 1);
            close(inp);
            close(input_fds);
            execvp(arg[0], arg);
        }
        else
        {
            FILE *fpb = fopen(argv[1], "w");
            time_t timer = time(NULL);
            fprintf(fpb, "%s", ctime(&timer));
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                fprintf(fpb, "%s $ ", cwd);
                for (int j = 2; j < argc; j++)
                {
                    fprintf(fpb, "%s ", argv[j]);
                }
                fprintf(fpb, "\n\n");
            }
            else
            {
                perror("getcwd() error");
                return 1;
            }
            fclose(fpb);
            int input_fds = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

            char **arg = (char **)malloc((argc - 1) * sizeof(char *));
            for (int i = 2; i <= argc; i++)
            {
                arg[i - 2] = argv[i];
            }
            arg[argc] = NULL;

            dup2(input_fds, 1);
            close(input_fds);
            execvp(arg[0], arg);
        }
    }
    else if (direct == -1)
    {
        perror("fork");
        exit(1);
    }
    else
    {
        while ((waitpid(direct, &status, WNOHANG)) == 0)
        {
            sleep(1);
        }
    }
}