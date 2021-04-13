#!/bin/sh

####################################
#### Sriyash Poddar | 18CS30040 ####
#### Assignment 4 Part C        ####
####################################

# creating namespaces
# command: sudo ip netns add <name of namespace>
sudo ip netns add N1
sudo ip netns add N2
sudo ip netns add N3
sudo ip netns add N4
sudo ip netns add N5
sudo ip netns add N6

# creating v-eth to connect the namspaces
# command: sudo ip link add <veth-name> type veth peer name <veth-peername>
sudo ip link add V1 type veth peer name V2
sudo ip link add V3 type veth peer name V4
sudo ip link add V5 type veth peer name V6
sudo ip link add V7 type veth peer name V8
sudo ip link add V9 type veth peer name V10
sudo ip link add V11 type veth peer name V12


# assigning interfaces to the namespaces
# command: sudo ip link set <veth-name> netns <namespace>
sudo ip link set V1 netns N1
sudo ip link set V12 netns N1

sudo ip link set V2 netns N2
sudo ip link set V3 netns N2

sudo ip link set V4 netns N3
sudo ip link set V5 netns N3

sudo ip link set V6 netns N4
sudo ip link set V7 netns N4

sudo ip link set V8 netns N5
sudo ip link set V9 netns N5

sudo ip link set V10 netns N6
sudo ip link set V11 netns N6

# adding ips to the interfaces, bringing them up, and setting routes
# command: sudo ip -n <namespace> addr add <ip-address/subnet> dev <veth-name>
sudo ip -n N1 addr add 10.0.10.40/24 dev V1
sudo ip -n N1 addr add 10.0.60.41/24 dev V12

sudo ip -n N2 addr add 10.0.10.41/24 dev V2
sudo ip -n N2 addr add 10.0.20.40/24 dev V3

sudo ip -n N3 addr add 10.0.20.41/24 dev V4
sudo ip -n N3 addr add 10.0.30.40/24 dev V5

sudo ip -n N4 addr add 10.0.30.41/24 dev V6
sudo ip -n N4 addr add 10.0.40.40/24 dev V7

sudo ip -n N5 addr add 10.0.40.41/24 dev V8
sudo ip -n N5 addr add 10.0.50.40/24 dev V9

sudo ip -n N6 addr add 10.0.50.41/24 dev V10
sudo ip -n N6 addr add 10.0.60.40/24 dev V11

# bringing veth, bridges and lo up
# command: sudo ip -n <namespace> link set dev <veth-name> up
sudo ip -n N1 link set dev V1 up
sudo ip -n N1 link set dev V12 up
sudo ip -n N2 link set dev V2 up
sudo ip -n N2 link set dev V3 up
sudo ip -n N3 link set dev V4 up
sudo ip -n N3 link set dev V5 up
sudo ip -n N4 link set dev V6 up
sudo ip -n N4 link set dev V7 up
sudo ip -n N5 link set dev V8 up
sudo ip -n N5 link set dev V9 up
sudo ip -n N6 link set dev V10 up
sudo ip -n N6 link set dev V11 up

sudo ip -n N1 link set dev lo up
sudo ip -n N2 link set dev lo up
sudo ip -n N3 link set dev lo up
sudo ip -n N4 link set dev lo up
sudo ip -n N5 link set dev lo up
sudo ip -n N6 link set dev lo up

# adding routes
# command: sudo ip netns exec <namespace> route add <dest-ip> via <route-ip> dev <veth-name>
sudo ip -n N1 route add 10.0.20.0/24 via 10.0.10.41 dev V1
sudo ip -n N1 route add 10.0.30.0/24 via 10.0.10.41 dev V1
sudo ip -n N1 route add 10.0.40.0/24 via 10.0.10.41 dev V1
sudo ip -n N1 route add 10.0.50.0/24 via 10.0.10.41 dev V1

sudo ip -n N2 route add 10.0.30.0/24 via 10.0.20.41 dev V3
sudo ip -n N2 route add 10.0.40.0/24 via 10.0.20.41 dev V3
sudo ip -n N2 route add 10.0.50.0/24 via 10.0.20.41 dev V3
sudo ip -n N2 route add 10.0.60.0/24 via 10.0.20.41 dev V3

sudo ip -n N3 route add 10.0.10.0/24 via 10.0.30.41 dev V5
sudo ip -n N3 route add 10.0.40.0/24 via 10.0.30.41 dev V5
sudo ip -n N3 route add 10.0.50.0/24 via 10.0.30.41 dev V5
sudo ip -n N3 route add 10.0.60.0/24 via 10.0.30.41 dev V5

sudo ip -n N4 route add 10.0.10.0/24 via 10.0.40.41 dev V7
sudo ip -n N4 route add 10.0.20.0/24 via 10.0.40.41 dev V7
sudo ip -n N4 route add 10.0.50.0/24 via 10.0.40.41 dev V7
sudo ip -n N4 route add 10.0.60.0/24 via 10.0.40.41 dev V7

sudo ip -n N5 route add 10.0.10.0/24 via 10.0.50.41 dev V9
sudo ip -n N5 route add 10.0.20.0/24 via 10.0.50.41 dev V9
sudo ip -n N5 route add 10.0.30.0/24 via 10.0.50.41 dev V9
sudo ip -n N5 route add 10.0.60.0/24 via 10.0.50.41 dev V9

sudo ip -n N6 route add 10.0.10.0/24 via 10.0.60.41 dev V11
sudo ip -n N6 route add 10.0.20.0/24 via 10.0.60.41 dev V11
sudo ip -n N6 route add 10.0.30.0/24 via 10.0.60.41 dev V11
sudo ip -n N6 route add 10.0.40.0/24 via 10.0.60.41 dev V11

# ip forwarding
# command: sudo ip netns exec <namespace> sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N1 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N2 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N3 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N4 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N5 sysctl -w net.ipv4.ip_forward=1
sudo ip netns exec N6 sysctl -w net.ipv4.ip_forward=1

# trace routes
sudo ip netns exec N1 traceroute 10.0.50.40
sleep 3
sudo ip netns exec N3 traceroute 10.0.50.40
sleep 3
sudo ip netns exec N3 traceroute 10.0.10.40
sleep 3

# ping test
for x in {1..6}
do
	y=$((x*10))
	sudo ip netns exec N1 ping -c3 10.0."$y".40 
	sudo ip netns exec N2 ping -c3 10.0."$y".40 
	sudo ip netns exec N3 ping -c3 10.0."$y".40 
	sudo ip netns exec N4 ping -c3 10.0."$y".40 
	sudo ip netns exec N5 ping -c3 10.0."$y".40 
	sudo ip netns exec N6 ping -c3 10.0."$y".40
	sudo ip netns exec N1 ping -c3 10.0."$y".41 
	sudo ip netns exec N2 ping -c3 10.0."$y".41 
	sudo ip netns exec N3 ping -c3 10.0."$y".41 
	sudo ip netns exec N4 ping -c3 10.0."$y".41 
	sudo ip netns exec N5 ping -c3 10.0."$y".41 
	sudo ip netns exec N6 ping -c3 10.0."$y".41
done

exit