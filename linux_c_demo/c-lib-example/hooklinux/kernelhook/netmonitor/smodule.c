#include <linux/init.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/skbuff.h>

static struct nf_hook_ops nfho;

// Hook函数，处理网络包
unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;

    // 获取IP头
    ip_header = ip_hdr(skb);

    // 只处理IPv4协议
    if (ip_header->protocol == IPPROTO_TCP)
    {
        // 获取TCP头
        tcp_header = tcp_hdr(skb);

        // 打印源IP、目标IP和目标端口
        printk(KERN_INFO "Source IP: %pI4\n", &ip_header->saddr);
        printk(KERN_INFO "Dest IP: %pI4\n", &ip_header->daddr);
        printk(KERN_INFO "Dest Port: %d\n", ntohs(tcp_header->dest));
    }

    return NF_ACCEPT; // 允许数据包继续传递
}

// 模块初始化函数
static int __init init_func(void)
{
    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_POST_ROUTING; // 后路由钩子
    nfho.pf = PF_INET;                   // IPv4
    nfho.priority = NF_IP_PRI_FIRST;     // 优先级

    nf_register_net_hook(&init_net, &nfho);

    printk(KERN_INFO "Netfilter hook registered\n");
    return 0;
}

// 模块清理函数
static void __exit exit_func(void)
{
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Netfilter hook unregistered\n");
}

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Netfilter Hook Module");
