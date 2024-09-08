#include "calculus_tree.h"
    const int keyword_count =13 ;

    const string key_words[keyword_count]={"sin","cos","tan","sec","csc","cotan",
                              "asin","acos","atan", "exp","ln","pi","log"};


    node * node::get_node(const string &symbol){
        node*ret_node = NULL;
        ret_node = new node ;
        if(ret_node){
            ret_node->symbol = symbol ;
            ret_node->parent = NULL;
            ret_node->children =NULL ;
            ret_node->next = ret_node;
            return ret_node  ;
        }
        return NULL;
    }

    bool node::append_next(const string &symbol){
        if(parent){
            node*tail = parent->children ;

            node*new_node= get_node(symbol);
            if(new_node){
                new_node->next= tail->next;
                new_node->parent=  this->parent;
                tail->next= new_node;
                parent->children=  new_node;

                return true;
            }
        }
        return false;
    }

    bool node::append_child(const string &symbol){
        if(children==NULL){
            children = get_node(symbol);
            children->parent = this;
        }
        else{
            children->append_next(symbol);
        }
    }

    bool node::apeend_parent(const string&symbol){
        if(parent==NULL){
            parent= get_node(symbol);
            if(parent){
                parent->children = this;
            }
        }
        else{
            //when adjusting parent of this
            //all his siblings must have same parent
            node *new_parent = get_node(symbol);
            if(new_parent){

                new_parent->parent=parent;
                new_parent->children = parent->children;
                parent->children = new_parent;

                parent = new_parent;

                //loop through each child of this siblings and adjust their parents
                node*ch_ptr = new_parent->children->next;
                do{
                    ch_ptr->parent=new_parent;
                    ch_ptr=ch_ptr->next;

                }while(ch_ptr!=new_parent->children->next);
            }
        }
    }
    /*
    previous operations but on pointers
    */
    bool node::append_next(node*&src_root){
        if(src_root&&parent){
            node*tail = parent->children ;

            src_root->next= tail->next;
            src_root->parent=  this->parent;
            tail->next= src_root;
            parent->children=  src_root;

            return true;
        }
        return false;
    }

    bool node::append_child(node*&src_root){
        if(src_root){
            if(children==NULL){
                children = src_root;
                children->parent = this;
            }
            else{
                children->append_next(src_root);
            }
        }
    }

    bool node::apeend_parent(node*&src_root){
        if(src_root){
            if(parent==NULL){
                parent= src_root;
                if(parent){
                    parent->children = this;
                }
            }
            else{
                //when adjusting parent of this
                //all his siblings must have same parent
                node *new_parent = src_root;
                if(new_parent){

                    src_root->parent=parent;
                    src_root->children = parent->children;
                    parent->children = src_root;

                    parent = src_root;

                    //loop through each child of this siblings and adjust their parents
                    node*ch_ptr = src_root->children->next;
                    do{
                        ch_ptr->parent=src_root;
                        ch_ptr=ch_ptr->next;

                    }while(ch_ptr!=src_root->children->next);
                }
            }
        }
    }

    bool calculus_tree::remove_node(node*&src) {
        if(src){
            if(src->parent){

                if(src->next==src){
                    src->parent->children = NULL;
                }
                else{
                    node*ptr = src->parent->children;
                    while(ptr->next!=src){
                        ptr=ptr->next ;
                    }
                    if(ptr->next==src->parent->children){
                        //then i'm deleting tail
                        src->parent->children= ptr ;
                    }
                    ptr->next=ptr->next->next;
                }
            }
            queue<node*>q ;
            q.push(src);
            while(!q.empty())
            {
                node*temp = q.front();
                q.pop();
                if(temp->children){
                    node*ptr= temp->children->next;
                    do{
                        q.push(ptr);
                        ptr=ptr->next;
                    }while(ptr!=temp->children->next);
                }
                delete temp ;
                temp=NULL;
            }
            src=NULL;
        }
    }

    calculus_tree::~calculus_tree(){
        remove_node(root);
        root= NULL;
    }
    calculus_tree ::calculus_tree(const string&expression){
        root =NULL ;
        node *temp = NULL ;
        unsigned int start = 0;
        while(start<expression.length()){
            if(expression[start]=='('){
                if(root==NULL){
                    root = parse_paranthese(expression,start) ;
               }
               else{
                    temp=parse_paranthese(expression,start) ;
                    root->append_child(temp);
               }
            }
            else if(expression[start]==')'){
                //then this is out of place closing bracket
                return ;
            }
            else if(is_op(expression,start)){
                if(root){
                    root->apeend_parent(extract(expression,start));
                    root=root->parent;
                }
                else{
                    remove_node(root);
                    root = NULL;
                }
            }
            else if(is_keyword(expression,start)){
                temp = parse_function(expression,start);
                if(root){
                    root->append_child(temp);
                }
                else{
                    root=temp;
                }
            }
            else{
                temp = parse_expression(expression,start) ;
                if(root){
                    root->append_child(temp);
                }
                else{
                    root = temp;
                }
            }
        }
    }

    calculus_tree::calculus_tree(void){
        root=NULL;
    }
    bool calculus_tree::remove_tree(void){
        remove_node(root);
        root =NULL;
    }
    void calculus_tree::print(node* ptr) const {
    if(root){
        if(ptr == NULL){
            ptr = root;
        }
        if(ptr->children){
            cout << "(";

            node* ch_ptr = ptr->children->next;
            do {
                print(ch_ptr);
                if(ptr->children!=ch_ptr){
                    cout << ptr->symbol;
                }
                ch_ptr = ch_ptr->next;
            } while(ch_ptr != ptr->children->next);

            cout << ")";
        }
        else{
            cout << ptr->symbol;
        }
    }
}

string calculus_tree::expression(node* ptr) const {
    if(root){
        string ret_exp = "";
        if(ptr == NULL){
            ptr = root;
        }
        if(ptr->children){
            ret_exp += "(";

            node* ch_ptr = ptr->children->next;
            do {
                ret_exp += expression(ch_ptr);
                if(ptr->children!=ch_ptr){

                    ret_exp += ptr->symbol;
                }
                ch_ptr = ch_ptr->next;
            } while(ch_ptr != ptr->children->next);

            ret_exp += ")";
        }
        else{
            ret_exp += ptr->symbol;
        }
        return ret_exp;

    }
    return "";
}

    bool calculus_tree::is_op(const string&expression,unsigned int pos ) {
        switch(expression[pos]){
            case '+':return true ;break;
            case '-':return true ;break;
            case '*':return true ;break;
            case '/':return true ;break;
            case '^':return true ;break;
            case '(':return true ;break;
            case ')':return true ;break;
        }
        return false ;
    }

    //this extracts operand (whatever its length) or operator
    //start goes to second operand or operator
    string calculus_tree::extract(const string&expression,unsigned int &start){
        string var="";
        if(start<expression.length()){
            if(is_op(expression,start)){
                var+=expression[start];
                start++;
            }
            else{
                while(start<expression.length()&&!is_op(expression,start)){
                    var+=expression[start];
                    start++;
                }
            }
        }
        return var ;
    }

    //assumming it's not '('
    node*calculus_tree::parse_operation(const string&expression,unsigned int &start){
        if(start<expression.length()){
            node*ret_root = NULL;
            //the moment i find a new operator return the subtree of that operation
            queue<string>q ;
            while(start<expression.length()&&expression[start]!='('&&expression[start]!=')'){
                if (!is_op(expression,start))
                {
                    q.push(extract(expression,start));
                }
                else{
                    if(ret_root==NULL){
                        if(!q.empty()){
                            ret_root = ret_root->get_node(extract(expression,start));
                        }
                        else{
                            //extracted operator without any values before it doesn't make sense
                            return NULL ;
                        }
                    }
                    else{
                        if(!(ret_root->symbol[0]==expression[start])){
                            fill_children(q,ret_root);
                            return ret_root;
                        }
                    }
                }
            }
            if(q.size()==1){
                //means this we found this -> 5-(
                //extracted 5 and - we only take the 5
                if(expression[start]=='('){
                    start--;
                    remove_node(ret_root) ;
                    ret_root = ret_root->get_node(q.front());
                    q.pop() ;
                    }
                if(ret_root==NULL){
                    ret_root=ret_root->get_node(q.front());
                }
            }
            else{
                fill_children(q,ret_root);
            }
            return ret_root;
        }

        return NULL;
    }

    void calculus_tree::fill_children(queue<string>&q ,node*&ret_root){
        if(ret_root){
            while(!q.empty()){
                ret_root->append_child(q.front());
                q.pop();
            }
        }
    }
    //assuming start at first encountered '('
    node* calculus_tree::parse_paranthese(const string& expression, unsigned int &start) {
        if(start < expression.length()) {
            node* ret_root = NULL;
            node* temp = NULL;
            if(expression[start] == '(') {
                start++;
                temp = parse_paranthese(expression, start); // Parse the contents of the parentheses
                if(ret_root == NULL) {
                    ret_root = temp;
                }
                else {
                    ret_root->append_child(temp);
                }
            }
            if(expression[start] == ')') {
                start++; // Skip the closing parenthesis
                return ret_root; // Return the root of the parsed subtree
            }
            if(is_op(expression, start)) {
                if(ret_root) {
                    ret_root->apeend_parent(extract(expression, start));
                    ret_root = ret_root->parent;
                } else {
                    // Something is wrong, clean up and return NULL
                    remove_node(ret_root);
                    return NULL;
                }
            }
            if(is_keyword(expression,start)){
                temp = parse_function(expression, start);
                if(ret_root == NULL) {
                    ret_root = temp;
                }
                else {
                    ret_root->append_child(temp);
                }
            }
            else {
                temp = parse_expression(expression, start);
                if(ret_root == NULL) {
                    ret_root = temp;
                } else {
                    ret_root->append_child(temp);
                }
            }
        }
        else{
            return NULL;
        }
    }
    //extract a normal expression without paranthese
    node*calculus_tree::parse_expression(const string&expression,unsigned int &start){
        if(expression[start]!='('&&expression[start]!=')'){
            node*ret_root = parse_operation(expression,start);
            node*temp= NULL;
            while(start<expression.length()&&expression[start]!='('&&expression[start]!=')'){
                //now we have a temp tree say
                /*
                    *
                 5  x  t
                */
                //ret_root is always on top
                //so if operator coming from extract is same as operator of the root
                //then root stays the same
                string temp_op = extract(expression,start);
                if(ret_root->symbol!=temp_op){
                    ret_root->apeend_parent(temp_op);
                    if(ret_root->parent){
                        ret_root=ret_root->parent;
                    }
                }
                if(start<expression.length()){
                    if(expression[start]!='('&&expression[start]!=')'){
                        temp = parse_operation(expression,start);
                        if(temp){
                            ret_root->append_child(temp);
                        }
                    }
                    else{
                        //we remove the latest operator
                        //and the root becomes children
                        remove_root_keep_children(ret_root);
                        start--;
                        return ret_root;
                    }
                }
                else{
                    //we remove the latest operator
                    //and the root becomes children
                    remove_root_keep_children(ret_root);
                    start--;
                    return ret_root;
                }
            }
            if(start<expression.length()&&(expression[start]=='(')){
                if(ret_root->children&&ret_root->children->next==ret_root->children){
                    //one child pointing at itself
                    remove_root_keep_children(ret_root);
                    start--;
                }
            }
            return ret_root ;
        }
        return NULL;
    }
    void calculus_tree::remove_root_keep_children(node*&ret_root){
        if(ret_root){
            node *temp= ret_root ;
            ret_root=ret_root->children;
            temp->children=NULL;
            ret_root->parent=NULL;
            remove_node(temp);
        }
    }

    bool calculus_tree:: is_keyword(const string&expression ,unsigned int pos){
         string temp=  extract(expression,pos);
         if(temp.substr(0,3)=="log"){
            return 1 ;
         }
         for(int i =0 ; i <keyword_count-1;i++){
            if(temp==key_words[i]){
                return 1;
            }
         }
         return 0 ;
    }
    //assuming it's a keywrod AND its a function
    node*calculus_tree::parse_function(const string&expression,unsigned int &start){
        if(start<expression.length()){
            string var = extract(expression,start);
            node*ret_root = parse_paranthese(expression,start);
            if(ret_root){
                ret_root->apeend_parent(var);
                ret_root=ret_root->parent;
                return ret_root ;
            }

        }
        return NULL;
    }
int main(){
    calculus_tree tree("sin(x*2)*2");
    cout<<tree;
    system("pause");
    return 0 ;
}
