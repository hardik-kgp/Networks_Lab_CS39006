"""
Sriyash Poddar 18CS30040
Networks Lab Assignment-2
"""

import os
import sys
import csv
from xml.etree import ElementTree
from geolite2 import geolite2
from collections import Counter

"""
Requirements:
- install maxminddb-geolite2, using pip installl maxminddb-geolite2

Run:
- python3 parse.py <PATH TO XML FILE>

Procedure:
- Parse the XML file, and extract packets
- Filter the packets, and extract source ips
- Find the country of given ips
- Generate frequency, and dump them in a csv

Tested on:
- Python 3.7.9
- maxminddb-geolite2 2018.703
"""


def extract_packets(PATH):
    """Function to parse XML file, and extract the packets.
    - Reads and parses the XML file using xml package
    - Finds all the package tags
    - Stores HTTP request data of the packet in a dict,
      and adds it to a list

    Args:
        PATH (str): path to XML file

    Returns:
        list(dict(str, str)): list of Dictionaries containing key, value pairs of data
        about the packets
    """
    tree = ElementTree.parse(PATH)
    packets = []

    for item in tree.findall("packet"):
        packet = {}
        for field in item.findall("proto/field"):
            if field.get("name") == "http.request.line":
                temp = field.get("showname").split(": ")
                packet[temp[0]] = temp[1].replace("\\r\\n", "").strip()
        packets.append(packet)
    return packets


def filter_ips(packets):
    """Function to filter packets not using Internet.org proxy,
    and extract source ips.
    - If the request is made through Internet.org's proxy, there is a 'Via' header
    with the given value.
    - As known when there is a proxy, the user ip is not the source/dest ip. To
    get the correct ip, we get the value from "X-Forwarded-For" header.

    Args:
        packets (list): list of Dictionaries containing key, value pairs of data
        about the packets

    Returns:
        list(str): list of unique user ips as captured in the pcap file.
    """
    ips = [packet["X-Forwarded-For"] for packet in packets if "Via" in packet.keys(
    ) and packet["Via"] == "Internet.org" and "X-Forwarded-For" in packet.keys()]
    return set(ips)


def get_country(ip, reader):
    """Function to get country of an ip, using the geolite2 package
    - Used this package as pycountry and python-geoip, are not compatible 
    for python3

    Args:
        ip (str): IP address to find the country
        reader (geolite2.reader): object of the geolite2 reader

    Returns:
        str: country of the given ip address
    """
    try:
        return reader.get(ip)["country"]["names"]["en"]
    except:
        return "Not known"


def get_freq(ips):
    """Function to generate dictionary of country vs freq of ips
    from that country using geolite2 and Counter

    Args:
        ips (list[str]): List of ips after filtering

    Returns:
        dict(str, int): dictionary of country vs freq of ips
    from that country
    """
    reader = geolite2.reader()
    return dict(Counter([get_country(ip, reader) for ip in ips]))


if __name__ == "__main__":
    if len(sys.argv) < 2:
        raise ("Please provide XML file")
    if not os.path.exists(sys.argv[1]):
        raise ("XML file doesnt exist")

    packets = extract_packets(sys.argv[1])
    freq = get_freq(filter_ips(packets))
    with open("data.csv", "w") as fout:
        writer = csv.writer(fout, delimiter=" ")
        for k, v in freq.items():
            writer.writerow([k.upper(), v])
        print(f"The output is written in {fout.name}")
