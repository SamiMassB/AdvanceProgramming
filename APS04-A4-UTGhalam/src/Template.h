#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

class Template {
private:
    string name;
    vector<tuple<string, string, int>> parts;

public:
    Template(string n);

    void addSubject(string sub, string diff, int count);
    vector<tuple<string, string, int>> getParts();
    string getName();
};

#endif
