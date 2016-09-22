//局域网的多人聊天，一对一
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define CLIENT_LOGIN    100
#define CLIENT_CHAT     200
#define CLIENT_QUIT     300

#define SERVER_CHAT     400
#define SERVER_QUIT     500

#define CLIENT_ONLINE   600

struct message
{
    long type;
    char name[20];//保存用户名
    char file_name[128];//保存发送文件的名字
    char dest[20];//保存目标用户名
    char mtext[512];//消息正文
};

void recv_message(int );
void send_message(int , struct sockaddr_in *, char *, pid_t);

void login_msg(struct message *);
void group_msg(int sockfd, struct message *msg);
void quit_msg(struct message *);
void server_msg(struct message *);
void server_quit(void);
void online_msg(struct message *msg);
void send_file(int socked_fd, struct message *msg, struct sockaddr *addr);
void recv_file(int sockfd, struct message *msg);
void send_string(int socked_fd, struct message *msg, struct sockaddr *addr);

int main(int argc, char *argv[])
{
    pid_t pid;
    int server_fd;
    struct sockaddr_in server_addr;

    if (argc < 4)
    {
        fprintf(stderr, "usages: %s ip port name\n", argv[0]);
        exit(-1);
    }

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("failed to create server_fd");
        exit(-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if ((pid = fork()) < 0)
    {
        perror("failed to fork pid");
        exit(-1);
    }

    if (pid == 0)
        recv_message(server_fd);
    else
        send_message(server_fd, &server_addr, argv[3], pid);

    return 0;
}

void recv_message(int server_fd)//接收服务器发送的消息
{
    struct message msg;

    while (1)
    {
        memset(&msg, 0, sizeof(msg));

        if (recvfrom(server_fd, &msg, sizeof(msg), 0, NULL, NULL) < 0)
        {
            perror("failed to recv server message");
            exit(-1);
        }

//      printf("msg.type=%ld\n",msg.type);
        switch(msg.type)
        {
            case CLIENT_LOGIN:
                login_msg(&msg);
                break;

            case CLIENT_CHAT:
                group_msg(server_fd, &msg);
                break;

            case CLIENT_QUIT:
                quit_msg(&msg);
                break;

            case CLIENT_ONLINE:
                online_msg(&msg);
                break;

            case SERVER_CHAT:
                server_msg(&msg);
                break;

            case SERVER_QUIT:
                server_quit();
                break;

            default:
                break;
        }
    }

    return ;
}

void send_message(int server_fd, struct sockaddr_in *server_addr, char *name, pid_t pid)//向服务器发送消息
{
    struct message msg;
    char buf[512];

    bzero(&msg, sizeof(msg));
    msg.type = CLIENT_LOGIN;//发送登录信息
    strcpy(msg.name, name);

    if (sendto(server_fd, &msg, sizeof(msg), 0,
                (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("failed to send login message");
        exit(-1);
    }

    msg.type = CLIENT_ONLINE;//发送查询在线信息
    strcpy(msg.name, name);

    if (sendto(server_fd, &msg, sizeof(msg), 0,
                (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("failed to send online message");
        exit(-1);
    }

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        memset(&msg, 0, sizeof(msg));

        usleep(500);

        printf("to:");//输入要通信的对方用户名，若是"all",表示向所有用户发送
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = 0;
        strcpy(msg.dest, buf);

        printf(">");//输入通信内容
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = 0;
        if(strncmp(buf, "FILE", 4) == 0)//表示要传送文件
        {
            printf("Input file name:");
            fgets(msg.file_name, sizeof(msg.file_name), stdin);
            (msg.file_name)[strlen(msg.file_name)-1] = 0;
        }

        strcpy(msg.mtext, buf);
        strcpy(msg.name, name);
        msg.type = CLIENT_CHAT;

        if (strncmp(buf, "quit", 4) == 0)
        {
            msg.type = CLIENT_QUIT;

            if (sendto(server_fd, &msg, sizeof(msg), 0,
                        (struct sockaddr *)server_addr, sizeof(struct sockaddr)) < 0)
            {
                perror("failed to send quit message");
                exit(-1);
            }

            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            exit(0);
        }

        send_string(server_fd, &msg, (struct sockaddr *)server_addr);
        if(strncmp(msg.mtext, "FILE", 4) == 0)//如果通信内容显示要传送文件，则进行传送文件处理
            send_file(server_fd, &msg, (struct sockaddr *)server_addr);
    }

    return ;
}

void login_msg(struct message *msg)//显示登录信息
{
    printf("######## Login in ########\n");
    printf("%s is login in\n", msg->name);
    printf("######## Login in ########\n");

    return ;
}

void online_msg(struct message *msg)//显示在线用户信息
{
    printf("======== Online Msg =======\n");
    printf("%s is online\n",msg->mtext);
    printf("======== Online Msg =======\n");
}

void group_msg(int sockfd, struct message *msg)//显示聊天信息
{
    printf("******** Group Msg ********\n");
    printf("from: %s\n", msg->name);
    printf("msg: %s\n", msg->mtext);
    if(strncmp(msg->mtext, "FILE", 4) == 0)//如果发送过来的消息是要接收文件，则进行接收文件处理
    {
        printf("recv file:%s\n",msg->file_name);
        recv_file(sockfd, msg);
    }
    printf("******** Group Msg ********\n");

    printf("to:");
    fflush(stdout);

    return ;
}

void quit_msg(struct message *msg)
{
    printf("######## Quit Msg ########\n");
    printf("%s is Quit\n", msg->name);
    printf("######## Quit Msg ########\n");

    return ;
}

void server_msg(struct message *msg)//显示服务器发送的信息
{
    printf("******** Server Msg ********\n");
    printf("server msg: %s\n", msg->mtext);
    printf("******** Server Msg ********\n");

    return ;
}

void server_quit(void )
{
    kill(getppid(), SIGKILL);
    exit(0);
}

void recv_file(int sockfd, struct message *msg)//接收文件
{
    FILE *fp;
    strcat(msg->file_name, "_copy");//在原文件名后添加_copy作为新文件名
    printf("start creat new file:%s\n",msg->file_name);
    fp = fopen(msg->file_name, "w");
    if(fp == NULL)
    {
        perror("open file error");
        return;
    }

    int write_bytes = 0;
    int len;
    while(recvfrom(sockfd, msg, sizeof(struct message), 0, NULL, NULL) > 0)//接收消息，并把消息内的正文写入文件
    {
        if(strncmp(msg->mtext, "end", 3) == 0)
            break;
        len = strlen(msg->mtext);
        write_bytes = fwrite(msg->mtext, 1, len, fp);
        printf("write_bytes=%d\n",write_bytes);
    }
    printf("end recv_file()...\n");
    fclose(fp);

    return;
}

void send_string(int socket_fd, struct message *msg, struct sockaddr *addr)//发送普通消息
{
    printf("start send_string()...\n");
        if(sendto(socket_fd, msg, sizeof(struct message), 0, addr, sizeof(struct sockaddr)) < 0)
        {
            perror("failed to send string");
            return;
        }
    printf("end send_string()...\n");
}

void send_file(int socket_fd, struct message *msg, struct sockaddr *addr)//发送文件
{
    printf("start send_file()...\n");
    FILE *fp;
    char buf[512];

//    printf("filename:%s",msg->file_name);
    fp = fopen(msg->file_name, "r");
    if(fp == NULL)
    {
        perror("fopen error");
        return ;
    }

    int read_bytes = 0;
    while((read_bytes = fread(buf, 1, 512, fp)) > 0)//传送文件
    {
        printf("read_bytes=%d\n",read_bytes);
        strncpy(msg->mtext, buf, read_bytes);
        if(sendto(socket_fd, msg, sizeof(struct message), 0, addr, sizeof(struct sockaddr)) < 0)
        {
            perror("failed to send file");
            return;
        }
        bzero(msg->mtext, sizeof(msg->mtext));
    }

    strcpy(msg->mtext, "end");//发送文件结束标志
    if(sendto(socket_fd, msg, sizeof(struct message), 0, addr, sizeof(struct sockaddr)) < 0)
    {
        perror("failed to send 'end'");
        return;
    }

    fclose(fp);
    printf("end send_file()...\n");
    return;
}
