#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>

#define SOURCE_PATH "SICXEsource.txt"
#define OPCODE_PATH "opcode.txt"

#define readable_mode true
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
    else if (c_ascii == int('\'') || c_ascii == int('?') || c_ascii == int('#') || c_ascii == int('+') || c_ascii == int('@') || c_ascii == int(','))
    {
        return 'm'; // Mark ' ? # + @ ,
    }
    else
    {
        cout << "ERROR!! " << char(c_ascii) << " ascii = " << c_ascii << " 沒被定義 請查看 char_type() 內容" << endl;
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
string hex_add(string _hex1, string _hex2)
{
    string temp = dec_to_hex(hex_to_dec(_hex1) + hex_to_dec(_hex2));
    // cout << "hex1 = " << _hex1 << " hex2 = " << _hex2 << " add: " << temp << endl;
    return temp;
}
string add_lecture(string _message, char _sign, int _length, string back_or_front)
{
    stringstream ss;
    if (back_or_front == "back")
    {
        while (_message.size() < _length)
        {
            _message.push_back(_sign);
        }
    }
    else if (back_or_front == "front")
    {
        ss << setw(_length) << setfill(_sign) << _message;
    }
    else
    {
        cout << "back or front not define" << endl;
    }
    ss >> _message;
    return _message;
}

class source_file
{
public:
    fstream source;
    ofstream output;
    vector<string> Loc;
    vector<string> col_1;
    vector<string> col_2;
    vector<string> col_3;
    string name;
    string start_hex;
    string end_hex;

    vector<string> label_name;
    vector<string> label_address;

    string the_loc_of(string _label)
    {
        if (_label == "")
        {
            return "0000";
        }

        if (_label.find(",") != std::string::npos) //找到 ","
        {
            string temp = "";
            for (size_t i = 0; i < _label.size(); i++)
            {
                if (_label.at(i) != ',')
                {
                    temp = temp + _label.at(i);
                }
                else
                {
                    break;
                }
            }
            int temp_int = hex_to_dec(the_loc_of(temp)); //遞迴
            bitset<16> bt(temp_int);
            // cout << the_loc_of(temp) << " | " << temp_int << " | " << bt << endl;
            bt.set(15);
            int after_int = (int)(bt.to_ulong());
            string after_string = dec_to_hex(after_int);
            return after_string;
        }

        for (size_t i = 0; i < label_name.size(); i++)
        {
            if (label_name[i] == _label)
            {
                return label_address[i];
            }
        }
        cout << "here is no label \"" << _label << "\" please check symbol table" << endl;
        return "";
    }

    string set()
    {
        for (size_t i = 0; i < col_2.size(); i++)
        {
            if (col_2[i] == "START")
            {
                start_hex = Loc[i];
                name = col_1[i];
            }
            if (col_2[i] == "END")
            {
                end_hex = Loc[i];
            }
        }
        return dec_to_hex(hex_to_dec(end_hex) - hex_to_dec(start_hex));
    }

    void print_loc_and_source(string output_path)
    {
        if (output_path != "")
        {
            ofstream location;
            location.open(output_path);
            for (int i = 0; i < col_1.size(); i++)
            {
                location << Loc[i] << "\t";
                location << col_1[i] << "\t";
                location << col_2[i] << "\t";
                location << col_3[i] << endl;
            }
            location.close();
        }
        for (int i = 0; i < col_1.size(); i++)
        {
            cout << i << "\t";
            cout << Loc[i] << "\t";
            cout << col_1[i] << "\t";
            cout << col_2[i] << "\t";
            cout << col_3[i] << endl;
        }
    }
    void print_source()
    {
        for (int i = 0; i < col_1.size(); i++)
        {
            cout << i << "\t";
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

    void loc_count_fetch(int _begin, int _end)
    {
        static bool start_is_find = false;
        static string start_num;
        for (size_t i = _begin; i < _end; i++) //讀取整裡過後的 Source Statement
        {
            if (col_2[i] == "START")
            {
                start_is_find = true;
                start_num = col_3[i];
                Loc.push_back(loc_counter(start_num, 0)); // START 那一行
                Loc.push_back(loc_counter(start_num, 0)); // START  的後一行 範例是 FIRST
                i++;
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
            else if (col_2[i] == "BASE")
            {
                Loc.push_back("");
                loc_count_fetch(i - 1, i); //抓取去一道指令的長度
            }

            else if (col_2[i] == "RESW")
            {
                cout << "find RESW at" << i << endl;
                Loc.push_back(loc_counter(start_num, atoi(col_3[i].c_str()) * 3));
            }

            else if (col_2[i] == "RESB")
            {
                cout << "find RESB at " << i << endl;
                Loc.push_back(loc_counter(start_num, atoi(col_3[i].c_str())));
            }
            else if (col_2[i] == "CLEAR" || col_2[i] == "COMPR" || col_2[i] == "TIXR")
            {
                Loc.push_back(loc_counter(start_num, 2));
            }

            else // 除了特殊規則以外
            {
                string format = "";
                format = format_classifier(i, col_2[i], col_3[i], "location"); //若沒有找到相應類型 會還傳 "default"
                if (format == "normal")
                {
                    Loc.push_back(loc_counter(start_num, 3));
                }
                else if (format == "default")
                {
                    Loc.push_back(loc_counter(start_num, 3));
                }

                else if (format == "Extended")
                {
                    Loc.push_back(loc_counter(start_num, 4));
                }
                else
                {
                    Loc.push_back(loc_counter(start_num, 3));
                }
                // cout << i << " : " << format << endl;
            }
        }
    }

    string format_classifier(int _line, string _col_2, string _col_3, string _mode)
    { // format分類器
        string format_message = "";
        //檢查col 3是否有 #Immediate 或 @Indirect
        if (_col_2 == "" || _col_3 == "")
        {
            return format_message;
        }
        //先檢查col_3 類型

        if (_col_3.at(0) == '#')
        {
            format_message = "Immediate";
            if (_mode == "object") // 在object 的計算下
            {
                if (_col_2.at(0) == '+')
                {
                    return "Extended+Immediate"; //同時是Exteded + Immediate
                }

                return format_message;
            }
        }
        else if (_col_3.at(0) == '@')
        {
            format_message = "Indirect";
            if (_mode == "object")
            {
                return format_message;
            }
        }
        else
        {
            /* code */
        }

        //第二次 對 col_2 進行判斷

        if (_col_2.at(0) == '+')
        {
            format_message = "Extended"; //在mode location 中只有 Extende 會影像 location counter 計算
            if (_mode == "location")
            {
                return format_message;
            }
            else if (_mode == "object")
            {
                if (char_type(_col_3.at(0)) == 'm')
                {

                    cout << "here is + and # at " << _line << endl;
                    return "Extended+Immediate";
                }
                else
                {
                }
            }
        }
        else if (_col_3.find(",") != std::string::npos)
        {
            format_message = "Indexed";
        }
        else
        {
            format_message = "normal";
        }
        return format_message;

        //檢查col 2是否有 + Extented 或 m,x Indexed addressing
        //都沒有就是PC relative 或 BC relative
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
                // cout << "------" << endl;
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
                        cout << "out control : " << type << " which ascii is " << int(type) << endl;
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

    void print_symbol_table(string output_path)
    {
        if (output_path != "")
        {
            ofstream symbol_table;
            symbol_table.open(output_path);
            for (size_t i = 0; i < label_name.size(); i++)
            {
                symbol_table << label_name[i] << "\t" << label_address[i] << endl;
            }
            symbol_table.close();
        }
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
        if (_statement == "BYTE" || _statement == "WORD" || _statement == "RESW" || _statement == "RESB" || _statement == "END")
        {
            return _statement; //遇到特殊狀況交由 col_3處理 直接通過
        }

        for (size_t i = 0; i < label_name.size(); i++) //在opcode 中尋找
        {
            if (label_name[i] == _statement)
            {
                return address[i];
            }
        }
        cout << "here is wrong op_code: " << _statement << " please check source file(.txt)" << endl;
        return "out";
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
                    if (type != 's' && type != 'r')
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
private:
    const static int object_max_num = 6; //每一行object code 有六個位元
public:
    vector<string> Obj_code;
    int length_of_object_code()
    {
        static int length = 0;
        if (Obj_code.size() == 0)
        {
            cout << "no object_code to count length" << endl;
            return 0;
        }

        for (size_t i = 0; i < Obj_code.size(); i++)
        {
            length = length + Obj_code[i].size();
            cout << length << endl;
        }
        cout << dec_to_hex(length) << endl;
        return length;
    }
    string get_BYTE_word(string _statement)
    {
        string temp = "";
        bool flag = false;
        char type = _statement.at(0);
        for (size_t i = 1; i < _statement.size(); i++)
        {
            if (_statement[i] == '\'')
            {
                flag = !flag;
                i++;
            }
            if (flag == true)
            {
                if (type == 'C')
                {
                    // cout<< dec_to_hex(_statement.at(i));
                    temp = temp + dec_to_hex(_statement.at(i));
                }
                else if (type == 'X')
                {
                    // cout << _statement.at(i);
                    temp = temp + _statement.at(i);
                }
            }
        }
        // cout << "get_BYTE " << temp << endl;
        return temp;
    }
    bool is_no_obj(string _col_2_)
    {
        if (_col_2_ == "START")
            return true;
        else if (_col_2_ == "RESW")
            return true;
        else if (_col_2_ == "RESB")
            return true;
        else if (_col_2_ == "RESB")
            return true;
        else
        {
            return false;
        }
    }
    bool is_no_relation_with_(string _col_2_, string _col_3_)
    {
        //直接對Loc動作
        if (_col_2_ == "BYTE")
        {
            cout << "\t is BYTE " << get_BYTE_word(_col_3_) << endl;
            Obj_code.push_back(get_BYTE_word(_col_3_));

            return true;
        }
        if (_col_2_ == "WORD") //遇到word補'0'
        {
            string result;
            stringstream ss;
            string num = dec_to_hex(atoi(_col_3_.c_str()));
            ss << setw(object_max_num) << setfill('0') << num; //補0 補到object code的最多的位元數
            ss >> result;
            Obj_code.push_back(result);
            cout << "\t opcode" << result << endl;
            return true;
        }
        return false;
    }
    string obj_process(string _format_message, string _col_2, string _col_3)
    {
        string result = "";
        if (_format_message == "normal")
        {
            /* code */
            return "n";
        }
        if (_format_message == "Immediate")
        {
            return "#";
        }
        if (_format_message == "Extended+Immediate")
        {
            return "+#";
        }

        return _format_message;
    }
    void generate(source_file &_source, opcode_file &_opcode) //生成object_code
    {
        for (size_t i = 0; i < _source.col_2.size(); i++)
        {
            if (is_no_obj(_source.col_2[i])) //如果不產生obj_code  添加“” ;
            {
                Obj_code.push_back("");
                continue;
            }

            if (is_no_relation_with_(_source.col_2[i], _source.col_3[i])) // BYTE , WORD
                continue;

            string format = _source.format_classifier(i, _source.col_2[i], _source.col_3[i], "object");
            // string here_obj_code = obj_process(format, _source.col_2[i], _source.col_3[i]);
            Obj_code.push_back(format);

            //剩下都是要計算obj code

            // string temp_string_opcode = _opcode.the_address_of(_source.col_2[i]); //輸入col_2 獲得 opcode
            // string temp_string_symbol_table = "";
            // string result = "";

            // else //非特殊情況
            // {
            //     temp_string_symbol_table = _source.the_loc_of(_source.col_3[i]); // 輸入col_3 獲得 loc
            //     result = temp_string_opcode + temp_string_symbol_table;
            // }
            // cout << temp_string_opcode << "|" << temp_string_symbol_table << endl;

            // cout << i << " : " << result << endl;
            // Obj_code.push_back(result);
        }
    }
    void print_loc_source_object(source_file &_source, string output_path)
    {
        ofstream source_obj_code;
        source_obj_code.open(output_path);
        if (output_path != "")
        {
            for (size_t i = 0; i < _source.col_1.size(); i++)
            {
                source_obj_code << _source.Loc[i] << "\t";
                source_obj_code << _source.col_1[i] << "\t";
                source_obj_code << _source.col_2[i] << "\t";
                source_obj_code << _source.col_3[i] << "\t";
                source_obj_code << endl;
            }
        }
        else
        {
            for (size_t i = 0; i < _source.col_1.size(); i++)
            {
                cout << i << "\t";
                cout << _source.Loc[i] << "\t";
                cout << _source.col_1[i] << "\t";
                cout << _source.col_2[i] << "\t";
                cout << _source.col_3[i] << "\t";
                cout << Obj_code[i] << endl;
            }
        }
    }
    void output_H(source_file &_source, ofstream &output_stream)
    {
        string range = _source.set(); //獲得基本常數 名稱 開始 結束 返回距離
        output_stream << "H";
        output_stream << add_lecture(_source.name, ' ', 6, "back"); //補滿
        output_stream << add_lecture(_source.start_hex, '0', 6, "front");
        output_stream << add_lecture(range, '0', 6, "front");
        output_stream << endl;
    }
    void output_T(source_file &_source, ofstream &output_stream)
    {
        string record_byte_hex = "";
        int record_byte_dec = 0;
        bool flag = false;
        bool readable = readable_mode;
        string output_message;
        string start_hex_this_record = _source.start_hex;
        int loc_row = 0;

        for (size_t i = 0; i < Obj_code.size(); i++)
        {
            record_byte_dec += Obj_code[i].size() / 2;
            if (Obj_code[i] != "")
            {
                flag = false;
            }

            if (record_byte_dec == hex_to_dec("1e"))
            {

                // cout << i << ":" << Obj_code[i] << " | " << Obj_code[i].size() << " | " << dec_to_hex(record_byte_dec) << endl;
                output_message += Obj_code[i];
                if (readable == true)
                {
                    output_message = "T^" + add_lecture(start_hex_this_record, '0', 6, "front") + "^" + dec_to_hex(record_byte_dec) + "^" + output_message;
                }
                else
                {
                    output_message = "T" + add_lecture(start_hex_this_record, '0', 6, "front") + dec_to_hex(record_byte_dec) + output_message;
                }

                output_stream << output_message << endl;
                // start_hex_this_record = hex_add(start_hex_this_record, dec_to_hex(record_byte_dec));
                loc_row = i + 2;
                start_hex_this_record = _source.Loc[loc_row]; //

                output_message = "";
                record_byte_dec = 0;
                cout << "---" << endl;
            }
            else if (record_byte_dec > hex_to_dec("1e"))
            {
                record_byte_dec -= Obj_code[i].size() / 2;
                i--;
                if (readable == true)
                {
                    output_message = "T^" + add_lecture(start_hex_this_record, '0', 6, "front") + "^" + dec_to_hex(record_byte_dec) + "^" + output_message;
                }
                else
                {
                    output_message = "T" + add_lecture(start_hex_this_record, '0', 6, "front") + dec_to_hex(record_byte_dec) + output_message;
                }
                output_stream << output_message << endl;

                loc_row = i + 2;
                start_hex_this_record = _source.Loc[loc_row]; //

                output_message = "";
                record_byte_dec = 0;
                cout << "---" << endl;
            }

            else if (Obj_code[i].size() == 0 && flag == false)
            {
                if (readable == true)
                {
                    output_message = "T^" + add_lecture(start_hex_this_record, '0', 6, "front") + "^" + dec_to_hex(record_byte_dec) + "^" + output_message;
                }
                else
                {
                    output_message = "T" + add_lecture(start_hex_this_record, '0', 6, "front") + dec_to_hex(record_byte_dec) + output_message;
                }
                output_stream << output_message << endl;
                loc_row = i + 4;                              // line 90,95 不輸出的object code 似乎還是佔有位子?
                start_hex_this_record = _source.Loc[loc_row]; //

                output_message = "";
                record_byte_dec = 0;
                cout << "---" << endl;
                flag = true;
            }
            else if (flag == false)
            {
                // cout << i << ":" << Obj_code[i] << " | " << Obj_code[i].size() << " | " << dec_to_hex(record_byte_dec) << endl;
                output_message += Obj_code[i];
                if (readable == true)
                {
                    output_message += "^";
                }
            }
        }
    }
    void output_E(source_file &_source, ofstream &output_stream)
    {
        output_stream << "E";
        output_stream << add_lecture(_source.start_hex, '0', 6, "front") << endl;
    }
};

int main()
{
    cout << "--start--" << endl;
    source_file source;
    source.load_data();
    // source.print_source();
    source.loc_count_fetch(0, source.col_2.size());
    source.print_loc_and_source("");
    cout << "--symbol-table--" << endl;
    source.symbol_table();
    // source.print_symbol_table("");
    cout << "--load-OPCode" << endl;
    opcode_file opcode;
    opcode.load_data();
    opcode.print_all_col();
    cout << "--object--code--" << endl;

    object_code XE_Obj;
    XE_Obj.generate(source, opcode);
    cout << "object-- code table" << endl;
    XE_Obj.print_loc_source_object(source, "");

    cout << "--fin--" << endl;
}