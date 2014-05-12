#include "app.h"

void
initServer(void){
    initServerConfig();
    return ;
}

int
socket_data_handle(struct socket_server *ss , struct socket_message * result){
    int used = client_fdset->used,
        i = 0,
        id = 0;
    for(i = 0;i < used ; i++){
        id = client_fdset->set[i];                
        socket_server_online_gc(ss);
        if(likely(id != result->id))            
            socket_server_send(global_ss , id , result->data , result->ud);        
    }
    return 0;
}

int
socket_accpet_handle(struct socket_message * result){
    
    if(client_fdset->used >= client_fdset->size){
        struct fdSet * fds;
        fds = realloc(client_fdset , sizeof(*client_fdset) + sizeof(int) * client_fdset->size * 2);
        if(!fds)
            return -1;
        client_fdset = fds;
        client_fdset->size *= 2;
    }
    client_fdset->set[client_fdset->used++] = result->ud;

    //TODO : online gc for client_fdset->set[]
    return 0;
}

void *
loopPoll(void * ud){
    struct socket_server *ss = ud;
    struct socket_message result;
    for (;;) {
        int type = socket_server_poll(ss, &result, NULL);
        switch (type) {
        case SOCKET_EXIT:
            return NULL;
        case SOCKET_DATA:
            printf("message(%lu) [id=%d] size=%d %s\n",result.opaque,result.id, result.ud , result.data);
            if(socket_data_handle(ss , &result) != 0)
                appLog(APP_WARNING ,"error(%lu) [type:SOCKET_DATA] [id=%d] size=%d %s\n",result.opaque,result.id);                
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
            if(socket_accpet_handle(&result) != 0)
                appLog(APP_WARNING ,"error(%lu) [type:SOCKET_ACCEPT] [id=%d] \n",result.opaque,result.id);
            break;
        }
    }
}

void
appWork(void){
    pthread_t pid;
    pthread_create(&pid , NULL , loopPoll , global_ss);
    
    int l = socket_server_listen(global_ss , 200 , "127.0.0.1" , 9000 , 32);
    printf("listening id : %d\n" , l);
    socket_server_start(global_ss , 201 , l);

    char cmd[5] = {0};
    while(strncmp(cmd , "exit" , 4) != 0)
        scanf("%4s" , cmd);
        
    socket_server_exit(global_ss);
    pthread_join(pid , NULL);
}

int 
main(void){
    
    initServer();

    client_fdset = malloc(sizeof(*client_fdset) + MIN_CLIENT_FDSET * sizeof(int));
    if(!client_fdset)
        return -1;
    client_fdset->size = MIN_CLIENT_FDSET;
    client_fdset->used = 0;

    struct sigaction sa;
    memset(&sa , 0 , sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE , &sa , 0);

    global_ss = socket_server_create();
    appWork();
    socket_server_release(global_ss);

    free(client_fdset);

    return 0;
}














