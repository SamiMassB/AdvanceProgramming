#ifndef SERVER_HPP_INCLUDE
#define SERVER_HPP_INCLUDE

#include <exception>
#include <string>
#include <vector>

#include "../utils/include.hpp"
#include "../utils/request.hpp"
#include "../utils/response.hpp"
#include "../utils/template_parser.hpp"
#include "route.hpp"
#include "../files/System.hpp"

#ifdef _WIN32
typedef unsigned _SOCKET;
#else
typedef int _SOCKET;
#endif

class TemplateParser;

class RequestHandler {
public:
    virtual ~RequestHandler();
    virtual Response* callback(Request* req) = 0;
};

class ShowFile : public RequestHandler {
    private:
    shared_ptr<System> system_;
        std::string filePath_;
        std::string fileType_;
    public:
    ShowFile(const std::string& filePath, const std::string& fileType, shared_ptr<System> sys);
    Response* callback(Request* req) override;


};

class ShowPage : public ShowFile {
public:
    ShowPage(const std::string& filePath, shared_ptr<System> sys);
};

class ShowImage : public ShowFile {
public:
    ShowImage(const std::string& filePath, shared_ptr<System> sys);
};

class TemplateHandler : public RequestHandler {
    
private:
    std::string filePath_;
    TemplateParser* parser_;
public:
    TemplateHandler(const std::string& filePath);
    ~TemplateHandler();

    Response* callback(Request* req) override;
    virtual std::map<std::string, std::string> handle(Request* req);


};

class Server {
public:
    Server(int port = 5000);
    ~Server();

    void run();

    void get(const std::string& path, RequestHandler* handler);
    void post(const std::string& path, RequestHandler* handler);
    void put(const std::string& path, RequestHandler* handler);
    void del(const std::string& path, RequestHandler* handler);
    void setNotFoundErrPage(const std::string& notFoundErrPage);

    class Exception : public std::exception {
    public:
        Exception() = default;
        Exception(const std::string message);
        std::string getMessage() const;

    private:
        std::string message_;
    };

private:
    _SOCKET sc_;
    int port_;
    std::vector<Route*> routes_;
    RequestHandler* notFoundHandler_;

    void mapRequest(const std::string& path, RequestHandler* handler, Request::Method method);
};

#endif 
