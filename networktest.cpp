

#include <iostream>
#include "networkinterfacesutil.h"


int main(int argc, char ** argv) {


    if (argc > 2){


        std::cout << "Search Interfaces Map: " << argv[1] << "\n";
        auto ifaceKey   = std::string{argv[1]};
        auto familyKey  = std::string{argv[2]};
        auto ifacesMap = Services::NetworkInterface::GetNetworkInterfacesMap();
        auto found = (ifacesMap.find(std::string{ifaceKey})!=ifacesMap.end()) && (ifacesMap[ifaceKey].find(familyKey) != ifacesMap[ifaceKey].end());

        if (!found){
            std::cout << "\t -- Not Found -- \n";
            std::cout << "\t Availale interfaces are: \n";
            Services::NetworkInterface::DisplayInterfaces();

        } else {



            std::cout << "\t ---- Found ---- \n";
            std::cout << "\t IP: " << ifacesMap[ifaceKey][familyKey].ip << "\n";

        }


    } else {

        std::cout << "Usage : networktest [ifacename ^ ip] [family]\n\tifacename : Interfacename eg. lo0, en0, bridge100\n\tfamily : The IP version family eg. IPV4, IPV6\n";

    }

    return 0;
}