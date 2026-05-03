#ifndef HANDLERS_HPP_INCLUDE
#define HANDLERS_HPP_INCLUDE

#include <map>
#include <string>
#include "./System.hpp"
#include "../server/server.hpp"

class RandomNumberHandler : public RequestHandler {
public:
    Response* callback(Request*) override;
};
class ShowRequests : public RequestHandler {
    private:    
        shared_ptr<System> system_;
    public:
        ShowRequests(shared_ptr<System> sys) : system_(sys){};
        Response* callback(Request*) override;

};
class LoginHandler : public RequestHandler {
private:
    shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    LoginHandler(shared_ptr<System> sys) : system_(sys){};
};
class LogoutHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
    public:
        Response* callback(Request*) override;
        LogoutHandler(shared_ptr<System> sys) : system_(sys){};
    };
class RedirectHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
        string page_;
    public:
        Response* callback(Request*) override;
        RedirectHandler(shared_ptr<System> sys, string page) : system_(sys), page_(page){};
    };
class UploadHandler : public RequestHandler {
public:
    Response* callback(Request*) override;
};
class SignupHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    SignupHandler(shared_ptr<System> system) : system_(system){};

};

class AddTaskHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    AddTaskHandler(shared_ptr<System> system) : system_(system){};

};
class AddEventHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    AddEventHandler(shared_ptr<System> system) : system_(system){};

};
class AddPerEventHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    AddPerEventHandler(shared_ptr<System> system) : system_(system){};

};
class AddJoinEventHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    AddJoinEventHandler(shared_ptr<System> system) : system_(system){};

};
class RemoveTaskHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    RemoveTaskHandler(shared_ptr<System> system) : system_(system){};

};
class EditTaskHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    EditTaskHandler(shared_ptr<System> system) : system_(system){};

};
class ShowReportPageHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    ShowReportPageHandler(shared_ptr<System> system) : system_(system){};

};
class ShowRequestsHandler : public RequestHandler {
    private:
        shared_ptr<System> system_;
public:
    Response* callback(Request*) override;
    ShowRequestsHandler(shared_ptr<System> system) : system_(system){};

};






class MainHandler : public TemplateHandler{
    private:
        shared_ptr<System> system_;
    public:
        MainHandler(const string& filePath, shared_ptr<System> sys);
        map<string, string> handle(Request* req) override;


};
class ColorHandler : public TemplateHandler {
public:
    ColorHandler(const std::string& filePath);
    std::map<std::string, std::string> handle(Request* req) override;
};

#endif 
