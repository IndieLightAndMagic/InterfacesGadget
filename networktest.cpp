

#include <iostream>
#include "networkinterfacesutil.h"


int main(int argc, char ** argv) {


    if (argc > 2){

        Services::NetworkInterface::DisplayInterfaces();

        std::cout << "Search Interfaces Map: " << argv[1] << "\n";

        //auto ifacesMap = Services::NetworkInterface::GetInterfacesMap(std::string{argv[2]});
        //auto itFound = ifacesMap.find(std::string{argv[1]});
        //auto found = itFound != ifacesMap.end();

        /*if (!found){

            std::cout << "\t -- Not Found -- \n";
            std::cout << "\t Availale interfaces are: \n";
            for (auto i : ifacesMap) {

                std::cout << "\t " << i.first << "\n";
            }

        } else {

            std::cout << "\t ---- Found ---- \n";
            std::cout << "\t IP: " << itFound->second.ip << "\n";

        }*/


    } else {

        std::cout << "Usage : networktest [ifacename ^ ip] [family]\n\tifacename : Interfacename eg. lo0, en0, bridge100\n\tfamily : The IP version family eg. IPV4, IPV6\n";

    }

    return 0;
}