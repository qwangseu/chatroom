#include "ptest.h"

#include <pthread.h>

#define MAX_FD (1000 * 9)
#define MAX_EV (1)

int global_flag = 0;


void *
_poll(void * ud) {
    struct socket_server *ss = ud;
    struct socket_message result;
    for (;;) {
        int type = socket_server_poll(ss, &result, NULL);
        // DO NOT use any ctrl command (socket_server_close , etc. ) in this thread.
        switch (type) {
        case SOCKET_EXIT:
            return NULL;
        case SOCKET_DATA:
            ++global_flag;
            //printf("[global : %d] : message(%lu) [id=%d] size=%d %s\n", global_flag , result.opaque,result.id, result.ud , result.data);            
            if(global_flag % (MAX_FD - 1) == 0 || 1)
                printf("[global : %d] : message(%lu) [id=%d] size=%d\n", global_flag , result.opaque, result.id , result.ud);    
            free(result.data);
            break;
        case SOCKET_CLOSE:
            printf("close(%lu) [id=%d]\n",result.opaque,result.id);
            break;
        case SOCKET_OPEN:
            printf("open(%lu) [id=%d] %s\n",result.opaque,result.id,result.data);
            break;
        case SOCKET_ERROR:

            printf("error(%lu) [id=%d]\n",result.opaque,result.id);
            break;
        case SOCKET_ACCEPT:
            printf("accept(%lu) [id=%d %s] from [%d]\n",result.opaque, result.ud, result.data, result.id);
            break;
        }
    }
}

void
test_by_phoneli(struct socket_server *ss) {

    pthread_t pid;
    pthread_create(&pid, NULL, _poll, ss);

    int i;
    for(i = 0 ; i < MAX_FD ;i++){
        socket_server_connect(ss , 10000+i , "127.0.0.1" , 9000);
    }

    sleep(1);
    for(i = 0 ; i < MAX_EV ; i++){
        socket_server_send(ss , 3 , "hello world." , 12);
        sleep(1);
    }

    char cmd[5] = {0};
    while(strncmp(cmd , "exit" , 4) != 0)
        scanf("%4s" , cmd);

    socket_server_exit(ss);

    pthread_join(pid, NULL); 
}

void
socket_server_test(void){
    struct sigaction sa;
    memset(&sa , 0 , sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);

    struct socket_server * ss = socket_server_create();
    test_by_phoneli(ss);
    socket_server_release(ss);
    return ;
}

int
main(int argv , char * args[])
{
    if(argv > 1){
        void (*fun[])(void) = {
            socket_server_test
        };
        putestInitWithFuncs(fun , *args[1]);
    }
    return 0;
}
