#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include "Question.h"
#include "Test.h"
#include "Template.h"

using namespace std;

class System {
private:
    map<pair<string, string>, vector<Question*>> Questions;
    vector<Template*> Templates;
    vector<Test*> Tests;
    int attended_tests;
    const int SUB1_EASY = 3;
    const int SUB1_MEDDIUM = 2;
    const int SUB1_HARD = 1;
    const int SUB2_EASY = 2;
    const int SUB2_MEDDIUM = 1;
    const int SUB2_HARD = 1;
    vector<pair<string, string>> Diff;

public:
    System();

    int getTestsHistory();
    void addQuestion(Question* Q);
    Template* templateEists(string name);
    Template* findTempByName(string t_name);
    Test* testExists(string name);
    void addTemplate(Template* temp);
    void addTest(Test* test);
    vector<Question*> generateTestByName(string t_name, Template* cur_temp);
    vector<Question*> generateAutoTest();
    void reportSub(string name);
    void reportAll();
    void reportAllTests();
    tuple<int, int, int> reportSubDiff(string name, string d_name);
    vector<pair<double, string>> getSubjectsRecord();
    vector<Question*> getObjQues(string sub, string diff, int count);
};

#endif
