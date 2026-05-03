#include <bits/stdc++.h>
using namespace std;

class System;
class Template;
const string EXAM_NOT_FOUND = "Invalid answer, please try again.";
const string EASY = "easy";
const string MEDDIUM = "medium";
const string HARD = "hard";
const string CREATE_TEMPLATE = "create_template";
const string PREVIOUS = "previous";
const string CREATE_TEST = "generate_test";
const string ATTEND_TEST = "attend";
const string AUTO_GENERATE = "auto_generate";
const string NULL_ANSWER = "";
const string OP1_ANS = "1";
const string OP2_ANS = "2";
const string OP3_ANS = "3";
const string OP4_ANS = "4";
const string REPORT = "report";
const vector<string> DIFFS = {EASY, MEDDIUM, HARD};
class Question{
    private:
         
        string text, diff, subject;
        vector <string> options;
        string cor_answer;
        string user_answer;
        int numOfCor, numOfInc, numOfBlnk, testCount;
    public:
        const int INC_INT = -1;
        const int COR_INT = 1;
        const int BLNK_INT = 0;
        string getCurAnswer(){
            return user_answer;
        }
        string getOptions(int ind){
            return options[ind];
        }
        string getTopic(){
            return subject;
        }
        string getDiff(){
            return diff;
        }
        Question(string t, string op1, string op2, string op3, string op4, string ans, string d, string sub){
            text = t;

            options.push_back(op1);
            options.push_back(op2);
            options.push_back(op3);
            options.push_back(op4);
            diff = d;
            cor_answer = ans;
            subject = sub;
            numOfBlnk = 0;
            numOfCor = 0;
            numOfInc = 0;
            user_answer = NULL_ANSWER;
        }
        void setAnswer(string x){
            user_answer = x;
        }
        int calc_prio(){
            return numOfInc * 3 + numOfBlnk - 2 * numOfCor; 

        }
        void reset(){
            user_answer = NULL_ANSWER;
        }
        string getText(){
            return text;
        }
        void updateRecord(){
            testCount += 1;
            int res = checkResult();
            tempResult(res);
           
        }
        int checkResult(){
          
            if(user_answer == cor_answer)
                return COR_INT;
            if(user_answer == NULL_ANSWER)
                return BLNK_INT;
            return INC_INT;

        }
        void tempResult(int r){
            if(r == BLNK_INT)
                numOfBlnk += 1;
            if(r == COR_INT)  
                numOfCor += 1;
            if(r == INC_INT)
                numOfInc += 1;
        }
        int getCorAns(){
            return numOfCor;
        }
        int getIncAns(){
            return numOfInc;
        }
        int getBlnkAns(){
          
            return numOfBlnk;
        }
        int getTestCount(){
            return testCount;
        }
        
};
bool checkBiggerQuestion(Question* a, Question* b){
    if(a->calc_prio() > b->calc_prio())
        return 1;
    if(b->calc_prio() > a->calc_prio())
        return 0;
    return (a->getText() < b->getText());
}
class Template{
    private:
        string name;
        vector <tuple<string, string, int>> parts;
    public:
        Template(string n){
            name = n;
        }
        void addSubject(string sub, string diff, int count){
            parts.push_back(make_tuple(sub, diff, count));
            

        }
        auto getParts(){
            return parts;
        }
        string getName(){
            return name;
        }

};
class Test{
    private:
        map <string, int> num_cor, num_inc, num_blnk;
        int attend_line;
        string name;
        string template_name;
        vector <Question*> Questions;    
        map <pair<string, string>, int> verdict;
        const int INF = 1000000;
    public:
        void updateHistory(int x){
            attend_line = x;
        }
        int getHistory(){
            return attend_line;
        }
        void report(){
            long double res = 0;
            int tot_cor = 0, tot_inc = 0, tot_blnk = 0;
            cout << "Results for " << name << ":" << endl;
            cout << endl;
            for(auto Q: num_cor){
                res = (long double) num_cor[Q.first] / (num_cor[Q.first] + num_blnk[Q.first] + num_inc[Q.first]); 
                
                cout << Q.first << ": " << num_cor[Q.first] << " corrects, " << num_inc[Q.first] << " incorrects and " << num_blnk[Q.first];
                cout << " blanks. Score: " << res * 100 << "%." << endl;
                tot_cor += num_cor[Q.first];
                tot_inc += num_inc[Q.first];
                tot_blnk += num_blnk[Q.first];
            }   
            res =  ((long double) tot_cor / (tot_cor + tot_inc + tot_blnk)) * 100;
            cout << "Total results: " << tot_cor << " corrects, " << tot_inc << " incorrects and " << tot_blnk << " blanks.";
            cout << "Total score: " << res << "%." << endl;
            cout << endl;
        }
        tuple <int, int, int> getTotResult(){
            int t_cor = 0, t_inc = 0, t_blnk = 0;
            for(auto Q: num_cor){
               
                t_cor += num_cor[Q.first];
                t_inc += num_inc[Q.first];
                t_blnk += num_blnk[Q.first];
            }
            return make_tuple(t_cor, t_inc, t_blnk);
        }
        void preprateQuestions(){
            for(auto* Q: Questions){
                Q->reset();
            }
            sort(Questions.begin(), Questions.end(), [](Question* a, Question* b){
                return (a->getTopic() == b->getTopic())? (a->getText() < b->getText()): (a->getTopic() < b->getTopic());});
        }
        Question* getQuestion(int x){
            if(Questions.size() == x)
                return NULL;
            return Questions[x];
        }
        string getName(){
            return name;
        }
        Test(string n, string t_name){
            name = n;
            template_name = t_name;
            attend_line = INF;
            
        }
        void setQuestions(vector<Question*> Q){
            Questions = Q;
        }
    
        void getVerdict(){
            num_cor.clear();
            num_inc.clear();
            num_blnk.clear();
            for(Question* Q: Questions){
                Q->updateRecord();
                if(Q->checkResult() == Q->COR_INT){
                    num_cor[Q->getTopic()] += 1;
                }
                else if(Q->checkResult() == Q->BLNK_INT){
                    num_blnk[Q->getTopic()] += 1;
                }
                else if(Q->checkResult() == Q->INC_INT){
                    num_inc[Q->getTopic()] += 1;
                }
                num_cor[Q->getTopic()] += 0;
            }
        }
};
string parseName(string name){
    istringstream iss(name);
    string res = "", temp;
    getline(iss, temp, '\'');
    getline(iss, res, '\'');
    return res;

}
class System{
    private:
        map <pair<string, string>, vector<Question*>> Questions;
        vector <Template*> Templates;
        vector <Test*> Tests;
        const int SUB1_EASY = 3;
        const int SUB1_MEDDIUM = 2;
        const int SUB1_HARD = 1;
        const int SUB2_EASY = 2;
        const int SUB2_MEDDIUM = 1;
        const int SUB2_HARD = 1;
        int attended_tests;
        vector <pair<string, string>> Diff;
    public: 
        System(){
            Diff.push_back(make_pair("Easy", "easy"));
            Diff.push_back(make_pair("Medium", "medium"));
            Diff.push_back(make_pair("Hard", "hard"));
            attended_tests = 0;
        }
        int getTestsHistory(){
            attended_tests += 1;
            return attended_tests - 1;
        }
        tuple<int, int, int> reportSubDiff(string name, string d_name){
            int cor = 0, inc = 0, blnk = 0;
            for(auto* Q: Questions[make_pair(name, d_name)]){
                cor += Q->getCorAns();
                inc += Q->getIncAns();
                blnk += Q->getBlnkAns();
            }
            return make_tuple(cor, inc, blnk);
        }
        void reportSub(string name){
            int cor = 0, inc = 0, blnk = 0;
            long double f = 0;
            cout << "Results for " << name <<":"<< endl;
            cout << endl;
            for(auto diff: Diff){
                tuple <int, int, int> res = reportSubDiff(name, diff.second);
                cout << diff.first << ": " << get<0>(res) << " corrects, " << get<1>(res) << " incorrects and " << get<2>(res) << " blanks." << endl;
                cor += get<0>(res);
                inc += get<1>(res);
                blnk += get<2>(res);
            }
            cout << endl;
            cout << "Total score: " << ((long double) cor / (inc + cor + blnk)) * 100 << "%." << endl;
            cout << endl;

        }
        void reportAll(){
            map<string, int> subs;
            int score[3], t_cor = 0, t_inc = 0, t_blnk = 0;
            for(auto Q: Questions){
                subs[Q.first.first] = 1;
            }
            for(auto sub: subs){
                score[0] = 0;
                score[1] = 0;
                score[2] = 0;
                for(auto x: DIFFS){
                    auto res = reportSubDiff(sub.first, x);
                    score[0] += get<0>(res);
                    score[1] += get<1>(res);
                    score[2] += get<2>(res);
                }
                cout << sub.first << ": " << score[0] << " corrects, " << score[1] << " incorrects and " << score[2];
                cout << " blanks. Score: " << ((long double)score[0] / (score[0] + score[1] + score[2])) * 100 << "%." << endl;
                t_cor += score[0];
                t_inc += score[1];
                t_blnk += score[2];
            }
            cout << endl;
            cout << "Total results: " << t_cor << " corrects, " << t_inc << " incorrects and " << t_blnk << " blanks.";
            cout << "Total score: " << ((long double) t_cor / (t_cor + t_inc + t_blnk)) * 100 << "%." << endl;
            cout << endl;

        }
        void reportAllTests(){
            cout << "Results per attended tests:" << endl;
            cout << endl;
            sort(Tests.begin(), Tests.end(), [](auto a, auto b){return a->getHistory() < b->getHistory();});
            for(auto *T: Tests){
                
                auto res = T->getTotResult();
                if(get<0>(res) == get<1>(res) && get<1>(res) == get<2>(res) && get<0>(res) == 0){
                    continue;
                }
                long double score =  ((long double) get<0>(res) / (get<0>(res) + get<1>(res) + get<2>(res))) * 100;
                cout << parseName(T->getName()) << ": "<< get<0>(res) << " corrects, " << get<1>(res) << " incorrects and ";
                cout << get<2>(res) << " blanks. Score: " << score << "%." << endl;   
            }
           
        }
        void addQuestion(Question* Q){
            Questions[make_pair(Q->getTopic(), Q->getDiff())].push_back(Q);
            return;
        }
        Template* templateEists(string name){
            for(auto* temp: Templates){
                if(temp->getName() == name)
                    return temp;
            }
            return NULL;

        }
        Test* testExists(string name){
            for(auto* test: Tests){
                if(test->getName() == name)
                    return test;
            }
            return NULL;


        }
        Template* findTempByName(string t_name){
            for(auto* T: Templates)
                if(T->getName() == t_name)
                    return T;
            return NULL;
        }
        vector <Question*> getObjQues(string sub, string diff, int count){
            vector <Question*> result;
    
            sort(Questions[make_pair(sub, diff)].begin(), Questions[make_pair(sub, diff)].end(), checkBiggerQuestion);
            for(int i = 0; i < count; i++)   {
                result.push_back(Questions[make_pair(sub, diff)][i]);
            }
            return result;
        }
        vector<Question*> generateTestByName(string t_name, Template* cur_temp){  
            vector <Question*> result, temp;
            
            for(auto part: cur_temp->getParts()){
       //         cout << get<0>(part) << ' '  << get<1>(part) << ' ' << get<2>(part) << endl;
                temp = getObjQues(get<0>(part), get<1>(part), get<2>(part));
                for(auto* x: temp)
                    result.push_back(x);
            }
            sort(result.begin(), result.end(), [](Question* a, Question* b){
                if(a->getTopic() < b->getTopic())
                    return 1;
                if(a->getTopic() > b->getTopic())
                    return 0;
                if(a->getText() < b->getText())
                    return 1;
                return 0;
            });
            return result;
        }
        vector <pair<double, string>> getSubjectsRecord(){
            map<string, int> sub_sum, sub_num;
            vector <pair<double, string>> result;
            string sub_name;
            for(auto Sub: Questions){
                sub_name = Sub.first.first;
               
                for(auto* Q: Sub.second){
                    sub_sum[sub_name] += Q->getCorAns();
                    sub_num[sub_name] += Q->getTestCount();
                }
            }
    
            for(auto i: sub_sum)
                result.push_back(make_pair((double) (i.second) / sub_num[i.first], i.first ));
            sort(result.begin(), result.end());
            return result;

        }
        vector<Question*> generateAutoTest(){
            vector <pair<double, string>> subs;
            vector <tuple<int, string, int>> orders;
            subs = getSubjectsRecord();
            sort(subs.begin(), subs.end());
            orders.push_back(make_tuple(0, EASY, SUB1_EASY));
            orders.push_back(make_tuple(0, MEDDIUM, SUB1_MEDDIUM));
            orders.push_back(make_tuple(0, HARD, SUB1_HARD));
            orders.push_back(make_tuple(1, EASY, SUB2_EASY));
            orders.push_back(make_tuple(1, MEDDIUM, SUB2_MEDDIUM));
            orders.push_back(make_tuple(1, HARD, SUB2_HARD));
            subs = getSubjectsRecord();
            vector <Question*> result, temp;
            for(auto order: orders){
                temp = getObjQues(subs[get<0>(order)].second, get<1>(order), get<2>(order));
              
                for(auto* x: temp){
                  
                    result.push_back(x);
                }
            }
            return result;
        }
        void addTemplate(Template* temp){
            Templates.push_back(temp);

        }
        void addTest(Test* test){
            Tests.push_back(test);
        }
};

void create_template(System* system){
   
    string name, s_name, diff, subs, sub, count;
    cin >> name;
    if(system->templateEists(name) != NULL){
        cout << "Duplicate name: " << name << endl;
        return;
    }
    Template* temp = new Template(name);
    getline(cin, subs);
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
void create_test(System* system){
    //*********************************************************************************deal with input */
    string inp, name, t_name, temp;
    getline(cin, inp);
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
void attend_test(System* system ){
    string name, cur_ans, temp, inp;
    int cur_q;
    bool done = 0;
    getline(cin, inp);
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
void autoGenerate(System *system){
    string name, inp, temp;
    getline(cin, inp);
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
void reportWhat(System *system){
    string command;
    cin >> command;
    if(command == "all")
        system->reportAll();
    else if(command == "test"){
        cin >> command;
        reportTest(system, command);
    }
    else if(command == "tests"){
        system->reportAllTests();
    }
    else if(command == "subject"){
        cin >> command;
        system->reportSub(command);
    }

}
int main(int argv_int, char** argv_c){
    cout << fixed << setprecision(3);
    ///////////////////////////////////////////////////////////////////////////////////////
    //Changeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee this part
    System* system = new System;
    string command;
    getQuestions(system, "./src/questions.csv");
    while(cin >> command){
        if(command == CREATE_TEMPLATE){
            create_template(system);
        }
        if(command == CREATE_TEST){
            create_test(system);
        }
        else if(command == ATTEND_TEST){
            attend_test(system);
        }
        else if(command == AUTO_GENERATE){
            autoGenerate(system);
        }
        else if(command == REPORT){
            reportWhat(system);
        }
      

    }


}
