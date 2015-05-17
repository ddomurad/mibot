#include <errno.h>
#include <signal.h>
#include <QCoreApplication>

void on_signall(int sig)
{
    if(sig == SIGTERM  || sig == SIGHUP  ||
        sig == SIGINT  || sig == SIGALRM)
    {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        return -1;

    if( signal(SIGINT, on_signall ) == SIG_ERR)
        return -2;

    if( signal(SIGTERM, on_signall ) == SIG_ERR)
        return -2;

    if( signal(SIGALRM, on_signall ) == SIG_ERR)
        return -2;

    if( signal(SIGALRM, on_signall ) == SIG_ERR)
        return -2;

    if(system( argv[1] ) == -1)
        return errno;

    return 0;
}
