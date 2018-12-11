#include "wrapsock.h"
#include "wrapstdio.h"
#include "udp_client.h"

#define	MAXLINE		4096	/* max text line length */
#define	SA	struct sockaddr

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int	n;
    char	sendline[MAXLINE], recvline[MAXLINE + 1];

    while (Fgets(sendline, MAXLINE, fp) != NULL) {

        Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

        n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

        recvline[n] = 0;	/* null terminate */
        Fputs(recvline, stdout);
    }
}

int main(int argc, char **argv)
{
    // Initialize Winsock
    WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    int					sockfd;
    socklen_t			salen;
    struct sockaddr		*sa;

    /*if (argc != 3)
        err_quit("usage: udpcli02 <hostname> <service>");

    sockfd = Udp_client(argv[1], argv[2], (SA **)&sa, &salen);*/

    sockfd = Udp_client("127.0.0.1", "9877", (SA **)&sa, &salen);

    dg_cli(stdin, sockfd, sa, salen);

    WSACleanup();

    exit(0);
}
