//
//  eval.cpp
//  hw2
//
//  Created by ZhimingZhuang on 2/1/16.
//  Copyright (c) 2016 ZhimingZhuang. All rights reserved.
//

#include <stdio.h>
#include "Map.h"
#include <string>
#include <stack>
using namespace std;

bool isValidExpression(string infix);
bool isLetter(char c);
bool isOperator(char c);
bool preOper(char ch, char post);

int evaluate(string infix, const Map& values, string& postfix, int& result){
    string s = "";
    bool mapcontains = true;
    
    if(!isValidExpression(infix))
        return 1;
    else{
        stack<char> stkoper;
        
        for(int i = 0; i < infix.size(); i++){
            char ch = infix[i];
            switch (ch) {
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'g':
                case 'h':
                case 'i':
                case 'j':
                case 'k':
                case 'l':
                case 'm':
                case 'n':
                case 'o':
                case 'p':
                case 'q':
                case 'r':
                case 's':
                case 't':
                case 'u':
                case 'v':
                case 'w':
                case 'x':
                case 'y':
                case 'z':
                    if(!values.contains(ch))
                        mapcontains = false;
                    s += ch;
                    break;
                    
                case '(':
                    stkoper.push(ch);
                    break;
                
                case ')':
                    while(stkoper.top() != '('){
                        s += stkoper.top();
                        stkoper.pop();
                    }
                    stkoper.pop();
                    break;
                    
                case '+':
                case '-':
                case '*':
                case '/':
                    while (!stkoper.empty() && stkoper.top() !=  '(' && preOper(ch, stkoper.top())) {
                        s += stkoper.top();
                        stkoper.pop();
                    }
                    stkoper.push(ch);
                    break;
                    
                default:
                    break;
            }
        }
        
        while(!stkoper.empty()){
            s += stkoper.top();
            stkoper.pop();
        }
    }
    postfix = s;
    if(!mapcontains)
        return 2;
    
    stack<int> stkEva;
    int realNum;
    int tmp = 0;
    
    for(int i = 0; i < postfix.size(); i++){
        char ch = postfix[i];
        
        if(isLetter(ch)){
            values.get(ch, realNum);
            stkEva.push(realNum);
        }
        else{
            int v2 = stkEva.top();
            stkEva.pop();
            int v1 = stkEva.top();
            stkEva.pop();
            if(ch == '/' && v2 == 0)
                return 3;
            else{
                switch (ch) {
                    case '+':
                        tmp = v1 + v2;
                        break;
                    case '-':
                        tmp = v1 - v2;
                        break;
                    case '*':
                        tmp = v1 * v2;
                        break;
                    case '/':
                        tmp = v1 / v2;
                        break;
                    default:
                        break;
                }
                stkEva.push(tmp);
            }
        }
    }
    result = stkEva.top();
    return 0;
}

bool isValidExpression(string infix){
    stack<char> parentheses;
    stack<char> stk;
    
    for(int i = 0; i < infix.size(); i++){
        char c = infix[i];
        
        
        if(c == '(')
            parentheses.push('(');
        else if(c == ')'){
            if(parentheses.size() == 0)
                return false;
            else{
                char tmp1 = parentheses.top();
                if(tmp1 == '(')
                    parentheses.pop();
                else
                    return false;
            }     
        }else if(isOperator(c)){
            if(stk.size() == 0)
                return false;
            else{
                char tmp2 = stk.top();
                if(isLetter(tmp2))
                    stk.push(c);
                else
                    return false;
            }
        
        }else if(isLetter(c)){
            if(stk.empty())
                stk.push(c);
            else{
                char tmp3 = stk.top();
                if(isOperator(tmp3))
                    stk.push(c);
                else
                    return false;
            }
        }else if(c == ' '){
            continue;
        }else{
            return false;
        }
    }
    
    if (parentheses.size() == 0 && stk.size() > 0 && isLetter(stk.top()))
        return true;
    else
        return false;



}


bool isLetter(char c){
    if(c >= 'a' && c <='z')
        return true;
    else
        return false;
}


bool isOperator(char c){
    if(c == '+' || c == '-' || c == '*' || c == '/')
        return true;
    else
        return false;
}


bool preOper(char ch, char post){
    if(ch == '+' || ch == '-')
        return true;
    
    if(ch == '*' || ch == '/'){
        if(post == '*' || post == '/')
            return true;
        else
            return false;
    }
    return false;
}