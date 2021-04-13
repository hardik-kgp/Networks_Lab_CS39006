#!/bin/sh

####################################
#### Sriyash Poddar | 18CS30040 ####
#### Assignment 3 Part 2        ####
####################################

# creating namespaces
# command: sudo ip netns add <name of namespace>
sudo ip netns add H1
sudo ip netns add H2
sudo ip netns add H3
sudo ip netns add R


# creating v-eth to connect the namspaces
# command: sudo ip link add <veth-name> type veth peer name <veth-peername>
sudo ip link add veth1 type veth peer name veth2
sudo ip link add veth3 type veth peer name veth6
sudo ip link add veth4 type veth peer name veth5


# assigning interfaces to the namespaces
# command: sudo ip link set <veth-name> netns <namespace>
sudo ip link set veth1 netns H1

sudo ip link set veth5 netns H3

sudo ip link set veth6 netns H2

sudo ip link set veth2 netns R
sudo ip link set veth3 netns R
sudo ip link set veth4 netns R


# adding ips to the interfaces, bringing them up, and setting routes
# command: sudo ip -n <namespace> addr add <ip-address/subnet> dev <veth-name>
#          sudo ip -n <namespace> link set dev <veth-name> up
#          sudo ip -n <namespace> route add default via <ip-address> <veth-name>
sudo ip -n H1 addr add 10.0.10.40/24 dev veth1
sudo ip -n H1 link set dev veth1 up
sudo ip -n H1 route add default via 10.0.10.40 dev veth1

sudo ip -n R addr add 10.0.10.1/24 dev veth2
sudo ip -n R link set dev veth2 up
sudo ip -n R addr add 10.0.20.1/24 dev veth3
sudo ip -n R link set dev veth3 up
sudo ip -n R addr add 10.0.30.1/24 dev veth4
sudo ip -n R link set dev veth4 up

sudo ip -n H3 addr add 10.0.30.40/24 dev veth5
sudo ip -n H3 link set dev veth5 up
sudo ip -n H3 route add default via 10.0.30.40 dev veth5

sudo ip -n H2 addr add 10.0.20.40/24 dev veth6
sudo ip -n H2 link set dev veth6 up
sudo ip -n H2 route add default via 10.0.20.40 dev veth6


# setting up ethernet bridge, attaching the ports, stopping the spanning tree protocol, and bringing it up
# command: sudo ip netns exec <namespace> brctl addbr <bridge-name>
#          sudo ip netns exec <namespace> brctl addif <bridge-name> <veth-name>
#          sudo ip netns exec <namespace> brctl stp <bridge-name> off
#          sudo ip -n <namespace> link set dev <bridge-name> up
sudo ip netns exec R brctl addbr RB
sudo ip netns exec R brctl addif RB veth2
sudo ip netns exec R brctl addif RB veth3
sudo ip netns exec R brctl addif RB veth4
sudo ip netns exec R brctl stp RB off
sudo ip -n R link set dev RB up

# ip forwarding
# command: sudo ip netns exec <namespace> sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec R sysctl -w net.ipv4.ip_forward=1

exit