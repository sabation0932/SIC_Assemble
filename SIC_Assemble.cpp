#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#define SOURCE_PATH "../110_Assemble_FinalProject/source.txt"
#define OPCODE_PATH "../110_Assemble_FinalProject/opcode.txt"
using namespace std;
void print_ascii(string _line)
{
    for (size_t i = 0; i < _line.size(); i++)
    {

        int int_char = _line.at(i);
        cout << i << ":" << int_char << " ";
    }
    cout << endl;
}

class file_operate
{
private:
    fstream file;
    string path;

public:
    void load_file(fstream _file, string _path)
    {
        file.open(path, ios::in);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                cout << line << endl;
                print_ascii(line);
            }
        }
    }
};

class source_file
{
private:
    fstream source;

public:
    void load_data(void)
    {
        source.open(SOURCE_PATH, ios::in);
        if (source.is_open())
        {
            string line;
            while (getline(source, line))
            {
                cout << line << endl;
                print_ascii(line);
            }
        }
        else
        {
            cout << "here is no source file" << endl;
        }
    }
};

class opcode_file
{
private:
    fstream opcode;

public:
    void load_data(void)
    {
        opcode.open(OPCODE_PATH, ios::in);
        if (opcode.is_open())
        {
            string line;
            while (getline(opcode, line))
            {
                cout << line << endl;
                print_ascii(line);
            }
        }
        else
        {
            cout << "here is no opcode file" << endl;
        }
    }
};

int main()
{
    cout << "--start--" << endl;
    source_file source;
    source.load_data();
    cout << "------" << endl;
    opcode_file opcode;
    // opcode.load_data();
    cout << "--End--" << endl;
}