#include <libc.h>

int ft_print(char *str, char *arg)
{
    while (*str)
        write (2, str++, 1);
    if (arg)
        while (*arg)
            write (2, arg++, 1);
    return (write (2, "\n", 1), 1);
}

int ft_exec(char *av[], int i, int tmp, char *env[])
{
    av[i] = NULL;
    dup2(tmp, 0);
    close(tmp);
    execve(av[0], av, env);
    return (ft_print("error: cannot execute ", av[0]));
}

int main(int ac, char **av, char **env)
{
    int i, fd[2], tmp;
    (void)ac;

    i = 0;
    tmp = dup(0);
    while (av[i] && av[i + 1])
    {
        av = &av[i + 1];
        i = 0;
        while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
            i++;
        if (!strcmp(av[0], "cd"))
        {
            if (i != 2)
                ft_print("error: cd: bad arguments", NULL);
            else if (chdir(av[1]) != 0)
                ft_print("error: error change directory to ", av[1]);
        }
        else if (i != 0 && (av[i] == NULL || !strcmp(av[i], ";")))
        {
            if (!fork())
            {
                if (ft_exec(av, i, tmp, env))
                    return (1);
            }
            else 
            {
                close (tmp);
                while (waitpid(-1, NULL, 0) != -1);
            }
            tmp = dup(0);
        }
        else if (i != 0 && !strcmp(av[i], "|"))
        {
            pipe(fd);
            if (!fork())
            {
                dup2(fd[1], 1);
                close(fd[0]), close(fd[1]);
                if (ft_exec(av, i, tmp, env))
                    return (1);
            }
            else 
            {
                close(fd[1]), close(tmp);
                tmp = fd[0];
            }
        }
    }
    return (close(tmp), 0);
}