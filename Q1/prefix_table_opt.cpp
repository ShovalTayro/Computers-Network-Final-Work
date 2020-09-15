#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

    class trie{        
        struct trieNode{
            char data;
            int countPath = 1;
            trieNode *parent;
            trieNode *left = NULL;
            trieNode *right = NULL; 
        };

        public:
        int size;
        trieNode *root;

        //defualt constractor
        trie() : root (new trieNode), size(1){
            root->countPath = 1;
        }

        public:
        
        void insert(string key, int prefix, char action){
            char index;
            trieNode *node = root;
            for (int i = 0; i < prefix-1; i++){
                index = key.at(i);
                if(index == '0'){ //go left
                    //if there is no left son add a new trieNode to the trie
			        if(node->left == NULL){ 
                        if(node->right != NULL)node->countPath++;
				        node->left = new trieNode;
				        this->size++;
                        node->left->parent = node;
                    }
                    //if there is a left son, update the path
                    else node->countPath++;
                    node = node->left;
                }
                else if(index == '1'){ //go right
                    //if there is no right son add a new trieNode to the trie
			        if(node->right == NULL){
                        if(node->left != NULL)node->countPath++;
				        node->right = new trieNode;
				        this->size++; 
                        node->right->parent  = node;
                    }
                    //if there is a right son, update the path
                    else node->countPath++;
                    node = node->right;
                }
            }
            //optimization operation
            if(key.at(prefix-1) == '1'){
                //if there is left son with the same action
                if(node->left != NULL && node->left->data == action){
                    //if only one path go through over left son node - delete him
                    if(node->left->countPath == 1){
                        node->left = NULL;
                        delete(node->left);
                        size--;
                        node->data = action;
                    }
                    //if there is more than one path- update tne countPath and the data
                    else{
                        node->left->countPath--;
                        node->left->data = '\0';
                        node->data = action;
                    }
                    node = node->parent;
                    //update all count path until the root
                    while(node != NULL){
                        node->countPath--;
                        node= node->parent;
                    }  
                }
                //if there isn't a left son with the same action
                else{ 
                    //if right son is NULL add a new node
                   if(node->right == NULL){
                       if(node->left != NULL)node->countPath++;
				        node->right = new trieNode;
				        this->size++;
                        node->right->parent = node;
				        node->right->left = NULL;
				        node->right->right = NULL;
                    }
                    node->right->data = action;
                }
            }
            if(key.at(prefix-1) == '0'){
                //if there is right son with the same action
                if(node->right != NULL && node->right->data == action){
                    //if only one path go through over right son node - delete him
                    if(node->right->countPath == 1){
                        node->right = NULL;
                        delete(node->right);
                        size--;
                        node->data = action;
                    }
                    //if there is more than one path- update tne countPath and the data
                    else{
                        node->right->countPath--;
                        node->right->data = '\0';
                        node->data = action;
                    };
                    //update all count path
                    node = node->parent;
                    while(node != NULL){
                        node->countPath--;
                        node= node->parent;
                    }   
                }
                //if there isn't a right son with the same action
                else{ 
                    //if left son is NULL add a new node 
                   if(node->left == NULL){
                       if(node->right != NULL)node->countPath++;
				        node->left = new trieNode;
				        this->size++;
                        node->left->parent = node;
				        node->left->left = NULL;
				        node->left->right = NULL;
                   }
                    node->left->data = action;
                }
            }
        }

        //find the last node of the IP adress
        trieNode* help_search(string key, int prefix, char action){
	        if(root != NULL){
                trieNode *node = root;
                char index;
                //go on the path of the key
                for (int i = 0; i < prefix; i++){
                    index = key.at(i);
                    if(index == '1'){ //go right
                        if(node->right != NULL) node = node->right;
                        else return NULL;
                    }
                    if(index == '0'){ //go left
                        if(node->left != NULL) node = node->left;
                        else return NULL;
                    }    
                }
                //if the action is the same return this node
                if(node->data == action) return node;
                else return NULL;                
            }
            else return NULL;
        }

        bool remove(string key,int  prefix, char action){
            //find the last node of the IP key
            trieNode* toRemove = help_search(key, prefix, action);
            if(toRemove != NULL){
                remove(key, prefix, toRemove);
                return true;
            }
            else return false;
        }
        
        //recursive function to remove the ip from the trie
        bool remove(string key, int prefix, trieNode* toRemove) {
            if (prefix == 0) return true;
            trieNode* temp = toRemove->parent;
            if(prefix > 0){
                //if there is more than one path going throw this node
                if(toRemove->countPath != 1) toRemove->countPath--;
                //else, if there is one path - delete this node from trie
                else {
                    toRemove = nullptr;
                    if(key.at(prefix-1)=='1') temp->right = nullptr;
                    if(key.at(prefix-1)=='0') temp->left = nullptr;
                    delete toRemove;
                    size--;
                }
            }
            return remove(key, --prefix, temp);
        }	 

        int search(string key){
            int steps = 0;
	        if(root != NULL){
                trieNode *node = root;
                char index;
                //go on the path of the key and count the steps
                for (int i = 0; i < key.length(); i++){
                    index = key.at(i);
                    if(index == '1'){ //go right
                        if(node->right != NULL) {
                            steps++;
                            node = node->right;
                        }
                        else break;
                    }
                    if(index == '0'){ //go left
                        if(node->left != NULL) {
                            steps++;
                            node = node->left;
                        }
                        else break;
                    }    
                }
                int count  = steps;
                while(count < key.length()){
                    if(key.at(count) != '0') return 0;
                    count++; 
                }
            }
            return steps;
        }

        //calculate max depth of the tree
        int depth(trieNode* node){
            if(node== NULL) return 0;
            else{
                int Ldepth = depth(node->left);
                int Rdepth = depth(node->right);

                if(Ldepth > Rdepth) return (Ldepth+1); //get the max depth
                else return (Rdepth+1);
            }
        }
    };

//convert to binary
string to_binary(string key){
    int num = stoi(key); //convert string to int
    string ans;
    for (int i = 0; i < 8; i++){
        if(num%2 == 1) ans = "1" + ans;
        else ans = "0" + ans;
        num = num/2;
    }
    return ans;
}

int main(int argc , char** argv){
    //read from file
    string line;
    ifstream readFromFile(argv[1]);
    trie *tr = new trie();
    while (getline (readFromFile, line)) {
        istringstream ss(line);
        string operation, ipAddress;
        char action;
        ss>>operation>>ipAddress>>action;

        //split the IP adress and prefix
        string temp = ipAddress;
        replace(temp.begin(), temp.end(), '/', ' ');
        stringstream splitPrefix(temp);
        string ip; //ip address
        int prefix; // prefix

        splitPrefix >> ip >> prefix;
        replace(ip.begin(), ip.end(), '.', ' ');
        //split the IP adress and convert to binary
        stringstream split(ip);
        string number, key;
        //convert to binary;
        while (split >> number){
            key = key + to_binary(number);
        }

        if(line!=""){
            if(operation == "ADD"){
                tr->insert(key, prefix, action);
                cout << "Added " << ipAddress << " " << action << " at the depth " << (tr->depth(tr->root)-1) << ", total nodes " <<  tr->size << endl;
            }
            else if(operation == "REMOVE"){
                if(tr->remove(key, prefix, action)){
                    cout<< "Removed " << ipAddress << " " << action << " at the depth " <<  (tr->depth(tr->root)-1) << ", total nodes " << tr->size <<endl;
                }
                else cout << "There is no " << ipAddress << " " << action << endl;
                
            }
            else if(operation == "FIND"){//FIND
                int steps = tr->search(key);
                if(steps > 0){
                    cout << "Found " << ipAddress << " at the depth " << steps << endl;
                }
                else cout << ipAddress << " not found" << endl;
            }
            else{
                throw("illegal operation");
            }
        }
    }
}