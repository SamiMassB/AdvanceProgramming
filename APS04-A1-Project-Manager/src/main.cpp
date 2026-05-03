#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;
#define MAX_TASK 1000
#define MAX_WORKER 1000
#define ADD_TASK "add_task"
#define ADD_EMPLOYEE "add_employee"
#define ASSIGN_EMPLOYEE "assign_employee"
#define ONGOING "ONGOING"
#define DONE "DONE"
#define TODO "TODO"
#define FINISH_TASK "finish_task"
#define REPORT_ALL "all"
#define REPORT_EMPLOYEE "employee"
#define REPORT "report"
#define REPORT_ONGOING "ongoing"
#define ONGO_VER 1
#define FIN_VER 2
#define START_VER 0
#define END "END"
void add_task(vector <string> &tasks, map<string, int> &task_id, int task_prio[MAX_TASK]){
    string task_name;
    int prio;
    cin >> task_name >> prio;
    tasks.push_back(task_name);
    task_id[task_name] = tasks.size();
    task_prio[tasks.size()] = prio;
}
void add_employee(vector <string> &workers, map<string, int> &worker_id){
    string emplo_name;
    cin >> emplo_name;
    workers.push_back(emplo_name);
    worker_id[emplo_name] = workers.size();
}
void assign_employee(vector <string> task_workers[MAX_TASK], int task_ver[MAX_TASK], vector<string> workers_task[MAX_TASK], map<string, int> task_id, map<string, int> worker_id){
    string task_name, emp_name;
    cin >> task_name >> emp_name;
    task_workers[task_id[task_name]].push_back(emp_name);
    task_ver[task_id[task_name]] = ONGO_VER;
    workers_task[worker_id[emp_name]].push_back(task_name);
}
void finish_task(int task_ver[MAX_TASK], map<string, int> task_id){
    string task_name;
    cin >> task_name;
    task_ver[task_id[task_name]] = FIN_VER;
}
void report_all(vector <string> tasks, map <string, int> task_id, int task_ver[MAX_TASK]){
    int type[3] = {0, 0, 0};
    for(int i = 0; i < tasks.size(); i++)
        type[task_ver[task_id[tasks[i]]]] += 1;
    cout << TODO << ": " << type[START_VER] << endl;
    cout << ONGOING << ": " << type[ONGO_VER] << endl;
    cout << DONE << ": " << type[FIN_VER] << endl;
}
void report_employee(vector <string> workers_task[MAX_TASK], map <string, int> worker_id, map<string, int> task_id, int task_ver[MAX_TASK]){
    int task_comp = 0;
    vector <string> ongo_task;
    string emp_name;
    cin >> emp_name;
   // cout << workers_task[worker_id[emp_name]].size() << endl;

    for(int i = 0; i < workers_task[worker_id[emp_name]].size(); i++){
        string cur_task = workers_task[worker_id[emp_name]][i];
        if(task_ver[task_id[cur_task]] == FIN_VER)
            task_comp += 1;
        else
            ongo_task.push_back(cur_task);
    }
    cout << emp_name << " has done " << task_comp <<" tasks." << endl;
    if(ongo_task.size() == 0){
        cout << emp_name << " is currently not working on any tasks." << endl;
        return;
    }
    cout << emp_name << " is currently working on these tasks:" << endl;
    for(int i = 0; i < ongo_task.size(); i++){
        string cur_task = ongo_task[i];
        cout << i + 1 << ". " << cur_task << endl;
    }


}
void report_ongoing(int task_prio[MAX_TASK], vector <string> tasks, int task_ver[MAX_TASK], vector <string> task_workers[MAX_WORKER], map<string, int> task_id){
    vector <pair<int, string>> ongo_task;
    vector <string> t_workers;
    int task;
    for(int i = 0; i < tasks.size(); i++){
        task = task_id[tasks[i]];
        if(task_ver[task] == ONGO_VER)
            ongo_task.push_back(make_pair(task_prio[task], tasks[i]));
    }
//  I used pair to sort tasks but there are several ways to do that with custmized sorting such as, bubble_sort, using struct and etc.
//  But i decided to choose pair for clearness of my code
    sort(ongo_task.begin(), ongo_task.end());

    for(int i = 0; i < ongo_task.size(); i++){
        t_workers.clear();
        cout << ongo_task[i].second << " (" << ongo_task[i].first << "): ";
        for(int j = 0; j < task_workers[task_id[ongo_task[i].second]].size(); j++){
            t_workers.push_back(task_workers[task_id[ongo_task[i].second]][j]);
            }
        sort(t_workers.begin(), t_workers.end());
        for(int j = 0; j < t_workers.size(); j++){
            cout << t_workers[j];
            if(j != t_workers.size() - 1)
                cout << ", ";
            }

        cout << endl;
    }


}
int main(){
    vector <string> tasks, workers, task_workers[MAX_WORKER];
    vector <string> workers_task[MAX_TASK];
    map<string, int> task_id, worker_id;
    int task_ver[MAX_TASK], task_prio[MAX_TASK];
    string operation;
    bool done = 0;
    while(cin >> operation){

        if(operation == ADD_TASK){
            add_task(tasks, task_id, task_prio);
        }
        else if(operation == ADD_EMPLOYEE){
            add_employee(workers, worker_id);
        }
        else if(operation == ASSIGN_EMPLOYEE){
            assign_employee(task_workers, task_ver, workers_task, task_id, worker_id);
        }
        else if(operation == FINISH_TASK){
            finish_task(task_ver, task_id);
        }
        else if(operation == REPORT){
            cin >> operation;
            if(operation == REPORT_ALL)
                report_all(tasks, task_id, task_ver);
            else if(operation == REPORT_EMPLOYEE)
                report_employee(workers_task, worker_id, task_id, task_ver);
            else if(operation == REPORT_ONGOING)
                report_ongoing(task_prio, tasks, task_ver, task_workers, task_id);
        }

      //  cout << "Done" << endl;


    }


}
