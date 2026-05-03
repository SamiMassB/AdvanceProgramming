#include "events.hpp"
#include "User.hpp"
#include "events.hpp"
#include "main.hpp"
#include "System.hpp"
#include "Date.hpp"



const pair<int, int> Task::TASK_PERIOD = make_pair(-1, -1);
tuple<string, string, string> Task::GetTypeTitle(){return make_tuple(COM_TASK, title_, time_);};
tuple<string, string, string> Event::GetTypeTitle(){return make_tuple(COM_EVENT, title_, start_time_);};
tuple<string, string, string> PeriodicEvent::GetTypeTitle(){return make_tuple(COM_PEREVE, title_, start_time_);};
tuple<string, string, string> JoinEvent::GetTypeTitle(){return make_tuple(COM_JOINEVE, title_, start_time_);};
Task::Task(vector<OneOrderType>& orders, int initialize){
    NECESSARY_ORDERS.push_back(COM_DATE);
    NECESSARY_ORDERS.push_back(COM_TIME);
    NECESSARY_ORDERS.push_back(COM_TITLE);
    order_func_[COM_DATE]  = [this](vector<string> v){SetDate(v);};
    order_func_[COM_TIME]  = [this](vector<string> v){SetTime(v);};
    order_func_[COM_TITLE] = [this](vector<string> v){SetTitle(v);};
    order_func_[COM_DESC]  = [this](vector<string> v){SetDescription(v);};
    HandleCommand(orders, initialize);
}
string JoinEvent::ReportJoinEvent(){
    string rep = "";
    rep += to_string(event_id_) + ": ";
    rep += "\"" + title_ + "\" - " + date_ + " - " + start_time_ + " - " + end_time_;
    return rep;
}
JoinEvent::JoinEvent(vector<OneOrderType>& orders, int initialize){
    NECESSARY_ORDERS.push_back(COM_DATE);
    NECESSARY_ORDERS.push_back(COM_TITLE);
    NECESSARY_ORDERS.push_back(COM_START_TIME);
    NECESSARY_ORDERS.push_back(COM_END_TIME);
    NECESSARY_ORDERS.push_back(COM_GUESTS);
    order_func_[COM_DATE]  = [this](vector<string> v){SetDate(v);};
    order_func_[COM_TITLE] = [this](vector<string> v){SetTitle(v);};
    order_func_[COM_DESC]  = [this](vector<string> v){SetDescription(v);};
    order_func_[COM_GUESTS] = [this](vector<string> v){SetGuests(v);};
    order_func_[COM_START_TIME] = [this](vector<string> v){SetStartTime(v);};
    order_func_[COM_END_TIME] = [this](vector<string> v){SetEndTime(v);};
    HandleCommand(orders, initialize);
}
void Task::SetTime(vector<string>& orders){
    time_ = orders[0];
    if(stoi(time_) < 0 || stoi(time_) >= 24)
        throw Exception(BAD_REQUEST);

}
void Event::SetStartTime(vector<string>& orders){
    start_time_ = orders[0];
    if(stoi(start_time_) < 0 || stoi(start_time_) >= 24)
        throw Exception(BAD_REQUEST);

}
void JoinEvent::SetStartTime(vector<string>& orders){
    start_time_ = orders[0];
    if(stoi(start_time_) < 0 || stoi(start_time_) >= 24)
        throw Exception(BAD_REQUEST);
}
void JoinEvent::SetEndTime(vector<string>& orders){
    end_time_ = orders[0];
    if(stoi(end_time_) < 0 || stoi(end_time_) >= 24)
        throw Exception(BAD_REQUEST);
}
void JoinEvent::SetGuests(vector<string>& orders){
    guests_ = orders;
    if(guests_.size() == 0)
        throw Exception(BAD_REQUEST);
}
const OneOrderType* find_command(vector<OneOrderType>& orders, string command){
    auto itr = find_if(orders.begin(), orders.end(), [&command](const OneOrderType& x){return (x.first == command);});
    if (itr != orders.end()) 
        return &(*itr); 
    
    return nullptr;

}
void Task::HandleCommand(vector<OneOrderType>& orders, int initialize){
    for(auto i: orders){
        if(order_func_[i.first] == NULL)
            continue;
        try{
           
            order_func_[i.first](i.second);
        }
        catch(Exception x){
            throw x;
        }
        catch(...){
            throw Exception(BAD_REQUEST);
        }
    }
    if(Validate(orders) == 0)
        throw Exception(BAD_REQUEST);
    if(initialize){
        for(auto i: GetNessaryOrders())
            if(find_if(orders.begin(), orders.end(), [i](OneOrderType x){return x.first == i;}) == orders.end()){
                throw Exception(BAD_REQUEST);

            }
    }
}
Event::Event(vector<OneOrderType>& orders, int initialize){
    NECESSARY_ORDERS.push_back(COM_DATE);
    NECESSARY_ORDERS.push_back(COM_DURATION);
    NECESSARY_ORDERS.push_back(COM_TITLE);
    NECESSARY_ORDERS.push_back(COM_START_TIME);
    order_func_[COM_DATE]       = [this](vector<string> v){SetDate(v);};
    order_func_[COM_START_TIME] = [this](vector<string> v){SetStartTime(v);};
    order_func_[COM_TITLE]      = [this](vector<string> v){SetTitle(v);};
    order_func_[COM_DURATION]   = [this](vector<string> v){SetDuration(v);};
    order_func_[COM_DESC]       = [this](vector<string> v){SetDescription(v);};
    HandleCommand(orders, initialize);
}
bool ValidateDaily(vector<OneOrderType>& orders){
    if(find_command(orders, COM_DAY) != nullptr)
        return 0;
    if(find_command(orders, COM_WDAYS) != nullptr)
        return 0;
    return 1;
}
bool ValidateMonthly(vector<OneOrderType>& orders){
    if(find_command(orders, COM_DAY) == nullptr)
        return 0;
    if(find_command(orders, COM_WDAYS) != nullptr)
        return 0;
    return 1;    
}
bool ValidateWeekly(vector<OneOrderType>& orders){
    auto itr = find_if(orders.begin(), orders.end(), [](const OneOrderType& x){return (x.first == COM_WDAYS);});
    if(itr == orders.end()) return 0; 
    for(auto i: itr->second) 
        if(find(VAR_WEEKDAYS.begin(), VAR_WEEKDAYS.end(), i) == VAR_WEEKDAYS.end())
            return 0;
    if(find_command(orders, COM_DAY) != nullptr)
            return 0;
    if(itr->second.size() == 0)
        return 0;
    return 1;
}
string JoinEvent::Report(string type, string day_date){

    if(type != "" && type != COM_JOINEVE)
        return "";
    string des = description_;
    if(des.size() == 0)
        des = "";
    else    
        des = "\"" + description_ + "\"";
    string rep = "";

    rep = "join_event \"" + title_ + "\" on " + day_date + " from " + start_time_;
    rep += " to " + end_time_ + " hosted by " + "\"" + host_ + "\": " + des;
    return rep;

}
PeriodicEvent::PeriodicEvent(vector<OneOrderType>& orders, int initialize){
    NECESSARY_ORDERS.push_back(COM_START_DATE);
    NECESSARY_ORDERS.push_back(COM_END_DATE);
    NECESSARY_ORDERS.push_back(COM_START_TIME);
    NECESSARY_ORDERS.push_back(COM_DURATION);
    NECESSARY_ORDERS.push_back(COM_TYPE);
    NECESSARY_ORDERS.push_back(COM_TITLE);
    order_func_[COM_START_DATE] = [this](vector<string> v){SetStartDate(v);};
    order_func_[COM_END_DATE] = [this](vector<string> v){SetEndDate(v);};
    order_func_[COM_START_TIME] = [this](vector<string> v){SetStartTime(v);};
    order_func_[COM_TITLE] = [this](vector<string> v){SetTitle(v);};
    order_func_[COM_TYPE] = [this](vector<string> v){SetType(v);};
    order_func_[COM_WDAYS] = [this](vector<string> v){SetWeekDays(v);};
    order_func_[COM_DURATION] = [this](vector<string> v){SetDuration(v);};
    order_func_[COM_DESC] = [this](vector<string> v){SetDescription(v);};
    order_func_[COM_DAY] = [this](vector<string> v){SetDay(v);};
    validate_type_[VAR_DAILY] = ValidateDaily;
    validate_type_[VAR_MONTHLY] = ValidateMonthly;
    validate_type_[VAR_WEEKLY] = ValidateWeekly;
    rel_day_int_[VAR_SATURDAY] = SATURDAY;
    rel_day_int_[VAR_SUNDAY] = SUNDAY;
    rel_day_int_[VAR_MONDAY] = MONDAY;
    rel_day_int_[VAR_TUESDAY] = TUESDAY;
    rel_day_int_[VAR_WEDNESDAY] = WEDNESDAY;
    rel_day_int_[VAR_THURSDAY] = THURSDAY;
    rel_day_int_[VAR_FRIDAY] = FRIDAY;
    HandleCommand(orders, initialize);


}
vector<string> Task::GetDates(){
    vector<string> rel_dates;
    rel_dates.push_back(date_);
    return rel_dates;
}
vector<string> JoinEvent::GetDates(){
    vector<string> rel_dates;
    rel_dates.push_back(date_);
    return rel_dates;
}
vector<string> Event::GetDates(){
    vector<string> rel_dates;
    rel_dates.push_back(date_);
    return rel_dates;
}
vector<string> PeriodicEvent::GetDates(){
    vector<string> rel_dates;
    int start_day;
    Date start(start_date_);
    Date end(end_date_);
    
   

    if(start.GetDate() >= end.GetDate())
        throw Exception(BAD_REQUEST);
    
    if(type_ == DAILY){
        end.AddOneDay();
        for(auto date = start; date != end; date.AddOneDay())
            rel_dates.push_back(date.GetDate());
        }
    else if(type_ == WEEKLY){
        
        end.AddOneDay();
        start_day = Date::FindWeekDay(start);
        
        for(auto date = start; date != end; date.AddOneDay()){
            
            if(find_if(week_days_.begin(), week_days_.end(), 
                [&start_day, this](const string& x){return (rel_day_int_[x] == start_day);}) != week_days_.end())
                rel_dates.push_back(date.GetDate());
            start_day = (start_day + 1) % WEEK_DAYS_NUMBER;
        }
    }
    else if(type_ == MONTHLY){
        end.AddOneDay();
        while(start < end && get<2>(start.GetDateInt()) != stoi(day_))
            start.AddOneDay();
        for(auto date = start; date < end; date.AddOneMonth())
            rel_dates.push_back(date.GetDate());
    }
    else    
        throw Exception(BAD_REQUEST);
    return rel_dates;


}
string Task::Report(string type, string day_date){
    if(type != "" && type != COM_TASK){
        return "";
    }
    string des = description_;
    if(des.size() == 0)
        des = "";
    else    
        des = "\"" + description_ + "\"";
    string rep = "";
    rep += "task \"" + title_ + "\" on " + day_date + " at " + time_ + ": " + des;
    return rep;
}
string Event::Report(string type, string day_date){
    if(type != "" && type != COM_EVENT)
        return "";
    string des = description_;
    if(des.size() == 0)
        des = "";
    else    
        des = "\"" + description_ + "\"";
    string rep = "";
    rep += "event \"" + title_ + "\" on " + day_date + " from " + start_time_;
    rep += " for " + duration_ + " hours: " + des ;
    return rep;
}
string PeriodicEvent::Report(string type, string day_date){
    if(type != "" && type != COM_PEREVE)
        return "";
    string des = description_;
    if(des.size() == 0)
        des = "";
    else    
        des = "\"" + description_ + "\"";
    string rep = "";
    rep  += "periodic_event \"" + title_ + "\" on " + day_date + " from " + start_time_;
    rep += " for " + duration_ + " hours " + type_ + ": " + des;
    return rep;
}
void Event::SetDuration(vector<string>& orders){
    if(stoi(orders[0]) <= 0)
        throw Exception(BAD_REQUEST);
    duration_ = orders[0];

}
bool IsTaskOverlap(const shared_ptr<Task>& T1, const shared_ptr<Task>& T2){
    auto baze1 = T1->GetPeriod();
    auto baze2 = T2->GetPeriod();
    if(baze1 == Task::TASK_PERIOD || baze2 == Task::TASK_PERIOD)
        return 0;
    if(baze1.second <= baze2.first || baze2.second <= baze1.first)
        return 0;
 
    return 1;
}
bool PeriodicEvent::Validate(vector<OneOrderType>& orders){
   
    auto itr = find_if(orders.begin(), orders.end(), [](const OneOrderType& x){return (x.first == COM_TYPE);});
    if(itr == orders.end())
        return 0;
    
    if(validate_type_[itr->second[0]] == NULL)
        return 0;
  
    return (validate_type_[itr->second[0]])(orders);


}






