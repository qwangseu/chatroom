#include "ptest.h"

#include <pthread.h>

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
            printf("message(%lu) [id=%d] size=%d %s\n",result.opaque,result.id, result.ud , result.data);
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

    int l = socket_server_listen(ss,200,"127.0.0.1",8888,32);
    printf("listening id:%d\n",l);
    socket_server_start(ss,201,l);

    int i;
    for (i=0;i<0;i++) {
        socket_server_connect(ss, 400+i, "127.0.0.1", 8888);
    }
    sleep(5);
    
    char * p = malloc(6);
    strncpy(p , "hello" , 5);
    p[5] = '\0';
    socket_server_send(ss , 2 , p , 6);
    sleep(3);
    
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
