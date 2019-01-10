#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <map>
#include <vector>
#include "parse.h"
#include "tokens.h"
using namespace std;

void Traverse(ParseTree* t){

    if (t == 0)
        return;
    if (t->getLeft() != 0) {
        cout << "Left";
        Traverse(t->getLeft());
    }
    if (t->getRight() != 0) {
        cout << "Right";
        Traverse(t->getRight());
    }

}

int main(int argc, char *argv[]){

    //cout << "Starting project3" << endl;

    ifstream file;
    istream *in = &cin;
    int lN = 0;
    string ident = ""; 

    for (int i = 1; i < argc; i++) {
        string arg(argv[i]);
        if (in != &cin) {
            cout << "TOO MANY FILES" << endl;
            return 0;
        }
        file.open(arg);
        if (file.is_open() == false) {
            cout << arg << " FILE NOT FOUND" << endl;
            return 0;
        }

        in = &file;
    }

    ParseTree *tree = Prog(in, &lN);
    map<int, string> counter;
    int j = 1;

    if (tree == 0) {
        cout << "Tree does not exist." << endl;
        return 1;
    }

    cout << "LEAF COUNT: " << tree->LeafCount() << endl;
    cout << "STRING COUNT: " << StringC << endl;

    if(IC != 0){
        cout << "IDENT COUNT: " << IC << endl;
            for(map<string, int>::iterator i = m.begin(); i!=m.end(); i++){
                counter[i->second] += i->first;
                counter[i->second] += ", ";
            }
            
            int size = counter.size();

            for(map<int, string>::iterator i = counter.begin(); i!=counter.end(); i++){
                if(j == size){
                    ident += i->second;
                }
                j++;
            }

        ident = ident.substr(0, ident.length()-2);    
        cout << ident << endl;
    }


}

/*
for(map<string, int>::iterator i = m.begin(); i!=m.end(); i++){
                ident += i->first;

                if(j!=size){
                    ident+=", ";
                }
                j++;

            }
            */

