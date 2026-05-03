#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;
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
string parseName(string a);
#endif