#include "calculus_tree.h"
    const int keyword_count =12 ;

    const string key_words[keyword_count]={"sin","cos","tan","sec","csc","cotan",
                              "asin","acos","atan", "exp","ln","log"};


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

    bool node::append_parent(const string&symbol){
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

    bool node::append_parent(node*&src_root){
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
    /*
    //where op is new parent of last op
    //and op is new children to parent of last op
    //and then last op becomes parent of last op
    last_op->exchange_parent(op);

    */
    bool node::exchange_parent(const string&op) {
        //parent of this becomes gparent
        node*new_parent = get_node(op);
        node*gparent = parent ;
        if(new_parent){
            new_parent->parent= gparent ;
            new_parent->children=this ;
            if(this->next!=this){
                //if not a singular node in a list
                node * before_this = gparent->children;
                while(before_this->next!=this){
                    before_this= before_this->next ;
                }
                if(this==parent->children){
                    gparent->children=before_this;
                }
                before_this ->next=before_this->next->next ;
                this->next=this;
            }
            node * temp = this ;
            new_parent->append_child(temp);
            new_parent->append_parent(gparent);
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
        unsigned int start= 0;
        root =parse_expression(expression,start) ;
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
            if(is_keyword(ptr)){
                cout<<ptr->symbol;
                print(ptr->children);
            }
            else{
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
    }

string calculus_tree::expression(node* ptr) const {
    if(root){
        string ret_exp = "";
        if(ptr == NULL){
            ptr = root;
        }
        if(is_keyword(ptr)){
            ret_exp+=ptr->symbol;
            ptr=ptr->children;
        }
        else{
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
    //+- ,*/ ,^, () & functions ,
    int calculus_tree::precedence(const string&expression,unsigned int pos){
            switch(expression[pos]){
                case '^':return 3 ;
                case '*':return 2 ;
                case '/':return 2 ;
            }
            return 1  ;
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
                    ret_root->append_parent(extract(expression, start));
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
    node* calculus_tree::parse_expression(const string&expression,unsigned int & start){
        if(start<expression.length()){
            /*
            parsing must be made on variable level
            no separation can be done sadly
            */
            node*ret_root =NULL;
            node*last_op=NULL;
            node*temp= NULL;
            string var ="";
            string op = "";
            bool new_op= false ;
            bool new_var = false;
            if(!is_op(expression,start)&&!is_keyword(expression,start)){
                var = extract(expression,start);
            }
            if(is_op(expression,start)&&expression[start]!='('){
                op=extract(expression,start);
            }
            if(var.length()&&op.length()){
                ret_root =ret_root->get_node(op);
                ret_root->append_child(var) ;
                last_op = ret_root;
                while(start<expression.length()){
                    new_var =false ;
                    new_op = false ;
                    if(!is_op(expression,start)&&!is_keyword(expression,start)){
                        var = extract(expression,start);
                        new_var = true ;
                    }
                    if(is_op(expression,start)&&expression[start]!='('){
                        op = extract(expression,start);
                        new_op = true ;
                    }
                    if(new_var){
                        if(new_op){
                            int diff = precedence(op,0)-precedence(last_op->symbol,0);
                            if(diff>0){
                                temp = temp->get_node(op) ;
                                temp->append_child(var) ;
                                last_op->append_child(temp) ;
                                last_op = last_op->children;
                            }
                            else{

                                last_op->append_child(var);
                                //where op is new parent of last op
                                //and op is new children to parent of last op
                                //and then last op becomes parent of last op
                                last_op->exchange_parent(op);
                                if(ret_root==last_op){
                                    ret_root=ret_root->parent;
                                }
                                last_op=last_op->parent;
                            }

                        }
                        else{
                            last_op->append_child(var);
                        }
                    }
                }
            }
        }

        return NULL ;
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

    bool calculus_tree:: is_keyword(node*&ptr)const{
        if(ptr){
             if(ptr->symbol.substr(0,3)=="log"){
                return 1 ;
             }
             for(int i =0 ; i <keyword_count-1;i++){
                if(ptr->symbol==key_words[i]){
                    return 1;
                }
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
                ret_root->append_parent(var);
                ret_root=ret_root->parent;
                return ret_root ;
            }
        }
        return NULL;
    }

    int main(){
        calculus_tree tree("9+10+18+5+8*2/5-9*9");
        cout<<tree;
        system("pause");
        return 0 ;
    }
