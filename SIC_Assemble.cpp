#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#define SOURCE_PATH "../110_Assemble_FinalProject/source.txt"
#define OPCODE_PATH "../110_Assemble_FinalProject/opcode.txt"

#define code_max_col 3 // 0,1,2,3 總共有4欄
#define loc_col 0      // location counter 定義在0
using namespace std;
void print_ascii(string _line)
{
    for (size_t i = 0; i < _line.size(); i++)
    {

        int int_char = _line.at(i);
        cout << i << ":" << int_char << " ";
    }
    cout << "  "
         << "size = " << _line.size() << endl;
}
char char_type(int c_ascii)
{                                       //判斷有幾種ascii類型 有無文字 或空白
    if (c_ascii >= 65 && c_ascii <= 90) // A~Z 大寫
    {
        return 'a'; // alphabet
    }
    else if (c_ascii >= 48 && c_ascii <= 57)
    {
        return 'n'; // number
    }
    else if (c_ascii == 9)
    {
        return 't'; // tab
    }
    else if (c_ascii == 13)
    {
        return 'r'; // return (CR)
    }
    else if (c_ascii == 32)
    {
        return 's'; // space
    }
    else if (c_ascii == 39 || c_ascii == 44)
    {
        return 'm'; // Mark "'" "?"
    }
    else
    {
        cout << "ERROR!! 有ascii沒被定義 請查看 char_type() 內容" << endl;
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
    vector<int> Loc;
    vector<string> col_1;
    vector<string> col_2;
    vector<string> col_3;

public:
    void print_all_col(void)
    {
        for (int i = 0; i < col_1.size(); i++)
        {
            cout << i << "\t";
            cout << col_1[i] << "\t";
            cout << col_2[i] << "\t";
            cout << col_3[i] << endl;
        }
    }
    void fetch_data(int _col_number, string _message)
    {
        cout << "[" << _col_number << "]"
             << "message:" << _message << endl;
        switch (_col_number)
        {
        case 1:
            col_1.push_back(_message);
            break;
        case 2:
            col_2.push_back(_message);
            break;
        case 3:
            col_3.push_back(_message);
            break;

        default:
            cout << "ERROR:here is no define col: " << _col_number << endl;
            break;
        }
    }

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
                string temp_string = "";
                int col_selector = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    int split_char_ascii = (int)line.at(i);
                    char type = char_type(split_char_ascii);

                    if (type == 'a' || type == 'n' || type == 'm') // 如果遇到文字、數字、符號 添加字元到暫存string
                    {
                        temp_string.push_back(line.at(i));
                        // cout << type << " " << line.at(i) << "|" << temp_string << endl;
                    }
                    else if (type == 's' || type == 't' || type == 'r' || i == line.size() - 1) //如果遇到空白 移動 或最後一個字元
                    {
                        col_selector++;
                        // cout << type << ": to col " << col_selector << endl;

                        fetch_data(col_selector, temp_string);
                        temp_string = "";
                        if (col_selector == 2 && i == line.size() - 1)
                        {
                            cout << "here need send message " << endl;
                            fetch_data(++col_selector, temp_string);
                        }
                        if (col_selector > code_max_col || type == 'r') //如果超過第4欄（0,1,2,3） 或遇到換行
                        {
                            // cout << "reset col_selector:" << type << " at " << i << endl;
                            col_selector = 1;
                        }
                    }
                    else
                    {
                        cout << "out control: " << type << endl;
                    }
                }
                if (temp_string != "")
                {
                    fetch_data(++col_selector, temp_string);
                    col_selector = 0;
                    temp_string = "";
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
    source.print_all_col();
    opcode_file opcode;
    // opcode.load_data();
    cout << "--End--" << endl;
}