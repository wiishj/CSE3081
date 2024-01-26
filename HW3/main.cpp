#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fp_in;
ofstream fp_out;

struct node{
     unsigned char c;
     int freq;
     node* left; 
     node* right;

     node(const char c, int freq, node* l, node* r)
          :c(c), freq(freq), left(l), right(r){};
};
struct node* root;
int tree_size=0;
vector<node*> tree;
vector<char> input;
vector<bool> output;
vector<bool> bits[256];

int cnt_freq[257];

void tree_push(node* elem){
     tree_size++;
     if(tree.size()<tree_size+1){
          tree.resize(tree_size+1);
     }
     int child = tree_size;
    while (child > 1 && tree[child / 2]->freq > elem->freq) {
        tree[child] = tree[child / 2];
        child /= 2;
    }
    tree[child] = elem;
     
}
node* tree_pop(){
     if(!tree_size) return nullptr; //if tree is empty
     node* tmp_root=tree[1];
     swap(tree[1], tree[tree_size--]);

     /*sort*/
     int parent = 1;
	int child = 2;
     if(child+1<=tree_size){ 
          if(tree[child+1]->freq <tree[child]->freq) child++;
     }
     
	while (child <= tree_size && tree[parent]->freq>tree[child]->freq)
	{    
          swap(tree[child], tree[parent]);
          parent=child;
          child*=2;
          if(child+1<=tree_size){ 
               if(tree[child+1]->freq <tree[child]->freq) child++;
          }
     
          else break;
	}
     
     return tmp_root;
}

void dfs(node* cur, vector<bool>& make_bit){
     if(cur->left == nullptr && cur->right == nullptr){
          bits[cur->c]=make_bit;
          return;
     }
     
     make_bit.push_back(1);
     dfs(cur->left, make_bit);
     make_bit.pop_back();
   
     make_bit.push_back(0);
     dfs(cur->right, make_bit);
     make_bit.pop_back();
     
}

/*construct function for min heap*/
void compress(string& file_name){
     /*base step*/
     fp_in.open(file_name, ifstream::binary);
     if(!fp_in.is_open()){
          cout <<"ERROR : Can not open input file\n";
          return;
     }
     fp_out.open(file_name +".zz", ifstream::binary);

     /*read input file*/
     char c;
     while(fp_in.get(c)){
          cnt_freq[(unsigned char)(c)]++; //count frequency for each c
          input.push_back(c);
     }
     fp_in.close();

     /*make tree for huffman coding*/
     for(int i=0; i<256; i++){
          if(!cnt_freq[i]) continue;
          tree_push(new node(i, cnt_freq[i], nullptr, nullptr));
     }
     /*huffman coing*/
     while(tree_size>1){
          node* left=tree_pop();
          node* right=tree_pop();
          tree_push(new node(0, left->freq + right->freq, left, right));
     }
     root=tree_pop();
     if(tree_size==1)
          root=new node(0, root->freq, root, nullptr);
     
     /*write outputfile*/
     /*1 : the number of char*/
     int cnt_char=0;
     for(int i=0; i<256; i++){
          if(cnt_freq[i]) cnt_char++;
     }
     fp_out << cnt_char;
     for(int i=0; i<256; i++){
          if(!cnt_freq[i]) continue;
          /*2 : character*/
          fp_out << char(i);
          /*3 : frequency using Big endian*/
          fp_out << (char)(cnt_freq[i]>>16);
          fp_out << (char)(cnt_freq[i]>>8);
          fp_out << (char)(cnt_freq[i]);
          //cout<<"char : "<<char(i)<<", freq : ";
          //cout << (cnt_freq[i]>>16);
          //cout << (cnt_freq[i]>>8);
          //cout << cnt_freq[i];
          //cout<<"\n";

     }
     /*make bit for each char*/
     vector<bool> make_bit;
     dfs(root, make_bit);
     /*4 : contents*/
     for(int i=0; i<input.size(); i++){
          char c=input[i];
          for(bool bit : bits[(unsigned char)(c)]){
               output.push_back(bit);
          }
     }
     //cout<<"output.size() : "<<output.size()<<"\n";
     //cout<<"padding len : "<<8-output.size()%8<<"\n";
     int total_len=output.size();
     int padding_len=8-total_len%8;
     for(int i=0; i<padding_len; i++) output.push_back(0); //add padding bits
     
     /*4-1 : the number of padding bits*/
     fp_out << padding_len; 
     /*4-2 : bits for each char*/
     for(int i=0; i<output.size(); i+=8){
          unsigned char write=0;
          for(int j=0; j<8 && j+i<output.size() ; j++){
               write = (write << 1) | output[i + j];
          }
          fp_out<<write;
     }
     fp_out.close();
}

void decompress(string& file_name){
     /*base step*/
     fp_in.open(file_name, ifstream::binary);
     if(!fp_in.is_open()){
          cout <<"ERROR : Can not open input file\n";
          return;
     }
     fp_out.open(file_name + ".yy", ifstream::binary);

     /*construct tree*/
     int cnt_char; char c;
     fp_in>>cnt_char; 
     //cout<<"cnt_char : "<<cnt_char<<"\n";
     
     int freq;
     char tmp;
     for(int i=0; i<cnt_char; i++){
          fp_in.get(c);
          freq=0;
          for(int j=0; j<3; j++){
               fp_in.get(tmp);
               freq=(freq<<8)+(unsigned char)tmp;
          }
          tree_push(new node(c, freq, nullptr, nullptr));
     }

     while(tree_size>1){
          node* left=tree_pop();
          node* right=tree_pop();
          tree_push(new node(0, left->freq + right->freq, left, right));
     }
     root=tree_pop();
     if(tree_size==1)
          root=new node(0, root->freq, root, nullptr);

     vector<bool> make_bits;
     dfs(root, make_bits);
     
     int padding_len;
     fp_in>> padding_len;
     
     while(fp_in.get(c)){
          unsigned char uc=c;
          for(int i=7; i>=0; i--) output.push_back((uc>>i)&1);
     }
     for(int i=0; i<padding_len; i++) output.pop_back();
     //cout<<"padding len :"<<padding_len<<"\n";
     //cout<<"total size : "<<output.size()<<"\n";
     node *cur=root;
     for(int i=0; i<output.size(); i++){
          if(output[i]) cur=cur->left;
          else cur=cur->right;

          if(cur->left == nullptr && cur->right==nullptr){
               fp_out<<cur->c;
               cur=root;
          }
     }
     fp_in.close();
     fp_out.close();
}

int main(int argc, char** argv){
     string file_name=argv[2];
     if(!strcmp("-c", argv[1])) compress(file_name);
     else if (!strcmp("-d",argv[1])) decompress(file_name);
     else{
          cout<<"ERROR : Select -c or -d\n";
          return 0;
     }
     return 0;
}
