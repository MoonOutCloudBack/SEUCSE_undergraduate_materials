#include<iostream>
#include<stack>
#include<vector>
#include<map>
#include<string>
#include<cstring>

using namespace std;

class Node{
public:
    string value;
    vector<Node*> children;
    Node(string v):value(v){}
    Node(){}
};

map<int, vector<string>> productions;
Node* root = NULL;
stack<Node*> st;

void fail(){
    cout<<"parsing failed!\n";
    exit(1);
}

void init(){
    // productions
    string temp1[11] = {"if", "num", "Enext", "cmp", "num", "Enext", "Bnext", "then", "S", "else", "S"};
    productions[1] = vector<string>(temp1, temp1 + 11);

    string temp2[4] = {"id", "=", "num", "Enext"};
    productions[2] = vector<string>(temp2, temp2 + 4);

    string temp3[7] = {"rop", "num", "Enext", "cmp", "num", "Enext", "Bnext"};
    productions[3] = vector<string>(temp3, temp3 + 7);

    productions[4] = vector<string>();

    string temp5[3] = {"op", "num", "Enext"};
    productions[5] = vector<string>(temp5, temp5 + 3);

    productions[6] = vector<string>();
}

bool is_non_terminal(string token){
    return (token == "S" || token == "Bnext" || token == "Enext");
}

int parsing_table(string stacktop, string nextval){
    if(stacktop == "S"){
        if(nextval == "if") return 1;
        if(nextval == "id") return 2;
        fail();
    }
    else if(stacktop == "Bnext"){
        if(nextval == "rop") return 3;
        if(nextval == "then") return 4;
        fail();
    }
    else if(stacktop == "Enext"){
        if(nextval == "op") return 5;
        if(nextval == "then" || nextval == "else" ||
            nextval == "cmp" || nextval == "rop" || nextval == "#") return 6;
        fail();
    }
    return -1;
}

void parse(vector<string> tokens){
    root = new Node("S");
    st.push(root);
    int cur = 0;
    while(!st.empty() && tokens[cur] != "#"){
        string stacktop = st.top()->value;
        if(!is_non_terminal(stacktop)){ // terminal
            if(stacktop == tokens[cur]){
                st.pop();++cur;
            }
            else fail();
        }
        else{ // non terminal
            string nextval = tokens[cur];
            int idx = parsing_table(stacktop, nextval);
            Node *temp = st.top();st.pop();
            int len = productions[idx].size();
            for(int i=0; i<len; ++i){
                Node* newNode = new Node(productions[idx][i]);
                temp->children.push_back(newNode);
            }
            for(int i=len-1; i>=0; --i){
                st.push(temp->children[i]);
            }
        }
    }    
}

void preorder_traversal(Node* node){
    cout<<node->value<<" ";
    for(Node* i:node->children) preorder_traversal(i);
}

void postorder_traversal(Node* node){
    for(Node* i:node->children) postorder_traversal(i);
    cout<<node->value<<" ";
}

int main(){
    init();
    string temptoken[17] = {"if", "num", "op", "num", "cmp", "num",
        "then", "id", "=", "num", "op", "num",
        "else", "id", "=", "num", "#"
    };
    vector<string> token_to_parse = vector<string>(temptoken, temptoken + 17);
    //string badtoken[4] = {"if", "then", "else", "#"}; // 不合法的token序列，报错
    //vector<string> token_to_parse = vector<string>(badtoken, badtoken + 4);

    parse(token_to_parse);
    cout<<"preorder:\n";
    preorder_traversal(root); cout<<"\n\npostorder:\n";
    postorder_traversal(root); cout<<"\n\n";
}