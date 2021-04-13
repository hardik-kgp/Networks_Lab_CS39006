#!/bin/sh

####################################
#### Sriyash Poddar | 18CS30040 ####
#### Lab Test-1 Q2              ####
####################################

# creating namespaces
# command: sudo ip netns add <name of namespace>
sudo ip netns add H1
sudo ip netns add H2
sudo ip netns add H3
sudo ip netns add H4

sudo ip netns add R1
sudo ip netns add R2
sudo ip netns add R3
sudo ip netns add R4
sudo ip netns add R5
sudo ip netns add R6

# creating v-eth to connect the namspaces
# command: sudo ip link add <veth-name> type veth peer name <veth-peername>
sudo ip link add V1 type veth peer name V2
sudo ip link add V3 type veth peer name V4
sudo ip link add V5 type veth peer name V6
sudo ip link add V7 type veth peer name V8
sudo ip link add V9 type veth peer name V10
sudo ip link add V11 type veth peer name V12
sudo ip link add V13 type veth peer name V14
sudo ip link add V15 type veth peer name V16
sudo ip link add V17 type veth peer name V18


# assigning interfaces to the namespaces
# command: sudo ip link set <veth-name> netns <namespace>
sudo ip link set V1 netns H1

sudo ip link set V2 netns R1
sudo ip link set V3 netns R1

sudo ip link set V4 netns R2
sudo ip link set V5 netns R2
sudo ip link set V9 netns R2

sudo ip link set V6 netns R3
sudo ip link set V7 netns R3

sudo ip link set V8 netns H2

sudo ip link set V10 netns R4
sudo ip link set V11 netns R4
sudo ip link set V15 netns R4

sudo ip link set V12 netns R5
sudo ip link set V13 netns R5

sudo ip link set V14 netns H3

sudo ip link set V16 netns R6
sudo ip link set V17 netns R6

sudo ip link set V18 netns H4

# adding ips to the interfaces, bringing them up, and setting routes
# command: sudo ip -n <namespace> addr add <ip-address/subnet> dev <veth-name>
sudo ip -n H1 addr add 10.10.10.40/24 dev V1

sudo ip -n R1 addr add 10.10.10.41/24 dev V2
sudo ip -n R1 addr add 10.10.20.40/24 dev V3

sudo ip -n R2 addr add 10.10.20.41/24 dev V4
sudo ip -n R2 addr add 10.10.30.40/24 dev V5
sudo ip -n R2 addr add 10.10.50.40/24 dev V9

sudo ip -n R3 addr add 10.10.30.41/24 dev V6
sudo ip -n R3 addr add 10.10.40.40/24 dev V7

sudo ip -n H2 addr add 10.10.40.41/24 dev V8

sudo ip -n R4 addr add 10.10.50.41/24 dev V10
sudo ip -n R4 addr add 10.20.10.40/24 dev V11
sudo ip -n R4 addr add 10.30.10.40/24 dev V15

sudo ip -n R5 addr add 10.20.10.41/24 dev V12
sudo ip -n R5 addr add 10.20.20.40/24 dev V13

sudo ip -n H3 addr add 10.20.20.41/24 dev V14

sudo ip -n R6 addr add 10.30.10.41/24 dev V16
sudo ip -n R6 addr add 10.30.20.40/24 dev V17

sudo ip -n H4 addr add 10.30.20.41/24 dev V18

# bringing veth, bridges and lo up
# command: sudo ip -n <namespace> link set dev <veth-name> up
sudo ip -n H1 link set dev V1 up
sudo ip -n R1 link set dev V2 up
sudo ip -n R1 link set dev V3 up
sudo ip -n R2 link set dev V4 up
sudo ip -n R2 link set dev V5 up
sudo ip -n R2 link set dev V9 up
sudo ip -n R3 link set dev V6 up
sudo ip -n R3 link set dev V7 up
sudo ip -n H2 link set dev V8 up
sudo ip -n R4 link set dev V10 up
sudo ip -n R4 link set dev V11 up
sudo ip -n R4 link set dev V15 up
sudo ip -n R5 link set dev V12 up
sudo ip -n R5 link set dev V13 up
sudo ip -n H3 link set dev V14 up
sudo ip -n R6 link set dev V16 up
sudo ip -n R6 link set dev V17 up
sudo ip -n H4 link set dev V18 up

sudo ip -n H1 link set dev lo up
sudo ip -n H2 link set dev lo up
sudo ip -n H3 link set dev lo up
sudo ip -n H4 link set dev lo up
sudo ip -n R1 link set dev lo up
sudo ip -n R2 link set dev lo up
sudo ip -n R3 link set dev lo up
sudo ip -n R4 link set dev lo up
sudo ip -n R5 link set dev lo up
sudo ip -n R6 link set dev lo up

# adding routes
# command: sudo ip netns exec <namespace> route add <dest-ip> via <route-ip> dev <veth-name>

# In H1 routes for only 10.10.10.0/24 in table, so adding routes for all other subnets and a single path is available
# Optimization: Can add default route to make code smaller
sudo ip -n H1 route add 10.10.20.0/24 via 10.10.10.41 dev V1
sudo ip -n H1 route add 10.10.30.0/24 via 10.10.10.41 dev V1
sudo ip -n H1 route add 10.10.40.0/24 via 10.10.10.41 dev V1
sudo ip -n H1 route add 10.10.50.0/24 via 10.10.10.41 dev V1
sudo ip -n H1 route add 10.20.10.0/24 via 10.10.10.41 dev V1
sudo ip -n H1 route add 10.20.20.0/24 via 10.10.10.41 dev V1
sudo ip -n H1 route add 10.30.10.0/24 via 10.10.10.41 dev V1
sudo ip -n H1 route add 10.30.20.0/24 via 10.10.10.41 dev V1

# In H2 routes for only 10.10.40.0/24 in table, so adding routes for all other subnets and a single path is available
sudo ip -n H2 route add 10.10.10.0/24 via 10.10.40.40 dev V8
sudo ip -n H2 route add 10.10.20.0/24 via 10.10.40.40 dev V8
sudo ip -n H2 route add 10.10.30.0/24 via 10.10.40.40 dev V8
sudo ip -n H2 route add 10.10.50.0/24 via 10.10.40.40 dev V8
sudo ip -n H2 route add 10.20.10.0/24 via 10.10.40.40 dev V8
sudo ip -n H2 route add 10.20.20.0/24 via 10.10.40.40 dev V8
sudo ip -n H2 route add 10.30.10.0/24 via 10.10.40.40 dev V8
sudo ip -n H2 route add 10.30.20.0/24 via 10.10.40.40 dev V8

# In H3 routes for only 10.20.20.0/24 in table, so adding routes for all other subnets and a single path is available
sudo ip -n H3 route add 10.10.10.0/24 via 10.20.20.40 dev V14
sudo ip -n H3 route add 10.10.20.0/24 via 10.20.20.40 dev V14
sudo ip -n H3 route add 10.10.30.0/24 via 10.20.20.40 dev V14
sudo ip -n H3 route add 10.10.40.0/24 via 10.20.20.40 dev V14
sudo ip -n H3 route add 10.10.50.0/24 via 10.20.20.40 dev V14
sudo ip -n H3 route add 10.20.10.0/24 via 10.20.20.40 dev V14
sudo ip -n H3 route add 10.30.10.0/24 via 10.20.20.40 dev V14
sudo ip -n H3 route add 10.30.20.0/24 via 10.20.20.40 dev V14

# In H4 routes for only 10.30.40.0/24 in table, so adding routes for all other subnets and a single path is available
sudo ip -n H4 route add 10.10.10.0/24 via 10.30.20.40 dev V18
sudo ip -n H4 route add 10.10.20.0/24 via 10.30.20.40 dev V18
sudo ip -n H4 route add 10.10.30.0/24 via 10.30.20.40 dev V18
sudo ip -n H4 route add 10.10.40.0/24 via 10.30.20.40 dev V18
sudo ip -n H4 route add 10.10.50.0/24 via 10.30.20.40 dev V18
sudo ip -n H4 route add 10.20.10.0/24 via 10.30.20.40 dev V18
sudo ip -n H4 route add 10.20.20.0/24 via 10.30.20.40 dev V18
sudo ip -n H4 route add 10.30.10.0/24 via 10.30.20.40 dev V18

# In R1 routes for 10.10.10.0/24, 10.10.20.0/24 in table, so adding routes for all other subnets and a single path is available
sudo ip -n R1 route add 10.10.30.0/24 via 10.10.20.41 dev V3
sudo ip -n R1 route add 10.10.40.0/24 via 10.10.20.41 dev V3
sudo ip -n R1 route add 10.10.50.0/24 via 10.10.20.41 dev V3
sudo ip -n R1 route add 10.20.10.0/24 via 10.10.20.41 dev V3
sudo ip -n R1 route add 10.20.20.0/24 via 10.10.20.41 dev V3
sudo ip -n R1 route add 10.30.10.0/24 via 10.10.20.41 dev V3
sudo ip -n R1 route add 10.30.20.0/24 via 10.10.20.41 dev V3

# In R2 routes for 10.10.20.0/24, 10.10.30.0/24, 10.10.50.0/24 in table, so adding routes for all other subnets and using appropriate paths
sudo ip -n R2 route add 10.10.10.0/24 via 10.10.20.40 dev V4
sudo ip -n R2 route add 10.10.40.0/24 via 10.10.30.41 dev V5
sudo ip -n R2 route add 10.20.10.0/24 via 10.10.50.41 dev V9
sudo ip -n R2 route add 10.20.20.0/24 via 10.10.50.41 dev V9
sudo ip -n R2 route add 10.30.10.0/24 via 10.10.50.41 dev V9
sudo ip -n R2 route add 10.30.20.0/24 via 10.10.50.41 dev V9

# In R3 routes for 10.10.30.0/24, 10.10.40.0/24 in table, so adding routes for all other subnets and a single path is available
sudo ip -n R3 route add 10.10.10.0/24 via 10.10.30.40 dev V6
sudo ip -n R3 route add 10.10.20.0/24 via 10.10.30.40 dev V6
sudo ip -n R3 route add 10.10.50.0/24 via 10.10.30.40 dev V6
sudo ip -n R3 route add 10.20.10.0/24 via 10.10.30.40 dev V6
sudo ip -n R3 route add 10.20.20.0/24 via 10.10.30.40 dev V6
sudo ip -n R3 route add 10.30.10.0/24 via 10.10.30.40 dev V6
sudo ip -n R3 route add 10.30.20.0/24 via 10.10.30.40 dev V6

# In R4 routes for 10.10.50.0/24, 10.20.10.0/24, 10.30.10.0/24 in table, so adding routes for all other subnets and using appropriate paths
sudo ip -n R4 route add 10.10.10.0/24 via 10.10.50.40 dev V10
sudo ip -n R4 route add 10.10.20.0/24 via 10.10.50.40 dev V10
sudo ip -n R4 route add 10.10.30.0/24 via 10.10.50.40 dev V10
sudo ip -n R4 route add 10.10.40.0/24 via 10.10.50.40 dev V10
sudo ip -n R4 route add 10.20.20.0/24 via 10.20.10.41 dev V11
sudo ip -n R4 route add 10.30.20.0/24 via 10.30.10.41 dev V15

# In R5 routes for 10.20.10.0/24, 10.20.20.0/24 in table, so adding routes for all other subnets and a single path is available
sudo ip -n R5 route add 10.10.10.0/24 via 10.20.10.40 dev V12
sudo ip -n R5 route add 10.10.20.0/24 via 10.20.10.40 dev V12
sudo ip -n R5 route add 10.10.30.0/24 via 10.20.10.40 dev V12
sudo ip -n R5 route add 10.10.40.0/24 via 10.20.10.40 dev V12
sudo ip -n R5 route add 10.10.50.0/24 via 10.20.10.40 dev V12
sudo ip -n R5 route add 10.30.10.0/24 via 10.20.10.40 dev V12
sudo ip -n R5 route add 10.30.20.0/24 via 10.20.10.40 dev V12

# In R6 routes for 10.30.10.0/24, 10.30.20.0/24 in table, so adding routes for all other subnets and a single path is available
sudo ip -n R6 route add 10.10.10.0/24 via 10.30.10.40 dev V16
sudo ip -n R6 route add 10.10.20.0/24 via 10.30.10.40 dev V16
sudo ip -n R6 route add 10.10.30.0/24 via 10.30.10.40 dev V16
sudo ip -n R6 route add 10.10.40.0/24 via 10.30.10.40 dev V16
sudo ip -n R6 route add 10.10.50.0/24 via 10.30.10.40 dev V16
sudo ip -n R6 route add 10.20.10.0/24 via 10.30.10.40 dev V16
sudo ip -n R6 route add 10.20.20.0/24 via 10.30.10.40 dev V16

# ip forwarding
# command: sudo ip netns exec <namespace> sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec R1 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec R2 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec R3 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec R4 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec R5 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec R6 sysctl -w net.ipv4.ip_forward=1

# trace routes
sudo ip netns exec H1 traceroute 10.30.20.41
sleep 2
sudo ip netns exec H3 traceroute 10.10.40.41
sleep 2
sudo ip netns exec H4 traceroute 10.20.20.41

# ping test
# for x in {1..5}
# do
# 	y=$((x*10))
# 	sudo ip netns exec H1 ping -c3 10.10."$y".40
# 	sudo ip netns exec H2 ping -c3 10.10."$y".40 
# 	sudo ip netns exec H3 ping -c3 10.10."$y".40 
# 	sudo ip netns exec H4 ping -c3 10.10."$y".40 
# 	sudo ip netns exec R1 ping -c3 10.10."$y".40 
# 	sudo ip netns exec R2 ping -c3 10.10."$y".40 
# 	sudo ip netns exec R3 ping -c3 10.10."$y".40 
#     sudo ip netns exec R4 ping -c3 10.10."$y".40 
# 	sudo ip netns exec R5 ping -c3 10.10."$y".40 
# 	sudo ip netns exec R6 ping -c3 10.10."$y".40 
# 	sudo ip netns exec H1 ping -c3 10.10."$y".41 
# 	sudo ip netns exec H2 ping -c3 10.10."$y".41 
# 	sudo ip netns exec H3 ping -c3 10.10."$y".41 
# 	sudo ip netns exec H4 ping -c3 10.10."$y".41 
# 	sudo ip netns exec R1 ping -c3 10.10."$y".41 
# 	sudo ip netns exec R2 ping -c3 10.10."$y".41 
# 	sudo ip netns exec R3 ping -c3 10.10."$y".41 
#     sudo ip netns exec R4 ping -c3 10.10."$y".41 
# 	sudo ip netns exec R5 ping -c3 10.10."$y".41 
# 	sudo ip netns exec R6 ping -c3 10.10."$y".41 
# done

# for x in {1..2}
# do
#     sudo ip netns exec H1 ping -c3 10.20."$y".40
#     sudo ip netns exec H2 ping -c3 10.20."$y".40 
#     sudo ip netns exec H3 ping -c3 10.20."$y".40 
#     sudo ip netns exec H4 ping -c3 10.20."$y".40 
#     sudo ip netns exec R1 ping -c3 10.20."$y".40 
#     sudo ip netns exec R2 ping -c3 10.20."$y".40 
#     sudo ip netns exec R3 ping -c3 10.20."$y".40 
#     sudo ip netns exec R4 ping -c3 10.20."$y".40 
#     sudo ip netns exec R5 ping -c3 10.20."$y".40 
#     sudo ip netns exec R6 ping -c3 10.20."$y".40 
#     sudo ip netns exec H1 ping -c3 10.20."$y".41 
#     sudo ip netns exec H2 ping -c3 10.20."$y".41 
#     sudo ip netns exec H3 ping -c3 10.20."$y".41 
#     sudo ip netns exec H4 ping -c3 10.20."$y".41
#     sudo ip netns exec R1 ping -c3 10.20."$y".41 
#     sudo ip netns exec R2 ping -c3 10.20."$y".41 
#     sudo ip netns exec R3 ping -c3 10.20."$y".41 
#     sudo ip netns exec R4 ping -c3 10.20."$y".41 
#     sudo ip netns exec R5 ping -c3 10.20."$y".41 
#     sudo ip netns exec R6 ping -c3 10.20."$y".41

#     sudo ip netns exec H1 ping -c3 10.30."$y".40
#     sudo ip netns exec H2 ping -c3 10.30."$y".40 
#     sudo ip netns exec H3 ping -c3 10.30."$y".40 
#     sudo ip netns exec H4 ping -c3 10.30."$y".40 
#     sudo ip netns exec R1 ping -c3 10.30."$y".40 
#     sudo ip netns exec R2 ping -c3 10.30."$y".40 
#     sudo ip netns exec R3 ping -c3 10.30."$y".40 
#     sudo ip netns exec R4 ping -c3 10.30."$y".40 
#     sudo ip netns exec R5 ping -c3 10.30."$y".40 
#     sudo ip netns exec R6 ping -c3 10.30."$y".40 
#     sudo ip netns exec H1 ping -c3 10.30."$y".41 
#     sudo ip netns exec H2 ping -c3 10.30."$y".41 
#     sudo ip netns exec H3 ping -c3 10.30."$y".41 
#     sudo ip netns exec H4 ping -c3 10.30."$y".41
#     sudo ip netns exec R1 ping -c3 10.30."$y".41 
#     sudo ip netns exec R2 ping -c3 10.30."$y".41 
#     sudo ip netns exec R3 ping -c3 10.30."$y".41 
#     sudo ip netns exec R4 ping -c3 10.30."$y".41 
#     sudo ip netns exec R5 ping -c3 10.30."$y".41 
#     sudo ip netns exec R6 ping -c3 10.30."$y".41
# done

exit