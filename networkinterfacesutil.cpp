
#include <sys/types.h>

//C
#include <cstdio>
#include <cstdlib>
#include <cstring>

//C++
#include <array>
#include <mutex>
#include <thread>
#include <iostream>

//POSIX
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <arpa/inet.h>

//LIBRARY HEADERS
#include "networkinterfacesutil.h"

Services::NetworkInterface::NetworkInterface(ifaddrs *ifa):
    family(ifa->ifa_addr->sa_family),
    familyString((ifa->ifa_addr->sa_family == AF_INET) ? std::string{"IPV4"} : (ifa->ifa_addr->sa_family == AF_INET6) ? std::string{"IPV6"} : std::string{"???"}),
    interfaceName(std::string{ifa->ifa_name}),
    scklen(ifa->ifa_addr->sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6))
{

    auto host       = std::array<char, NI_MAXHOST>{};
    auto hostPtr    = host.data();
    getnameinfo(ifa->ifa_addr, scklen, hostPtr, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);

    ip              = std::string{hostPtr};

    memcpy(&m_ifa, ifa, sizeof(ifaddrs));

    if (ifa->ifa_addr) {
        std::memcpy(&sckadd, ifa->ifa_addr, sizeof(sockaddr));
        m_pIfa->ifa_addr = &sckadd;
     } else {
        std::memset(&sckadd, 0, sizeof(sockaddr));
        m_pIfa -> ifa_addr = nullptr;
    }

    if (ifa->ifa_netmask) {
        std::memcpy(&netmsk, ifa->ifa_netmask, sizeof(sockaddr));
        m_pIfa->ifa_netmask = &netmsk;
    } else {
        std::memset(&netmsk, 0, sizeof(sockaddr));
        m_pIfa -> ifa_netmask = nullptr;
    }

    if (ifa->ifa_dstaddr) {
        std::memcpy(&dstadr, ifa->ifa_dstaddr, sizeof(sockaddr));
    } else {
        std::memset(&dstadr, 0, sizeof(sockaddr));
        m_pIfa -> ifa_dstaddr = nullptr;
    }

    m_pIfa -> ifa_next = nullptr;
    m_pIfa -> ifa_name = interfaceName.data();

}


const std::map <std::string, Services::NetworkInterface::NetworkInterfaceMap> Services::NetworkInterface::GetNetworkInterfacesMap() {

    std::map<std::string, Services::NetworkInterface::NetworkInterfaceMap > networkInterfacesMap;

    ifaddrs *ptrIfaddr;
    ifaddrs *ifa;
    int n, family, s;

    if (getifaddrs(&ptrIfaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for (ifa = ptrIfaddr; ifa != NULL; ifa = ifa->ifa_next) {

        if (ifa->ifa_addr == NULL)
            continue;

        auto networkInterface = Services::NetworkInterface(ifa);
        networkInterfacesMap[networkInterface.interfaceName][networkInterface.familyString] = networkInterface;

    }
    freeifaddrs(ptrIfaddr);

    return networkInterfacesMap;

}
void Services::NetworkInterface::DisplayInterfaces() {

    auto networkInterfacesMap = Services::NetworkInterface::GetNetworkInterfacesMap();
    for (auto &interfaceName_interfaceMap : networkInterfacesMap){

        auto& interfaceName = interfaceName_interfaceMap.first;
        auto& interfaceMap  = interfaceName_interfaceMap.second;
        auto lineSize       = (interfaceName + std::string{"Interface: "}).size();

        std::cout << "Interface: " << interfaceName << "\n" << std::string(lineSize, '=') << "\n";
        for (auto& ipfamily_interface : interfaceMap ){

            auto& interface = ipfamily_interface.second;

            std::cout << "\n\tFamily: " << interface.familyString;
            std::cout << "\n\tIp add: " << interface.ip << "\n\n";

        }

    }
}

std::tuple<bool, Services::NetworkInterface> Services::NetworkInterface::IsAValidIp(std::string ipUt) {

    auto networkInterfacesMap = Services::NetworkInterface::GetNetworkInterfacesMap();
    for ( auto& interfaceName_interfaceMap : networkInterfacesMap ) {

        auto& interfaceName = interfaceName_interfaceMap.first;
        auto& interfaceMap  = interfaceName_interfaceMap.second;
        for ( auto& ipfamiy_interface : interfaceMap) {
            auto interface = ipfamiy_interface.second;
            if (interface.ip == ipUt) return std::make_tuple(true, interface);
        }
    }
    return std::make_tuple(false, NetworkInterface());
}



