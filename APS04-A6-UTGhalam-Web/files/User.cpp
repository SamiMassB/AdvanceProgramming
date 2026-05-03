#include "User.hpp"
#include "events.hpp"
#include "main.hpp"
#include "System.hpp"
#include "Date.hpp"




User::User(string name, string pass){
    name_ = name;
    password_ = pass;
    task_id_ = FIRST_ID;
    event_id_ = FIRST_ID;
    perevent_id_ = FIRST_ID;
    tasks_.clear();
}
vector<string> User::GetJoinEvent(vector<OneOrderType>& orders){
    vector<pair<string, shared_ptr<Task>>> pos;
    vector<string> res;
    bool empty = true;

    for(auto task: event_requests_){
        if(task.second == nullptr)
            continue;
    
        res.push_back(task.second->ReportJoinEvent());
       
        empty = false;
    }
    if(empty)
        throw Exception(EMPTY);
    return res;

}
void User::AddTask(vector<OneOrderType>& orders){
    auto task = make_shared<Task>(orders, 1);
    auto dates = task->GetDates();
    if(dates.size() == 0)
        throw Exception(BAD_REQUEST);
    
    for(string date: dates)
        tasks_[date].push_back(task);
    tasks_by_id_[make_pair(COM_TASK, task_id_)] = task;
    task_id_ += 1;
}
void User::GetTaskId(vector<OneOrderType>& orders, string& id, const string& command){
    auto find_taskid = [command](const OneOrderType& x){return x.first == command;};
    auto itr = find_if(orders.begin(), orders.end(), find_taskid);
    if(itr == orders.end())
        throw Exception(BAD_REQUEST);
    id = itr->second[0];
    orders.erase(remove_if(orders.begin(), orders.end(), find_taskid), orders.end());
    
    
}
void User::DeleteTask(vector<OneOrderType>& orders){
    string id, date;
    GetTaskId(orders, id, COM_TASKID);
    auto task = tasks_by_id_[make_pair(COM_TASK, stoi(id))];
    if(task == nullptr || task == NULL)
        throw Exception(NOT_FOUND);
    date = task->GetDates()[0];
    tasks_[date].erase(remove(tasks_[date].begin(), tasks_[date].end(), task), tasks_[date].end());
    tasks_by_id_[make_pair(COM_TASK, stoi(id))] = nullptr;
}
void User::AddEvent(vector<OneOrderType>& orders){

    auto task = make_shared<Event>(orders, 1);
    auto dates = task->GetDates();
    if(dates.size() == 0)
        throw Exception(BAD_REQUEST);
    for(string date: dates)
        for(auto day_task: tasks_[date])
            if(IsTaskOverlap(task, day_task))
                throw Exception(OVERLAP);
    for(string date: dates)
        if(Date::IsHoliday(date))
            throw Exception(HOLIDAY);
    
    for(string date: dates)
        tasks_[date].push_back(task);
    tasks_by_id_[make_pair(COM_EVENT, event_id_)] = task;
    event_id_ += 1;
}
void User::AddPerEvent(vector<OneOrderType>& orders){
    bool is_holiday = false;
    auto task = make_shared<PeriodicEvent>(orders, 1);
    auto dates = task->GetDates();
    if(dates.size() == 0)
        throw Exception(BAD_REQUEST);
    for(string date: dates)
        for(auto day_task: tasks_[date])
            if(IsTaskOverlap(task, day_task))
                throw Exception(OVERLAP);
    for(string date: dates){
        if(Date::IsHoliday(date) == 0){
            tasks_[date].push_back(task);
        }
        else
            is_holiday = 1;
    }
    tasks_by_id_[make_pair(COM_PEREVE, perevent_id_)] = task;
    perevent_id_ += 1;
    if(is_holiday)
        throw Exception(HOLIDAY);
}
vector<string> User::Report(vector<OneOrderType>& orders){
    vector<string> res;
    string start_str = BASE_DATE, type = "", end_str = "";
    bool something_reported = 0;
    auto itr = find_if(orders.begin(), orders.end(), [](const OneOrderType& order){return order.first == COM_FROM;});
    if(itr != orders.end())
        start_str = itr->second[0];
    itr = find_if(orders.begin(), orders.end(), [](const OneOrderType& order){return order.first == COM_TYPE;});
    if(itr != orders.end())
        type = itr->second[0];
    itr = find_if(orders.begin(), orders.end(), [](const OneOrderType& order){return order.first == COM_TO;});
    if(itr != orders.end())
        end_str = itr->second[0];
    else
        throw Exception(BAD_REQUEST);
    if(type != "" && find(POSSIBLE_TYPES.begin(), POSSIBLE_TYPES.end(), type) == POSSIBLE_TYPES.end())
        throw Exception(BAD_REQUEST);
    Date start(start_str);
    Date end(end_str);
    end.AddOneDay();
    for(auto date = start; date != end; date.AddOneDay()){
        sort(tasks_[date.GetDate()].begin(), tasks_[date.GetDate()].end(), [](const shared_ptr<Task> & t1, const shared_ptr<Task>& t2)
                                    {
                                        auto p1 = t1->GetTypeTitle();
                                     auto p2 = t2->GetTypeTitle();
                                    if(get<2>(p1) != get<2>(p2)){
                                        return (stoi(get<2>(p1)) < stoi(get<2>(p2)));
                                        
                                    }
                                    if(get<0>(p1) == get<0>(p2))
                                        return (get<1>(p1) < get<1>(p2));
                                    if(get<0>(p1) == COM_JOINEVE)
                                        return true;
                                    if(get<0>(p2) == COM_JOINEVE)
                                        return false;
                                    if(get<0>(p1) == COM_PEREVE)
                                        return true;
                                    if(get<0>(p2) == COM_PEREVE)
                                        return false;
                                    
                                    return (get<0>(p1) < get<0>(p2));
            }) ;
        for(auto task: tasks_[date.GetDate()]){
            string rep = task->Report(type, date.GetDate());
            if(rep.size())
                res.push_back(rep);
        }
      

    }
    if(res.size() == 0)
        throw Exception(EMPTY);
    return res;

}
shared_ptr<JoinEvent> User::AddJoinEvent(vector<OneOrderType>& orders, int id, bool add_task, shared_ptr<Task> rel_task){
    shared_ptr<JoinEvent> task;
    if(rel_task == nullptr){
        task = make_shared<JoinEvent>(orders, 1);
    }
    else
        task = dynamic_pointer_cast<JoinEvent> (rel_task);
    
    auto dates = task->GetDates();
    if(dates.size() == 0)
        throw Exception(BAD_REQUEST);
    for(string date: dates)
        for(auto day_task: tasks_[date])
            if(IsTaskOverlap(task, day_task))
                throw Exception(OVERLAP);
    for(string date: dates)
        if(Date::IsHoliday(date))
            throw Exception(HOLIDAY);
    if(rel_task == nullptr)
        task->SetHost(name_);
    if(add_task){
        for(string date: dates){
            tasks_[date].push_back(task);
           
        }
        tasks_by_id_[make_pair(COM_JOINEVE, id)] = task;
    }
    return task;
}
void User::AcceptJoinEvent(vector<OneOrderType>& orders){
    auto itr = find_command(orders, COM_INVID);

    if(itr == nullptr)
        throw Exception(BAD_REQUEST);
    int id = stoi(itr->second[0]);
    auto task = event_requests_[id];
    if(event_requests_[id] == nullptr)
        throw Exception(NOT_FOUND);
    event_requests_[id] = nullptr;
    AddJoinEvent(orders, id, 1, task);
}
void User::DeclineJoinEvent(vector<OneOrderType>& orders){
    auto itr = find_command(orders, COM_INVID);
    if(itr == nullptr)
        throw Exception(BAD_REQUEST);
    int id = stoi(itr->second[0]);
    if(event_requests_[id] == nullptr)
        throw Exception(NOT_FOUND);
    event_requests_[id] = nullptr;
}
void User::AddReqToQueue(int id, shared_ptr<Task> task){
    join_event_queue_[id] = task;
}
void User::AddForHost(int id){

    if(join_event_queue_[id] == nullptr)
        return;
    vector<OneOrderType> orders;

    AddJoinEvent(orders, id, 1, join_event_queue_[id]);
    join_event_queue_[id] = nullptr;
}
void User::EditTask(vector<OneOrderType>& orders){
    string id, date;
    GetTaskId(orders, id, COM_TASKID);
    auto task = tasks_by_id_[make_pair(COM_TASK, stoi(id))];
    if(task == nullptr || task == NULL )
        throw Exception(NOT_FOUND);
    if(orders.size() == 0)
        throw Exception(BAD_REQUEST);
    date = task->GetDates()[0]; 
    tasks_[date].erase(remove(tasks_[date].begin(), tasks_[date].end(), task), tasks_[date].end());
    task->HandleCommand(orders, 0);
    date = task->GetDates()[0];

    tasks_[date].push_back(task);

}




