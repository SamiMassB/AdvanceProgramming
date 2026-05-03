#include "Template.h"
#include "config.h"
#include <vector>
#include <tuple>
#include <string>

using namespace std;


Template::Template(string n){
    name = n;
}
void Template::addSubject(string sub, string diff, int count){
    parts.push_back(make_tuple(sub, diff, count));
    

}
vector<tuple<string, string, int>> Template::getParts(){
    return parts;
}
string Template::getName(){
    return name;
}



