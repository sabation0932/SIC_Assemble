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
char char_type(int c_ascii)
{                                       //判斷有幾種ascii類型 有無文字 或空白
    if (c_ascii >= 65 && c_ascii <= 90) // A~Z 大寫
    {
        return 'a'; //alphabet
    }
    else if (c_ascii >= 48 && c_ascii <= 57)
    {
        return 'n'; //number
    }
    else if (c_ascii == 9)
    {
        return 't'; //tab
    }
    else if (c_ascii == 13)
    {
        return 'r'; //return (CR)
    }
    else if (c_ascii == 32)
    {
        return 's'; //space
    }
    else if (c_ascii == 39 || c_ascii== 44)
    {
        return 'm'; //Mark "'" "?"
    }
    else
    {
        cout<<"ERROR!! 有ascii沒被定義 請查看 char_type() 內容"<<endl;
        return '?';
    }
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
    vector<string> col_1;
    vector<string> col_2;
    vector<string> col_3;

public:
    void load_data(void)
    {
        source.open(SOURCE_PATH, ios::in);
        if (source.is_open())
        {
            string line;
            int row_num = 0;
            while (getline(source, line))
            {

                cout << line << endl;
                print_ascii(line);
                cout << endl;
                for (int i = 0; i < line.size(); i++)
                {
                    int split_char_ascii = (int)line.at(i);
                    char type = char_type(split_char_ascii);
                    cout << type;
                    if (type == '?')
                    {
                        cout << line.at(i) << "|" << split_char_ascii << endl;
                    }
                    else
                    {
                        cout << endl;
                    }
                }

                cout << row_num << "------" << endl;
                row_num++;
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