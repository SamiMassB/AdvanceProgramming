#ifndef TEST_H
#define TEST_H

#include <string>
#include <vector>
#include <map>
#include "Question.h"

using namespace std;

class Test {
private:
    map<string, int> num_cor, num_inc, num_blnk;
    int attend_line;
    string name, template_name;
    vector<Question*> Questions;
    map<pair<string, string>, int> verdict;
    const int INF = 1000000;

public:
    Test(string n, string t_name);

    void updateHistory(int x);
    int getHistory();
    void report();
    tuple<int, int, int> getTotResult();
    void preprateQuestions();
    Question* getQuestion(int x);
    string getName();
    void setQuestions(vector<Question*> Q);
    void getVerdict();
};

#endif
