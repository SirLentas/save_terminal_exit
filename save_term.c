#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>

int main(int argc, char **argv)
{
    FILE *fpb = fopen(argv[1], "w");
    time_t timer = time(NULL);
    fprintf(fpb, "%s", ctime(&timer));
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        fprintf(fpb,"%s $ ", cwd);
        for(int j=2;j<argc;j++){
            fprintf(fpb,"%s ",argv[j]);
        }
        fprintf(fpb,"\n\n");
    }
    else
    {
        perror("getcwd() error");
        return 1;
    }
    fclose(fpb);

    int input_fds = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    int status;

    int direct = fork();
    if (direct == 0)
    {
        char **arg = (char **)malloc((argc-1) * sizeof(char *));
        for (int i = 2; i <= argc; i++)
        {
            arg[i - 2] = argv[i];
        }
        arg[argc] = NULL;

        dup2(input_fds, 1);
        execvp(arg[0], arg);
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