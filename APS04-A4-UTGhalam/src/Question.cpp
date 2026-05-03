#include "Question.h"
#include "config.h"
using namespace std;


string Question::getCurAnswer(){
    return user_answer;
}
string Question::getOptions(int ind){
    return options[ind];
}
string Question::getTopic(){
    return subject;
}
string Question::getDiff(){
    return diff;
}
Question::Question(string t, string op1, string op2, string op3, string op4, string ans, string d, string sub){
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
void Question::setAnswer(string x){
    user_answer = x;
}
int Question::calc_prio(){
    return numOfInc * 3 + numOfBlnk - 2 * numOfCor; 

}
void Question::reset(){
    user_answer = NULL_ANSWER;
}
string Question::getText(){
    return text;
}
void Question::updateRecord(){
    testCount += 1;
    int res = checkResult();
    tempResult(res);
    
}
int Question::checkResult(){
    
    if(user_answer == cor_answer)
        return COR_INT;
    if(user_answer == NULL_ANSWER)
        return BLNK_INT;
    return INC_INT;

}
void Question::tempResult(int r){
    if(r == BLNK_INT)
        numOfBlnk += 1;
    if(r == COR_INT)  
        numOfCor += 1;
    if(r == INC_INT)
        numOfInc += 1;
}
int Question::getCorAns(){
    return numOfCor;
}
int Question::getIncAns(){
    return numOfInc;
}
int Question::getBlnkAns(){
    
    return numOfBlnk;
}
int Question::getTestCount(){
    return testCount;
}
        

bool checkBiggerQuestion(Question* a, Question* b){
    if(a->calc_prio() > b->calc_prio())
        return 1;
    if(b->calc_prio() > a->calc_prio())
        return 0;
    return (a->getText() < b->getText());
}
