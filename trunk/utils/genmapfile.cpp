#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

int main(char argc, char** argv)
{
    if(argc != 3)
    {
        cout << "Usage: " << argv[0] << " <ld-mapfile> <output>" << endl;
        return 1;
    }
    
    ifstream infile(argv[1]);
    string temp;
    
    fstream outfile(argv[2], ios::out | ios::binary);
    
    while(getline(infile, temp))
    {
        //check if line begins with spaces
        if(temp.find("                ") == 0)
        {
            //cut spaces and "0x"
            temp = temp.substr(18);
            
            //then comes the address and again 16 spaces
            if(temp.substr(8, 16) == "                ")
            {
                string addressStr = temp.substr(0, 8);
                unsigned int address;
                sscanf(addressStr.c_str(), "%x", &address);
                string name = temp.substr(24);
                
                cout << "Found symbol '" << name << "' at 0x" << setbase(16) << address << endl;
                
                outfile.write((char *)&address, sizeof(address));
                outfile.write(name.c_str(), name.length());
                char c = 0;
                outfile.write(&c, sizeof(char));
            }
        }
    }
    
    unsigned int end = 0;
    outfile.write((char *)&end, sizeof(end));
    
    infile.close();
    outfile.close();
        
    return 0;
}
