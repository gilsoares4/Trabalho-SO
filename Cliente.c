#include "dados.h"

int fr;
int fw;

void desligaCliente(int sig){

    if(sig == SIGINT || sig == SIGUSR1){
        printf("\nA terminar Cliente...\n");
        close(fr);
        close(fw);
        unlink(FIFO_CLI);
        exit(EXIT_FAILURE);
        }
}

int main(){
    msg men;
    resp res;
    char str[50];

    signal(SIGINT, desligaCliente);
    signal(SIGUSR1, desligaCliente);

    men.pid = getpid();
    printf("%d\n", men.pid);
    int fw = open(FIFO_SERV, O_WRONLY);     //Abrir fifo do servidor escrita
    if(fw == -1){
        perror("A abrir o fifo servidor");
        exit(EXIT_FAILURE);
        }

    mkfifo(FIFO_CLI, 0777);             //criar fifo de resposta

    int fr = open(FIFO_CLI, O_RDONLY);  //Abrir fifo do cliente para leitura
    if(fr == -1){
        perror("A abrir o fifo cliente");
        close(fw);
        exit(EXIT_FAILURE);
        }

    printf("-Login-\n");
    men.login = 0;
    do{
        printf("Username: ");
        scanf("%s", men.username);
        printf("Password: ");
        scanf("%s", men.password);

        write(fw, &men, sizeof(men));
        read(fr, &res, sizeof(res));
        sleep(1);
        printf("%s", res.str);
    }while(res.log == 0);

    close(fr);
    close(fw);
    unlink(FIFO_CLI);

    return 0;
}
