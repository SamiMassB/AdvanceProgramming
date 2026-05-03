#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <bits/stdc++.h>
#include <memory>
using namespace std;
typedef pair<string, vector<string>> OneOrderType;
class User;


class System{
    private:
        map<string, int> orders_;
        vector <string> START_ORDERS;
        vector<shared_ptr<User>> users_;
        shared_ptr<User> cur_user_;
        map<tuple<string, string, string>, vector<string> (System::*)(vector<OneOrderType>& orders)> func_map;
        map<int, string> event_host_;
        int join_event_id_;
    public:
        System();
        shared_ptr<User> FindUser(string name);
        vector<string> SignupUser(vector<OneOrderType>& orders);
        vector<string> LogoutUser(vector<OneOrderType>& orders);
        vector<string> LoginUser(vector<OneOrderType>& orders);
        vector<string> AddTask(vector<OneOrderType>& orders);
        vector<string> AddEvent(vector<OneOrderType>& orders);
        vector<string> AddPerEvent(vector<OneOrderType>& orders);
        vector<string> MessageHandler(vector<OneOrderType>& orders);
        vector<string> Report(vector<OneOrderType>& orders);
        vector<string> EditTask(vector<OneOrderType>& orders);
        vector<string> DeleteTask(vector<OneOrderType>& orders);
        vector<string> AddJoinEvent(vector<OneOrderType>& orders);
        vector<string> ConfEvent(vector<OneOrderType>& orders);
        vector<string> RejEvent(vector<OneOrderType>& orders);
        vector<string> GetJoinEvent(vector<OneOrderType>& orders);
        vector<string> GetCommandString(string inp);
        string GetUsername();
        void MakeInputVector(vector<OneOrderType>& orders, string command_line);
        void InitializeCommands();
        
};
#endif