#ifndef DATE_HPP
#define DATE_HPP
#include <bits/stdc++.h>
using namespace std;
const int SATURDAY = 1;
const int SUNDAY = 2;
const int MONDAY = 3;
const int TUESDAY = 4;
const int WEDNESDAY = 5;
const int THURSDAY = 6;
const int FRIDAY = 0;
const string DAILY = "Daily";
const string WEEKLY = "Weekly";
const string MONTHLY = "Monthly";
const string BASE_DATE = "1404/01/01";
const int WEEK_DAYS_NUMBER = 7;
const int BASE_DAY = FRIDAY;
const char DATE_DIVIDER = '/';
typedef pair<string, vector<string>> OneOrderType;
class Date{
    public:
        static int FindWeekDay(const Date& x);
        Date(string date_string);
        Date(int d, int m, int y);
        void AddOneDay();
        void AddOneMonth();
        bool operator==(const Date& d1) const{return GetDate() == d1.GetDate();};
        bool operator!=(const Date& d1) const{return !(*this == d1);}
        bool operator<(const Date& d1) const{return (GetDate() < d1.GetDate());};
        string GetDate() const;
        tuple<int, int, int> GetDateInt() const{return make_tuple(year_, month_, day_);};
        static void GetHolidays(const string& filename);
        static bool IsHoliday(const string& x){return (find(HOLIDAYS.begin(), HOLIDAYS.end(), x) != HOLIDAYS.end());};    
    private:
        static vector<string> HOLIDAYS;
        
        
        int day_, year_, month_;
        
};
#endif