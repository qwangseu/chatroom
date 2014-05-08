#include "app.h"

void
initServer(void){
    initServerConfig();
    return ;
}

void *
eventPoll(void * ud){
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
appWork(struct socket_server * ss){
    pthread_t pid;
    pthread_create(&pid , NULL , eventPoll , ss);
    
    int l = socket_server_listen(ss , 200 , "127.0.0.1" , 9000 , 32);
    printf("listening id : %d\n" , l);
    socket_server_start(ss , 201 , l);

    char cmd[5] = {0};
    while(strncmp(cmd , "exit" , 4) != 0)
        scanf("%4s" , cmd);
        
    socket_server_exit(ss);
    pthread_join(pid , NULL);
}

int 
main(void){
    
    initServer();

    struct sigaction sa;
    memset(&sa , 0 , sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE , &sa , 0);

    struct socket_server * ss = socket_server_create();
    appWork(ss);
    socket_server_release(ss);

    return 0;
}














