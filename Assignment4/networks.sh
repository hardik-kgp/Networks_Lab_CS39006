#!/bin/sh

####################################
#### Sriyash Poddar | 18CS30040 ####
#### Assignment 4 Part A        ####
####################################

# creating namespaces
# command: sudo ip netns add <name of namespace>
sudo ip netns add N1
sudo ip netns add N2
sudo ip netns add N3
sudo ip netns add N4

# creating v-eth to connect the namspaces
# command: sudo ip link add <veth-name> type veth peer name <veth-peername>
sudo ip link add V1 type veth peer name V2
sudo ip link add V3 type veth peer name V4
sudo ip link add V5 type veth peer name V6


# assigning interfaces to the namespaces
# command: sudo ip link set <veth-name> netns <namespace>
sudo ip link set V1 netns N1

sudo ip link set V2 netns N2
sudo ip link set V3 netns N2

sudo ip link set V4 netns N3
sudo ip link set V5 netns N3

sudo ip link set V6 netns N4

# adding ips to the interfaces, bringing them up, and setting routes
# command: sudo ip -n <namespace> addr add <ip-address/subnet> dev <veth-name>
sudo ip -n N1 addr add 10.0.10.40/24 dev V1

sudo ip -n N2 addr add 10.0.10.41/24 dev V2
sudo ip -n N2 addr add 10.0.20.40/24 dev V3

sudo ip -n N3 addr add 10.0.20.41/24 dev V4
sudo ip -n N3 addr add 10.0.30.40/24 dev V5

sudo ip -n N4 addr add 10.0.30.41/24 dev V6

# bringing veth, bridges and lo up
# command: sudo ip -n <namespace> link set dev <veth-name> up
sudo ip -n N1 link set dev V1 up
sudo ip -n N2 link set dev V2 up
sudo ip -n N2 link set dev V3 up
sudo ip -n N3 link set dev V4 up
sudo ip -n N3 link set dev V5 up
sudo ip -n N4 link set dev V6 up

sudo ip -n N1 link set dev lo up
sudo ip -n N2 link set dev lo up
sudo ip -n N3 link set dev lo up
sudo ip -n N4 link set dev lo up

# setting up ethernet bridge, attaching the ports, stopping the spanning tree protocol, and bringing it up
# command: sudo ip netns exec <namespace> brctl addbr <bridge-name>
#          sudo ip netns exec <namespace> brctl addif <bridge-name> <veth-name>
#          sudo ip netns exec <namespace> brctl stp <bridge-name> off
#          sudo ip -n <namespace> link set dev <bridge-name> up

#############################################
#### ERROR on uncommenting line 71 - 82: ####
#### During ping N1 -> V3, it shows      ####
#### Destination Host Unreachable        ####
#############################################

sudo ip netns exec N2 brctl addbr RB1
sudo ip netns exec N2 brctl addif RB1 V2
sudo ip netns exec N2 brctl addif RB1 V3
sudo ip netns exec N2 brctl stp RB1 off

sudo ip netns exec N3 brctl addbr RB2
sudo ip netns exec N3 brctl addif RB2 V4
sudo ip netns exec N3 brctl addif RB2 V5
sudo ip netns exec N3 brctl stp RB2 off

sudo ip -n N2 link set dev RB1 up
sudo ip -n N3 link set dev RB2 up

# adding routes
# command: sudo ip netns exec <namespace> route add <dest-ip> via <route-ip> dev <veth-name>
sudo ip -n N1 route add 10.0.20.0/24 via 10.0.10.41 dev V1
sudo ip -n N1 route add 10.0.30.0/24 via 10.0.10.41 dev V1

sudo ip -n N2 route add 10.0.30.0/24 via 10.0.20.41 dev V3

sudo ip -n N3 route add 10.0.10.0/24 via 10.0.20.40 dev V4

sudo ip -n N4 route add 10.0.20.0/24 via 10.0.30.40 dev V6
sudo ip -n N4 route add 10.0.10.0/24 via 10.0.30.40 dev V6

# ip forwarding
# command: sudo ip netns exec <namespace> sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N2 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N3 sysctl -w net.ipv4.ip_forward=1

# ping test
sudo ip netns exec N1 ping -c3 10.0.10.40 # N1 -> V1
sudo ip netns exec N1 ping -c3 10.0.10.41 # N1 -> V2
sudo ip netns exec N1 ping -c3 10.0.20.40 # N1 -> V3
sudo ip netns exec N1 ping -c3 10.0.20.41 # N1 -> V4
sudo ip netns exec N1 ping -c3 10.0.30.40 # N1 -> V5
sudo ip netns exec N1 ping -c3 10.0.30.41 # N1 -> V6

sudo ip netns exec N2 ping -c3 10.0.10.40 # N2 -> V1
sudo ip netns exec N2 ping -c3 10.0.10.41 # N2 -> V2
sudo ip netns exec N2 ping -c3 10.0.20.40 # N2 -> V3
sudo ip netns exec N2 ping -c3 10.0.20.41 # N2 -> V4
sudo ip netns exec N2 ping -c3 10.0.30.40 # N2 -> V5
sudo ip netns exec N2 ping -c3 10.0.30.41 # N2 -> V6

sudo ip netns exec N3 ping -c3 10.0.10.40 # N3 -> V1
sudo ip netns exec N3 ping -c3 10.0.10.41 # N3 -> V2
sudo ip netns exec N3 ping -c3 10.0.20.40 # N3 -> V3
sudo ip netns exec N3 ping -c3 10.0.30.40 # N3 -> V4
sudo ip netns exec N3 ping -c3 10.0.20.41 # N3 -> V5
sudo ip netns exec N3 ping -c3 10.0.30.41 # N3 -> V6

sudo ip netns exec N4 ping -c3 10.0.10.40 # N4 -> V1
sudo ip netns exec N4 ping -c3 10.0.10.41 # N4 -> V2
sudo ip netns exec N4 ping -c3 10.0.20.40 # N4 -> V3
sudo ip netns exec N4 ping -c3 10.0.20.41 # N4 -> V4
sudo ip netns exec N4 ping -c3 10.0.30.40 # N4 -> V5
sudo ip netns exec N4 ping -c3 10.0.30.41 # N4 -> V6

exit