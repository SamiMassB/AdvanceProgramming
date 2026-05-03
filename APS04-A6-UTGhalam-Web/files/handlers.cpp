#include "handlers.hpp"
#include "./main.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;
const string ERROR_PAGE = "ERROR";
const string OK_PAGE = "OK";
const string ACC_REQ = "accept";
const string REJ_REQ = "decline";
void AddSingleOrder(string c1, vector<string> c2, vector<OneOrderType>& orders){
    OneOrderType order;
    order.first = c1;
    order.second = c2;
    orders.push_back(order);

}
void AddCommands(string c1, string c2, string c3, vector<OneOrderType>&orders){
    OneOrderType order;
    order.first = c1;
    order.second = {};
    orders.push_back(order);

    order.first = c2;
    order.second = {};
    orders.push_back(order);

    order.first = c3;
    order.second = {};
    orders.push_back(order);

}

Response* RandomNumberHandler::callback(Request* req) {
    Response* res = new Response();
    res->setHeader("Content-Type", "text/html");

    std::string randomNumber = std::to_string(std::rand() % 10 + 1);
    std::string body;

    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";

    body += "<head>";
    body += "  <title>Random Number Page</title>";
    body += "</head>";

    body += "<body style=\"text-align: center;\">";
    body += "  <h1>AP HTTP</h1>";
    body += "  <p>A random number in [1, 10] is: " + randomNumber + "</p>";
    body += "  <p>SessionId: " + req->getSessionId() + "</p>";
    body += "</body>";

    body += "</html>";
    res->setBody(body);
    return res;
}
Response* ResultPageHandler(string page, string result, string type ){
    Response* res = new Response();
    res->setHeader("Content-Type", "text/html");
    string body = "";

    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";

    body += "<head>";
    body += "<meta charset=\"UTF-8\">";
    body += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    body += "<title>Colors Page</title>";
    body += "<style>";
    body += "body { margin: 0; height: 100vh; display: flex; align-items: center; justify-content: center; }";
    body += ".center { text-align: center; }";
    body += "</style>";
    body += "</head>";

    body += "<body>";
    body += "<div class=\"center\">";
    body += "<h1>Following result received:</h1>";
    if (type == ERROR_PAGE)
        body += "<h5 style=\"color:red\">" + result + "</h5>";
    else if (type == OK_PAGE)
        body += "<h5 style=\"color:green\">" + result + "</h5>";

    body += "<a href=\"" + page + "\"><input type=\"submit\" value=\"OK\"></a>";
    body += "</div>";
    body += "</body>";

    body += "</html>";
    res->setBody(body);
    return res;
}
Response* ShowRequests::callback(Request* req){
    Response* res = new Response();
   
    res->setHeader("Content-Type", "text/html");
    string body = "";
    vector<OneOrderType> orders;
    vector<string> result;
    AddCommands(COM_GET, COM_JOINEVE, COM_QUESMARK, orders);
    try{
       
        result = system_->MessageHandler(orders);
 
    }
    catch(Exception x){
        return ResultPageHandler("/home", x.GetCommand(), ERROR_PAGE);
    }
   
    
    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";
    body += "<head>";
    body += "  <meta charset=\"UTF-8\">";
    body += "  <title>Scrollable Item List with Buttons</title>";
    body += "  <style>";
    body += "    body {";
    body += "      font-family: Arial, sans-serif;";
    body += "      background-color: #f8f9fa;";
    body += "      padding: 30px;";
    body += "    }";
    body += "";
    body += "    h2 {";
    body += "      color: #333;";
    body += "    }";
    body += "";
    body += "    .scrollable-list {";
    body += "      width: 600px;";
    body += "      height: 300px;";
    body += "      border: 1px solid #ccc;";
    body += "      border-radius: 8px;";
    body += "      overflow-y: auto;";
    body += "      background-color: #fff;";
    body += "      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);";
    body += "    }";
    body += "";
    body += "    .scrollable-list ul {";
    body += "      list-style: none;";
    body += "      margin: 0;";
    body += "      padding: 0;";
    body += "    }";
    body += "";
    body += "    .scrollable-list li {";
    body += "      display: flex;";
    body += "      justify-content: space-between;";
    body += "      align-items: center;";
    body += "      padding: 10px 15px;";
    body += "      border-bottom: 1px solid #eee;";
    body += "      transition: background-color 0.2s;";
    body += "    }";
    body += "";
    body += "    .scrollable-list li:hover {";
    body += "      background-color: #f1f1f1;";
    body += "    }";
    body += "";
    body += "    .buttons {";
    body += "      display: flex;";
    body += "      gap: 8px;";
    body += "    }";
    body += "";
    body += "    .btn {";
    body += "      padding: 5px 10px;";
    body += "      border: none;";
    body += "      border-radius: 4px;";
    body += "      cursor: pointer;";
    body += "      font-size: 14px;";
    body += "    }";
    body += "";
    body += "    .accept {";
    body += "      background-color: #28a745;";
    body += "      color: white;";
    body += "    }";
    body += "";
    body += "    .decline {";
    body += "      background-color: #dc3545;";
    body += "      color: white;";
    body += "    }";
    body += "";
    body += "    .scrollable-list li:last-child {";
    body += "      border-bottom: none;";
    body += "    }";
    body += "  </style>";
    body += "</head>";
    body += "<body>";
    body += "";
    body += "<button ";
    body += "    onclick=\"goToPage()\" ";
    body += "    style=\"position: absolute; top: 10px; left: 10px; padding: 8px 16px; background-color: red; color: white; border: none; border-radius: 5px; cursor: pointer;\">";
    body += "    Back";
    body += "</button>";

   
    body += "<script>";
    body += "    function goToPage() {";
    body += "        window.location.href = \"/home\"; ";
    body += "    }";
    body += "</script>";
    body += "<h2>Item List</h2>";
    body += "<div class=\"scrollable-list\">";
    body += "  <ul>";
    for(auto req: result){
        string id = req.substr(0, req.find(":"));
        body += "    <li>";
        body += req;
        body += "      <div class=\"buttons\">";
        body += "        <form action=\"/home/show-requests\" method=\"POST\">";
        body += "          <input type=\"hidden\" name=\"item_id\" value=\"" + id + "\">";
        body += "          <input type=\"hidden\" name=\"action\" value=\"accept\">";
        body += "          <button type=\"submit\" class=\"btn accept\">Accept</button>";
        body += "        </form>";
        body += "        <form action=\"/home/show-requests\" method=\"POST\">";
        body += "          <input type=\"hidden\" name=\"item_id\" value=\" " + id + "\">";
        body += "          <input type=\"hidden\" name=\"action\" value=\"decline\">";
        body += "          <button type=\"submit\" class=\"btn decline\">Decline</button>";
        body += "        </form>";
        body += "      </div>";
        body += "    </li>";

    }
    
    body += "  </ul>";
    body += "</div>";
    body += "";
    body += "</body>";
    body += "</html>";
    res->setBody(body);
    
    return res;


}
Response* ShowReports(vector<string> reports){
    Response* res = new Response();
    res->setHeader("Content-Type", "text/html");
    string body = "";
    body += "<!DOCTYPE html>";
    body += "<html lang=\"en\">";
    body += "<head>";
    body += "  <meta charset=\"UTF-8\">";
    body += "  <title>Scrollable Item List</title>";
    body += "  <style>";
    body += "    body {";
    body += "      display: flex;";
    body += "      flex-direction: column;";
    body += "      align-items: center;";
    body += "      justify-content: center;";
    body += "      min-height: 100vh;";
    body += "      margin: 0;";
    body += "      font-family: Arial, sans-serif;";
    body += "    }";
    body += "    ";
    body += "    .scrollable-list {";
    body += "      width: 700px;";
    body += "      height: 400px;";
    body += "      border: 1px solid #ccc;";
    body += "      overflow-y: scroll;";
    body += "      padding: 10px;";
    body += "      margin-top: 20px;";
    body += "    }";
    body += "";
    body += "    .scrollable-list ul {";
    body += "      list-style-type: none;";
    body += "      padding: 0;";
    body += "      margin: 0;";
    body += "    }";
    body += "";
    body += "    .scrollable-list li {";
    body += "      padding: 8px;";
    body += "      border-bottom: 1px solid #ddd;";
    body += "    }";
    body += "    ";
    body += "    h2 {";
    body += "      margin-bottom: 10px;";
    body += "    }";
    body += "    ";
    body += "    .back-button {";
    body += "      position: absolute;";
    body += "      top: 20px;";
    body += "      left: 20px;";
    body += "      padding: 8px 16px;";
    body += "      background-color: red;";
    body += "      color: white;";
    body += "      border: none;";
    body += "      border-radius: 5px;";
    body += "      cursor: pointer;";
    body += "    }";
    body += "    ";
    body += "    .content-wrapper {";
    body += "      text-align: center;";
    body += "      margin-top: 50px;";
    body += "    }";
    body += "  </style>";
    body += "</head>";
    body += "<body>";
    body += "  <button class=\"back-button\" onclick=\"goToPage()\">Back</button>";
    body += "";
    body += "  <div class=\"content-wrapper\">";
    body += "    <h2>Item List</h2>";
    body += "    <div class=\"scrollable-list\">";
    body += "      <ul>";
    for(auto report: reports)
        body += "<li>" + report +"</li>";
    body += "      </ul>";
    body += "    </div>";
    body += "  </div>";
    body += "";
    body += "  <script>";
    body += "    function goToPage() {";
    body += "      window.location.href = \"/home\";";
    body += "    }";
    body += "  </script>";
    body += "</body>";
    body += "</html>";
    
    res->setBody(body);
    return res;
}
Response* LoginHandler::callback(Request* req) {
    string username = req->getBodyParam("username");
    string password = req->getBodyParam("password");
    vector<OneOrderType> orders;
    AddCommands(COM_POS, COM_LOGIN, COM_QUESMARK, orders);
    AddSingleOrder(COM_USERNAME, {username}, orders);
    AddSingleOrder(COM_PASSWORD, {password}, orders);
    Response* res;
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = Response::redirect("/home");
        res->setSessionId("SID");
    }
    catch(Exception x){
        res = ResultPageHandler("/login", x.GetCommand(), ERROR_PAGE);
    }
    return res;
}

Response* UploadHandler::callback(Request* req) {
    std::string name = req->getBodyParam("file_name");
    std::string file = req->getBodyParam("file");
    utils::writeToFile(file, name);
    Response* res = Response::redirect("/");
    return res;
}
Response* SignupHandler::callback(Request* req){
    string username = req->getBodyParam("username");
    string password = req->getBodyParam("password");
    vector<OneOrderType> orders;
    OneOrderType order;
    AddCommands(COM_POS, COM_SIGNUP, COM_QUESMARK, orders);
    AddSingleOrder(COM_USERNAME, {username}, orders);
    AddSingleOrder(COM_PASSWORD, {password}, orders);
    Response* res;
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = Response::redirect("/home");
        res->setSessionId("SID");
    }
    catch(Exception x){
        res = ResultPageHandler("/signup", x.GetCommand(), ERROR_PAGE);
    }
    return res;
    
}
Response* LogoutHandler::callback(Request* req){
    vector<OneOrderType> orders;
    AddCommands(COM_POS, COM_LOGOUT, COM_QUESMARK, orders);
    Response* res;
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = Response::redirect("/");
        res->setSessionId("SID");
    }
    catch(Exception x){
        res = ResultPageHandler("/home", x.GetCommand(), ERROR_PAGE);
    }
    return res;

}
map<string, string> MainHandler::handle(Request* req){
    map<string, string> context;
    context["username"] = system_->GetUsername();
    return context;
}
std::map<std::string, std::string> ColorHandler::handle(Request* req) {
    std::string newName = "I am " + req->getQueryParam("name");
    std::map<std::string, std::string> context;
    context["name"] = newName;
    context["color"] = req->getQueryParam("color");
    return context;
}
MainHandler::MainHandler(const string& filePath, shared_ptr<System> sys):
    TemplateHandler(filePath), system_(sys){}
ColorHandler::ColorHandler(const std::string& filePath)
    : TemplateHandler(filePath) {}


Response* AddTaskHandler::callback(Request* req) {
    string date = req->getBodyParam("Date");
    string time = req->getBodyParam("Time");
    string title = req->getBodyParam("Title");
    string description = req->getBodyParam("Description");
    vector<OneOrderType> orders;
    AddCommands(COM_POS, COM_TASK, COM_QUESMARK, orders);
    AddSingleOrder(COM_DATE, {date}, orders);
    AddSingleOrder(COM_TIME, {time}, orders);
    AddSingleOrder(COM_TITLE, {title}, orders);
    if(description.size())
        AddSingleOrder(COM_DESC, {description}, orders);
    Response* res;
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = ResultPageHandler("/home", result[0], OK_PAGE);
    }
    catch(Exception x){
        res = ResultPageHandler("/home/add-task", x.GetCommand(), ERROR_PAGE);
    }
    return res;
}
Response* EditTaskHandler::callback(Request* req) {
    string task_id = req->getBodyParam("Task_id");
    string date = req->getBodyParam("Date");
    string time = req->getBodyParam("Time");
    string title = req->getBodyParam("Title");
    string description = req->getBodyParam("Description");
    vector<OneOrderType> orders;
    AddCommands(COM_PUT, COM_TASK, COM_QUESMARK, orders);
    Response* res;
    if(date.size())
        AddSingleOrder(COM_DATE, {date}, orders);
    if(time.size())
        AddSingleOrder(COM_TIME, {time}, orders);
    if(title.size())
        AddSingleOrder(COM_TITLE, {title}, orders);
    if(description.size())
        AddSingleOrder(COM_DESC, {description}, orders);
    AddSingleOrder(COM_TASKID, {task_id}, orders);
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = ResultPageHandler("/home", result[0], OK_PAGE);
    }
    catch(Exception x){
        res = ResultPageHandler("/home/edit-task", x.GetCommand(), ERROR_PAGE);
    }
    return res;
}
Response* RemoveTaskHandler::callback(Request* req) {
    string task_id = req->getBodyParam("Task_id");
    vector<OneOrderType> orders;
    AddCommands(COM_DELETE, COM_TASK, COM_QUESMARK, orders);
    AddSingleOrder(COM_TASKID, {task_id}, orders);
    Response* res;
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = ResultPageHandler("/home", result[0], OK_PAGE);
    }
    catch(Exception x){
        res = ResultPageHandler("/home/remove-task", x.GetCommand(), ERROR_PAGE);
    }
    return res;
}
Response* AddEventHandler::callback(Request* req) {
    
    string date = req->getBodyParam("Date");
    string start_time = req->getBodyParam("Start_Time");
    string duration = req->getBodyParam("Duration");
    string title = req->getBodyParam("Title");
    string description = req->getBodyParam("Description");
    vector<OneOrderType> orders;
    AddCommands(COM_POS, COM_EVENT, COM_QUESMARK, orders);
    AddSingleOrder(COM_DATE, {date}, orders);
    AddSingleOrder(COM_START_TIME, {start_time}, orders);
    AddSingleOrder(COM_DURATION, {duration}, orders);
    AddSingleOrder(COM_TITLE, {title}, orders);
    if(description.size())
        AddSingleOrder(COM_DESC, {description}, orders);
    Response* res;
   
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = ResultPageHandler("/home", result[0], OK_PAGE);

    }
    catch(Exception x){
        res = ResultPageHandler("/home/add-event", x.GetCommand(), ERROR_PAGE);
    }
    return res;
}
vector<string> parseList(string line){
    istringstream iss(line);
    string qer, day;
    vector<string> res;
    while(getline(iss, qer, ',')){
        istringstream qss(qer);
        qss >> day;
        res.push_back(day);
    }
    return res;

}
Response* AddPerEventHandler::callback(Request* req) {
    string start_date = req->getBodyParam("Start_Date");
    string end_date = req->getBodyParam("End_Date");
    string start_time = req->getBodyParam("Start_Time");
    string duration = req->getBodyParam("Duration");
    string days_of_week = req->getBodyParam("Days_of_Week");
    string title = req->getBodyParam("Title");
    string description = req->getBodyParam("Description");
    vector<OneOrderType> orders;
    AddCommands(COM_POS, COM_PEREVE, COM_QUESMARK, orders);
    AddSingleOrder(COM_START_DATE, {start_date}, orders);
    AddSingleOrder(COM_END_DATE, {end_date}, orders);
    AddSingleOrder(COM_START_TIME, {start_time}, orders);
    AddSingleOrder(COM_DURATION, {duration}, orders);
    AddSingleOrder(COM_TITLE, {title}, orders);
    AddSingleOrder(COM_WDAYS, parseList(days_of_week), orders);
    AddSingleOrder(COM_TYPE, {VAR_WEEKLY}, orders);
    
    if(description.size())
        AddSingleOrder(COM_DESC, {description}, orders);
    
    Response* res;
    
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = ResultPageHandler("/home", result[0], OK_PAGE);

    }
    catch(Exception x){
        res = ResultPageHandler("/home/add-periodic-event", x.GetCommand(), ERROR_PAGE);
    }
   
    return res;
}
Response* AddJoinEventHandler::callback(Request* req) {
    string guests = req->getBodyParam("Guests");
    string date = req->getBodyParam("Date");
    string start_time = req->getBodyParam("Start_Time");
    string end_time = req->getBodyParam("End_Time");
    string title = req->getBodyParam("Title");
    string description = req->getBodyParam("Description");
    vector<OneOrderType> orders;
    AddCommands(COM_POS, COM_JOINEVE, COM_QUESMARK, orders);
    AddSingleOrder(COM_DATE, {date}, orders);
    AddSingleOrder(COM_END_TIME, {end_time}, orders);
    AddSingleOrder(COM_START_TIME, {start_time}, orders);
    AddSingleOrder(COM_TITLE, {title}, orders);
    AddSingleOrder(COM_GUESTS, parseList(guests), orders);
    if(description.size())
        AddSingleOrder(COM_DESC, {description}, orders);
    Response* res;

    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = ResultPageHandler("/home", result[0], OK_PAGE);

    }
    catch(Exception x){
        res = ResultPageHandler("/home/add-join-event", x.GetCommand(), ERROR_PAGE);
    }
    return res;
}
Response* ShowReportPageHandler::callback(Request* req) {
    string from = req->getBodyParam("From");
    string to = req->getBodyParam("To");
    string type = req->getBodyParam("Type");
    vector<OneOrderType> orders;
    AddCommands(COM_GET, COM_REPORT, COM_QUESMARK, orders);
    if(from.size())
        AddSingleOrder(COM_FROM, {from}, orders);
    AddSingleOrder(COM_TO, {to}, orders);
    if(type != "all")   
        AddSingleOrder(COM_TYPE, {type}, orders);
    Response* res;
    
    try{
        vector<string> result;
    
        result = system_->MessageHandler(orders);
    
        res = ShowReports(result);
    }
    catch(Exception x){
        res = ResultPageHandler("/home/show-reports", x.GetCommand(), ERROR_PAGE);
    }
    return res;    
    
}
Response* ShowRequestsHandler::callback(Request* req) {
    string id = req->getBodyParam("item_id");
    string action = req->getBodyParam("action");
    vector<OneOrderType> orders;
    Response* res;
    if(action == ACC_REQ){
        AddCommands(COM_POS, COM_CONFJE, COM_QUESMARK, orders);
        AddSingleOrder(COM_INVID, {id}, orders);
    }
    else if(action == REJ_REQ){
        AddCommands(COM_POS, COM_REJJE, COM_QUESMARK, orders);
        AddSingleOrder(COM_INVID, {id}, orders);
    }
    try{
        vector<string> result;
        result = system_->MessageHandler(orders);
        res = ResultPageHandler("/home/show-requests", result[0], OK_PAGE);

    }
    catch(Exception x){
        res = ResultPageHandler("/home/show-requests", x.GetCommand(), ERROR_PAGE);
    }
    return res;
}