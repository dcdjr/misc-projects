// Enable modern Winsock features like inet_pton.
#define _WIN32_WINNT 0x0601

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Store the target IP so every thread can read it.
static const char *TARGET_IP;

// Mutex so multiple threads don't print at the same time.
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

// Job queue structure
typedef struct {
    int *ports;            // list of ports (ex: 1 to 1023)
    int size;              // total number of ports
    int index;             // next port to hand out
    pthread_mutex_t lock;  // protects index
} JobQueue;

// Prototypes
void *worker(void *arg);
int get_next_port(JobQueue *q);

// Main function
int main(int argc, char *argv[]) {

    // Initialize Winsock before doing anything network-related.
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Ensure the user provided an IP.
    if (argc < 2) {
        printf("Usage: %s <ip> [start_port end_port] <num_threads>\n", argv[0]);
        return 1;
    }

    TARGET_IP = argv[1];

    // Validate the IP once here instead of in every thread.
    struct sockaddr_in tmp = {0};
    if (inet_pton(AF_INET, TARGET_IP, &tmp.sin_addr) != 1) {
        printf("Invalid IPv4 address: %s\n", TARGET_IP);
        return 1;
    }

    printf("Scanning %s ...\n", TARGET_IP);

    // Default port range and number of threads. User can override these.
    int start = 1;
    int end = 1023;
    int num_threads = 50;

    // If user provided both ports, use them. If they provided ports + thread count, use all.
    if (argc == 4) {
        start = atoi(argv[2]);
        end = atoi(argv[3]);
    } else if (argc == 5) {
        start = atoi(argv[2]);
        end = atoi(argv[3]);
        num_threads = atoi(argv[4]);
    }

    // Bounds checking for thread count.
    if (num_threads < 1) num_threads = 1;
    if (num_threads > 5000) num_threads = 5000;

    // Initialize job queue.
    JobQueue q;
    q.size = end - start + 1;
    q.ports = malloc(q.size * sizeof(int));
    q.index = 0;
    pthread_mutex_init(&q.lock, NULL);

    for (int i = 0; i < q.size; i++) {
        q.ports[i] = start + i;
    }

    // Create N worker threads.
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, &q);
    }

    // Join worker threads.
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    free(q.ports);
    free(threads);
    pthread_mutex_destroy(&q.lock);
    WSACleanup();

    return 0;
}

// Thread entry point. Must take void* and return void*.
void *worker(void *arg) {
    JobQueue *q = (JobQueue*)arg;

    while (1) {

        // Get the next port from the job queue.
        int port = get_next_port(q);
        if (port == -1)
            break;

        // Build sockaddr_in for this port.
        struct sockaddr_in target = {0};
        target.sin_family = AF_INET;

        // Convert IP string into binary IPv4 form.
        if (inet_pton(AF_INET, TARGET_IP, &target.sin_addr) != 1)
            return NULL;

        // Port numbers must be stored in network byte order.
        target.sin_port = htons(port);

        // Create a TCP socket.
        SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == INVALID_SOCKET)
            return NULL;

        // Short timeout so scans don't hang.
        DWORD timeout = 200;
        setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

        // Try connecting to the port and attempt banner grab.
        int result = connect(s, (struct sockaddr*)&target, sizeof(target));

        if (result == 0) {
            char banner[245];
            int n = recv(s, banner, sizeof(banner) - 1, 0);

            if (n > 0) {
                banner[n] = '\0';
                pthread_mutex_lock(&print_lock);
                printf("Port %d OPEN - banner: %s\n", port, banner);
                pthread_mutex_unlock(&print_lock);
            } else {
                pthread_mutex_lock(&print_lock);
                printf("Port %d OPEN\n", port);
                pthread_mutex_unlock(&print_lock);
            }
        }

        closesocket(s);
    }

    return NULL;
}

// Thread helper function to get next port from job queue.
int get_next_port(JobQueue *q) {

    // Lock queue while reading/updating index.
    pthread_mutex_lock(&q->lock);

    // If all ports have been handed out, signal completion.
    if (q->index >= q->size) {
        pthread_mutex_unlock(&q->lock);
        return -1;
    }

    // Assign next port and increment index.
    int port = q->ports[q->index++];

    // Unlock for next thread.
    pthread_mutex_unlock(&q->lock);

    return port;
}