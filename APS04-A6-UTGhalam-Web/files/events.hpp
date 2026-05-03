#ifndef EVENTS_HPP
#define EVENTS_HPP
#include <bits/stdc++.h>
using namespace std;
typedef pair<string, vector<string>> OneOrderType;



class Task{
    protected:
        
        map<string, function<void(vector<string>)>> order_func_;
        string date_, title_, description_ = "", time_;
    private:
        vector<string> NECESSARY_ORDERS; 
    public:
        static const pair<int, int> TASK_PERIOD;     
        Task(){};
        Task(vector<OneOrderType>& orders, int initialize);
        void HandleCommand(vector<OneOrderType>& orders, int initialize);
        void SetTitle(vector<string>& order){title_ = order[0];}
        void SetTime(vector<string>& order);
        void SetDescription(vector<string>& order){description_ = order[0];}
        void SetDate(vector<string>& order){date_ = order[0];}
        vector<string> GetDates();
        virtual string Report(string type, string day);
        virtual bool Validate(vector<OneOrderType>& orders){return 1;};
        virtual pair<int, int> GetPeriod(){return Task::TASK_PERIOD;};
        virtual vector<string> GetNessaryOrders(){return NECESSARY_ORDERS;};
        virtual tuple<string, string, string> GetTypeTitle();
        virtual string ReportJoinEvent(){return "";};
    };
class Event : public Task{
    protected:
       
        string start_time_, duration_;

    private:
        vector<string> NECESSARY_ORDERS; 
    public:     
        Event(){};
        Event(vector<OneOrderType>& orders, int initialize);
        void SetDuration(vector<string>& order);
        void SetStartTime(vector<string>& order);
        vector<string> GetDates();
        virtual string Report(string type, string day);
        virtual bool Validate(vector<OneOrderType>& orders){return 1;};
        virtual pair<int, int> GetPeriod(){return make_pair(stoi(start_time_), stoi(start_time_) + stoi(duration_));};
        virtual vector<string> GetNessaryOrders(){return NECESSARY_ORDERS;};
        virtual tuple<string, string, string> GetTypeTitle();
    };
class PeriodicEvent : public Event{
    protected:
        
        string start_date_, end_date_, type_, day_;
        vector<string> week_days_;
    private:
        map<string, int> rel_day_int_;
        vector<string> NECESSARY_ORDERS; 
        map<string, function<bool(vector<OneOrderType>&)>> validate_type_;
    public:
        PeriodicEvent(){};
        PeriodicEvent(vector<OneOrderType>& orders, int initialize);
        void SetStartDate(vector<string>& order){start_date_ = order[0];}
        void SetEndDate(vector<string>& order){end_date_ = order[0];}
        void SetType(vector<string>& order){type_ = order[0];}
        void SetWeekDays(vector<string>& order){week_days_ = order;};
        void SetDay(vector <string> order){day_ = order[0];};
        vector<string> GetDates();
        virtual string Report(string type, string day);
        virtual bool Validate(vector<OneOrderType>& orders);
        virtual pair<int, int> GetPeriod(){return make_pair(stoi(start_time_), stoi(start_time_) + stoi(duration_));};
        virtual tuple<string, string, string> GetTypeTitle();
        virtual vector<string> GetNessaryOrders(){return NECESSARY_ORDERS;};
};
class JoinEvent : public Task{
    protected:
        string start_time_, duration_, end_time_, host_;
        vector<string> guests_;
        int event_id_;
    private:
        vector<string> NECESSARY_ORDERS; 
    public:
        JoinEvent(){};
        JoinEvent(vector<OneOrderType>& orders, int initialize);
        void SetStartTime(vector<string>& order);
        void SetEndTime(vector<string>& order);
        void SetGuests(vector<string>& order);
        vector<string> GetDates();
        virtual string Report(string type, string day);
        virtual bool Validate(vector<OneOrderType>& orders){return 1;};
        virtual pair<int, int> GetPeriod(){return make_pair(stoi(start_time_), stoi(end_time_));};
        virtual tuple<string, string, string> GetTypeTitle();
        virtual vector<string> GetNessaryOrders(){return NECESSARY_ORDERS;};
        virtual string ReportJoinEvent();
        void SetHost(string name){host_ = name;};
        void SetId(int id){event_id_ = id;};


};
bool IsTaskOverlap(const shared_ptr<Task>& T1, const shared_ptr<Task>& T2);
const OneOrderType* find_command(vector<OneOrderType>& orders, string command);
#endif
