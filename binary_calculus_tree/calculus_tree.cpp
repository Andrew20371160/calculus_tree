#include "calculus_tree.h"
    const int keyword_count =12 ;

    const string key_words[keyword_count]={"sin","cos","tan","sec","csc","cotan",
                              "asin","acos","atan","exp","ln","log"};


    node * node::get_node(const string &symbol){
        node*ret_node = NULL;
        ret_node = new node ;
        if(ret_node){
            ret_node->symbol = symbol ;
            ret_node->parent = NULL;
            ret_node->left =NULL ;
            ret_node->right = NULL;
            return ret_node  ;
        }
        return NULL;
    }
    //appends symbol as a new sibling to this
    //and a new child to parent of this
    //must have a parent to do the operation
    //so that the tree structure isn't ruined
    bool node::append_next(const string &symbol){
        if(parent&&parent->right==NULL){
            node*new_node= get_node(symbol);
            if(new_node){
                new_node->parent= parent;
                parent->right = new_node ;
                return true;
            }
        }
        return false;
    }
    /*
    appends symbol as a new child to this
    */
    bool node::append_child(const string &symbol){
        if(left==NULL||right==NULL){
            node*new_node= get_node(symbol);
            if(new_node){
                if(left==NULL){
                    left= new_node ;
                }
                else{
                    right= new_node ;
                }
                new_node->parent= this ;
                return true ;
            }
        }
        return false ;
    }
    /*
    disconnects this from his parent and his siblings
    not from his children
    */
    bool node::disconnect_self(void) {
        if(parent){
            if(parent->left==this){
                parent->left=NULL;
            }
            else{
                parent->right=NULL ;
            }
            parent=NULL ;
            return true ;
        }
        return false;
    }
    /*
    symbol is a new parent and his first child becomes this
    this function disconnects this from its sibling and assign a new parent
    to it which is symbol
    */
    bool node::append_parent(const string&symbol){
        node*new_parent = get_node(symbol);
        if(new_parent){
            //disconnect this from its parent and its sibling
            this->disconnect_self();
            //then append this as the new child of symbol
            this->parent = new_parent ;
            new_parent->left= this ;
            return true;
        }
        return false ;
    }

    /*
    previous operations but on pointers
    */
    /*
    append src_root as a sibling to this
    and a child of its parent
    so first disconnect src_root from his parent and siblings
    assign his new parent as parent of this
    */
    bool node::append_next(node*&src_root){
        if(parent&&parent->right==NULL&&src_root){
            src_root->disconnect_self() ;
            //then connect src_root as a new sibling of this
            src_root->parent= parent ;
            parent->right = src_root ;
            return true ;
        }
        return false ;
    }
    /*
    append src_root as a child to this
    so first disconnect src_root from his parent and siblings
    then does the connections
    */
    bool node::append_child(node*&src_root){
        if(src_root&&(left==NULL||right==NULL)){
            src_root->disconnect_self() ;
            if(left==NULL){
                left= src_root ;
            }
            else{
                right= src_root ;
            }
            src_root->parent= this ;
            return true ;
        }
        return false ;
    }
    //this function separates this from his parent's list
    //and put this into the childrent of src_root
    //if null then its new parent is null
    bool node::append_parent(node*&src_root){
        if(src_root==NULL){
            this->disconnect_self() ;
        }
        else if(src_root->left==NULL||src_root->right==NULL){
            this->disconnect_self();
            node*temp = this ;
            src_root->append_child(temp) ;
        }
        return true ;
    }
    /*
    //where op is new parent of last op
    //and op is new children to parent of last op
    //and then last op becomes parent of last op
    last_op->exchange_parent(op);
    */
    bool node::exchange_parent(const string&op) {
         if(parent==NULL){
            return this->append_parent(op) ;
         }
         else{
            node*new_parent = get_node(op);
            if(new_parent){
                node*gparent  =parent ;
                this->disconnect_self();
                gparent->append_child(new_parent);
                node*temp = this;
                new_parent->append_child(temp) ;
                return true ;
            }
        }
         return false;
    }

    bool calculus_tree::remove_node(node*&src) {
        if(src){
            src->disconnect_self();

            queue<node*>q ;
            q.push(src);
            while(!q.empty())
            {
                node*temp = q.front();
                q.pop();
                if(temp->left){
                    q.push(temp->left);
                }
                if(temp->right){
                    q.push(temp->right) ;
                }
                delete temp ;
                temp=NULL;
            }
            src=NULL;
            return true;
        }
        return false ;
    }
    /*
    calculus tree part
    */
    calculus_tree::~calculus_tree(){
        remove_node(root);
        root= NULL;
    }

    calculus_tree ::calculus_tree(const string&expression){
        unsigned int start= 0;
        root =parse(expression,start) ;
    }

    calculus_tree::calculus_tree(void){
        root=NULL;
    }

    bool calculus_tree::remove_tree(void){
        if(root){
            remove_node(root);
            root =NULL;
            return true ;
        }
        return 0 ;
    }

    void calculus_tree::print(node* ptr) const {
        if(root){
            bool found_keyword = false;
            if(ptr == NULL){
                ptr = root;
            }
            if(is_keyword(ptr)){
                cout<<ptr->symbol;
                found_keyword = true;
            }
            if(ptr->left){
                cout<<"(";
                print(ptr->left);
            }
            if(ptr->right){
                cout<<ptr->symbol;
                print(ptr->right);
            }
            cout<<")";
            if(!found_keyword){
                cout<<ptr->symbol;
            }
        }
    }

string calculus_tree::expression(node* ptr) const {
    if(root){
        bool found_keyword = false;
        string ret_exp = "";
        if(ptr == NULL){
            ptr = root;
        }
        if(is_keyword(ptr)){
            ret_exp += ptr->symbol;
            found_keyword = true;
        }
        if(ptr->left){
            ret_exp += "(";
            ret_exp += expression(ptr->left);
            if(ptr->right){
                ret_exp += ptr->symbol;
                ret_exp += expression(ptr->right);
            }
            ret_exp += ")";
        }
        else if(!found_keyword){
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


    node* calculus_tree::parse_parenthese(const string& expression, unsigned int &start) {
        if(expression[start]=='('){
            start++;
            node * last_op = NULL ;
            node * ret_root = NULL ;
            node * var = NULL;
            node * temp = NULL ;
            string op  ="" ;
            bool new_op = false;
            while(start<expression.length() && expression[start]!=')'){
                var= NULL ;
                new_op =false ;
                if(expression[start]=='('){
                    var = parse_parenthese(expression, start);
                }
                else if(!is_op(expression,start)){
                    var = parse_block(expression,start);
                }
                if(is_op(expression,start)&&expression[start]!='('&&expression[start]!=')'){
                    new_op= true  ;
                    op = extract(expression,start);
                }
                if(var){
                    if(new_op){
                        if(ret_root==NULL){
                            ret_root = var ;
                            ret_root->append_parent(op);
                            ret_root=ret_root->parent ;
                            last_op = ret_root ;
                        }
                        else{
                        //void var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root){
                            var_op_func(op,var,last_op,ret_root) ;
                        }
                    }
                    else{
                        if(ret_root==NULL){
                            ret_root=var;
                        }
                        else{
                            last_op->append_child(var);
                        }
                    }
                }
                else{
                    if(new_op){
                        start--;
                    }
                    return ret_root;
                }
            }
            if(expression[start] == ')') {
                start++; // Skip the closing parenthesis
            }
            return ret_root ;
        }
        return NULL ;
    }
    void calculus_tree::var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root){
        node *temp = NULL;
        int diff = precedence(op,0)-precedence(last_op->symbol,0);
        if(diff>0||op=="^"){
            temp = temp->get_node(op) ;
            temp->append_child(var) ;
            last_op->append_child(temp) ;
            last_op = last_op->right;
        }
        else{
            last_op->append_child(var);
            //where op is new parent of last op
            //and op is new children to parent of last op
            //and then last op becomes parent of last op
            while(last_op->parent&&(precedence(op,0)-precedence(last_op->parent->symbol,0)<=0)){
                last_op=last_op->parent;
            }
            last_op->exchange_parent(op);
            if(last_op==ret_root){
                ret_root =ret_root ->parent ;
            }
            last_op=last_op->parent;
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
            if(is_op(expression,start)&&expression[start]!='('&&expression[start]!=')'){
                op=extract(expression,start);
            }
            if(expression[start]=='('||is_keyword(expression,start)){
                //expression is -> x+(
                start--;
                return ret_root->get_node(var);
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
                    if(is_op(expression,start)&&expression[start]!='('&&expression[start]!=')'){
                        op = extract(expression,start);
                        new_op = true ;
                    }
                    if(new_var){
                        if(new_op){
                            if(expression[start]=='('||expression[start]==')'||is_keyword(expression,start)){
                                //expression is -> x+(
                                start--;
                                last_op->append_child(var);
                                return ret_root;
                            }
                            var_op_func(op,var,last_op,ret_root)  ;
                            }
                            else{
                                last_op->append_child(var);
                                return ret_root  ;
                            }
                        }
                    else{
                        if(new_op){
                            start--;
                        }
                        return ret_root;

                    }
                }
                return ret_root;
            }
            else if(var.length()){
                return ret_root->get_node(var) ;
            }
        }
        return NULL ;
    }



    void calculus_tree::var_op_func(const string&op,const string&var,node*&last_op,node*&ret_root){
        node *temp = NULL;
        int diff = precedence(op,0)-precedence(last_op->symbol,0);
        if(diff>0||op=="^"){
            temp = temp->get_node(op) ;
            temp->append_child(var) ;
            last_op->append_child(temp) ;
            last_op = last_op->right;
        }
        else{
            last_op->append_child(var);
            //where op is new parent of last op
            //and op is new children to parent of last op
            //and then last op becomes parent of last op
            while(last_op->parent&&(precedence(op,0)-precedence(last_op->parent->symbol,0)<=0)){
                last_op=last_op->parent;
            }
            last_op->exchange_parent(op);
            if(last_op==ret_root){
                ret_root =ret_root ->parent ;
            }
            last_op=last_op->parent;
        }
    }
    node*calculus_tree::parse_block(const string &expression,unsigned int &start){
        if(start<expression.length()){
            if(is_keyword(expression,start)){
                return parse_function(expression,start);
            }
            else if(expression[start]=='('){
                return parse_parenthese(expression,start);
            }
            else{
                return parse_expression(expression,start);
            }
        }
        return NULL ;
    }
    //this forms the complete tree from an expression and returns
    //its root
    node*calculus_tree::parse(const string &expression,unsigned int &start){
        if(start<expression.length()){
            node*ret_root = NULL ;
            node*block =NULL ;
            node*temp = NULL ;
            node*last_op= NULL ;
            string op ="";
            //extract the thing
            ret_root=parse_block(expression,start);

            if(ret_root&&start<expression.length()&&is_op(expression,start)&&expression[start]!='('&&expression[start]!=')'){

                ret_root->append_parent(extract(expression,start));
                ret_root=ret_root->parent;
                last_op = ret_root;
                while(start<expression.length()){
                    bool new_op= false ;
                    //extract the thing
                    block= parse_block(expression,start) ;

                    if(is_op(expression,start)&&expression[start]!='('&&expression[start]!=')'){
                        op=extract(expression,start);
                        new_op =true;
                    }
                    if(block){
                        if(new_op){
                            //    void var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root){
                            var_op_func(op,block,last_op,ret_root);
                        }
                        else{
                            last_op->append_child(block);
                            return ret_root  ;
                        }
                    }
                }
            }
            return ret_root ;
        }
    return NULL ;
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
            node*ret_root = parse_parenthese(expression,start);
            if(ret_root){
                ret_root->append_parent(var);
                ret_root=ret_root->parent;
                return ret_root ;
            }
        }
        return NULL;
    }

    int main(){

/*
string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))+tan(log2(x+5))+sec(x*asin(1/(x+1)))+csc((x^3+2*x)/4)-cotan(exp(x/2))+
acos(1/(x+2)^0.5)-atan(x^2/3-ln(x))+exp(sin(pi/6+x))+ln(cos(x^2+exp(x)))+log2(1/(x+3))";

test4
string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))^tan(log2(x+5))*sec(x*asin(1/(x+1)))/"
                   "csc((x^3+2*x)/4)^cotan(exp(x/2))*acos(1/(x+2)^0.5)";

test6 string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))^tan(log2(x+5))^sec(x*asin(1/(x+1)))/"
                   "csc((x^3+2*x)/4)*(cotan(exp(x/2))+acos(1/(x+2)^0.5))";

*/
string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))*tan(log2(x+5))^(sec(x*asin(1/(x+1)))+5*"
                   "csc((x^3+2*x)/4))^cotan(exp(x/2))*acos(1/(x+2)^0.5)";
        calculus_tree tree(operation);
        cout<<tree;
        system("pause");
        return 0 ;
    }
