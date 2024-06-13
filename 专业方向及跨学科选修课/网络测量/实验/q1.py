import dpkt
from dpkt.compat import compat_ord
import time
import sys
import socket
import os
import configparser
import binascii
import datetime
import ast


def mac_addr(address):
    return ':'.join('%02x' % compat_ord(b) for b in address)


def inet_to_str(inet):
    try:
        return socket.inet_ntop(socket.AF_INET, inet)
    except ValueError:
        return socket.inet_ntop(socket.AF_INET6, inet)

f = open('test1.pcap', "rb")
pcap = dpkt.pcap.Reader(f)

flow_len = {}

for timestamp, buf in pcap:

    eth = dpkt.ethernet.Ethernet(buf)
    ip = eth.data

    fingerprint = str(str(ip.data.sport) + "," + str(ip.data.dport) + "," + str(
        eth.type) + "," + inet_to_str(ip.src) + "," + inet_to_str(ip.dst))
    if fingerprint in flow_len:
        flow_len[fingerprint][2] += len(ip)
    else:
        flow_len[fingerprint] = [mac_addr(eth.src), mac_addr(eth.dst) , len(ip)]
        
print("Number of flows: " + str(len(flow_len)) + "\n")

for key, value in flow_len.items():
    key_list = key.split(",")
    print("IP: " + key_list[3] + ":" + key_list[0] + " -> " + key_list[4] + ":" + key_list[1])
    
    if key_list[2] == str(dpkt.ethernet.ETH_TYPE_IP):
        print("Type: IP")
    elif key_list[2] == str(dpkt.ethernet.ETH_TYPE_IP6):
        print("Type: IPv6")

    print("Length: " + str(value[2]))
    print("MAC: " + value[0] + " -> " + value[1] + "\n")
