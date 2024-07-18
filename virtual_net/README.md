# Notes on new configuration 

- This example has eth0 in /sys/class/net/eth0

Output from commands:

## ip route show
<No output>

## ip addr show eth0
2: eth0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop qlen 1000
    link/ether f6:f7:73:af:5b:45 brd ff:ff:ff:ff:ff:ff

## ip route add 10.0.0.0/24 via 10.0.0.253 dev eth0
ip: RTNETLINK answers: Network is unreachable


