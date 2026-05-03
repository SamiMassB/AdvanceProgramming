#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

using namespace std;

class Question {
private:
    string text, diff, subject;
    vector<string> options;
    string cor_answer;
    string user_answer;
    int numOfCor, numOfInc, numOfBlnk, testCount;

public:
    const int INC_INT = -1;
    const int COR_INT = 1;
    const int BLNK_INT = 0;
    const string NULL_ANSWER;

    Question(string t, string op1, string op2, string op3, string op4, string ans, string d, string sub);

    string getCurAnswer();
    string getOptions(int ind);
    string getTopic();
    string getDiff();
    string getText();
    void setAnswer(string x);
    void reset();
    int calc_prio();
    void updateRecord();
    int checkResult();
    void tempResult(int r);
    int getCorAns();
    int getIncAns();
    int getBlnkAns();
    int getTestCount();
};

bool checkBiggerQuestion(Question* a, Question* b);

#endif
