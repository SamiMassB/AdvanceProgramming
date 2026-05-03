#ifndef USER_HPP
#define USER_HPP
#include <bits/stdc++.h>
using namespace std;
typedef pair<string, vector<string>> OneOrderType;
class Task;
class JoinEvent;


class User{
    private:
        const int FIRST_ID = 1;
        
        string name_, password_;
        map<string, vector<shared_ptr<Task>>> tasks_;
        map<pair<string, int>, shared_ptr<Task>> tasks_by_id_;
        map<int, shared_ptr<Task>> event_requests_, join_event_queue_;
        int task_id_, event_id_, perevent_id_;
    public:
        User(string name, string pass);
        string GetName(){return name_;}
        string GetPass(){return password_;}
        void AddTask(vector<OneOrderType>& orders);
        void AddEvent(vector<OneOrderType>& orders);
        void AddPerEvent(vector<OneOrderType>& orders);
        vector<string> Report(vector<OneOrderType>& orders);
        void EditTask(vector<OneOrderType>& orders);
        void GetTaskId(vector<OneOrderType>& orders, string& id, const string& command);
        void DeleteTask(vector<OneOrderType>& orders);
        shared_ptr<JoinEvent> AddJoinEvent(vector<OneOrderType>& orders, int id, bool add_task, shared_ptr<Task> rel_task = nullptr);
        void AddReq(int id, shared_ptr<Task> task){event_requests_[id] = task;};
        void AddReqToQueue(int id, shared_ptr<Task> task);
        void AddForHost(int id);
        void AcceptJoinEvent(vector<OneOrderType>& orders);
        void DeclineJoinEvent(vector<OneOrderType>& orders);
        vector<string> GetJoinEvent(vector<OneOrderType>& orders);
};
#endif