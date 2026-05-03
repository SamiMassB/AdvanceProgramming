#ifndef MAIN_HPP
#define MAIN_HPP
#include <bits/stdc++.h>
using namespace std;
typedef pair<string, vector<string>> OneOrderType;
const int NO_VAR = 1;
const int ONE_VAR = 2;
const int ONE_VAR_IN_QUOTA = 3;
const int LIST_VARS = 4;
const int LIST_VARS_IN_QUOTA = 5;


const string BAD_REQUEST = "Bad Request";
const string PERIMISSION_DENIED = "Permission Denied";
const string EMPTY = "Empty";
const string OK = "OK";
const string NOT_FOUND = "Not Found";
const string HOLIDAY = "Holiday Found";
const string OVERLAP = "Overlap";

const string COM_USERNAME = "username";
const string COM_PASSWORD = "password";
const string COM_DATE = "date";
const string COM_START_TIME = "start_time";
const string COM_POS = "POST";
const string COM_LOGIN = "login";
const string COM_QUESMARK = "?";
const string COM_SIGNUP = "signup";
const string COM_LOGOUT = "logout";
const string COM_EVENT = "event";
const string COM_DURATION = "duration";
const string COM_TITLE = "title";
const string COM_DESC = "description";
const string COM_PEREVE = "periodic_event";
const string COM_END_DATE = "end_date";
const string COM_START_DATE = "start_date";
const string COM_WDAYS = "week_days";
const string COM_TIME = "time";
const string COM_TYPE = "type";
const string COM_TASK = "task";
const string COM_GET = "GET";
const string COM_REPORT = "report";
const string COM_FROM = "from";
const string COM_TO = "to";
const string COM_TASKID = "task_id";
const string COM_PUT = "PUT";
const string COM_DELETE = "DELETE";
const string COM_DAY = "day";
const string COM_EVEID = "event_id";
const string COM_PEREVEID = "periodic_id";
const string COM_JOINEVE = "join_event";
const string COM_GUESTS = "guests";
const string COM_END_TIME = "end_time";
const string COM_INVID = "invitation_id";
const string COM_CONFJE = "confirm_join_event";
const string COM_REJJE = "reject_join_event";


const string VAR_DAILY = "Daily";
const string VAR_WEEKLY = "Weekly";
const string VAR_MONTHLY = "Monthly";
const string VAR_SUNDAY = "Sunday";
const string VAR_SATURDAY = "Saturday";
const string VAR_TUESDAY = "Tuesday";
const string VAR_WEDNESDAY = "Wednesday";
const string VAR_MONDAY = "Monday";
const string VAR_THURSDAY = "Thursday";
const string VAR_FRIDAY = "Friday";

const vector<string> VAR_WEEKDAYS = {VAR_SATURDAY, VAR_SUNDAY, VAR_MONDAY, VAR_TUESDAY, VAR_WEDNESDAY, VAR_THURSDAY, VAR_FRIDAY};
const vector<string> POSSIBLE_TYPES = {COM_TASK, COM_EVENT, COM_PEREVE, COM_JOINEVE};

class Exception{
    public:
        Exception(string command){str = command;}
        string GetCommand(){return str;}
    private:
        string str;
};


#endif

