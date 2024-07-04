#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int inotify_fd = inotify_init();
    if (inotify_fd == -1)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int watch_descriptor = inotify_add_watch(inotify_fd, argv[1], IN_DELETE);
    if (watch_descriptor == -1)
    {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_LEN];

    while (1)
    {
        ssize_t len = read(inotify_fd, buffer, BUF_LEN);
        if (len == -1 && errno != EINTR)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (len > 0)
        {
            for (char *ptr = buffer; ptr < buffer + len;)
            {
                struct inotify_event *event = (struct inotify_event *)ptr;

                if (event->mask & IN_DELETE)
                {
                    printf("File deleted: %s\n", event->name);
                }

                ptr += EVENT_SIZE + event->len;
            }
        }
    }

    // Cleanup (not reached in this example)
    inotify_rm_watch(inotify_fd, watch_descriptor);
    close(inotify_fd);

    return EXIT_SUCCESS;
}
