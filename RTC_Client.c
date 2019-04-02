#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h> 
#include <string.h> 
#include <stdlib.h> 
struct addrinfo serv_adr, *res; 
int sockfd, con, get_adr, len, b_sent, rcv, c; 
char *msg="GET /HTTP/1.0\r\n\r\n", buf[100]; 
 
int com_socket_conn(); //functia de conectare la server 
int com_send_rcv(); // functia de cererea de date si de receptare 
int com_close(); // functia de inchidere a programului 
 
int main() 
{ 
        char s[10]; 
        int n, m, ok; 
        c=0; 
        while(1) 
        { 
        ok=1;//variabila in care consideram ca optiunea introdusa este valida 
        printf("1. Conectare la server\n2. Deconectare de la server\n3. Trimite cerere\n4. Iesire din aplicatie\n Introduecti optiunea: "); 
        scanf("%s", s); 
        printf("\n"); 
        if(strlen(s)>1) //verificam daca sirul de caractere este defapt un caracter 
                {  
                printf("Introduceti un singur caracter\n"); 
                ok=0;// optiunea este invalida 
                } 
        else 
                n=atoi(s); 
        if((n==0 || n>4 || n<0)&& ok==1)// verificam daca optiunea introdusa este intre 1 si 4 
                { 
                printf("Introduceti o optiune valida, o cifra de la 1 la 4\n"); 
                ok=0; 
                } 
        if(m==n && ok==1)// verificam daca s-a repetat optiunea introdusa anterior 
                { 
                printf("Nu se poate repeta o optiune\nIntrodceti o alta optiune de la 1 la 4\n."); 
                ok=0; 
                } 
        if(n==1)// in cazul in care optiunea este de  conexiune la server 
        { 
       if(c == 0)// modificam variabila care tine cont daca exista sau nu cerere de conexiune la server efectuata 
                c = 1; 
           else 
        { 
                printf("Conexiune deja creata\nIntroduceti o noua optiune de la 1 la 4\n"); 
                ok=0; 
        } 
        } 
        if(c==0 && n==2 && ok==1)//daca nu s-a facut conexiune la server si se cere deconectare 
                { 
 
                printf("Nu se poate face deconectare, pentru ca nu s-a facut conectarea\n. Introduceti o alta optiune de la 1 la 4\n"); 
                ok=0; 
                } 
        if(c==0 && n==3 && ok==1)// daca nu s-a facut conexiune la server si se cere transmisie de date 
                { 
                printf("Nu se poate face trimiterea, pentru ca nu s-a facut conectarea\n. Introduceti o alta optiune de la 1 la 4\n"); 
                ok=0; 
                } 
        if(ok==1)//in cazul in care optiunea e valida 
        { 
                switch(n)// se alege unul din cazurile 
                { 
                        case 1: com_socket_conn(); 
                                        break; 
                        case 2: com_close(); 
                                        break; 
                        case 3: com_send_rcv(); 
                                        break; 
                        default: exit(0); 
                } 
        } 
        m=n;// retinem optiunea curenta 
        if(n==2)// daca am facut deconecatre com modifica valoarea variabilei care contorizeaza conectarea la server 
                c=0; 
        } 
        return 0; 
} 
 
int com_socket_conn() 
{ 
        memset(&serv_adr,0, sizeof(serv_adr)); 
        serv_adr.ai_family=AF_INET; 
        serv_adr.ai_socktype=SOCK_STREAM; 
 
        get_adr=getaddrinfo("www.youtube.com","80",&serv_adr, &res);//obtinem adresa 
        if(get_adr!=0)//verificam daca functia de obtinere a adresei functioneaza 
                { 
                        printf("Eroare la obtinerea adresei\n"); 
                        return 1; 
                } 
        printf("Adresa ok\n"); 
        sockfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);//ne reutrneaza descriptorul 
 
        if(sockfd<0)//verificam daca functia de socket s-a creat 
        { 
                printf("Eroare la crearea socket-ului\n"); 
                return 1; 
        } 
        printf("Socket creat\n"); 
 
        con=connect(sockfd, res->ai_addr, res->ai_addrlen); 
        if(con==-1)//verificam daca s-a facut conexiunea cu server-ul adresei 
        { 
                printf("Eroare la conectare\n"); 
                return 1; 
        } 
        printf("Conectare cu succes\n"); 
} 
 
int com_send_rcv() 
{ 
        len=strlen(msg); 
        b_sent=send(sockfd, msg, len, 0); 
        if(b_sent==-1) // verificam daca cererea a fost trimisa 
        { 
                printf("Eroare la trimitere\n"); 
                return 1; 
        } 
        printf("Cererea a fost trimisa\n"); 
 
        FILE * fp= fopen("./index.html","w");//deschiderea fisierului in care von scrie datele 
 
        while(1)// facem receptionarea de date, cat timp mai este loc un buffer 
        { 
                int i; 
                rcv=recv(sockfd, buf,len, 0);//receptionarea datelor 
                if(rcv==-1) 
                { 
                        perror("Recv");//functie care descrie un mesaj de eroare 
                        return 1;                                                                                                                      
                } 
                if(rcv==0)//conexiunea a fost inchisa pentru client 
                        break; 
                for(i=0; i<rcv; i++)//aisarea datelor 
                { 
                        printf("%c", buf[i]); 
                        fprintf(fp, "%c", buf[i]); 
                } 
        } 
} 
 
int com_close() 
{ 
        printf("Deconectare cu succes\n Introduceti o alta optiune de la 1 la 4\n"); 
        return close(sockfd);//inchiderea socket-ului 
}