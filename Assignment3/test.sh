#!/bin/sh

#Q1
echo -e "\nTesting 0->1"
sudo ip netns exec ns0 ping -c10 10.1.2.0
echo -e "Done!!\n"
echo -e "\nTesting 1->0"
sudo ip netns exec ns1 ping -c10 10.1.1.0
echo -e "Done!!\n"

#Q2
echo -e "\nTesting H1->H2"
sudo ip netns exec H1 ping -c10 10.0.20.40
echo -e "Done!!\n"
echo -e "\nTesting H1->H3"
sudo ip netns exec H1 ping -c10 10.0.30.40
echo -e "Done!!\n"

echo -e "\nTesting H2->H1"
sudo ip netns exec H2 ping -c10 10.0.10.40
echo -e "Done!!\n"
echo -e "\nTesting H2->H3"
sudo ip netns exec H2 ping -c10 10.0.30.40
echo -e "Done!!\n"

echo -e "\nTesting H3->H1"
sudo ip netns exec H3 ping -c10 10.0.10.40
echo -e "Done!!\n"
echo -e "\nTesting H3->H2"
sudo ip netns exec H3 ping -c10 10.0.20.40
echo -e "Done!!\n"