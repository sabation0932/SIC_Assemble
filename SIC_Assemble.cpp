#include <iostream>
#include <vector>
#include <fstream>
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
        cout << "load_data" << endl;
    }
};

int main()
{
    cout << "--start--" << endl;
    source_file source;
    source.load_data();
    cout << "--End--" << endl;
}