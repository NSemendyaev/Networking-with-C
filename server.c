#include "net.h"
#include "hash_table.h"

char *int_to_str(int n);

int main(void)
{
    HT *MIME_TYPES = create_ht();
    ht_put(MIME_TYPES, ".html", "text/html");
    ht_put(MIME_TYPES, ".css", "text/css");

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    bind(server, (struct sockaddr *)&address, sizeof(address));

    fprintf(stdout, "The server is up and running ...\n");
    while (1)
    {
        listen(server, 100);
        int client = accept(server, NULL, NULL);

        // Need a new process/thread

        HTTP_REQUEST req = {
            .method = NULL,
            .headers = calloc(1, sizeof(HT)),
        };

        if (client == -1)
        {
            fprintf(stderr, "%d\n", errno);
            continue;
        }
        else
        {
            // Receive an HTTP request

            // Read the whole request's header
            char *tokens[1024] = {0}, buf[HEADER_LIM], *buf_ptr = buf;
            ssize_t bread = 0;
            while ((bread = recv(client, buf_ptr, sizeof(buf), 0)) > 0)
            {
                // fprintf(stdout, "Current buf chunk: %s\n", buf);
                if (strpbrk(buf_ptr, CR_NF) != NULL)
                {
                    break;
                }
                buf_ptr += bread;
            }
            buf_ptr = buf;

            // fprintf(stdout, "%s", buf);

            // Tokenize the header
            char *line;
            while ((line = strsep(&buf_ptr, "\r\n")) != NULL)
            {
                if (strcmp(line, CR_NF) == 0)
                    break;

                if (line[0] == '\0')
                {
                    continue;
                }

                if (req.method == NULL)
                {
                    char *method = strsep(&line, " ");
                    char *resource = strsep(&line, " ");
                    char *version = strsep(&line, "\0");
                    if (method == NULL || resource == NULL || version == NULL)
                    {
                        ; // Send 404??
                    }

                    req.method = method;
                    req.resource = resource;
                    if (*(req.resource) == '/')
                    {
                        req.resource++;
                    }
                    req.http_ver = version;

                    continue;
                }

                char *key = strsep(&line, ":");
                line++;
                char *value = strsep(&(line), "\0");
                ht_put(req.headers, key, value);
            }
            printf("{Method: %s | Resource: %s | Version: %s}\n", req.method, req.resource, req.http_ver);
            ht_print(req.headers);
        }

        if (strcmp(req.method, GET) == 0)
        {
            HTTP_RESPONSE resp;

            // Open resource
            FILE *resource = fopen(req.resource, "r");

            // Check if resource exists
            if (resource == NULL)
            {
                send(client, NOT_FOUND, strlen(NOT_FOUND), 0);
                destroy_ht(req.headers);
                continue;
            }

            // Get resource's size
            fseek(resource, 0L, SEEK_END);
            ssize_t size = ftell(resource);
            fseek(resource, 0, SEEK_SET);
            // printf("Size of the resource: %zu\n", size);

            // Prepare response header
            char *ext = strrchr(req.resource, '.');
            char *mime = ht_get(MIME_TYPES, ext);
            if (mime == NULL)
            {
                ; // Handle missing type
            }
            resp.status_code = OK;
            resp.content_type = mime;
            resp.content_size = int_to_str(size);

            ssize_t head_size = strlen(resp.status_code) + strlen(resp.content_size) + strlen(resp.content_type) + strlen("Connection: close") + strlen("Content-Length: ") + strlen("Content-Type: ") + (strlen(CR_NF) * 5) + 1;
            char head[head_size], *head_ptr = head;
            snprintf(head_ptr, head_size,
                     "%s%s"
                     "Content-Length: %s%s"
                     "Content-Type: %s%s"
                     "%s%s"
                     "%s",
                     resp.status_code, CR_NF, resp.content_size, CR_NF, resp.content_type, CR_NF, "Connection: close", CR_NF, CR_NF);

            printf("%s", head);

            // Send response header
            head_ptr = head;
            ssize_t bsent = 0;
            // send(client, head_ptr, head_size, 0);
            while ((bsent = send(client, head_ptr, head_size, 0)) > 0)
            {
                head_ptr += bsent;
                head_size -= bsent;
            }
            printf("The headers has been sent\n");

            // Send body
            char data[4096];
            ssize_t bread = 0;
            while ((bread = fread(data, sizeof(char), 4096, resource)) > 0)
            {

                printf("%s", data);
                if (send(client, data, bread, 0) < 0)
                {
                    ; // Handle error
                }
            }
            printf("The body has been sent\n");
            destroy_ht(req.headers);
            close(client); // ??
            fclose(resource);
        }
        //     fprintf(stdout, "PUT method\n");
        // else if (strcmp(method, POST) == 0)
        //     fprintf(stdout, "POST method\n");
        // else if (strcmp(method, HEAD) == 0)
        //     fprintf(stdout, "HEAD method\n");
        // else if (strcmp(method, DELETE) == 0)
        //     fprintf(stdout, "DELETE method\n");
        // else fprintf(stdout, "Invalid method\n");
    }
    close(server);
    return 0;
}

char *int_to_str(int n)
{
    char stack[128];
    int size = n, i = 0;
    while ((size % 10) > 0)
    {
        stack[i++] = (char)((size % 10) + 48);
        size = size / 10;
    }

    char *ssize = (char *)malloc((sizeof(char) * i) + 1);
    char *ssize_ptr = ssize;
    while (i > -1)
    {
        *ssize_ptr++ = stack[--i];
    }
    *ssize_ptr = '\0';
    return ssize;
}