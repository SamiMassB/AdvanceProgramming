#include "Date.hpp"
#include "main.hpp"
const int MONTH_DAYS = 30;
const int YEAR_MONTH = 12;

Date::Date(string date_string){
    istringstream iss(date_string);
    string d, y, m;
    getline(iss, y, DATE_DIVIDER);
    getline(iss, m, DATE_DIVIDER);
    getline(iss, d);
   
    day_ = stoi(d);
    year_ = stoi(y);
    month_ = stoi(m);
    if(GetDate() < BASE_DATE){
        throw Exception(BAD_REQUEST);
    }
}
Date::Date(int d, int m, int y){
    day_ = d;
    year_ = y;
    month_ = m;
   
}
void Date::AddOneDay(){
    if(day_ != MONTH_DAYS)
        day_ += 1;
    else if(month_ != YEAR_MONTH){
        month_ += 1;
        day_ = 1;
    }
    else{
        year_ += 1;
        month_ = 1;
        day_ = 1;
    }
}

void Date::AddOneMonth(){
    for(int i = 0; i < MONTH_DAYS; i++)
        AddOneDay();

}
string Date::GetDate() const{
    string m = to_string(month_);
    string d = to_string(day_);
    string y = to_string(year_);
    if(d.size() == 1)
        d = "0" + d;
    if(m.size() == 1)
        m = "0" + m;
    if(y.size() < 4){
        string x= "";
        for(int j = 0; j < 4 - (int)y.size(); j++)
            x += "0";
        y = x + y;

    }
    return y + DATE_DIVIDER + m + DATE_DIVIDER + d;
}
vector<string> Date::HOLIDAYS;
void Date::GetHolidays(const string& filename){
    ifstream file(filename);
    const char DATE_INPUT_DIVIDER = ',';
    const int TEMP_INDEX = 0;
    string line, var, date, d, y, m;
    getline(file, line);
    while(getline(file, line)){
        istringstream iss(line);
        date = "";
        getline(iss, d, DATE_INPUT_DIVIDER);
        getline(iss, m, DATE_INPUT_DIVIDER);
        getline(iss, y, DATE_INPUT_DIVIDER);
        if(m.size() == 1)
            m = "0" + m;
        if(d.size() == 1)
            d = "0" + d;
        date = y + DATE_DIVIDER + m + DATE_DIVIDER + d;
        Date::HOLIDAYS.push_back(date);
        
    }
}
int Date::FindWeekDay(const Date& x){
    int res = BASE_DAY;
    Date now_date(BASE_DATE);
    while(now_date != x){
        res++;
        res %= WEEK_DAYS_NUMBER;
        now_date.AddOneDay();
    }
    return res;


}