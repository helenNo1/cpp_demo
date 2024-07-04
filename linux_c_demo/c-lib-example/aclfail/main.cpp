#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netfilter/xt_tcpudp.h>

#include <libiptc/libiptc.h>

void add_network_acl_rule(const char *table, const char *chain, char *source_ip, char *destination_ip, int port)
{
    struct xtc_handle *handle;
    struct ipt_entry *entry;
    struct ipt_entry_match *match;
    struct xt_tcp *tcpinfo;

    handle = iptc_init(table);
    if (!handle)
    {
        perror("Error initializing iptc handle");
        exit(EXIT_FAILURE);
    }

    entry = (struct ipt_entry *)calloc(1, XT_ALIGN(sizeof(struct ipt_entry)) + XT_ALIGN(sizeof(struct ipt_entry_match)) + XT_ALIGN(sizeof(struct xt_tcp)));
    if (!entry)
    {
        perror("Error allocating memory for rule");
        iptc_free(handle);
        exit(EXIT_FAILURE);
    }

    entry->target_offset = XT_ALIGN(sizeof(struct ipt_entry));
    entry->next_offset = XT_ALIGN(sizeof(struct ipt_entry)) + XT_ALIGN(sizeof(struct ipt_entry_match)) + XT_ALIGN(sizeof(struct xt_tcp));
    entry->ip.proto = IPPROTO_TCP;

    match = (struct ipt_entry_match *)(entry + 1);
    match->u.match_size = XT_ALIGN(sizeof(struct ipt_entry_match)) + XT_ALIGN(sizeof(struct xt_tcp));
    match->u.user.match_size = XT_ALIGN(sizeof(struct xt_tcp));
    // match->u.tcp.flags |= XT_TCP_INV_SRCPT;
    // match->u.tcp.src.sport = htons(port);

    tcpinfo = (struct xt_tcp *)(match + 1);
    tcpinfo->spts[1] = htons((uint16_t)port);
    tcpinfo->invflags |= XT_TCP_INV_SRCPT;

    // Set source and destination IPs
    if (inet_pton(AF_INET, source_ip, &(entry->ip.src.s_addr)) != 1 || inet_pton(AF_INET, destination_ip, &(entry->ip.dst.s_addr)) != 1)
    {
        perror("Error converting IP address");
        free(entry);
        iptc_free(handle);
        exit(EXIT_FAILURE);
    }

    // Add the rule to the chain
    if (iptc_append_entry(chain, entry, handle) != 0)
    {
        perror("Error adding rule to chain");
        free(entry);
        iptc_free(handle);
        exit(EXIT_FAILURE);
    }

    printf("Network ACL rule added successfully.\n");

    // Clean up
    free(entry);
    iptc_free(handle);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("argc < 4\n");
        exit(1);
    }
    printf("%s %s %s\n", argv[1], argv[2], argv[3]);
    const char *table = "filter";
    const char *chain = "INPUT";
    char source_ip[256], destination_ip[256], portstr[256];
    strcpy(source_ip, argv[1]);
    strcpy(destination_ip, argv[2]);
    strcpy(portstr, argv[3]);
    int port = atoi(portstr);

    add_network_acl_rule(table, chain, source_ip, destination_ip, port);

    return 0;
}
