/* include udp_client */
#pragma once

#define	bzero(ptr,n) memset(ptr, 0, n)
#define	SA struct sockaddr

void *
Malloc(size_t size)
{
    void	*ptr;

    if ((ptr = malloc(size)) == NULL)
        err_sys("malloc error");
    return(ptr);
}

int udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("udp_client error for %s, %s: %s");// ,
				 //host, serv, gai_strerror(n));
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd >= 0)
			break;		/* success */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL)	/* errno set from final socket() */
        err_sys("udp_client error for %s, %s");// , host, serv);

	*saptr = (SA*)Malloc(res->ai_addrlen);
	memcpy(*saptr, res->ai_addr, res->ai_addrlen);
	*lenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return(sockfd);
}
/* end udp_client */

int
Udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenptr)
{
	return(udp_client(host, serv, saptr, lenptr));
}
