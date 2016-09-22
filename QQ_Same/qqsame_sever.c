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

struct node
{
    char name[20];
    struct sockaddr_in client_addr;
    struct node *next;
};

struct message
{
    long type;
    char name[20];//保存用户名
    char file_name[128];//保存发送文件的名字
    char dest[20];//保存目标用户名
    char mtext[512];//消息正文
};

struct node *create_list(void);
void insert_list(struct node *, char *, struct sockaddr_in *);
void delete_list(struct node *, char *);

void recv_message(int , struct node *);
void send_message(int , struct sockaddr *, pid_t);
void send_to_someone(int , struct node *, struct message *);
void send_to_all(int,  struct node *, struct message *);

void client_login(int , struct node *, struct message *, struct sockaddr_in *);
void client_online(int , struct node *, struct message *);
void client_chat(int , struct node *, struct message *);
void client_quit(int , struct node *, struct message *);
void server_chat(int , struct node *, struct message *);
void server_quit(int , struct node *, struct message *);

int main(int argc, const char *argv[])
{
    int socket_fd;
    pid_t pid;
    struct sockaddr_in server_addr;
    struct node *head;

    if (argc < 3)
    {
        fprintf(stderr, "usages : %s ip port\n", argv[0]);
        exit(-1);
    }

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("failed to create socket");
        exit(-1);
    }

    head = create_list();

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("failed to bind");
        exit(-1);
    }

    if ((pid = fork()) < 0)
    {
        perror("failed to fork pid");
        exit(-1);
    }

    if (pid == 0)
        recv_message(socket_fd, head);
    else
        send_message(socket_fd, (struct sockaddr *)&server_addr, pid);
    return 0;
}

struct node *create_list(void)
{
    struct node *head;

    head = (struct node *)malloc(sizeof(struct node));
    head->next = NULL;

    return head;
}

void insert_list(struct node *head, char *name, struct sockaddr_in *client_addr)
{
    struct node *new;

    new = (struct node *)malloc(sizeof(struct node));
    strcpy(new->name, name);
    new->client_addr = *client_addr;

    new->next = head->next;
    head->next = new;

    return ;
}

void delete_list(struct node *head, char *name)
{
    struct node *p = head->next;
    struct node *q = head;

    while (p != NULL)
    {
        if (strcmp(p->name, name) == 0)
            break;

        p = p->next;
        q = q->next;
    }

    q->next = p->next;
    p->next = NULL;
    free(p);

    return ;
}

void recv_message(int socket_fd, struct node *head)//接受客户端发送过来的消息
{
    struct message msg;
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(struct sockaddr);

    while (1)
    {
        if (recvfrom(socket_fd, &msg, sizeof(msg), 0, (struct sockaddr *)&client_addr, &client_addrlen) < 0)
        {
            perror("failed to recvform client");
            exit(-1);
        }

        switch(msg.type)
        {
            case CLIENT_LOGIN:
                client_login(socket_fd, head, &msg, &client_addr);
                break;

            case CLIENT_ONLINE:
                client_online(socket_fd, head, &msg);
                break;

            case CLIENT_CHAT:
                client_chat(socket_fd, head, &msg);
                break;

            case CLIENT_QUIT:
                client_quit(socket_fd, head, &msg);
                break;

            case SERVER_CHAT:
                server_chat(socket_fd, head, &msg);
                break;

            case SERVER_QUIT:
                server_quit(socket_fd, head, &msg);
                break;

            default:
                break;
        }
    }

    return ;
}

void send_message(int socket_fd, struct sockaddr *server_addr, pid_t pid)//用于服务器向所有在线用户发送消息
{
    struct message msg;
    char buf[512];

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        memset(&msg, 0, sizeof(msg));

        usleep(500);

        printf(">");//输入通信内容
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = 0;

        strcpy(msg.mtext, buf);
        msg.type = SERVER_CHAT;
        if (strncmp(buf, "quit", 4) == 0)
        {
            msg.type = SERVER_QUIT;
            sendto(socket_fd, &msg, sizeof(msg), 0, server_addr, sizeof(struct sockaddr));
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            exit(0);
        }
            sendto(socket_fd, &msg, sizeof(msg), 0, server_addr, sizeof(struct sockaddr));
    }

}

void client_login(int socket_fd, struct node *head, struct message *msg, struct sockaddr_in *client_addr)
{
    printf("********Login In********\n");
    printf("%s is Login In\n", msg->name);
    printf("************************\n");

    insert_list(head, msg->name, client_addr);
    send_to_all(socket_fd, head, msg);//向其他用户发送该用户的登录信息
    return ;
}

void client_online(int socket_fd, struct node *head, struct message *msg)//把在线人员信息发送给客户端
{
//  printf("=======Online Msg=======\n");
    struct node *p = head->next, *q = NULL;
    char buf[20];
    bzero(msg->mtext,20);
    while(p != NULL)
    {
        if(strcmp(p->name, msg->name) == 0)
        {
                q = p;
        }
        else
        {
            sprintf(buf,"%s ",p->name);
            strcat(msg->mtext, buf);
        }
        p = p->next;
    }
    sendto(socket_fd, msg, sizeof(struct message), 0, (struct sockaddr *)&(q->client_addr), sizeof(struct sockaddr));

//  printf("************************\n");
    return ;
}

void client_chat(int socket_fd, struct node *head, struct message *msg)
{
    printf("********Group Chat********\n");
    if(strlen(msg->file_name) == 0)//通过file_name成员是否为空判断是否要发送文件
    {
        printf("from:%s to:%s\n", msg->name,msg->dest);
        printf("msg: %s\n", msg->mtext);
    }
    else
    {
        printf("send file:%s\n",msg->file_name);
    }
    printf("**************************\n");

    if(strncmp("all", msg->dest, 3) != 0)//根据发送用户名确定是单个发送还是全体发送
        send_to_someone(socket_fd, head, msg);
    else
        send_to_all(socket_fd, head, msg);

    return ;
}

void client_quit(int socket_fd, struct node *head, struct message *msg)
{
    printf("*********Quit Msg********\n");
    printf("%s is Quit\n", msg->name);
    printf("*************************\n");

    delete_list(head, msg->name);
    send_to_all(socket_fd, head, msg);

    return ;
}

void server_quit(int socket_fd, struct node *head, struct message *msg)
{
    printf("server_quit()...\n");
    send_to_all(socket_fd, head, msg);

    return ;
}

void server_chat(int socket_fd, struct node *head, struct message *msg)
{
    printf("server_chat()...\n");
    printf("server msg:%s\n",msg->mtext);
    send_to_all(socket_fd, head, msg);

    return;
}

void send_to_someone(int socket_fd, struct node *head, struct message *msg)//向指定用户发送消息
{
    printf("send_to_someone()...\n");
    struct node *p = head->next;

    while(p != NULL)
    {
        if (strcmp(p->name, msg->dest) != 0)
        {
                p = p->next;
                continue;
        }
        sendto(socket_fd, msg, sizeof(struct message), 0, (struct sockaddr *)&p->client_addr, sizeof(struct sockaddr));
        break;
    }
    printf("send_to_someone()...end!\n");

    return ;
}

void send_to_all(int socket_fd, struct node *head, struct message *msg)//向所有在线用户发送消息
{
    printf("send_to_all()...\n");
    struct node *p = head->next;

    while(p != NULL)
    {
        if (strcmp(p->name, msg->name) == 0)
        {
            p = p->next;
            continue;
        }
        sendto(socket_fd, msg, sizeof(struct message), 0, (struct sockaddr *)&p->client_addr, sizeof(struct sockaddr));
        printf("send to all:%s\n",msg->mtext);
        p = p->next;
    }
    printf("send_to_all()...end!\n");

    return ;
}
