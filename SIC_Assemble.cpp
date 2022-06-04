#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>

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

string dec_to_hex(int _dec_value)
{
    stringstream ss;
    ss << hex << _dec_value; // int decimal_value
    string res(ss.str());
    // cout << "dec = " << _dec_value << " hex = " << res << endl;
    return res;
}

int hex_to_dec(string _hex_value)
{
    int dec_value;
    stringstream ss;
    ss << _hex_value;       // std::string hex_value
    ss >> hex >> dec_value; // int decimal_value
    // cout << "hex = " << _hex_value << " dec = " << dec_value << endl;
    return dec_value;
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
public:
    fstream source;
    vector<string> Loc;
    vector<string> col_1;
    vector<string> col_2;
    vector<string> col_3;

    vector<string> label_name;
    vector<string> label_address;

    string the_loc_of(string _label)
    {
        for (size_t i = 0; i < label_name.size(); i++)
        {
            if (label_name[i] == _label)
            {
                cout << "label: " << label_name[i] << " -> " << label_address[i] << endl;
                return label_address[i];
            }
        }
        cout << "here is no label " << _label << "please check symbol table" << endl;
        return "";
    };

    void print_all_col(void)
    {
        for (int i = 0; i < col_1.size(); i++)
        {
            cout << i << "\t";
            cout << Loc[i] << "\t";
            cout << col_1[i] << "\t";
            cout << col_2[i] << "\t";
            cout << col_3[i] << endl;
        }
    }
    void print_loc(void)
    {
        for (int i = 0; i < Loc.size(); i++)
        {
            cout << i << "\t";
            cout << Loc[i] << endl;
        }
    }
    string loc_counter(string _start_hex, int add_num)
    {
        static string start_hex = _start_hex;
        static string hex_num = start_hex;
        int dec_num = hex_to_dec(hex_num);
        dec_num = dec_num + add_num;
        hex_num = dec_to_hex(dec_num);
        return hex_num;
    }
    int find_BYTE_size(string _message)
    {
        // string find ' position in string
        int size = 0;
        bool open_flag = false;
        if (_message.find("EOF") != std::string::npos) // 如果是EOF size = 3
        {
            size = 3;
            // cout << "here is EOF size = " << size << endl;
            return size; //"EOF佔3個BYTE"
        }
        for (int i = 0; i < _message.size(); i++)
        {
            if (_message.at(i) == '\'')
            {
                // cout << "find ' at " << i << endl;
                open_flag = !open_flag; //反轉狀態
            }
            if (open_flag == true)
            {
                size++;
            }
        }
        size = size - 1; //第一個 ' 會算進去 要扣掉;
        size = size / 2; //兩個16進位 組成一個BYTE
        // cout << "size is " << size << endl;

        return size;
    }

    void loc_count_fetch()
    {
        static bool start_is_find = false;
        static string start_num;
        for (size_t i = 0; i < col_1.size(); i++) //讀取整裡過後的 Source Statement
        {
            if (col_2[i] == "START")
            {
                start_is_find = true;
                start_num = col_3[i];
                Loc.push_back(loc_counter(start_num, 0)); // START 那一行
                Loc.push_back(loc_counter(start_num, 0)); // START  的後一行 範例是 FIRST
            }

            else if (col_2[i] == "BYTE") //找到BYTE
            {
                if (start_is_find == false) //在找到BYTE 前沒有找到 START 則失敗，意味需要以START作為開頭。
                {
                    cout << "lose \"START\" position please check the source file(.txt)" << endl;
                    return exit(0);
                }
                // location counter + BYTE大小
                // cout << "find BYTE at " << i << endl;
                string temp = col_3[i];
                int size = find_BYTE_size(temp);
                Loc.push_back(loc_counter(start_num, size));
            }
            else if (col_2[i] == "RESB")
            {
                cout << "find RESB at " << i << endl;
                Loc.push_back(loc_counter(start_num, atoi(col_3[i].c_str())));
            }

            else
            {
                Loc.push_back(loc_counter(start_num, 3));
            }
        }
    }

    void fetch_data(int _col_number, string _message)
    {
        // cout << "[" << _col_number << "]"<< "message:" << _message << endl;
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

                // cout << line << endl;
                // print_ascii(line);
                // cout << endl;
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
                            // cout << "here need send message " << endl;
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

                // cout << row_num << "------" << endl;
                row_num++;
            }
        }
        else
        {
            cout << "here is no source file" << endl;
        }
    }

    void print_symbol_table()
    {
        for (size_t i = 0; i < label_name.size(); i++)
        {
            cout << label_name[i] << "\t" << label_address[i] << endl;
        }
    }

    void symbol_table()
    {
        for (size_t i = 0; i < col_1.size(); i++)
        {
            if (col_1[i] != "" && i != 0)
            {
                // cout << col_1[i] << " \t " << Loc[i] << endl;
                label_name.push_back(col_1[i]);
                label_address.push_back(Loc[i]);
            }
        }
    }
};

class opcode_file
{
public:
    fstream opcode;
    vector<string> label_name;
    vector<string> address;

    string the_address_of(string _statement)
    { //輸入OPCODE 返還 address
        for (size_t i = 0; i < label_name.size(); i++)
        {
            if (label_name[i] == _statement)
            {
                cout << "op_code: " << label_name[i] << " -> " << address[i] << endl;
                return address[i];
            }
        }
        cout << "here is wrong op_code: " << _statement << " please check source file(.txt)" << endl;
        return "";
    }

    void load_data(void)
    {
        opcode.open(OPCODE_PATH, ios::in);
        if (opcode.is_open())
        {
            string line;
            int row_num = 0;
            while (getline(opcode, line))
            {
                string temp_string = "";
                int col_selector = 0;

                for (size_t i = 0; i < line.size(); i++)
                {
                    int split_char_ascii = (int)line.at(i);
                    char type = char_type(split_char_ascii);
                    if (type != 's')
                    {
                        temp_string.push_back(line.at(i));
                    }
                    else if (type == 's' || type == 'r')
                    {
                        //遇到空白 發送字串給vector
                        fetch_data(col_selector, temp_string);
                        temp_string = "";
                        col_selector++;
                    }
                }
                if (temp_string != "")
                {
                    fetch_data(col_selector, temp_string);
                    col_selector = 0;
                    temp_string = "";
                }
                row_num++;
            }
        }
        else
        {
            cout << "here is no opcode file" << endl;
        }
    }
    void print_all_col()
    {
        for (size_t i = 0; i < label_name.size(); i++)
        {
            cout << i << " " << label_name[i] << "\t" << address[i] << endl;
        }
    }
    void fetch_data(int _col_number, string _message)
    {
        switch (_col_number)
        {
        case 0:
            label_name.push_back(_message);
            break;
        case 1:
            address.push_back(_message);
            break;

        default:
            cout << "ERROR:here is no define col: " << _col_number << " at OPcode" << endl;
            break;
        }
    }
};
class object_code : public source_file, public opcode_file //繼承自source,opcode
{
public:
    vector<string> Obj_code;
    void generate(source_file &_source, opcode_file &_opcode) //生成object_code
    {
        for (size_t i = 0; i < _source.col_2.size(); i++)
        {
            if (_source.col_2[i] == "START")i++;
            
            // cout<<_source.col_2[i]<<_source.col_3[i]<<endl;

            string temp_string_opcode = _opcode.the_address_of(_source.col_2[i]);
            string temp_string_symbol_table = _source.the_loc_of(_source.col_3[i]);
            // if (temp_string_opcode != "" && temp_string_symbol_table != "") //有抓取到值
            // {
            //     cout << temp_string_opcode << temp_string_symbol_table << endl;
            // }
            // else
            // {
            //     cout << "can't find the OPcode" << endl;
            //     return;
            // }
        }
    }
};

int main()
{
    cout << "--start--" << endl;
    source_file source;
    source.load_data();
    cout << "------" << endl;
    cout << "--loc-count--" << endl;
    source.loc_count_fetch();
    // source.print_all_col();
    cout << "---symbol-table--" << endl;
    source.symbol_table();
    // source.print_symbol_table();

    cout << "--op-code--" << endl;
    opcode_file opcode;
    opcode.load_data();
    // opcode.print_all_col();
    cout << "--count-object-code" << endl;
    object_code object;
    object.generate(source, opcode);

    cout << "--End--" << endl;
}