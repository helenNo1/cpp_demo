#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/dcache.h>
#include <linux/fsnotify_backend.h>
#include <linux/fsnotify.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("File Addition Monitor Kernel Module");

#define WATCH_PATH "/tmp" // Replace with the path you want to monitor

static struct fsnotify_group *group;

static void handle_event(struct fsnotify_group *group, struct fsnotify_mark *mark,
                         u32 mask, const void *data, int data_type)
{
    struct path *path = (struct path *)data;
    pr_info("File added: %s\n", d_path(path, NULL, 0));
}

static const struct fsnotify_ops file_addition_ops = {
    .handle_event = handle_event,
};

static int __init file_addition_init(void)
{
    int ret;

    pr_info("File Addition Monitor Module loaded\n");

    group = fsnotify_alloc_group(&file_addition_ops, FSNOTIFY_MARK_FLAG_ALIVE);
    if (!group)
    {
        pr_err("Failed to allocate fsnotify group\n");
        return -ENOMEM;
    }

    ret = fsnotify_add_mark(group, FS_CREATE | FS_ISDIR, WATCH_PATH, NULL, NULL);
    if (ret < 0)
    {
        pr_err("Failed to add fsnotify mark\n");
        fsnotify_put_group(group);
        return ret;
    }

    return 0;
}

static void __exit file_addition_exit(void)
{
    pr_info("File Addition Monitor Module unloaded\n");
    fsnotify_put_group(group);
}

module_init(file_addition_init);
module_exit(file_addition_exit);
