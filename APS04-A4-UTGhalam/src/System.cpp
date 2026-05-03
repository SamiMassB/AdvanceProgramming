#include "System.h"
#include <iostream>
#include <algorithm>
#include "config.h"
using namespace std;


System::System(){
    Diff.push_back(make_pair("Easy", "easy"));
    Diff.push_back(make_pair("Medium", "medium"));
    Diff.push_back(make_pair("Hard", "hard"));
    attended_tests = 0;
}
int System::getTestsHistory(){
    attended_tests += 1;
    return attended_tests - 1;
}
tuple<int, int, int> System::reportSubDiff(string name, string d_name){
    int cor = 0, inc = 0, blnk = 0;
    for(auto* Q: Questions[make_pair(name, d_name)]){
        cor += Q->getCorAns();
        inc += Q->getIncAns();
        blnk += Q->getBlnkAns();
    }
    return make_tuple(cor, inc, blnk);
}
void System::reportSub(string name){
    int cor = 0, inc = 0, blnk = 0;
    long double f = 0;
    cout << "Results for " << name << ":" << endl;
    cout << endl;
    for(auto diff: Diff){
        tuple <int, int, int> res = reportSubDiff(name, diff.second);
        cout << diff.first << ": " << get<0>(res) << " corrects, " << get<1>(res) << " incorrects and " << get<2>(res) << " blanks." << endl;
        cor += get<0>(res);
        inc += get<1>(res);
        blnk += get<2>(res);
    }
    cout << endl;
    if(inc + cor + blnk != 0)
        cout << "Total score: " << ((long double) cor / (inc + cor + blnk)) * 100 << "%." << endl;
    else{
        cout << "Total score: " << ((long double) 0) << "%." << endl;
    }

}
void System::reportAll(){
    map<string, int> subs;
    int score[3], t_cor = 0, t_inc = 0, t_blnk = 0;
    for(auto Q: Questions){
        subs[Q.first.first] = 1;
    }
    cout << "Total report:" << endl << endl;
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
        if(score[0] + score[1] + score[2] == 0)
            continue;
        cout << sub.first << ": " << score[0] << " corrects, " << score[1] << " incorrects and " << score[2];
        cout << " blanks. Score: " << ((long double)score[0] / (score[0] + score[1] + score[2])) * 100 << "%." << endl;
        t_cor += score[0];
        t_inc += score[1];
        t_blnk += score[2];
    }
    cout << endl;
    cout << "Total results: " << t_cor << " corrects, " << t_inc << " incorrects and " << t_blnk << " blanks." << endl;
    cout << "Total score: " << ((long double) t_cor / (t_cor + t_inc + t_blnk)) * 100 << "%." << endl;
   

}
void System::reportAllTests(){
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
void System::addQuestion(Question* Q){
    Questions[make_pair(Q->getTopic(), Q->getDiff())].push_back(Q);
    return;
}
Template* System::templateEists(string name){
    for(auto* temp: Templates){
        if(temp->getName() == name)
            return temp;
    }
    return NULL;

}
Test* System::testExists(string name){
    for(auto* test: Tests){
        if(test->getName() == name)
            return test;
    }
    return NULL;


}
Template* System::findTempByName(string t_name){
    for(auto* T: Templates)
        if(T->getName() == t_name)
            return T;
    return NULL;
}
vector <Question*> System::getObjQues(string sub, string diff, int count){
    vector <Question*> result;

    sort(Questions[make_pair(sub, diff)].begin(), Questions[make_pair(sub, diff)].end(), checkBiggerQuestion);
    for(int i = 0; i < count; i++)   {
        result.push_back(Questions[make_pair(sub, diff)][i]);
    }
    return result;
}
vector<Question*> System::generateTestByName(string t_name, Template* cur_temp){  
    vector <Question*> result, temp;
    
    for(auto part: cur_temp->getParts()){
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
vector <pair<double, string>> System::getSubjectsRecord(){
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
vector<Question*> System::generateAutoTest(){
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
void System::addTemplate(Template* temp){
    Templates.push_back(temp);

}
void System::addTest(Test* test){
    Tests.push_back(test);
}

