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
    vector<string> operators = {"=", "<=", ">=", "<>", "!=", "!", ">", "<", "%"};
    ifstream ip_file;
    ofstream op_file, datatype, col_names, constraint, constrLength;

    // SQL(string query)
    // {
    //     read_schema();
    //     command = split(query);
    //     // for (int i = 0; i < command.size(); i++)
    //     // {
    //     //     cout << command[i] << endl;
    //     // }
    //     execute(command);
    // }

    void setQuery(string query)
    {
        read_schema();
        command = split(query);
        execute(command);
    }

    vector<string> split(string s)
    {
        stringstream stringStream(s);
        string line;
        vector<string> wordVector;
        while (getline(stringStream, line))
        {
            size_t prev = 0, pos;
            while ((pos = line.find_first_of(" (),#;", prev)) != string ::npos)
            {
                if (pos > prev)
                    wordVector.push_back(line.substr(prev, pos - prev));
                prev = pos + 1;
            }
            if (prev < line.length())
                wordVector.push_back(line.substr(prev, string ::npos));
        }

        // for (int i = 0; i < wordVector.size(); i++)
        // {
        //     cout << wordVector[i] << " ";
        // }
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
    vector<vector<string>> splitFile(fstream &, string, bool);
    bool operation(vector<int> values, string command, string table_name, int attr_no);
    bool isInteger(string command);
    bool isFloat(string command);
    bool checkDuplicates(string command, string table);
    void dropTable(string tableName);
    void replaceFiles(string fileName, string tableName);
    void select(vector<string> command);
    void fetchAll(string table);
    void fetchRecordProjection(string table, vector<string> temp);
    void fetchAllbyCondition(string table, vector<string> whereClause);
    void fetchRecordProjectionbyCondition(string table, vector<string> temp, vector<string> whereClause);
    vector<string> getColumns(string table);
    void alterTable(vector<string> command);
    void alterFiles(vector<string> command, int index, int flag);
    void updateFiles(string fileName, int index, vector<string> append_str);
<<<<<<< HEAD
    void update(vector<string> command);
    void helpTables();
    void helpCommand(vector<string> command);
    void quit();
=======
    void deleteRecords(vector<string> command);
    void deleteAllbyCondition(string table, vector<string> whereClause);
    void deleteAll(string table);
    void deleteFromFile(string table, vector<int> indexes);
>>>>>>> 95bbc34cff705426bb05108e8211c5fad86bf70b

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
        ip_file.close();
    }

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
        cout << "select()\n";
        select(command);
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
        cout << "update()\n";
        update(command);
    }
    else if (lower(command[0]) == "delete")
    {
        cout << "delete()";
    }
    else if (lower(command[0]) == "drop")
    {
        cout << "drop()";
        dropTable(command[2]);
    }
    else if (lower(command[0]) == "alter")
    {
        cout << "alter()";
        alterTable(command);
    }
    else if (lower(command[0]) == "help" && command[1] == "tables")
    {
        cout << "Help Tables()\n";
        helpTables();
    }
    else if (lower(command[0]) == "help")
    {
        cout << "Help Command()\n";
        helpCommand(command);
    }
    else if (lower(command[0]) == "quit")
    {
        cout << "Quit()\n";
        quit();
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

                // id --> 4 2
                // name --> 25
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
        fstream f1("ConstrLength.txt");
        fstream f2("Column.txt");
        fstream f3("Constraint.txt");
        fstream f4("Datatype.txt");
        string table_name = command[2];
        vector<vector<string>> Length, Constraint, Datatype;
        op_table.open(command[2] + ".txt", ios::app | ios::out);
        unordered_map<string, vector<vector<string>>> attribute_constrints;
        map<int, vector<int>> attr_table;

        Length = splitFile(f1, command[2], true);
        Constraint = splitFile(f3, command[2], true);
        Datatype = splitFile(f4, command[2], false);

        // add values in map attr_table
        for (int i = 1; i <= Length.size(); i++)
        {
            vector<int> boolValues(11, 0);
            // Int Float Varchar Number(Length) Precisoin PK FK NotNUll Unique check default
            if (lower(Datatype[i - 1][0]) == "int")
                boolValues[0] = 1;
            else if (lower(Datatype[i - 1][0]) == "float")
                boolValues[1] = 1;
            else if (lower(Datatype[i - 1][0]) == "varchar")
                boolValues[2] = 1;

            boolValues[3] = stoi(Length[i - 1][0]);
            if (Length[i - 1].size() == 2)
                boolValues[4] = stoi(Length[i - 1][1]);

            if (lower(Constraint[i - 1][0]) == "primary_key")
                boolValues[5] = 1;
            if (lower(Constraint[i - 1][0]) == "foreign_key")
                boolValues[6] = 1;
            if (lower(Constraint[i - 1][0]) == "not_null")
                boolValues[7] = 1;
            if (lower(Constraint[i - 1][0]) == "unique")
                boolValues[8] = 1;
            if (lower(Constraint[i - 1][0]) == "check")
                boolValues[9] = 1;
            if (lower(Constraint[i - 1][0]) == "default")
                boolValues[10] = 1;

            attr_table[i] = boolValues;
        }

        // for (auto x : attr_table)
        // {
        //     cout << x.first << " ";
        //     for (auto i : x.second)
        //         cout << i << " ";
        //     cout << endl;
        // }

        bool check;
        for (int i = 4; i < command.size(); i++)
        {
            command[i].erase(
                remove(command[i].begin(), command[i].end(), '\''),
                command[i].end());
            if (command[i] == ";")
                break;

            vector<int> values = attr_table[i - 3];
            check = operation(values, command[i], table_name, i - 3);
            // cout << "Bool - " << check << endl;
            if (!check)
                break;
            op_table << command[i] << "#";
        }
        if (check)
        {
            cout << "\nData inserted successfully\n";
            op_table << "\n";
        }
        op_table.close();
    }
}
bool SQL ::isInteger(string command)
{
    return isNumber(command);
}
bool SQL ::isFloat(string command)
{
    string s;
    int flag = 0;
    for (auto i : command)
    {
        if (i != '.')
        {
            s.push_back(i);
            flag = 1;
        }
    }
    if (isNumber(command) && flag)
    {
        return true;
    }
    return false;
}
bool SQL ::checkDuplicates(string command, string table_name)
{
    ifstream table;
    table.open(table_name + ".txt", ios::in);
    string attr;
    vector<string> values;
    // vector<string> tables;
    while (getline(table, attr))
    {
        int i = 0;
        string temp;
        while (attr[i] != '#')
        {
            temp.push_back(attr[i]);
            i++;
        }
        values.push_back(temp);
    }
    values.push_back(command);
    set<string> value_set;
    for (auto i : values)
    {
        value_set.insert(i);
    }
    if (value_set.size() != values.size())
        return true; // if duplicate found
    return false;    // if duplicate not found
}

bool SQL::operation(vector<int> values, string command, string table_name, int attr_no)
{
    if (values[0] == 1)
    {
        if (!isInteger(command))
        {
            cout << "Error " << command << " is not of type integer" << endl;
            return false;
        }
    }

    else if (values[1] == 1)
    {
        if (!isFloat(command))
        {
            cout << "Error " << command << " is not of type float" << endl;
            return false;
        }
    }
    // else if (values[2] == 1)
    // {
    // // isVarchar(command);
    // }

    if (values[3])
    {
        int t = command.size();
        if (values[1] == 1)
            t--;

        if (t > values[3])
        {
            cout << "Error : Length Constraint Violated due to " << command << endl;
            return false;
        }
    }

    if (values[4])
    {
        vector<string> temp = split(command);
        if (temp[1].size() > values[4])
        {
            cout << "Error : Precision Constraint Violated due to " << command << endl;
            return false;
        }
    }

    if (values[5] == 1)
    {
        if (lower(command) == "na" || lower(command) == "null")
        {
            cout << "Error : Null value detected - This attribute can't be null\n";
            return false;
        }
        else if (checkDuplicates(command, table_name))
        {
            cout << "Error : Your entered key is already exists in the table " << endl;
            return false;
        }
    }

    // if (values[6] == 1)
    // {
    // }
    // // checkForignkey

    if (values[7] == 1)
    {
        // checkNull(command)
        if (lower(command) == "na" || lower(command) == "null")
        {
            cout << "Error : Null value detected - This attribute can't be null\n";
            return false;
        }
    }

    if (values[8] == 1)
    {
        // unique
        if (checkDuplicates(command, table_name))
        {
            cout << "Error : Unique constraint violated " << endl;
            return false;
        }
    }

    // if (values[9] == 1)
    // {
    // }
    // // checkcheck(command)

    // if (values[10] == 1)
    // {
    // }
    // // checkdefault
    return true;
}

vector<vector<string>> SQL::splitFile(fstream &f1, string s, bool b) // if b=true, that means its for files constraint.txt and constraintLength.txt, else it is for datatype.txt
{

    vector<vector<string>> Length;
    int index = getIndex(tables, s);
    GotoLine(f1, index + 1);
    string fline;
    vector<string> l, l1;
    f1 >> fline;
    l = split(fline);
    if (b)
    {
        for (int i = 1; i < l.size(); i++)
        {
            stringstream stringStream(l[i]);
            string line;
            vector<string> wordVector;
            while (getline(stringStream, line))
            {
                size_t prev = 0, pos;
                while ((pos = line.find_first_of("-", prev)) != string ::npos)
                {
                    if (pos > prev)
                        wordVector.push_back(line.substr(prev, pos - prev));
                    prev = pos + 1;
                }
                if (prev < line.length())
                    wordVector.push_back(line.substr(prev, string ::npos));
            }
            Length.push_back(wordVector);
        }
    }
    else
    {
        for (int i = 1; i < l.size(); i++)
        {
            Length.push_back({l[i]});
        }
    }
    return Length;
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

void SQL::dropTable(string tableName)
{
    if (notin(lower(tableName), tables))
    {
        cout << "\nError : No such table exsits\n";
    }
    else
    {
        replaceFiles("Schema.txt", tableName);
        replaceFiles("Column.txt", tableName);
        replaceFiles("Constraint.txt", tableName);
        replaceFiles("ConstrLength.txt", tableName);
        replaceFiles("Datatype.txt", tableName);
        cout << "\nTable deleted successfully" << endl;
    }
}

void SQL::replaceFiles(string fileName, string tableName)
{
    fstream f1;
    ofstream temp;
    string line;
    f1.open(fileName);
    temp.open("temp.txt");
    int index = getIndex(tables, tableName);
    int count = 0;

    char *writable = new char[fileName.size() + 1];
    copy(fileName.begin(), fileName.end(), writable);
    writable[fileName.size()] = '\0'; // don't forget the terminating 0

    while (getline(f1, line))
    {
        if (count == index)
        {
            count++;
        }
        else
        {
            temp << line << endl;
            count++;
        }
    }

    temp.close();
    f1.close();
    remove(writable);
    rename("temp.txt", writable);
    delete[] writable;
}

void SQL::select(vector<string> command)
{
    string table;
    vector<string>::iterator itr = find(command.begin(), command.end(), "from");
    if (itr != end(command))
    {
        table = command[itr - command.begin() + 1]; // itr-vector.begin gives index of "from" element
    }
    else
    {
        cout << "Syntax Error";
    }

    // check if there is where clause present or not
    bool present;
    int index;
    vector<string> whereClause;
    vector<string>::iterator it = find(command.begin(), command.end(), "where");
    if (it != end(command))
    {
        present = true;
        index = it - command.begin(); // index of where keyword
        for (int i = index + 1; i < command.size(); i++)
        {
            if (command[i] == ";")
                continue;
            whereClause.push_back(command[i]);
        }
    }

    if (notin(lower(table), tables))
    {
        cout << "Error : No such table exists\n";
    }
    else
    {
        // select * from table_name
        if (command[1] == "*")
        {
            if (present)
            {
                fetchAllbyCondition(table, whereClause);
            }
            else
            {
                fetchAll(table);
            }
        }
        else
        {
            // select grno, name from stud;
            int index;
            auto it = find(command.begin(), command.end(), "from");
            if (it != command.end())
            {
                index = it - command.begin();
            }

            // get projected col names in temp vector
            vector<string> temp;
            for (int i = 1; i < index; i++)
                temp.push_back(command[i]); // temp = {grno,name}

            if (present)
            {
                fetchRecordProjectionbyCondition(table, temp, whereClause);
            }
            else
            {
                fetchRecordProjection(table, temp);
            }
        }
    }
}

vector<string> SQL::getColumns(string table)
{
    fstream f2;
    f2.open("Column.txt");
    int index = getIndex(tables, table);
    GotoLine(f2, index + 1);
    string line;
    f2 >> line;
    vector<string> columns = split(line);
    columns.erase(columns.begin());
    f2.close();
    return columns;
}

void SQL::fetchAll(string table)
{
    ifstream f1;
    f1.open(table + ".txt");
    vector<string> columns = getColumns(table);
    string temp;
    for (int i = 0; i < columns.size(); i++)
    {
        cout << columns[i] << "\t|\t";
    }
    cout << endl;
    while (getline(f1, temp))
    {
        vector<string> records = split(temp);
        for (int i = 0; i < records.size(); i++)
        {
            cout << records[i] << "\t|\t";
        }
        cout << endl;
    }
    f1.close();
}

void SQL::fetchRecordProjectionbyCondition(string table, vector<string> temp, vector<string> whereClause)
{
    ifstream f1;
    f1.open(table + ".txt");
    vector<string> columns = getColumns(table);
    vector<int> indexes;
    for (int i = 0; i < temp.size(); i++)
    {
        int index;
        auto it = find(columns.begin(), columns.end(), temp[i]);
        if (it != columns.end())
        {
            cout << temp[i] << "\t|\t";
            index = it - columns.begin();
            // cout << "index = " << index << endl;
            indexes.push_back(index);
        }
        else
        {
            cout << "Column \'" << temp[i] << "\' does not exists in table " << table << endl;
            return;
        }
    }
    cout << endl;

    vector<string> col, opr, val;
    for (int i = 0; i < whereClause.size(); i++)
    {
        whereClause[i].erase(
            remove(whereClause[i].begin(), whereClause[i].end(), '\''),
            whereClause[i].end());
        if (find(columns.begin(), columns.end(), whereClause[i]) != columns.end())
        {
            col.push_back(whereClause[i]);
        }
        else if (find(operators.begin(), operators.end(), whereClause[i]) != operators.end())
        {
            opr.push_back(whereClause[i]);
        }
        else
        {
            val.push_back(whereClause[i]);
        }
    }

    string line;
    int flag = 0, flag1 = 0;
    while (getline(f1, line))
    {
        flag = 1;
        vector<string> records = split(line);
        auto it = find(columns.begin(), columns.end(), col[0]);
        int index = it - columns.begin();
        for (int i = 0; i < indexes.size(); i++)
        {
            flag1 = 0;
            if (opr[0] == "=")
            {
                if (records[index] == val[0])
                {
                    cout << records[indexes[i]] << "\t|\t";
                    flag1 = 1;
                }
            }
            else if (opr[0] == "!=" || opr[0] == "<>")
            {
                if (records[index] != val[0])
                {
                    cout << records[indexes[i]] << "\t|\t";
                    flag1 = 1;
                }
            }
            else if (opr[0] == "<=")
            {
                if (records[index] <= val[0])
                {
                    cout << records[indexes[i]] << "\t|\t";
                    flag1 = 1;
                }
            }
            else if (opr[0] == ">=")
            {
                if (records[index] >= val[0])
                {
                    cout << records[indexes[i]] << "\t|\t";
                    flag1 = 1;
                }
            }
            else if (opr[0] == ">")
            {
                if (records[index] > val[0])
                {
                    cout << records[indexes[i]] << "\t|\t";
                    flag1 = 1;
                }
            }
            else if (opr[0] == "<")
            {
                if (records[index] < val[0])
                {
                    cout << records[indexes[i]] << "\t|\t";
                    flag1 = 1;
                }
            }
            else
            {
                flag1 = 0;
            }
        }
        if (flag1 == 1)
            cout << endl;
    }
    if (flag == 0)
    {
        cout << "No data found" << endl;
        return;
    }
    f1.close();
}

void SQL::fetchRecordProjection(string table, vector<string> temp)
{
    ifstream f1;
    f1.open(table + ".txt");
    vector<string> columns = getColumns(table);
    vector<int> indexes;
    for (int i = 0; i < temp.size(); i++)
    {
        int index;
        auto it = find(columns.begin(), columns.end(), temp[i]);
        if (it != columns.end())
        {
            cout << temp[i] << "\t|\t";
            index = it - columns.begin();
            // cout << "index = " << index << endl;
            indexes.push_back(index);
        }
        else
        {
            cout << "Column \'" << temp[i] << "\' does not exists in table " << table << endl;
            return;
        }
    }
    cout << endl;

    string line;
    int flag = 0;
    while (getline(f1, line))
    {
        flag = 1;
        vector<string> records = split(line);
        for (int i = 0; i < indexes.size(); i++)
        {
            cout << records[indexes[i]] << "\t|\t";
        }
        cout << endl;
    }
    if (flag == 0)
    {
        cout << "No data found" << endl;
        return;
    }
    f1.close();
}

void SQL::fetchAllbyCondition(string table, vector<string> whereClause)
{
    vector<string> columns = getColumns(table);
    vector<string> col, opr, val;
    for (int i = 0; i < whereClause.size(); i++)
    {
        whereClause[i].erase(
            remove(whereClause[i].begin(), whereClause[i].end(), '\''),
            whereClause[i].end());
        if (find(columns.begin(), columns.end(), whereClause[i]) != columns.end())
        {
            col.push_back(whereClause[i]);
        }
        else if (find(operators.begin(), operators.end(), whereClause[i]) != operators.end())
        {
            opr.push_back(whereClause[i]);
        }
        else
        {
            val.push_back(whereClause[i]);
        }
    }

    int flag = 0;
    ifstream f1;
    int index;
    f1.open(table + ".txt");
    string temp;
    for (int i = 0; i < columns.size(); i++)
    {
        cout << columns[i] << "\t|\t";
    }
    cout << endl;
    while (getline(f1, temp))
    {
        vector<string> records = split(temp);
        auto it = find(columns.begin(), columns.end(), col[0]);
        index = it - columns.begin();

        for (int i = 0; i < records.size(); i++)
        {
            flag = 0;
            if (opr[0] == "=")
            {
                if (records[index] == val[0])
                {
                    cout << records[i] << "\t|\t";
                    flag = 1;
                }
            }
            else if (opr[0] == "!=" || opr[0] == "<>")
            {
                if (records[index] != val[0])
                {
                    cout << records[i] << "\t|\t";
                    flag = 1;
                }
            }
            else if (opr[0] == "<")
            {
                if (records[index] < val[0])
                {
                    cout << records[i] << "\t|\t";
                    flag = 1;
                }
            }
            else if (opr[0] == ">")
            {
                if (records[index] > val[0])
                {
                    cout << records[i] << "\t|\t";
                    flag = 1;
                }
            }
            else if (opr[0] == "<=")
            {
                if (records[index] <= val[0])
                {
                    cout << records[i] << "\t|\t";
                    flag = 1;
                }
            }
            else if (opr[0] == ">=")
            {
                if (records[index] >= val[0])
                {
                    cout << records[i] << "\t|\t";
                    flag = 1;
                }
            }
            else
            {
                flag = 0;
            }
        }
        if (flag == 1)
            cout << endl;
    }
    f1.close();
}

void SQL ::updateFiles(string fileName, int index, vector<string> append_str)
{
    ofstream temp;
    string line;
    fstream f1;
    f1.open(fileName);
    temp.open("temp.txt");
    int count = 0;
    char *writable = new char[fileName.size() + 1];
    copy(fileName.begin(), fileName.end(), writable);
    writable[fileName.size()] = '\0'; // don't forget the terminating 0

    while (getline(f1, line))
    {
        if (count == index)
        {
            count++;
            if (append_str.size() == 1)
            {
                temp << line << append_str[0] << endl; // Check if "," is required or not
            }
            else
            {
                temp << line;
                for (int i = 0; i < append_str.size(); i++)
                {
                    temp << append_str[i] << "-";
                }
                temp << endl;
            }
        }
        else
        {
            temp << line << endl;
            count++;
        }
    }

    temp.close();
    f1.close();
    remove(writable);
    rename("temp.txt", writable);
    delete[] writable;
}
void SQL ::alterFiles(vector<string> command, int index, int flag) // flag = 0 for add 1 for drop 2 for rename 3 for modify
{
    // command starts from 4 from previous command
    if (flag == 0) // col_name datatype constraint/constraints
    {
        // Adding new col

        updateFiles("Column.txt", index, {command[0]});
        updateFiles("Datatype.txt", index, {command[1]});
        if (command.size() == 3)
        {
            if (isNumber(command[3]))
            {
                updateFiles("ConstrLength.txt", index, {command[2]});
            }
            updateFiles("Constraint.txt", index, {command[2]});
        }
        else if (command.size() > 3)
        {
            if (isNumber(command[3]))
            {
                if (command.size() == 5)
                    updateFiles("ConstrLength.txt", index, {command[3], command[4]});
                else
                    updateFiles("ConstrLength.txt", index, {command[3], "NA"});
            }
            else
            {
                updateFiles("ConstrLength.txt", index, vector<string>(command.begin() + 3, command.end()));
            }
        }
        else
        {
            updateFiles("Constraint.txt", index, {"NA"});
            updateFiles("ConstrLength.txt", index, {"0"});
        }
    }
    else if (flag == 1)
    {
        // Drop column
        // Alter table table_name drop column col_name
        fstream f1, f2, f3, f4;
        f1.open("Column.txt");
        f2.open("Constraint.txt");
        f3.open("ConstrLength.txt");
        f4.open("Datatype.txt");
        vector<vector<string>> Columns, Constraint, Constrlength, Datatype;
        Columns = splitFile(f1, tables[index], false);     // Check this
        Constraint = splitFile(f2, tables[index], true);   // Check this
        Constrlength = splitFile(f3, tables[index], true); // Check this
        Datatype = splitFile(f4, tables[index], false);    // Check this
        vector<string> columns;
        for (auto i : Columns)
        {
            columns.push_back(i[0]);
        }
        int index_1 = -1;
        for (int i = 0; i < columns.size(); i++)
        {
            if (columns[i] == command[0])
            {
                index_1 = i;
            }
        }
        auto it = columns.begin() + index_1;
        auto it1 = Constraint.begin() + index_1;
        auto it2 = Constrlength.begin() + index_1;
        auto it3 = Datatype.begin() + index_1;
        columns.erase(it);
        Constraint.erase(it1);
        Constrlength.erase(it2);
        Datatype.erase(it3);
        string replace_col, replace_cons, replace_len, replace_dt;
        replace_col = tables[index] + "#";
        replace_cons = tables[index] + "#";
        replace_len = tables[index] + "#";
        replace_dt = tables[index] + "#";
        for (auto i : columns)
        {
            replace_col += i + ",";
        }
        for (auto i : Datatype)
        {
            for (auto j : i)
                replace_dt += j + ",";
        }
        for (auto i : Constraint)
        {
            for (auto j : i)
                replace_cons += j + "-";
            replace_cons += ",";
        }
        for (auto i : Constrlength)
        {
            for (auto j : i)
                replace_len += j + "-";
            replace_len += ",";
        }
        ofstream temp;
        ofstream temp1;
        ofstream temp2;
        ofstream temp3;
        string line;
        string line1;
        string line2;
        string line3;
        string fileName = "Column.txt";
        string fileName1 = "Constraint.txt";
        string fileName2 = "ConstrLength.txt";
        string fileName3 = "Datatype.txt";
        temp.open("temp.txt");
        temp1.open("temp1.txt");
        temp2.open("temp2.txt");
        temp3.open("temp3.txt");
        int count = 0;
        int count1 = 0;
        int count2 = 0;
        int count3 = 0;
        char *writable = new char[fileName.size() + 1];
        copy(fileName.begin(), fileName.end(), writable);
        writable[fileName.size()] = '\0'; // don't forget the terminating 0
        char *writable1 = new char[fileName1.size() + 1];
        copy(fileName1.begin(), fileName1.end(), writable1);
        writable1[fileName1.size()] = '\0'; // don't forget the terminating 0
        char *writable2 = new char[fileName2.size() + 1];
        copy(fileName2.begin(), fileName2.end(), writable2);
        writable2[fileName2.size()] = '\0'; // don't forget the terminating 0
        char *writable3 = new char[fileName3.size() + 1];
        copy(fileName3.begin(), fileName3.end(), writable3);
        writable3[fileName3.size()] = '\0'; // don't forget the terminating 0
        while (getline(f1, line))
        {
            if (count == index)
            {
                count++;
                temp << replace_col << endl;
            }
            else
            {
                temp << line << endl;
                count++;
            }
        }
        while (getline(f2, line1))
        {
            if (count1 == index)
            {
                count1++;
                temp1 << replace_cons;
            }
            else
            {
                temp1 << line1 << endl;
                count1++;
            }
        }
        while (getline(f3, line2))
        {
            if (count2 == index)
            {
                count2++;
                temp2 << replace_len;
            }
            else
            {
                temp2 << line2 << endl;
                count2++;
            }
        }
        while (getline(f4, line3))
        {
            if (count3 == index)
            {
                count3++;
                temp3 << replace_dt;
            }
            else
            {
                temp3 << line3 << endl;
                count3++;
            }
        }
        temp.close();
        temp1.close();
        temp2.close();
        temp3.close();
        f1.close();
        f2.close();
        f3.close();
        f4.close();
        remove(writable);
        remove(writable1);
        remove(writable2);
        remove(writable3);
        rename("temp.txt", writable);
        rename("temp1.txt", writable1);
        rename("temp2.txt", writable2);
        rename("temp3.txt", writable3);
        delete[] writable;
        delete[] writable1;
        delete[] writable2;
        delete[] writable3;
    }
    else if (flag == 2)
    {
    }
    else
    {
    }
}
void SQL ::alterTable(vector<string> command)
{
    // Alter table table_name add col_name datatype
    // Alter table table_name drop column col_name
    // Alter table table_name rename col_name to new_col_name
    // Alter table table_name modify col_name new_datatype
    string table = command[2];

    if (notin(lower(table), tables))
    {
        cout << "Error : No such table created\n";
    }
    else
    {
        int index = getIndex(tables, table);
        string types = command[3];
        fstream f2("Column.txt");
        vector<vector<string>> columns = splitFile(f2, table, false);
        vector<string> Columns_main;
        f2.close();
        for (int i = 0; i < columns.size(); i++)
        {
            Columns_main.push_back(columns[i][0]);
        }
        if (lower(types) == "add")
        {
            if (notin(lower(command[4]), constraints))
            {
                if (notin(lower(command[4]), Columns_main))
                {
                    alterFiles(vector<string>(command.begin() + 4, command.end()), index, 0);
                    // Add col to files schema, column, constraint, constrlength, datatype
                }
                else
                {
                    cout << "Error : Column name already exists" << endl;
                }
            }
            else
            {
                alterFiles(vector<string>(command.begin() + 4, command.end()), index, 0);
            }
        }
        else if (lower(types) == "drop")
        {
            if (command.size() < 6)
            {
                cout << "Error: Mention Column name" << endl;
            }
            if (notin(lower(command[5]), Columns_main))
            {
                cout << "Error : No such column exists" << endl;
            }
            else
            {
                // delete col from files schema, column, constraint, constrlength, datatype
                alterFiles({command[5]}, index, 1);
            }
        }
        else if (lower(types) == "rename")
        {
            if (notin(lower(command[4]), Columns_main))
            {
                cout << "Error : No such column exists" << endl;
            }
            else
            {
                // update col_name to files schema, column, constraint, constrlength, datatype
                alterFiles(vector<string>(command.begin() + 4, command.end()), index, 2);
            }
        }
        else if (lower(types) == "modify")
        {
            if (notin(lower(command[4]), Columns_main))
            {
                cout << "Error : No such column exists" << endl;
            }
            else
            {
                // add constraints to col in files schema, column, constraint, constrlength, datatype
                alterFiles(vector<string>(command.begin() + 4, command.end()), index, 3);
            }
        }
        else
        {
            cout << "Syntax Error" << endl;
        }
    }
}
void SQL ::deleteRecords(vector<string> command)
{
    string table;
    cout << "In Delete Records" << endl;
    vector<string>::iterator itr = find(command.begin(), command.end(), "from");
    if (itr != end(command))
    {
        table = command[itr - command.begin() + 1]; // itr-vector.begin gives index of "from" element
    }
    else
    {
        cout << "Syntax Error";
    }

    // check if there is where clause present or not
    bool present;
    int index;
    vector<string> whereClause;
    vector<string>::iterator it = find(command.begin(), command.end(), "where");
    if (it != end(command))
    {
        present = true;
        index = it - command.begin(); // index of where keyword
        for (int i = index + 1; i < command.size(); i++)
        {
            if (command[i] == ";")
                continue;
            whereClause.push_back(command[i]);
        }
    }

    if (notin(lower(table), tables))
    {
        cout << "Error : No such table exists\n";
    }
    else
    {
        // delete from stud where name = 'PQRS';
        if (command.size() == 3)
        {
                deleteAll(table);
        }
        else
        {
            // select grno, name from stud;
            int index;
            auto it = find(command.begin(), command.end(), "from");
            if (it != command.end())
            {
                index = it - command.begin();
            }

            // get projected col names in temp vector
            vector<string> temp;
            for (int i = 1; i < index; i++)
                temp.push_back(command[i]); // temp = {grno,name}
            deleteAllbyCondition(table, whereClause);
            // else
            // {
            //     deleteRecordProjection(table, temp);
            // }
        }
    }
}
void SQL ::deleteAllbyCondition(string table, vector<string> whereClause)
{

    cout << "In Delete all by Condition" << endl;
    vector<string> columns = getColumns(table);
    vector<int> indexes;
    vector<string> col, opr, val;
    for (int i = 0; i < whereClause.size(); i++)
    {
        whereClause[i].erase(
            remove(whereClause[i].begin(), whereClause[i].end(), '\''),
            whereClause[i].end());
        if (find(columns.begin(), columns.end(), whereClause[i]) != columns.end())
        {
            col.push_back(whereClause[i]);
        }
        else if (find(operators.begin(), operators.end(), whereClause[i]) != operators.end())
        {
            opr.push_back(whereClause[i]);
        }
        else
        {
            val.push_back(whereClause[i]);
        }
    }

    // int flag = 0;
    ifstream f1;
    int index;
    f1.open(table + ".txt");
    string temp;
    for (int i = 0; i < columns.size(); i++)
    {
        cout << columns[i] << "\t|\t";
    }
    cout << endl;
    int count = 0;
    while (getline(f1, temp))
    {
        vector<string> records = split(temp);
        auto it = find(columns.begin(), columns.end(), col[0]);
        index = it - columns.begin();

        for (int i = 0; i < records.size(); i++)
        {
            // flag = 0;
            if (opr[0] == "=")
            {
                if (records[index] == val[0])
                {
                    // cout << records[i] << "\t|\t";
                    indexes.push_back(count);
                    // flag = 1;
                }
            }
            else if (opr[0] == "!=" || opr[0] == "<>")
            {
                if (records[index] != val[0])
                {
                    // cout << records[i] << "\t|\t";
                    indexes.push_back(count);
                    // flag = 1;
                }
            }
            else if (opr[0] == "<")
            {
                if (records[index] < val[0])
                {
                    // cout << records[i] << "\t|\t";
                    indexes.push_back(count);
                    // flag = 1;
                }
            }
            else if (opr[0] == ">")
            {
                if (records[index] > val[0])
                {
                    // cout << records[i] << "\t|\t";
                    indexes.push_back(count);
                    // flag = 1;
                }
            }
            else if (opr[0] == "<=")
            {
                if (records[index] <= val[0])
                {
                    // cout << records[i] << "\t|\t";
                    indexes.push_back(count);
                    // flag = 1;
                }
            }
            else if (opr[0] == ">=")
            {
                if (records[index] >= val[0])
                {
                    // cout << records[i] << "\t|\t";
                    indexes.push_back(count);
                    // flag = 1;
                }
            }
        }
        count++;
    }
    // for(auto i:indexes)
    // {
    //     cout << "Indexes " << i << endl; 
    // }
    f1.close();
    deleteFromFile(table, indexes);

}

void SQL ::deleteAll(string table)
{
    ofstream f1;
    f1.open(table + ".txt", std::ofstream::out | std::ofstream::trunc);
    f1.close();
}
void SQL ::deleteFromFile(string table, vector<int> indexes)
{
    cout << "In deleteFromFile" << endl;
    string fileName = table + ".txt";
    ofstream temp;
    string line;
    fstream f1;
    f1.open(fileName);
    temp.open("temp.txt");
    int count = 0;
    char *writable = new char[fileName.size() + 1];
    copy(fileName.begin(), fileName.end(), writable);
    writable[fileName.size()] = '\0'; // don't forget the terminating 0
    int i = 0;
    while (getline(f1, line))
    {
        if (count == indexes[i])
        {
            count++;
            if (i < indexes.size())
                i++;
        }
        else
        {
            temp << line << endl;
            count++;
        }
    }

    temp.close();
    f1.close();
    remove(writable);
    rename("temp.txt", writable);
    delete[] writable;
}

void SQL::update(vector<string> command)
{
    // UPDATE table_name set col_name1 = val1, col_name2 = val2 where col_name = val;
    for (int i = 0; i < command.size(); i++)
    {
        command[i].erase(
            remove(command[i].begin(), command[i].end(), '\''),
            command[i].end());
    }

    string table = command[1];
    if (notin(lower(table), tables))
    {
        cout << "Error : No such table exists\n";
    }
    else
    {
        vector<string> col, val;
        vector<string> columns = getColumns(table);
        int index = 0;
        vector<string>::iterator itr = find(command.begin(), command.end(), "where");
        if (itr != end(command))
        {
            index = itr - command.begin(); // itr-vector.begin gives index of "where" element
        }

        if (index == 0)
        {
            for (int i = 3; i < command.size(); i++) // i=3 because col names start from index 3 in update command
            {
                if (find(columns.begin(), columns.end(), command[i]) != columns.end())
                {
                    col.push_back(command[i]);
                }
                else if (notin(command[i], operators))
                {
                    // command[i].erase(
                    //     remove(command[i].begin(), command[i].end(), '\''),
                    //     command[i].end());
                    val.push_back(command[i]);
                }
            }
        }
        else
        {
            for (int i = 3; i < index; i++) // i=3 because col names start from index 3 in update command
            {
                if (find(columns.begin(), columns.end(), command[i]) != columns.end())
                {
                    col.push_back(command[i]);
                }
                else if (notin(command[i], operators))
                {
                    // command[i].erase(
                    //     remove(command[i].begin(), command[i].end(), '\''),
                    //     command[i].end());
                    val.push_back(command[i]);
                }
            }
        }

        vector<int> colIndexes;
        for (int i = 0; i < col.size(); i++)
        {
            vector<string>::iterator itr = find(columns.begin(), columns.end(), col[i]);
            if (itr != end(columns))
            {
                int index1 = itr - columns.begin();
                colIndexes.push_back(index1);
            }
        }

        // where condition
        string col_name, value;
        int col_index;
        if (index != 0) // means 'where' is present in command
        {
            col_name = command[index + 1];
            value = command[index + 3];
            // check index of col_name in columns vector
            vector<string>::iterator itr = find(columns.begin(), columns.end(), col_name);
            if (itr != end(columns))
            {
                col_index = itr - columns.begin();
            }
            else
            {
                cout << "Column \'" << col_name << "\' does not exists in table \'" << table << "\'" << endl;
            }
        }

        // Fetch records to upadte
        ifstream f1;
        ofstream f2;
        string fileName = table + ".txt";
        f1.open(table + ".txt");
        f2.open("temp.txt", ios::out);

        char *writable = new char[fileName.size() + 1];
        copy(fileName.begin(), fileName.end(), writable);
        writable[fileName.size()] = '\0'; // don't forget the terminating 0

        string temp;
        int count = 0;
        while (getline(f1, temp))
        {
            vector<string> records = split(temp);
            for (int i = 0; i < records.size(); i++)
            {
                if (index != 0) // where clause present and update specific record
                {
                    if (records[col_index] == value) // condition becomes true
                    {
                        for (int j = 0; j < colIndexes.size(); j++)
                        {
                            if (i == colIndexes[j]) // change specified column value
                            {
                                records[i] = val[j];
                                count++;
                            }
                        }
                    }
                    cout << records[i] << "\t|\t";
                    f2 << records[i] << "#";
                }
                else
                {
                    // update all records
                    for (int j = 0; j < colIndexes.size(); j++)
                    {
                        if (i == colIndexes[j]) // change specified column value
                        {
                            records[i] = val[j];
                            count++;
                        }
                    }
                    cout << records[i] << "\t|\t";
                    f2 << records[i] << "#";
                }
            }
            cout << endl;
            f2 << endl;
        }
        cout << count / colIndexes.size() << " rows updated" << endl;
        f1.close();
        f2.close();
        remove(writable);
        rename("temp.txt", writable);
        delete[] writable;
    }
}

void SQL::helpTables()
{
    cout << "Existing Tables - " << endl;
    cout << "-------------------------" << endl;
    for (int i = 0; i < tables.size(); i++)
    {
        if (tables[i].size() < 6)
        {
            cout << "|\t" << tables[i] << "\t\t|" << endl;
            cout << "-------------------------" << endl;
        }
        else
        {
            cout << "|\t" << tables[i] << "\t|" << endl;
            cout << "-------------------------" << endl;
        }
    }
}

void SQL::helpCommand(vector<string> command)
{
    if (lower(command[1]) == "create")
    {
        cout << "Syntax for \'Create Table\' command is - " << endl;
        cout << "\n----------------------------------------------------------------------------" << endl;
        cout << "CREATE TABLE table_name ( \n  attribute_1 attribute1_type(length limit), \n  attribute_2 attribute2_type(length limit) PRIMARY_KEY, \n  FOREIGN KEY ( attribute_y ) REFERENCES table_x ( attribute_t ), \n  ... \n);" << endl;
        cout << "--------------------------------------------------------------------------\n"
             << endl;
    }
    else if (lower(command[1]) == "drop")
    {
        cout << "Syntax for \'Drop Table\' command is - " << endl;
        cout << "\n-------------------------------------------" << endl;
        cout << "DROP TABLE table_name;" << endl;
        cout << "-------------------------------------------\n"
             << endl;
    }
    else if (lower(command[1]) == "insert")
    {
        cout << "Syntax for \'Insert\' command is - " << endl;
        cout << "\n---------------------------------------------------" << endl;
        cout << "INSERT INTO table_name VALUES ( val1, val2, ... );" << endl;
        cout << "---------------------------------------------------\n"
             << endl;
    }
    else if (lower(command[1]) == "delete")
    {
        cout << "Syntax for \'Insert\' command is - " << endl;
        cout << "\n---------------------------------------------------" << endl;
        cout << "DELETE FROM table_name WHERE condition_list;" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "\ncondition_list - contains condition to delete specific record\n"
             << endl;
    }
    else if (lower(command[1]) == "update")
    {
        cout << "Syntax for \'Update\' command is - " << endl;
        cout << "\n---------------------------------------------------" << endl;
        cout << "UPDATE table_name SET attr1 = val1, attr2 = val2 ... \nWHERE condition_list;" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "\ncondition_list - contains condition to update specific record\n"
             << endl;
    }
    else if (lower(command[1]) == "select")
    {
        cout << "Syntax for \'Select\' command is - " << endl;
        cout << "\n---------------------------------------------------------------" << endl;
        cout << "SELECT attribute_list FROM table_list WHERE condition_list;" << endl;
        cout << "---------------------------------------------------------------" << endl;
        cout << "\nattribute_list - contains attr names from the table or *";
        cout << "\ntable_list - contains table names from which you want to retrieve data";
        cout << "\ncondition_list - contains condition (where clause) to filter the data\n"
             << endl;
    }
    else
    {
        cout << "Syntax Error" << endl;
    }
}

void SQL::quit()
{
    cout << "Terminating the program ..." << endl;
    exit(3);
}

int main()
{
    SQL sql;
    while(true)
    {
        string s;
        cout << "\n\nEnter SQL command - (Enter \'quit\' for exiting) " << endl;
        getline(cin, s);
        sql.setQuery(s);
    }
    //"create table grade (id int primary_key, grade varchar(2)";
    //"quit;";
    // "help tables;"
    // "HELP select;";
    //"update stud set grno = 15155 where name = 'ben' ;"
    //"select * from stud where name < 'XYZ';"
    //"alter table phase drop column ttl";
    //"alter table phase add ttl int";
    //"drop table stud";
    // "select * from stud ;"
    // "insert into demo values (345135699999,'abcdef');";
    //"insert into stud values (11817, 'john');";
    //"create table stud(grno int primary_key, name varchar(20));";
    //"create table phase(phaseno int(3,2) primary_key, cardname varchar(20));"
    // cout << "Enter the SQL command :- ";
    // getline(cin, s);
    return 0;
}
