#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int pid, leuPid;

    int tunnel[2];
    pipe(tunnel);

    pid = fork();
    if (pid > 0)
    {
        printf("Pai: Pid do pai %d\n", pid);
        close(tunnel[0]);
        write(tunnel[1], &pid, sizeof(pid));
        close(tunnel[1]);
    }
    else
    {
        close(tunnel[1]);
        read(tunnel[0], &leuPid, sizeof(pid));
        printf("Filho: Pid do pai %d\n", leuPid);
        close(tunnel[0]);
    }
}