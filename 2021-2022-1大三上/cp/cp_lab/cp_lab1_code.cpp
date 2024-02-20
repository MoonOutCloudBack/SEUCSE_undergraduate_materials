#include<iostream>
#include<queue>
#include<string>
#include<cstring>

using namespace std;

class LexicalAnalyzer{
private:
	// 成员变量
	string input;
	queue<string> output;
	int state;
	int pos; int cur;
public:
    // init
    void init(string i){
        pos = cur = 0; state = 1;
        input = i + '#';
    }

	// utility function
	char curChar() {return input[cur];}
	char nextChar() {return input[cur+1];}
	void fail(){
		cout<<"lexical analysis fail!\n";
		exit(1);
	}
	bool isLetter(char c){
		return (c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z');
	}
	bool isZero(char c) {return c == '0';}
	bool isNonzeroDigit(char c){return c >= '1' && c <= '9';}
	bool isDigit(char c) {return isZero(c) || isNonzeroDigit(c);}
	bool isPoint(char c) {return c == '.';}
	bool isOperator(char c){
		return (c == '+') || (c == '-') || (c == '*') || (c == '/');
	}

	// get next token
    bool anaIdentifier(){
        cur = pos;
        char curc = curChar();
        while(1){
            switch(state){
            case 1:
                ++cur; state = 2;
                break;
            case 2:
                curc = curChar();
                if(!(isLetter(curc) || isDigit(curc))){
                    output.push("<identifier>");
                    pos = cur;
                    state = 1;
                    return true;
                }
                else{
                    ++cur; break;
                }
            default:
                state = 1; cur = pos; return false;
            }
        }
    }
    bool anaInteger(){
        cur = pos;
        char curc = curChar();
        while(1){
            switch(state){
            case 1:
                ++cur; state = 3;
                break;
            case 3:
                if(!isDigit(curChar())){
                    if(isPoint(curChar())){
                        state = 1;
                        cur = pos;
                        return false;
                    }
                    output.push("<integer>");
                    pos = cur;
                    state = 1;
                    return true;
                }
                else{
                    ++cur; break;
                }
            default:
                state = 1; cur = pos; return false;
            }
        }
    }
    bool anaFloat(){
        cur = pos;
        char curc = curChar();
        while(1){
            switch(state){
            case 1:
                if(isZero(curChar())) state = 6;
                else state = 5;
                ++cur; 
                break;
            case 5:
                if(isDigit(curChar())){
                    ++cur; break;
                }
                else if(isPoint(curChar())){
                    state = 7; ++cur; break;
                }
                else {
                    state = 1; cur = pos; return false;
                }
            case 6:
                if(isPoint(curChar())){
                    state = 7; ++cur; break;
                }
                else {
                    state = 1; cur = pos; return false;
                }
            case 7:
                if(isDigit(curChar())){
                    state = 8; ++cur; break;
                }
                else {
                    state = 1; cur = pos; return false;
                }
            case 8:
                if(!isDigit(curChar())){
                    output.push("<float>");
                    pos = cur;
                    state = 1;
                    return true;
                }
                else{
                    ++cur; break;
                }
            default:
                state = 1; cur = pos; return false;
            }
        }
    }
    bool anaOperator(){
        cur = pos;
        char curc = curChar();
        while(1){
            switch(state){
            case 1:
                ++cur; state = 9; break;
            case 9:
                output.push("<operator>");
                pos = cur;
                state = 1;
                return true;
            default:
                state = 1; cur = pos; return false;
            }
        }
    }
	
    // analysis
    void analysis(){
        while(curChar() != '#'){
            if(isLetter(curChar())){
                if(anaIdentifier()) continue;
            };
            if(isNonzeroDigit(curChar())){
                if(anaFloat()) continue;
                if(anaInteger()) continue;
            }
            if(isZero(curChar())){
                if(anaFloat()) continue;
            }
            if(isOperator(curChar())){
                if(anaOperator()) continue;
            }
            fail();        
        }
    }

    // print result
    void printOutput(){
        cout<<"the result of lexical analysis is:\n";
        while(!output.empty()){
            cout<<output.front()<<" "; output.pop();
        }
        cout<<"\n\n";
    }
};

int main(){
    LexicalAnalyzer la;
    string input;
    while(true){
        cout<<"please input your string:\n";
        cin>>input;
        la.init(input);
        la.analysis();
        la.printOutput();
    }
}