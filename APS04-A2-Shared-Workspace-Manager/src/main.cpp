#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
const string SHOW_TABLE_INFO="show_table_info";
const string ENTER="enter";
const string RESERVE_TABLE="reserve_table";
const string EXIT="exit";
const string SWITCH="switch";
const int UNDEFIEND=-1;
typedef struct Table_Conf Table;
typedef struct Student_Conf Student;
typedef struct System_Conf System;
void erase_from_table(System &system, int ind, int id);
int Type_Bonus(System &system, int t_id);
int get_score_by_id(System &system, int s_id, int t_id);
int Distance_To_Friend(System &system, int id, int t_id);
int Distance_To_Enemy(System &system, int id, int t_id);
void reserve_table(System &system);
int get_table_by_id(vector <Table> tables, int id);
int find_student_by_id(vector <Student> students, int id);
void sort_table(System &system, int s_id);
void show_table_info(System &system);
void show_tables(System &system);
void exit_table(System &system);
void switch_friend(System &system);
struct Student_Conf{
    int id, friend_id, enemy_id, cur_table = -1;
    string name;
};
struct Table_Conf{
    int id, X, Y, capacity;
    vector <int> people, queue;
    string type;
};
struct System_Conf{
    vector <Table> tables;
    vector <Student> students;
    map <string, int> type_score;
};
int get_table_by_id(vector <Table> tables, int id){
    for(int i = 0; i < tables.size(); i++)
        if(tables[i].id == id)
            return i;
    return UNDEFIEND;

}
int find_student_by_id(vector <Student> students, int id){
    for(int i = 0; i < students.size(); i++)
        if(students[i].id == id)
            return i;
    return UNDEFIEND;


}

void sort_table(System &system, int s_id) {
    for(int i = 0; i < system.tables.size(); i++)
        for(int j = 0; j < system.tables.size() - 1; j++){
            Table t1 = system.tables[j];
            Table t2 = system.tables[j + 1];
            int score1 = get_score_by_id(system, s_id, t1.id);
            int score2 = get_score_by_id(system, s_id, t2.id);
            if (score1 == score2) {
                if(t1.id > t2.id)
                    swap(system.tables[j], system.tables[j + 1]);
            } else {
                if(score1 < score2)
                    swap(system.tables[j], system.tables[j + 1]);
            }

    
        }
    
}

void show_table_info(System &system){
    int t_id, t_ind, person;
    string person_name;
    Table table;
    vector <string> s_names;
    cin >> t_id;
    t_ind = get_table_by_id(system.tables, t_id);
    table = system.tables[t_ind];
    cout << "Table ID: " << table.id << endl;
    cout << "People at the table: ";
    for(int i = 0; i < table.people.size(); i++){
        person = table.people[i];
        person_name = system.students[find_student_by_id(system.students, person)].name;
        s_names.push_back(person_name);
        
    }
    sort(s_names.begin(), s_names.end());
    for(int i = 0; i < s_names.size(); i++){
        person_name = s_names[i];
        if(i != table.people.size() - 1)
            cout << person_name << ", ";
        else
            cout << person_name;
    }
    cout << endl;
    cout << "Table remaining capacity: " << table.capacity - table.people.size() << endl;
    cout << "Waiting queue length: " << table.queue.size() << endl;
}
void show_tables(System &system){
    int s_id;
    cin >> s_id;
    
    sort_table(system, s_id);
    for(auto i: system.tables)
        cout << "Table " << i.id << ": " << i.capacity - i.people.size() << ' ' << i.queue.size() << endl;
        
}
void reserve_table(System &system){
    string temp;
    int s_id, t_id = UNDEFIEND, t_ind, s_ind;
    getline(cin, temp);
    stringstream s(temp);
    s >> s_id >> t_id;
    
    sort_table(system, s_id);
    if(t_id == -1){
        t_id = system.tables[0].id;
    }
    

    t_ind = get_table_by_id(system.tables, t_id);
    s_ind = find_student_by_id(system.students, s_id);
    
    if(system.tables[t_ind].capacity > system.tables[t_ind].people.size()){
        system.tables[t_ind].people.push_back(s_id);
        system.students[s_ind].cur_table = t_id;
        cout << system.students[s_ind].name << " sits at table " << t_id << endl;
        return;

    }
    system.tables[t_ind].queue.push_back(s_id);
    
    cout << system.students[s_ind].name << " enters the waiting queue of table " << t_id << endl;
    
}

void exit_table(System &system){
    int s_id, s_ind, t_id, t_ind; 
    string name;
    cin >> s_id;
    s_ind = find_student_by_id(system.students, s_id);
    t_id = system.students[s_ind].cur_table;
    if(t_id == -1){
        cout << "You are not signed in any table" << endl;
        return;
    }
    t_ind = get_table_by_id(system.tables, t_id);
    name = system.students[s_ind].name;    
    erase_from_table(system, s_ind, s_id);
    system.students[s_ind].cur_table = -1;
    if(system.tables[t_ind].queue.size()){
        int q_id = system.tables[t_ind].queue[0];
        for(int i: system.tables[t_ind].queue)
            if(i == system.students[s_ind].friend_id)
                q_id = i;
        system.tables[t_ind].people.push_back(q_id);
        s_id = q_id;
        s_ind = find_student_by_id(system.students, s_id);
        system.students[s_ind].cur_table = t_id;
        system.tables[t_ind].queue.erase(system.tables[t_ind].queue.begin());

    }
    cout << name << " exits!" << endl;
  
}
void erase_from_table(System &system, int ind, int id){
    int t_ind;
    ind = find_student_by_id(system.students, id);
    t_ind = get_table_by_id(system.tables, system.students[ind].cur_table);
    for(auto i = system.tables[t_ind].people.begin(); i < system.tables[t_ind].people.end(); i++)
        if((*i) == id){
            system.tables[t_ind].people.erase(i);
            return;
        }


}
void switch_friend(System &system){
    int s_id, f_id, s_ind, f_ind, t_ind, tf_ind;
    cin >> s_id;
    s_ind = find_student_by_id(system.students, s_id);
    f_id = system.students[s_ind].friend_id;
    f_ind = find_student_by_id(system.students, f_id);
    
    erase_from_table(system, s_ind, s_id);
    erase_from_table(system, f_ind, f_id);
    t_ind = get_table_by_id(system.tables, system.students[s_ind].cur_table);
    tf_ind = get_table_by_id(system.tables, system.students[f_ind].cur_table);
    system.tables[t_ind].people.push_back(f_id);
    system.tables[tf_ind].people.push_back(s_id);
    
    swap(system.students[s_ind].cur_table, system.students[f_ind].cur_table);
    cout << system.students[s_ind].name << ' ' << "switches seats with " << system.students[f_ind].name << "!" << endl;
   
 
}
int Distance_To_Enemy(System &system, int id, int t_id){
    int ind, e_id, e_ind, et_id, et_ind, t_ind, score;
    Table eTable, sTable;
    ind = find_student_by_id(system.students, id);
    e_id = system.students[ind].enemy_id;
    e_ind = find_student_by_id(system.students, e_id);
    et_ind = system.students[e_ind].cur_table;
    if(et_ind == UNDEFIEND)
        return 0;
    t_ind = get_table_by_id(system.tables, t_id);
    et_ind = get_table_by_id(system.tables, et_ind);
    eTable = system.tables[et_ind];
    sTable = system.tables[t_ind];
    score = abs(eTable.X - sTable.X) + abs(eTable.Y - sTable.Y);
    return score;

}
int Distance_To_Friend(System &system, int id, int t_id){
    int ind, f_id, f_ind, ft_id, ft_ind, t_ind, score;
    Table eTable, sTable;
    ind = find_student_by_id(system.students, id);
    f_id = system.students[ind].friend_id;
    f_ind = find_student_by_id(system.students, f_id);
    ft_ind = system.students[f_ind].cur_table;
    if(ft_ind == UNDEFIEND)
        return 0;
    
    t_ind = get_table_by_id(system.tables, t_id);
    ft_ind = get_table_by_id(system.tables, ft_ind);
    eTable = system.tables[ft_ind];
    sTable = system.tables[t_ind];
    score = abs(eTable.X - sTable.X) + abs(eTable.Y - sTable.Y);
    return score;

}
int Type_Bonus(System &system, int t_id){
    int t_ind;
    t_ind = get_table_by_id(system.tables, t_id);
    return system.type_score[system.tables[t_ind].type];
}
int get_score_by_id(System &system, int s_id, int t_id ){
    int score = 0;
    
    score = Distance_To_Enemy(system, s_id, t_id) - Distance_To_Friend(system, s_id, t_id) + Type_Bonus(system, t_id);

    return score;


}
void get_tables(System &system, string file_name){
    fstream file;
    Table table;
    int id, X, Y, Cap;
    string type, line, temp;
    file.open(file_name, ios::in);
    if(file.is_open() == 0)
        cout << "No file found" << endl;
    getline(file, line);
    while(getline(file, line)){
        stringstream s(line);
        getline(s, temp, ',');
        id = stoi(temp);
        getline(s, temp, ',');
        X = stoi(temp);
        getline(s, temp, ',');
        Y = stoi(temp);
        getline(s, temp, ',');
        Cap = stoi(temp);
        getline(s, temp, ',');
        type = temp;


        
        table.id = id;
        table.X = X;
        table.Y = Y;
        table.capacity = Cap;
        table.type = type;
        system.tables.push_back(table);
    }

}
void get_students(System &system, string file_name){
    fstream file;
    Student student;
    int roll, id, f_id, e_id;
    string name, line, temp;
    file.open(file_name, ios::in);
    if(file.is_open() == 0)
        cout << "No file found" << endl;
    getline(file, line);
    while(getline(file, line)){
        stringstream s(line);
        getline(s, temp, ',');
        id = stoi(temp);
        getline(s, temp, ',');
        name = temp;
        getline(s, temp, ',');
        f_id = stoi(temp);
        getline(s, temp, ',');
        e_id = stoi(temp);

        
        student.id = id;
        student.name = name;
        student.friend_id = f_id;
        student.enemy_id = e_id;
        system.students.push_back(student);
    }



}
void set_type_scores(System &system){
    system.type_score["window"] = 6;
    system.type_score["door"] = 4;
    system.type_score["middle"] = 2; 


}
void set_orders(map <string, function<void(System&)>> &mp){
    mp[SHOW_TABLE_INFO] = show_table_info;
    mp[ENTER] = show_tables;
    mp[RESERVE_TABLE] = reserve_table;
    mp[EXIT] = exit_table;
    mp[SWITCH] = switch_friend;
}

int main(int argc, char** argv){
    System system;
    string command;
    map <string, function<void(System&)>> orders;
    get_tables(system, argv[1]);
    get_students(system, argv[2]);
 
    set_type_scores(system);
    set_orders(orders);
    while(cin >> command){
        if(orders[command] == NULL)
            cout << "No command found" << endl;
        else   
            orders[command](system);
    }
     




}
