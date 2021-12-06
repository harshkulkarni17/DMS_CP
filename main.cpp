#include <bits/stdc++.h>
using namespace std;
vector<string> tables;
class SQL
{
public:
    vector<string> keywords = {"select", "create", "table", "alter", "drop", "desc", "describe", "*", "insert", "delete", "where", "orderby", "from", "values", "into", "and", "or", "between", "in", "references", "set", "distinct", "null", "not", "(", ")", ";"};
    vector<string> types = {"int", "varchar", "float"};
    vector<string> command;
    vector<string> constraints = {"primary_key", "foreign_key", "unique", "check", "not_null", "default", "references"};
    ifstream ip_file;
    ofstream op_file, datatype, col_names, constraint, constrLength;

    vector<string> split(string s)
    {
        stringstream stringStream(s);
        string line;
        vector<string> wordVector;
        while (getline(stringStream, line))
        {
            size_t prev = 0, pos;
            while ((pos = line.find_first_of(" (),", prev)) != string ::npos)
            {
                if (pos > prev)
                    wordVector.push_back(line.substr(prev, pos - prev));
                prev = pos + 1;
            }
            if (prev < line.length())
                wordVector.push_back(line.substr(prev, string ::npos));
        }

        for (int i = 0; i < wordVector.size(); i++)
        {
            cout << wordVector[i] << " ";
        }
        return wordVector;
    }

    bool isNumber(const string &str)
    {
        for (char const &c : str)
        {
            if (std::isdigit(c) == 0)
                return false;
        }
        return true;
    }

    int getIndex(vector<string> v, string K)
    {
        auto it = find(v.begin(), v.end(), K);
        if (it != v.end())
        {
            int index = it - v.begin();
            return index;
        }
        return -1;
    }

    fstream &GotoLine(fstream &file, unsigned int num)
    {
        file.seekg(ios::beg);
        for (int i = 0; i < num - 1; ++i)
        {
            file.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        return file;
    }

    // Read until no more lines in text file to read
    void execute(vector<string> s);
    void create(vector<string> s);
    void insert(vector<string> s);
    void desc(vector<string> s);
    void read_schema()
    {
        ip_file.open("Schema.txt", ios ::in);
        // char c = ' ';
        string table;
        // vector<string> tables;
        while (getline(ip_file, table))
        {
            int i = 0;
            string temp;
            while (table[i] != '#')
            {
                temp.push_back(table[i]);
                i++;
            }
            tables.push_back(temp);
        }
    }
    SQL(string query)
    {
        read_schema();
        command = split(query);
        // for (int i = 0; i < command.size(); i++)
        // {
        //     cout << command[i] << endl;
        // }
        execute(command);
    }

    // void read()
    // {
    //     ip_file.open("Schema.txt", ios::in);
    //     char c;
    //     int i = 0;
    //     clear();

    //     do
    //     {
    //         ip_file.get(c);
    //         buffer[i] = c;
    //         i++;
    //     } while (c != '\n');
    // }
    string lower(string upTxt)
    {
        std::transform(upTxt.begin(), upTxt.end(), upTxt.begin(), ::tolower);
        return upTxt;
    }
    bool notin(string s, vector<string> c)
    {
        for (int i = 0; i < c.size(); i++)
        {
            if (s == c[i])
                return false;
        }
        return true;
    }
};
void SQL ::execute(vector<string> command)
{
    if (lower(command[0]) == "create")
    {
        cout << "create()";
        create(command);
    }
    else if (lower(command[0]) == "select")
    {
        cout << "select()";
    }
    else if (lower(command[0]) == "insert")
    {
        // cout << "insert()";
        insert(command);
    }
    else if (lower(command[0]) == "desc" || lower(command[0]) == "describe")
    {
        // cout << "desc()";
        desc(command);
    }
    else if (lower(command[0]) == "update")
    {
        cout << "update()";
    }
    else if (lower(command[0]) == "delete")
    {
        cout << "delete()";
    }
    else if (lower(command[0]) == "drop")
    {
        cout << "drop()";
    }
    else if (lower(command[0]) == "alter")
    {
        cout << "alter()";
    }
    else
    {
        cout << "Error" << endl;
    }
}
void SQL::create(vector<string> command)
{
    op_file.open("Schema.txt", ios::app | ios::out);
    op_file << command[2];
    col_names.open("Column.txt", ios::app | ios::out);
    datatype.open("Datatype.txt", ios::app | ios::out);
    constraint.open("Constraint.txt", ios::app | ios::out);
    constrLength.open("ConstrLength.txt", ios::app | ios::out);
    col_names << command[2] << "#";
    datatype << command[2] << "#";
    constraint << command[2] << "#";
    constrLength << command[2] << "#";
    tables.push_back(command[2]);
    vector<string> att_name, typ, constr;
    unordered_map<string, vector<string>> att_constr;
    unordered_map<string, vector<int>> att_length;

    for (int i = 3; i < command.size(); i++)
    {
        if (notin(lower(command[i]), keywords))
        {
            if (isNumber(command[i]))
            {
                if (att_length[att_name[att_name.size() - 1]].size() == 1 && att_length[att_name[att_name.size() - 1]][0] == 0)
                {
                    att_length[att_name[att_name.size() - 1]] = {stoi(command[i])};
                }
                else
                    att_length[att_name[att_name.size() - 1]].push_back(stoi(command[i]));
            }
            else if (notin(lower(command[i]), types) && notin(lower(command[i]), constraints))
            {
                att_name.push_back(command[i]);
                att_constr[command[i]] = {"NA"};
                att_length[command[i]] = {0};
            }
            else if (notin(lower(command[i]), types) && !notin(lower(command[i]), constraints))
            {
                constr.push_back(command[i]);
                if (att_constr[att_name[att_name.size() - 1]].size() == 1 && att_constr[att_name[att_name.size() - 1]][0] == "NA")
                {
                    att_constr[att_name[att_name.size() - 1]] = {command[i]};
                }
                else
                    att_constr[att_name[att_name.size() - 1]].push_back(command[i]);
            }
            else
            {
                typ.push_back(command[i]);
            }
        }

        // else
        // {
        //     constraints.push_back(command[i]);
        // }
    }
    cout << "Table created successfully" << endl;

    for (int i = 0; i < att_name.size(); i++)
    {
        op_file << "#" << att_name[i];
        col_names << att_name[i] << ",";
        // cout << att_name[i] << "#";
        op_file << "#" << typ[i];
        datatype << typ[i] << ",";
        // cout << typ[i] << "#";
        for (int j = 0; j < att_constr[att_name[i]].size(); j++)
            constraint << att_constr[att_name[i]][j] << "-";
        constraint << ",";

        for (int j = 0; j < att_length[att_name[i]].size(); j++)
            constrLength << att_length[att_name[i]][j] << "-";
        constrLength << ",";
    }

    op_file << endl;
    col_names << endl;
    datatype << endl;
    constraint << endl;
    constrLength << endl;
    op_file.close();
    col_names.close();
    datatype.close();
    constraint.close();
    constrLength.close();
}
void SQL::insert(vector<string> command)
{
    // op_file.open("Schema.txt", ios::app | ios::out);

    if (notin(lower(command[2]), tables))
    {
        cout << "Error : No such table created\n";
    }
    else
    {
        ofstream op_table;
        fstream f1("Column.txt");
        op_table.open(command[2] + ".txt", ios::app | ios::out);
        int index = getIndex(tables, command[2]);
        GotoLine(f1, index + 1);
        string line;
        f1 >> line;
        cout << line;
        for (int i = 4; i < command.size(); i++)
        {
            command[i].erase(
                remove(command[i].begin(), command[i].end(), '\''),
                command[i].end());
            if (command[i] == ";")
                break;
            // else if (command[i] == "(")
            //     i++;
            // if (i == 5)
            // {

            op_table << command[i] << "#";
            // }
            // else
            // {
            //     op_table << "#" << command[i];
            // }
        }
        cout << "\nData inserted successfully\n";
        op_table << "\n";
        op_table.close();
    }
}
void SQL::desc(vector<string> command)
{
    string table = command[1];
    if (notin(lower(table), tables))
    {
        cout << "Error : No such table created\n";
    }
    else
    {
        // map<string, string> info;
        ifstream ip_file1, ip_file2, ip_file3;
        ip_file1.open("Column.txt", ios::in);
        vector<string> columns, datatype, cons;
        ip_file2.open("Datatype.txt", ios::in);
        ip_file3.open("Constraint.txt", ios::in);
        string temp;
        while (getline(ip_file1, temp))
        {
            int i = 0;
            string temp1;
            while (temp[i] != '#')
            {
                temp1.push_back(table[i]);
                i++;
            }
            if (temp1 == table)
            {
                columns = split(temp.substr(i + 1));
            }
        }
        while (getline(ip_file2, temp))
        {
            int i = 0;
            string temp1;
            while (temp[i] != '#')
            {
                temp1.push_back(table[i]);
                i++;
            }
            if (temp1 == table)
            {
                datatype = split(temp.substr(i + 1));
            }
        }
        while (getline(ip_file3, temp))
        {
            int i = 0;
            string temp1;
            while (temp[i] != '#')
            {
                temp1.push_back(table[i]);
                i++;
            }
            if (temp1 == table)
            {
                cons = split(temp.substr(i + 1));
            }
        }

        map<int, vector<string>> tempmap;
        for (int i = 0; i < cons.size(); i++)
        {
            vector<string> str;
            stringstream stringStream(cons[i]);
            string line;
            while (getline(stringStream, line))
            {
                size_t prev = 0, pos;
                while ((pos = line.find_first_of("-", prev)) != string ::npos)
                {
                    if (pos > prev)
                        str.push_back(line.substr(prev, pos - prev));
                    prev = pos + 1;
                }
                if (prev < line.length())
                    str.push_back(line.substr(prev, string ::npos));
            }
            tempmap[i] = str;
        }

        // cout << "\tTable -  " << table << endl;
        cout << "-------------------------------------------------------------\n";
        cout << "TABLE\t|  COLUMN NAME\t|  DATATYPE\t|  CONSTRAINTS\t" << endl;
        cout << "-------------------------------------------------------------\n";
        cout << table;
        for (int i = 0; i < columns.size(); i++)
        {
            if (columns[i].size() > 4 && datatype[i].size() > 4)
            {
                cout << "\t|  " << columns[i] << "\t|  " << datatype[i] << "\t| ";
                for (int j = 0; j < tempmap[i].size(); j++)
                {
                    cout << "  " << tempmap[i][j] << " ";
                }
                cout << "\n-------------------------------------------------------------\n";
            }
            else if (columns[i].size() > 4 && datatype[i].size() <= 4)
            {
                cout << "\t|  " << columns[i] << "\t|  " << datatype[i] << "\t\t|";
                for (int j = 0; j < tempmap[i].size(); j++)
                {
                    cout << "  " << tempmap[i][j] << " ";
                }

                cout << "\n-------------------------------------------------------------\n";
            }
            else if (columns[i].size() <= 4 && datatype[i].size() > 4)
            {
                cout << "\t|  " << columns[i] << "\t\t|  " << datatype[i] << "\t|";
                for (int j = 0; j < tempmap[i].size(); j++)
                {
                    cout << "  " << tempmap[i][j] << " ";
                }

                cout << "\n-------------------------------------------------------------\n";
            }
            else
            {
                cout << "\t|  " << columns[i] << "\t\t|  " << datatype[i] << "\t\t|";
                for (int j = 0; j < tempmap[i].size(); j++)
                {
                    cout << "  " << tempmap[i][j] << " ";
                }

                cout << "\n-------------------------------------------------------------\n";
            }
        }
        ip_file1.close();
        ip_file2.close();
        ip_file3.close();
    }
}
int main()
{
    string s;
    //"insert into stud values (11817, 'john');";
    //"create table stud(grno int primary_key, name varchar(20));";
    //"create table phase(phaseno int(3,2) primary_key, cardname varchar(20));"
    int c = 1;
    cout << "Enter the SQL command :- ";
    getline(cin, s);
    SQL sql(s);
    return 0;
}