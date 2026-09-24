#ifndef NET
#define NET

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"

// Limit of a single header
#define HEADER_LIM 8190
#define BUF_SIZE 4096

// Methods
#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"
#define HEAD "HEAD"

// 2xx: Success - The action was successfully received, understood, and accepted
#define OK "HTTP/1.1 200 OK"

// 4xx: Client Error - The request contains bad syntax or cannot be fulfilled
#define NOT_FOUND "HTTP/1.1 404 NOT FOUND\r\nContent-Length: 0\r\n\r\n"

// Request Fields
#define ACCEPT "Accept"

#define CR_NF "\r\n"

typedef struct
{
    char *method;
    char *http_ver;
    char *resource;
    HT *headers;
} HTTP_REQUEST;

typedef struct
{
    char *content_size;
    char *status_code;
    char *content_type;
} HTTP_RESPONSE;

#endif
