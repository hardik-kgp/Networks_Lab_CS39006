#!/bin/sh

####################################
#### Sriyash Poddar | 18CS30040 ####
#### Assignment 3 Part 1        ####
####################################

# creating namespaces
sudo ip netns add ns0
sudo ip netns add ns1


# creating v-eth to connect the namspaces
sudo ip link add veth0 type veth peer name veth1


# assigning interfaces to the namespaces
sudo ip link set veth0 netns ns0
sudo ip link set veth1 netns ns1


# adding ips to the interfaces, and bringing them up, and setting routes
sudo ip -n ns0 addr add 10.1.1.0/24 dev veth0
sudo ip -n ns0 link set dev veth0 up
sudo ip -n ns0 route add default via 10.1.1.0 dev veth0

sudo ip -n ns1 addr add 10.1.2.0/24 dev veth1
sudo ip -n ns1 link set dev veth1 up
sudo ip -n ns1 route add default via 10.1.2.0 dev veth1

exit