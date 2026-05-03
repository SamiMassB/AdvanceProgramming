#include "User.hpp"
#include "events.hpp"
#include "main.hpp"
#include "System.hpp"
#include "Date.hpp"


string System::GetUsername(){
    return cur_user_->GetName();
}
System::System(){
    START_ORDERS.push_back(COM_POS);
    START_ORDERS.push_back(COM_GET);
    START_ORDERS.push_back(COM_DELETE);
    START_ORDERS.push_back(COM_PUT);
    func_map[make_tuple(COM_POS, COM_LOGIN, COM_QUESMARK)] = &System::LoginUser;
    func_map[make_tuple(COM_POS, COM_SIGNUP, COM_QUESMARK)] = &System::SignupUser;
    func_map[make_tuple(COM_POS, COM_LOGOUT, COM_QUESMARK)] = &System::LogoutUser;
    func_map[make_tuple(COM_POS, COM_EVENT, COM_QUESMARK)] = &System::AddEvent;
    func_map[make_tuple(COM_POS, COM_TASK, COM_QUESMARK)] = &System::AddTask;
    func_map[make_tuple(COM_POS, COM_PEREVE, COM_QUESMARK)] = &System::AddPerEvent;
    func_map[make_tuple(COM_GET, COM_REPORT, COM_QUESMARK)] = &System::Report;
    func_map[make_tuple(COM_PUT, COM_TASK, COM_QUESMARK)] = &System::EditTask;
    func_map[make_tuple(COM_DELETE, COM_TASK, COM_QUESMARK)] = &System::DeleteTask;
    func_map[make_tuple(COM_POS, COM_JOINEVE, COM_QUESMARK)] = &System::AddJoinEvent;
    func_map[make_tuple(COM_POS, COM_CONFJE, COM_QUESMARK)] = &System::ConfEvent;
    func_map[make_tuple(COM_POS, COM_REJJE, COM_QUESMARK)] = &System::RejEvent;
    func_map[make_tuple(COM_GET, COM_JOINEVE, COM_QUESMARK)] = &System::GetJoinEvent;
    InitializeCommands();
    cur_user_ = nullptr;
    join_event_id_ = 1;
    
}
void System::InitializeCommands(){
    orders_[COM_USERNAME] = ONE_VAR_IN_QUOTA;
    orders_[COM_PASSWORD] = ONE_VAR_IN_QUOTA;
    orders_[COM_DATE] = ONE_VAR;
    orders_[COM_START_TIME] = ONE_VAR;
    orders_[COM_POS] = NO_VAR;
    orders_[COM_LOGIN] = NO_VAR;
    orders_[COM_LOGOUT] = NO_VAR;
    orders_[COM_DURATION] = ONE_VAR;
    orders_[COM_TITLE] = ONE_VAR_IN_QUOTA;
    orders_[COM_QUESMARK] = NO_VAR;
    orders_[COM_SIGNUP] = NO_VAR;
    orders_[COM_EVENT] = NO_VAR;
    orders_[COM_DESC] = ONE_VAR_IN_QUOTA;
    orders_[COM_END_DATE] = ONE_VAR;
    orders_[COM_START_DATE] = ONE_VAR;
    orders_[COM_GET] = NO_VAR;
    orders_[COM_REPORT] = NO_VAR;
    orders_[COM_FROM] = ONE_VAR;
    orders_[COM_TO] = ONE_VAR;
    orders_[COM_TASK] = NO_VAR;
    orders_[COM_TIME] = ONE_VAR;
    orders_[COM_TYPE] = ONE_VAR;
    orders_[COM_TASKID] = ONE_VAR;
    orders_[COM_PUT] = NO_VAR;
    orders_[COM_DELETE] = NO_VAR;
    orders_[COM_WDAYS] = LIST_VARS;
    orders_[COM_PEREVE] = NO_VAR;
    orders_[COM_DAY] = ONE_VAR;
    orders_[COM_EVEID] = ONE_VAR;
    orders_[COM_PEREVEID] = ONE_VAR;
    
    orders_[COM_JOINEVE] = NO_VAR;
    orders_[COM_REJJE] = NO_VAR;
    orders_[COM_INVID] = ONE_VAR;
    orders_[COM_END_TIME] = ONE_VAR;
    orders_[COM_CONFJE] = NO_VAR;
    orders_[COM_REJJE] = NO_VAR;
    orders_[COM_GUESTS] = LIST_VARS_IN_QUOTA;
}
void System::MakeInputVector(vector<OneOrderType> &vec, string command_line){
    
    istringstream iss(command_line);
    string command, var;
    while(iss >> command){
      
        vec.push_back(make_pair(command, vector<string>()));
        if(orders_[command] == 0)
            continue;
        if(orders_[command] == NO_VAR)
            continue;
        if(orders_[command] == ONE_VAR){
            iss >> var;
            vec.back().second.push_back(var);
        }
        else if(orders_[command] == ONE_VAR_IN_QUOTA){
            getline(iss, var, '"');
          
            getline(iss, var, '"');
            vec.back().second.push_back(var);
        }
        else if(orders_[command] == LIST_VARS){
            iss >> var;
            istringstream days(var);
            while(getline(days, var, ','))
                vec.back().second.push_back(var);
        }
        else if(orders_[command] == LIST_VARS_IN_QUOTA){
            getline(iss, var, '\"');
            getline(iss, var, '\"');
            istringstream vars(var);
            while(getline(vars, var, ','))
                vec.back().second.push_back(var);

        }
    }


}
vector<string> System::GetCommandString(string command_line){
    vector<OneOrderType> orders;
    MakeInputVector(orders, command_line);
    return MessageHandler(orders);
}

vector<string> System::GetJoinEvent(vector<OneOrderType>& orders){
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    
    return cur_user_->GetJoinEvent(orders);


}
vector<string> System:: AddJoinEvent(vector<OneOrderType> & orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    auto itr = find_if(orders.begin(), orders.end(), [](const OneOrderType& x){return x.first == COM_GUESTS;});
    if(itr == orders.end())
        throw Exception(BAD_REQUEST);
    vector<string> guests = itr->second;
    for(auto guest: guests)
        if(FindUser(guest) == nullptr){
            throw Exception(NOT_FOUND);
        }
    
    auto task = cur_user_->AddJoinEvent(orders, join_event_id_, 0);
    task->SetId(join_event_id_);
    for(auto guest: guests){
        auto user = FindUser(guest);
        user->AddReq(join_event_id_, task);
    }
    event_host_[join_event_id_] = cur_user_->GetName();
    cur_user_->AddReqToQueue(join_event_id_, task);
    join_event_id_ += 1;
    
    res.push_back(OK);
    return res;
}
vector<string> System::ConfEvent(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    cur_user_->AcceptJoinEvent(orders);
    auto host = FindUser(event_host_[stoi(find_command(orders, COM_INVID)->second[0])]);
    if(host == nullptr)
        throw Exception(NOT_FOUND);
    host->AddForHost(stoi(find_command(orders, COM_INVID)->second[0]));
   res.push_back(OK);
   return res;
}
vector<string> System::RejEvent(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    cur_user_->DeclineJoinEvent(orders);
    res.push_back(OK);
    return res;
}
shared_ptr<User> System::FindUser(string name){
    for(auto i: users_)
        if(i->GetName() == name)
            return i;
    return nullptr;

}
vector<string> System::LoginUser(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ != nullptr)
        throw Exception(PERIMISSION_DENIED);
    auto username = find_if(orders.begin(), orders.end(), [](const OneOrderType& order){return order.first == COM_USERNAME;});
    auto password = find_if(orders.begin(), orders.end(), [](const OneOrderType& order){return order.first == COM_PASSWORD;});
    shared_ptr<User> temp_user;
    if(username == orders.end() || password == orders.end())
        throw Exception(BAD_REQUEST);
    temp_user = FindUser(username->second[0]);
    if(temp_user == nullptr)
        throw Exception(NOT_FOUND);
    if(temp_user->GetName() != username->second[0] || temp_user->GetPass() != password->second[0])
        throw Exception(PERIMISSION_DENIED);
    cur_user_ = temp_user;
    res.push_back(OK);
    return res;
}
vector<string> System::LogoutUser(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    if(orders.size() != 0)
        throw Exception(BAD_REQUEST);
    cur_user_ = nullptr;
    res.push_back(OK);
    return res;
}
vector<string> System::SignupUser(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ != nullptr)
        throw Exception(PERIMISSION_DENIED);
    const int ORDERS_SIZE = 2;
    auto username = find_if(orders.begin(), orders.end(), [](OneOrderType order){return order.first == COM_USERNAME;});
    auto password = find_if(orders.begin(), orders.end(), [](OneOrderType order){return order.first == COM_PASSWORD;});
    shared_ptr<User> temp_user;
    if(username == orders.end() || password == orders.end())
        throw Exception(BAD_REQUEST);
    
    temp_user = FindUser(username->second[0]);
    if(temp_user != nullptr)
        throw Exception(BAD_REQUEST);
    users_.push_back(make_shared<User>(username->second[0], password->second[0]));
    cur_user_ = users_.back();
    res.push_back(OK);
    return res;
}
vector<string> System::MessageHandler(vector<OneOrderType>& orders){
    const int ORDERS_SIZE = 3;
    if(orders.size() < ORDERS_SIZE){
        throw Exception(BAD_REQUEST);
    }
    if(orders[2].first != COM_QUESMARK)
        throw Exception(BAD_REQUEST);
    if(find(START_ORDERS.begin(), START_ORDERS.end(), orders[0].first) == START_ORDERS.end())
        throw Exception(BAD_REQUEST);
    if(func_map[make_tuple(orders[0].first, orders[1].first, orders[2].first)] == NULL)
        throw Exception(NOT_FOUND);
    
    vector<OneOrderType> sub_orders;
    for(int i = ORDERS_SIZE; i < orders.size(); i++)
        sub_orders.push_back(orders[i]);
    map<string, int> duplicate_order;
    for(auto i: sub_orders){
        if(duplicate_order[i.first] != 0)
            throw Exception(BAD_REQUEST);
        duplicate_order[i.first] = 1;
    }
    
    return (this->*func_map[make_tuple(orders[0].first, orders[1].first, orders[2].first)])(sub_orders);
}
vector<string> System::AddTask(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    cur_user_->AddTask(orders);
    res.push_back(OK);
    return res;
}
vector<string> System::AddEvent(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    cur_user_->AddEvent(orders);
    res.push_back(OK);
    return res;
}
vector<string> System::AddPerEvent(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    cur_user_->AddPerEvent(orders);
    res.push_back(OK);
    return res;
}

vector<string> System::Report(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    res = cur_user_->Report(orders);
    return res;
}
vector<string> System::EditTask(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    cur_user_->EditTask(orders);
    res.push_back(OK);
    return res;
}
vector<string> System::DeleteTask(vector<OneOrderType>& orders){
    vector<string> res;
    if(cur_user_ == nullptr)
        throw Exception(PERIMISSION_DENIED);
    cur_user_->DeleteTask(orders);
    res.push_back(OK);
    return res;
}
