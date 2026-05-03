#include "Test.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "config.h"
using namespace std;


void Test::updateHistory(int x){
    attend_line = x;
}
int Test::getHistory(){
    return attend_line;
}
void Test::report(){
    long double res = 0;
    int tot_cor = 0, tot_inc = 0, tot_blnk = 0;
    cout << "Results for " << parseName(name) << ":" << endl;
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
    cout << endl;
    cout << "Total results: " << tot_cor << " corrects, " << tot_inc << " incorrects and " << tot_blnk << " blanks." << endl;
    cout << "Total score: " << res << "%." << endl;
   
}
tuple <int, int, int> Test::getTotResult(){
    int t_cor = 0, t_inc = 0, t_blnk = 0;
    for(auto Q: num_cor){
    
        t_cor += num_cor[Q.first];
        t_inc += num_inc[Q.first];
        t_blnk += num_blnk[Q.first];
    }
    return make_tuple(t_cor, t_inc, t_blnk);
}
void Test::preprateQuestions(){
    for(auto* Q: Questions){
        Q->reset();
    }
    sort(Questions.begin(), Questions.end(), [](Question* a, Question* b){
        return (a->getTopic() == b->getTopic())? (a->getText() < b->getText()): (a->getTopic() < b->getTopic());});
}
Question* Test::getQuestion(int x){
    if(Questions.size() == x)
        return NULL;
    return Questions[x];
}
string Test::getName(){
    return name;
}
Test::Test(string n, string t_name){
    name = n;
    template_name = t_name;
    attend_line = INF;
    
}
void Test::setQuestions(vector<Question*> Q){
    Questions = Q;
}

void Test::getVerdict(){
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

    