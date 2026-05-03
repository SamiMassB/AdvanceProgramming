#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include "../server/server.hpp"
#include "handlers.hpp"
#include "User.hpp"
#include "events.hpp"
#include "main.hpp"
#include "System.hpp"
#include "Date.hpp"
using namespace std;

void mapServerPaths(Server& server, shared_ptr<System> system) {
    server.setNotFoundErrPage("static/404.html");
    server.get("/", new ShowPage("static/home.html", system));
    server.get("/home", new MainHandler("template/main.html", system));
    server.post("/logout", new LogoutHandler(system));
    server.get("/signup", new ShowPage("static/signup.html", system));
    server.post("/signup", new SignupHandler(system));

    server.get("/login", new ShowPage("static/login.html", system));
    server.post("/login", new LoginHandler(system));

    server.get("/home/add-task", new ShowPage("static/AddTask.html", system));
    server.get("/home/add-event", new ShowPage("static/AddEvent.html", system));
    server.get("/home/add-periodic-event", new ShowPage("static/AddPerEvent.html", system));
    server.get("/home/add-join-event", new ShowPage("static/AddJoinEvent.html", system));
    server.get("/home/edit-task", new ShowPage("static/EditTask.html", system));
    server.get("/home/remove-task", new ShowPage("static/RemoveTask.html", system));
    server.get("/home/show-reports", new ShowPage("static/ShowReportPage.html", system));
    server.get("/home/show-requests", new ShowRequests(system));
    
    server.post("/home/add-task", new AddTaskHandler(system));
    server.post("/home/add-event", new AddEventHandler(system));
    server.post("/home/add-periodic-event", new AddPerEventHandler(system));
    server.post("/home/add-join-event", new AddJoinEventHandler(system));
    server.post("/home/edit-task", new EditTaskHandler(system));
    server.post("/home/remove-task", new RemoveTaskHandler(system));
    server.post("/home/show-reports", new ShowReportPageHandler(system));
    server.post("/home/show-requests", new ShowRequestsHandler(system));
    
}

int main(int argc, char** argv) {
    shared_ptr<System> system = make_shared<System>();
    Date::GetHolidays(argv[2]);

    

    try {
        int port = argc > 1 ? std::stoi(argv[1]) : 5000;
        Server server(port);
        mapServerPaths(server, system);
        std::cout << "Server running on port: " << port << std::endl;
        server.run();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const Server::Exception& e) {
        std::cerr << e.getMessage() << std::endl;
    }
    return 0;
}
