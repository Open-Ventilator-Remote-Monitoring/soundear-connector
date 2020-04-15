#include "helpers.hpp"

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <cstdio>
#include <iostream>
#include <sstream>

Helpers::Helpers()
{

}

/*bool Helpers::GetIpAddress(std::string nicName, uint8_t *ip)
{
    std::string test("iface ");
    test=test.append(nicName);

    int indexOfNic=0;
    std::string nicContent;
    if (FindInFile(_nicConfigPath, test, &indexOfNic, nicContent))
    {

    }
    return false;
}*/

bool Helpers::GetDhcpEnabled(std::string nicName)
{
    std::string test("iface ");
    test=test.append(nicName); test= test.append(" inet dhcp");

    int unused=0;
    std::string unusedContent;
    bool isFound = FindInFile(_nicConfigPath, test, &unused, unusedContent);
    (void)unused; (void) unusedContent;

    return isFound;
}

bool Helpers::FindInFile(std::string file, std::string item, int *sof, std::string& contents)
{
    std::ifstream iffile(file);
    std::string contentslocal((std::istreambuf_iterator<char>(iffile)),
                           std::istreambuf_iterator<char>());

    contents = contentslocal;
    return IndexOf(contentslocal,item,sof);
}

bool Helpers::IndexOf(std::string source, std::string item, int* sof)
{
    std::size_t start = source.find(item);
    if (start!=std::string::npos)
    {
        *sof = start;
        return true;
    }
    else
    {
        return false;
    }
}

bool Helpers::EqualFiles(std::string fileA, std::string fileB)
{
  std::ifstream streamA(fileA);
  std::string file1((std::istreambuf_iterator<char>(streamA)),
                    std::istreambuf_iterator<char>());

  std::ifstream streamB = std::ifstream(fileB);
  std::string file2((std::istreambuf_iterator<char>(streamB)),
                    std::istreambuf_iterator<char>());

  streamA.close();
  streamB.close();

  return file1 == file2;
}

void Helpers::ReplaceFile(std::string fileSource, std::string fileDest)
{
    std::ifstream streamA(fileSource);
    std::string file1((std::istreambuf_iterator<char>(streamA)),
                      std::istreambuf_iterator<char>());

    streamA.close();
    std::ofstream dest(fileDest);
    dest << file1;
    dest.close();
}

std::string Helpers::Exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

bool Helpers::Replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool Helpers::FindEthSection(std::string interface, int* sofWlan, int* lengthWlan)
{
    std::size_t start = interface.find("# Wired or wireless interfaces");
    std::size_t end = interface.find("# Ethernet/RNDIS gadget (g_ether)");
    if (start!=std::string::npos && end!=std::string::npos)
    {
        *sofWlan = start;
        *lengthWlan = (int)(end-start);
        return true;
    }
    else
    {
        return false;
    }
}

bool Helpers::FindWlanSection(std::string interface, int* sofWlan, int* lengthWlan)
{
    std::size_t start = interface.find("# Wireless interfaces");
    std::size_t end = interface.find("# Wired or wireless interfaces");
    if (start!=std::string::npos && end!=std::string::npos)
    {
        *sofWlan = start;
        *lengthWlan = (int)(end-start);
        return true;
    }
    else
    {
        return false;
    }
}

void Helpers::UpdateModem(Se3::UsbProtocol::ConnectivityConfig config)
{
    std::cout << "\r\nUpdating wlan...";
    std::string wvdial =
"[Dialer defaults]\n\
Modem = /dev/ttyUSB1\n\
Baud = 115200\n\
Init1 = ATZ\n\
Init2 = AT+CGDCONT=1,\"IP\",\"[APN]\"\n\
Phone = *99#\n\
Username = admin\n\
Password = admin\n\
Dial Command = ATD\n\
Modem Type = Analog Modem\n\
NEW PPPD = yes\n\
Stupid Mode = yes\n\
Auto DNS = off\n\
dns1 = 8.8.8.8\n\
dns2 = 8.8.4.4\n\
\n\
[Dialer pin]\n";

    Replace(wvdial,"[APN]", config.APN);

    std::ofstream wpafile(tmpWvdialFilename);
    wpafile << wvdial;
    wpafile.close();

}

void Helpers::UpdateWLAN(Se3::UsbProtocol::ConnectivityConfig config)
{
    //update the file /etc/wpa_supplicant.conf
    std::cout << "\r\nUpdating wlan...";
    std::string wpa =
"ctrl_interface=/var/run/wpa_supplicant\n\
ctrl_interface_group=0\n\
update_config=1\n\
\n\
network={\n\
   ssid=\"[SSID]\"\n\
   psk=\"[PASS]\"\n\
}\n";

    Replace(wpa,"[SSID]", config.SSID);
    Replace(wpa,"[PASS]", config.Password);

    std::ofstream wpafile(tmpWpaFilename);
    wpafile << wpa;
    wpafile.close();


    //update the file /etc/network/interfaces

    std::ifstream iffile(ifFilename);
    std::string interfaces((std::istreambuf_iterator<char>(iffile)),
                           std::istreambuf_iterator<char>());

    if (config.Dhcp)
    {
        std::string ifcfg=
"# Wireless interfaces\n\
iface wlan0 inet dhcp\n\
     wireless_mode managed\n\
     wireless_essid any\n\
     wpa-driver wext\n\
     wpa-conf /etc/wpa_supplicant.conf\n";

        int sofWlan=0;
        int lengthWlan=0;
        if (FindWlanSection(interfaces,&sofWlan,&lengthWlan))
        {
            interfaces.replace(sofWlan,lengthWlan,ifcfg);
            std::ofstream iffile(tmpIfFilename);
            iffile << interfaces;
            iffile.close();
        }
    }
    else
    {
        std::stringstream ifcfg(
"# Wireless interfaces\n\
iface wlan0 inet static\n\
     wireless_mode managed\n\
     wireless_essid any\n\
     wpa-driver wext\n\
     wpa-conf /etc/wpa_supplicant.conf", std::ios_base::app | std::ios_base::out);

        ifcfg << "\n     address " << (int)config.Ip[0]<<"."<< (int)config.Ip[1]<<"."<< (int)config.Ip[2]<<"."<< (int)config.Ip[3];
        ifcfg << "\n     netmask " << (int)config.Sub[0]<<"."<< (int)config.Sub[1]<<"."<< (int)config.Sub[2]<<"."<< (int)config.Sub[3];
        ifcfg << "\n     gateway " << (int)config.Gw[0]<<"."<< (int)config.Gw[1]<<"."<< (int)config.Gw[2]<<"."<< (int)config.Gw[3] << "\n";

        int sofWlan=0;
        int lengthWlan=0;
        if (FindWlanSection(interfaces,&sofWlan,&lengthWlan))
        {
            interfaces.replace(sofWlan,lengthWlan,ifcfg.str());
            std::ofstream iffile(tmpIfFilename);
            iffile << interfaces;
            iffile.close();
        }
    }
}

void Helpers::UpdateEth(Se3::UsbProtocol::ConnectivityConfig config)
{
    std::cout << "\r\nUpdating ethernet...";
    std::ifstream iffile(ifFilename);
    std::string interfaces((std::istreambuf_iterator<char>(iffile)),
                           std::istreambuf_iterator<char>());

    if (config.Dhcp)
    {
        std::string ifcfg=
"# Wired or wireless interfaces\n\
auto eth0\n\
iface eth0 inet dhcp\n";

        int sofWlan=0;
        int lengthWlan=0;
        if (FindEthSection(interfaces,&sofWlan,&lengthWlan))
        {
            interfaces.replace(sofWlan,lengthWlan,ifcfg);
            std::ofstream iffile(tmpIfFilename);
            iffile << interfaces;
            iffile.close();
        }
    }
    else
    {
        std::stringstream ifcfg(
"# Wired or wireless interfaces\n\
auto eth0\n\
iface eth0 inet static\n", std::ios_base::app | std::ios_base::out);

        ifcfg << "     address " << (int)config.Ip[0]<<"."<< (int)config.Ip[1]<<"."<< (int)config.Ip[2]<<"."<< (int)config.Ip[3] << "\n";
        ifcfg << "     netmask " << (int)config.Sub[0]<<"."<< (int)config.Sub[1]<<"."<< (int)config.Sub[2]<<"."<< (int)config.Sub[3] << "\n";
        ifcfg << "     gateway " << (int)config.Gw[0]<<"."<< (int)config.Gw[1]<<"."<< (int)config.Gw[2]<<"."<< (int)config.Gw[3] << "\n";

        int sofWlan=0;
        int lengthWlan=0;
        if (FindEthSection(interfaces,&sofWlan,&lengthWlan))
        {
            interfaces.replace(sofWlan,lengthWlan,ifcfg.str());
            std::ofstream iffile(tmpIfFilename);
            iffile << interfaces;
            iffile.close();
        }
    }
}
