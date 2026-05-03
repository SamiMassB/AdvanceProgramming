#include <bits/stdc++.h>
#include "config.h"
#include "System.h"
#include "Template.h"
#include "Question.h"
#include "Test.h"
string parseName(string name){
    istringstream iss(name);
    string res = "", temp;
    getline(iss, temp, '\'');
    getline(iss, res, '\'');
    return res;

}
void create_template(System* system, string command_line){
   
    string name, s_name, diff, subs, sub, count;
    istringstream sys(command_line);
    getline(sys, name, '\'');
    getline(sys, name, '\'');
    name = "'" + name + "'";
    if(system->templateEists(name) != NULL){
        cout << "Duplicate name: " << name << endl;
        return;
    }
    Template* temp = new Template(name);
    getline(sys, subs);
    istringstream iss(subs);
    while(iss >> sub){
        istringstream splt(sub);
        getline(splt, s_name, ':');
        getline(splt, diff, ':');
        getline(splt, count, ' ');  
        temp->addSubject(s_name, diff, stoi(count));
    }
    system->addTemplate(temp);
    cout << "Template " << name << " was created successfully." << endl;
}
void create_test(System* system, string command_line){
    //*********************************************************************************deal with input */
    string inp, name, t_name, temp;
    istringstream sys(command_line);
    getline(sys, inp);
    istringstream iss(inp);
    getline(iss, temp, '\'');
    getline(iss, name, '\'');
    getline(iss, temp, '\'');
    getline(iss, t_name, '\'');
    name = '\'' + name + '\'';
    t_name = '\'' + t_name + '\'';
    if(system->testExists(name) != NULL){
        cout << "Error" << endl;
        return;
    }
    if(system->templateEists(t_name) == NULL){
        cout << "Could not find template: " << t_name << endl;
        return;

    }

    Test* test = new Test(name, t_name);


    test->setQuestions(system->generateTestByName(t_name, system->templateEists(t_name)));
    
    system->addTest(test);
    cout << "Test " << name << " was generated successfully." << endl;
}
bool checkValidAnswer(string ans){
    if(ans == PREVIOUS)
        return 1;
    if(ans == NULL_ANSWER)
        return 1;
    if(ans.length() != 1)
        return 0;
    if(ans != OP1_ANS && ans != OP2_ANS && ans != OP3_ANS && ans != OP4_ANS)
        return 0;
    return 1;
}
string prevAnswer(Question* a, int x){
    if(a->getCurAnswer() == "")
        return "";
    if(stoi(a->getCurAnswer()) == x)
        return " <-";
    return "";

}
void printQuestionData(Question* Q, int cur_q){
    cout << cur_q + 1 << ')' << ' ' << Q->getText() << endl;
    cout <<"    1. " << Q->getOptions(0) << prevAnswer(Q, 1) << endl;
    cout <<"    2. " << Q->getOptions(1) << prevAnswer(Q, 2) << endl;
    cout <<"    3. " << Q->getOptions(2) << prevAnswer(Q, 3) << endl;
    cout <<"    4. " << Q->getOptions(3) << prevAnswer(Q, 4) << endl;
    cout << "Your answer: ";

}
string getAnswer(int cur_q){
    string cur_ans;
    getline(cin, cur_ans);
    while(checkValidAnswer(cur_ans) == 0 || (cur_q == 0 && cur_ans == PREVIOUS)){
        cout << "Invalid answer, please try again." << endl;
        cout << "Your answer: ";

        getline(cin, cur_ans);
    }
    cout << endl;
    return cur_ans;

}
void attend_test(System* system, string command_line ){
    string name, cur_ans, temp, inp;
    int cur_q;
    bool done = 0;
    istringstream sys(command_line);
    getline(sys, inp);
    istringstream iss(inp);
    getline(iss, temp, '\'');
    getline(iss, name, '\'');
    name = '\'' + name + '\'';
 
    Test* test = system->testExists(name);
    Question* Q;
    if(test == NULL){
        cout << "Could not find test: " << name << endl;
        return;
    }
    cout << parseName(name) << ":" << endl << endl;
    test->preprateQuestions();
    test->updateHistory(system->getTestsHistory());
    cur_q = 0;
    while(!done){
        Q = test->getQuestion(cur_q);
        if(Q == NULL){
            done = 1;
            continue;
        }
        printQuestionData(Q, cur_q);
        cur_ans = getAnswer(cur_q);
        if(cur_ans == PREVIOUS){
            if(cur_q != 0)
                cur_q -= 1;
            continue;
        }
        Q->setAnswer(cur_ans);
        cur_q += 1;

    }
    test->getVerdict();
    auto res = test->getTotResult();
    cout << "Finished " << parseName(name) << "." << endl;

}
void getQuestions(System* system, string file_loc){
    ifstream file(file_loc);  
    string inp, text, op1, op2, op3, op4, cor_ans, diff, sub;
    getline(file, inp);
    while(getline(file, inp)){
        istringstream iss(inp);
        getline(iss, text, ',');
        getline(iss, op1, ',');
        getline(iss, op2, ',');
        getline(iss, op3, ',');
        getline(iss, op4, ',');
        getline(iss, cor_ans, ',');
        getline(iss, diff, ',');
        getline(iss, sub);
    

        Question* Q = new Question(text, op1, op2, op3, op4, cor_ans, diff, sub);
        system->addQuestion(Q);
    }


}
void autoGenerate(System *system, string command_line){
    string name, inp, temp;
    istringstream sys(command_line);
    getline(sys, inp);
    istringstream iss(inp);
    getline(iss, temp, '\'');
    getline(iss, name, '\'');

    name = "'" + name + "'";
    
    Test* t = new Test(name, "");
    t->setQuestions(system->generateAutoTest());
    
    system->addTest(t);

    cout << "Test " << t->getName() << " was generated successfully." << endl;
}
void reportTest(System* system, string name){
    Test* t = system->testExists(name);
    if(t == NULL){
        cout << "no such test" << endl;
        return;
    }
    t->report();
}
void reportWhat(System *system, string command_line){
    string command;
    istringstream iss(command_line);
    iss >> command;
    if(command == "all")
        system->reportAll();
    else if(command == "test"){
        iss >> command;
        reportTest(system, command);
    }
    else if(command == "tests"){
        system->reportAllTests();
    }
    else if(command == "subject"){
        iss >> command;
        system->reportSub(command);
    }

}
int main(int argv_int, char** argv_c){
    cout << fixed << setprecision(3);

    System* system = new System;
    string command_line, command;
    getQuestions(system, argv_c[1]);

    while(cin >> command){
        if(command == CREATE_TEMPLATE){
            getline(cin, command_line);
            create_template(system, command_line);
        }
        if(command == CREATE_TEST){
            getline(cin, command_line);
            create_test(system, command_line);
        }
        else if(command == ATTEND_TEST){
            getline(cin, command_line);
            attend_test(system, command_line);
        }
        else if(command == AUTO_GENERATE){
            getline(cin, command_line);
            autoGenerate(system, command_line);
        }
        else if(command == REPORT){
            getline(cin, command_line);
            reportWhat(system, command_line);
        }
      

    }


}