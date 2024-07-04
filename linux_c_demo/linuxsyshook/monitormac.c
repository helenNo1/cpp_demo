#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

// 回调函数，用于处理每个网络数据包
static int packet_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
                           struct nfq_data *nfa, void *data) {
    // 获取数据包的源MAC地址和目的MAC地址
    unsigned char *payload;
    int len = nfq_get_payload(nfa, &payload);
    if (len > 0) {
        // 源MAC地址偏移量为6字节，目的MAC地址偏移量为0字节
        unsigned char *src_mac = payload + 6;
        unsigned char *dst_mac = payload;
        
        // 打印源MAC地址和目的MAC地址
        printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
               src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5]);
        printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
               dst_mac[0], dst_mac[1], dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5]);
    }

    // 接受数据包
    return nfq_set_verdict(qh, nfq_get_msg_packet_hdr(nfa), NF_ACCEPT, 0, NULL);
}

int main() {
    struct nfq_handle *h;
    struct nfq_q_handle *qh;
    struct nfnl_handle *nh;
    int fd;
    char buf[4096];

    // 创建Netfilter队列处理句柄
    h = nfq_open();
    if (!h) {
        perror("nfq_open");
        exit(EXIT_FAILURE);
    }

    // 设置处理队列
    if (nfq_unbind_pf(h, AF_INET) < 0) {
        perror("nfq_unbind_pf");
        exit(EXIT_FAILURE);
    }
    if (nfq_bind_pf(h, AF_INET) < 0) {
        perror("nfq_bind_pf");
        exit(EXIT_FAILURE);
    }

    // 创建队列
    qh = nfq_create_queue(h,  0, &packet_callback, NULL);
    if (!qh) {
        perror("nfq_create_queue");
        exit(EXIT_FAILURE);
    }

    // 设置队列长度
    if (nfq_set_queue_maxlen(qh, 4096) < 0) {
        perror("nfq_set_queue_maxlen");
        exit(EXIT_FAILURE);
    }

    // 打开Netlink套接字
    nh = nfq_nfnlh(h);
    fd = nfnl_fd(nh);

    // 主循环，接收数据包
    while (1) {
        int rv = recv(fd, buf, sizeof(buf), 0);
        if (rv >= 0) {
            nfq_handle_packet(h, buf, rv);
            continue;
        }
        // 错误处理
        perror("recv");
        break;
    }

    // 关闭队列句柄
    nfq_destroy_queue(qh);
    // 关闭Netfilter处理句柄
    nfq_close(h);

    return 0;
}

