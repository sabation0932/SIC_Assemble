#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#define SOURCE_PATH "../110_Assemble_FinalProject/source.txt"
#define OPCODE_PATH "../110_Assemble_FinalProject/opcode.txt"

using namespace std;

class source_file
{
private:
    fstream source;

public:
    void load_data(void)
    {
        source.open(SOURCE_PATH,ios::in);
        if(source.is_open()){
            string line;
            while (getline(source,line))
            {
                cout<<line<<endl;
            }
            
        }
    }
};

int main()
{
    cout << "--start--" << endl;
    source_file source;
    source.load_data();
    cout << "--End--" << endl;
}