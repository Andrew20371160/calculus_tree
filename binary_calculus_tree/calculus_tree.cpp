#include "calculus_tree.h"


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

    template<typename DataType>
    node * calculus_tree<DataType>::copy_tree(node*src_root){
        if(src_root){
            node *ret_root = NULL ;
             ret_root = ret_root->get_node(src_root->symbol);
            if(src_root->left){
                ret_root->left = copy_tree(src_root->left);
                if(ret_root->left){
                    ret_root->left->parent = ret_root ;
                }
            }
            if(src_root->right){
                ret_root->right = copy_tree(src_root->right);
                if(ret_root->right){
                    ret_root->right->parent = ret_root ;
                }
            }
            return ret_root ;
        }
        return NULL ;
    }
    template<typename DataType>
    calculus_tree<DataType> ::calculus_tree(const calculus_tree<DataType>&src_tree){
        root =NULL ;
        if(src_tree.root){
            root = copy_tree(src_tree.root);
        }
    }
    template<typename DataType>
    void calculus_tree<DataType> ::set_exp(const string &expression){
        if(root){
            remove_tree();
        }
        unsigned int start= 0;
        root = create_tree(expression,start);
    }

    template<typename DataType>
    calculus_tree<DataType>& calculus_tree<DataType>::operator=(const calculus_tree&src_tree){
        if(root){
            remove_node(root);
            root =NULL;
        }
        if(src_tree.root){
            root = copy_tree(src_tree.root) ;
        }
        return *this ;
    }

    template<typename DataType>
    bool calculus_tree<DataType>::remove_node(node*&src) {
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
    template<typename DataType>
    calculus_tree<DataType>::~calculus_tree(){
        remove_node(root);
        root= NULL;
    }
    template<typename DataType>
    calculus_tree<DataType> ::calculus_tree(const string&expression){
        unsigned int start= 0;
        root =create_tree(expression,start) ;
    }
    template<typename DataType>
    calculus_tree<DataType>::calculus_tree(void){
        root=NULL;
    }

    template<typename DataType>
    bool calculus_tree<DataType>::remove_tree(void){
        if(root){
            remove_node(root);
            root =NULL;
            return true ;
        }
        return 0 ;
    }

    template<typename DataType>
    void calculus_tree<DataType>::print(node* ptr) const {
        if(root){
            bool found_keyword = false;
            if(ptr == NULL){
                ptr = root;
            }
            if(is_function(ptr)!=-1){
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

    template<typename DataType>
    string calculus_tree<DataType>::expression(node* ptr) const {
        if(root){
            bool found_keyword = false;
            string ret_exp = "";
            if(ptr == NULL){
                ptr = root;
            }
            if(is_function(ptr)!=-1){
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

    template<typename DataType>
    bool calculus_tree<DataType>::is_op(const string&expression,unsigned int pos ) {
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
    template<typename DataType>
    int calculus_tree<DataType>::precedence(const string&expression,unsigned int pos){
            switch(expression[pos]){
                case '^':return 3 ;
                case '*':return 2 ;
                case '/':return 2 ;
            }
            return 1  ;
        }


    //this extracts operand (whatever its length) or operator
    //start goes to second operand or operator
    template<typename DataType>
    string calculus_tree<DataType>::extract(const string&expression,unsigned int &start){
        string var="";
        if(start<expression.length()){
            unsigned int original_start = start;
            if(is_op(expression,start)){
                var+=expression[start];
                start++;
            }
            else{
                while(start<expression.length()&&!is_op(expression,start)){
                    start++;
                }
                var = expression.substr(original_start, start - original_start);
            }
        }
        return var ;
    }
    template<typename DataType>
    node* calculus_tree<DataType>::parse_parenthese(const string& expression, unsigned int &start) {
        if(expression[start]=='('){
            start++;
            node * last_op = NULL ;
            node * ret_root = NULL ;
            node * var = NULL;
            string op  ="" ;
            bool new_op = false;
            while(start<expression.length() && expression[start]!=')'){
                var= NULL ;
                new_op =false ;
                if(expression[start]=='('){
                    var = parse_parenthese(expression, start);
                }
                else{
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

    template<typename DataType>
    void calculus_tree<DataType>::var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root){
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
            while(last_op->parent&&((precedence(op,0)-precedence(last_op->parent->symbol,0))<=0)){
                last_op=last_op->parent;
            }
            last_op->exchange_parent(op);
            if(last_op==ret_root){
                ret_root =ret_root->parent ;
            }
            last_op=last_op->parent;
        }
    }

    template<typename DataType>
    node*calculus_tree<DataType>::parse_block(const string &expression,unsigned int &start){
        if(start<expression.length()){
            if(is_function(expression,start)!=-1){
                return parse_function(expression,start);
            }
            else if(expression[start]=='('){
                return parse_parenthese(expression,start);
            }
            else{
                node *temp = NULL;
                string op = "";
                if(is_op(expression,start)){
                    if(expression[start]=='+'||expression[start]=='-'){
                        if((start==0||is_op(expression,start-1))&&(start+1)<=expression.length()){
                            bool signed_var =false;
                            op = expression[start];
                            start++;
                            if(is_function(expression,start)!=-1){
                                //-1*sin
                                temp =parse_function(expression,start) ;
                                signed_var=true;
                            }
                            else if(expression[start]=='('){
                                // *-(242+....)
                                temp =parse_parenthese(expression,start);
                                signed_var=true;
                            }
                            else if(!is_op(expression,start)){
                                temp =temp->get_node(extract(expression,start));
                                signed_var=true;
                            }
                            if(temp&&signed_var){
                                node *ret_root =NULL;
                                ret_root=ret_root->get_node("*");
                                ret_root->append_child(op+"1");
                                ret_root->append_child(temp) ;
                                temp= ret_root;
                            }
                        }
                    }
                    return temp ;
                }
                else{
                    return temp->get_node(extract(expression,start));
                }
            }
        }
        return NULL ;
    }
    //this forms the complete tree from an expression and returns
    //its root
    template<typename DataType>
    node*calculus_tree<DataType>::create_tree(const string &expression,unsigned int &start){
        if(start<expression.length()){
            node*ret_root = NULL ;
            node*block =NULL ;
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
                    else{
                        return ret_root;
                    }
                }
            }
            return ret_root ;
        }
        return NULL ;
    }

    template<typename DataType>
    int calculus_tree<DataType>:: is_function(const string&expression ,unsigned int pos){
         string temp=  extract(expression,pos);
         if(temp.substr(0,3)=="log"){
            return LOG ;
         }
         for(int i =0 ; i <function_count-1;i++){
            if(temp==key_words[i]){
                return i;
            }
         }
         return -1 ;
    }

    template<typename DataType>
    int calculus_tree<DataType>:: is_function(node*&ptr)const{
        if(ptr){
             if(ptr->symbol.substr(0,3)=="log"){
                return LOG ;
             }
             for(int i =0 ; i <function_count-1;i++){
                if(ptr->symbol==key_words[i]){
                    return i;
                }
             }
        }
         return -1 ;
    }

    //assuming it's a keywrod AND its a function
    template<typename DataType>
    node*calculus_tree<DataType>::parse_function(const string&expression,unsigned int &start){
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
    /*
    some evaluation functions
    */
    template<typename DataType>
    bool calculus_tree<DataType>::is_num(const string &var){
        if(var.length()){
            unsigned int i=0 ;
            unsigned int dot_counter = 0 ;
            while(i<var.length()){
                if(!(var[i]>='0'&&var[i]<='9')){
                    if(var[i]=='-'||var[i]=='+'){
                        i++;
                    }
                    else if(var[i]!='.'){
                       return false ;
                    }
                    else{
                        if(dot_counter){
                            return false ;
                        }
                        else{
                            dot_counter=1;
                        }
                    }
                }
                i++;
            }
            return true ;
        }
        return false ;
    }

    template<typename DataType>
    DataType calculus_tree<DataType>::evaluate_function(const int fn,const DataType var, const  DataType base) {
        switch(fn){
            case EXP: return exp(var);
            case LN: return log(var);
            case SIN: return sin(var);
            case COS: return cos(var);
            case TAN: return tan(var);
            case SQRT: return pow(var,0.5);
            case ABS : return abs(var);
            case LOG : return log(var) / log(base);
            case SEC: return DataType(1)/cos(var) ;
            case CSC:return DataType(1)/sin(var) ;
            case COTAN: return DataType(1)/tan(var);
            case ASIN: return asin(var);
            case ACOS: return acos(var);
            case ATAN: return atan(var);
            case SINH: return sinh(var);
            case COSH: return cosh(var);
            case TANH: return tanh(var);
            case ASINH: return asinh(var);
            case ACOSH: return acosh(var);
            case ATANH: return atanh(var);
            #ifdef COMPLEX_MODE
                case IMG :return complex<long double>(0,1)*var;
            #endif
        }
    }

    template<typename DataType>
    string calculus_tree<DataType>::eval_extract(const string&expression,unsigned int &start){
        string var="";
        if(start<expression.length()){
            while(start<expression.length()&&expression[start]!='='&&expression[start]!=','){
                var+=expression[start];
                start++;
            }
        }
        return var ;
    }

    template<typename DataType>
    DataType calculus_tree<DataType>::evaluate_at(string vars_equal){
        if(root){
            list<string>variables_and_values;
            string value="";
            string var="";
            if(vars_equal.length()){
                unsigned int i =0;
                while(i<vars_equal.length()){
                    string var = eval_extract(vars_equal,i);
                    if(var.length()){
                       if(i<vars_equal.length()&&vars_equal[i]=='='){
                            i++ ;
                       }
                       if(i<vars_equal.length()){
                           value = eval_extract(vars_equal,i);
                       }
                       if(value.length()&&is_num(value)){
                            variables_and_values.push_back(var);
                            variables_and_values.push_back(value);
                       }
                       i++;
                   }
                   else{
                        break ;
                   }
                }
            }
            return evaluate(root,variables_and_values) ;
        }
        return 0 ;
    }

    template<typename DataType>
    DataType calculus_tree<DataType>::evaluate_operator(char op,const DataType&left_operand,const DataType&right_operand){
       switch(op){
            case '+':return left_operand+right_operand ;
            case '-':return left_operand-right_operand ;
            case '*':return left_operand*right_operand ;
            case '/':return left_operand/right_operand ;
            case '^':return pow(left_operand,right_operand);
        }
    }

    template<typename DataType>
    DataType calculus_tree<DataType>::evaluate_constant(node*ptr){
        if(ptr->symbol=="pi"){
            return DataType(M_PI) ;
        }
        else if(ptr->symbol=="e"){
            return DataType(exp(1)) ;
        }
        #ifdef COMPLEX_MODE
            else if(ptr->symbol=="i"){
                return complex<long double>(0,1) ;
            }
        #endif
        else{
            cout<<"\nUNDEFINED\n";
            cout<<ptr->symbol;
        }
    }

    template<typename DataType>
    DataType calculus_tree<DataType>::evaluate(node*ptr,const list<string>&variables_and_values){
        if(root){
            if(ptr==NULL){
                ptr= root ;
            }
            DataType left_operand = DataType(0) ;
            DataType right_operand = DataType(0)  ;
            //visit kids first
            if(ptr->left){
                left_operand =evaluate(ptr->left,variables_and_values);
            }
            if(ptr->right){
                right_operand= evaluate(ptr->right,variables_and_values) ;
            }
            if(ptr->left==NULL&&ptr->right==NULL){
                if(is_num(ptr->symbol)){
                    return DataType(stold(ptr->symbol)) ;
                }
                else if(variables_and_values.size()){
                    list<string>::const_iterator it =variables_and_values.begin() ;
                    while(it!=variables_and_values.end()){
                        if(*it==ptr->symbol){
                            ++it ;
                            return DataType(stold(*it));
                        }
                        ++it;
                        ++it;
                    }
                }
                return evaluate_constant(ptr);
            }
            else{
                if(is_op(ptr->symbol,0)){
                    return evaluate_operator(ptr->symbol[0],left_operand,right_operand);
                }
                else{
                    int fn_code = is_function(ptr);
                    if(fn_code!=-1){
                        //since one of them must be zero
                        //since the function is the root of that expression
                        //f(expression) after evaluating the expression
                        //i return the value
                        DataType base_log = DataType(10);
                        if(fn_code==LOG){
                            if(ptr->symbol.length()>3){
                                base_log = DataType(stold(ptr->symbol.substr(3)));
                            }
                            return evaluate_function(fn_code,(left_operand+right_operand),base_log) ;
                        }
                        else{
                            return evaluate_function(fn_code,(left_operand+right_operand),base_log);
                        }
                    }
                    else{
                        cout<<"\nUNDEFINED\n";
                        cout<<ptr->symbol;
                    }
                }
            }
        }
        return 0;
    }



    template<typename DataType>
    string calculus_tree<DataType>::simplify_mult(const string&v1,const string &v2){
        if(v1=="0"){
            return "0";
        }
        else if(v2=="0"){
            return "0";
        }
        else{
            return "("+v1+")*("+v2+")";
        }
    }

    template<typename DataType>
    string calculus_tree<DataType>::simplify_div(const string&v1,const string &v2){
        if(v1=="0"&&v2=="0"){
            return "nan";
        }
        else if(v1=="0"){
            return "0";
        }
        else if(v2=="0"){
            return "inf";
        }
        return "(("+v1+")/("+v2+"))";
    }

    template<typename DataType>
    string calculus_tree<DataType>::simplify_add(const string&v1,const string &v2){
        if(v1=="0"&&v2=="0"){
            return "0";
        }
        else if(v1=="0"){
            return "("+v2+")";
        }
        else if(v2=="0"){
            return "("+v1+")";
        }
        return "(("+v1+")+("+v2+"))";
    }

    template<typename DataType>
    string calculus_tree<DataType>::simplify_sub(const string&v1,const string &v2){
        if(v1=="0"&&v2=="0"){
            return "0";
        }
        else if(v1=="0"){
            return "(-1*("+v2+"))";
        }
        else if(v2=="0"){
            return "("+v1+")";
        }
        return "(("+v1+")-("+v2+"))";
    }

    template<typename DataType>
    string calculus_tree<DataType>::diff_plus_minus(node*ptr,const string&var){
        string left_prime =diff(ptr->left,var) ;
        string right_prime =diff(ptr->right,var) ;
        if(ptr->symbol=="-"){
            return simplify_sub(left_prime,right_prime);
        }
        else{
            return simplify_add(left_prime,right_prime);
        }
    }

    template<typename DataType>
    string calculus_tree<DataType>::diff_mult(node*ptr,const string&var){
        string left =expression(ptr->left);
        string left_prime = diff(ptr->left,var);

        string right =expression(ptr->right);
        string right_prime =diff(ptr->right,var);

        left = simplify_mult(left,right_prime);
        right = simplify_mult(right,left_prime);

        return simplify_add(left,right);
    }

    template<typename DataType>
    string calculus_tree<DataType>::diff_div(node*ptr,const string&var){
        string right =expression(ptr->right);
        if(right!="0"){
            string left_prime = diff(ptr->left,var);
            //-
            string left =expression(ptr->left);
            string right_prime =diff(ptr->right,var);

            left_prime = simplify_mult(left,right_prime);
            right_prime = simplify_mult(right,left_prime);
            return simplify_div(simplify_sub(left_prime,right_prime),"("+right+")^2");
        }
        return "inf";
    }

    template<typename DataType>
    string calculus_tree<DataType>::diff_op(node*ptr,const string&var){
        switch(ptr->symbol[0]){
            case '+': return diff_plus_minus(ptr,var);
            case '-': return diff_plus_minus(ptr,var);
            case '*': return diff_mult(ptr,var);
            case '/': return diff_div(ptr,var) ;
            //we'll see for ^ later
            case '^':{
                if(is_num(ptr->right->symbol)){
                    long double  exponent = stold(ptr->right->symbol);

                    if(exponent==0){
                        return "0" ;
                    }
                    else if(exponent==1){
                        return "(" + diff(ptr->left,var) + ")";
                    }
                    else{
                        return "(("+ptr->right->symbol+"*"+diff(ptr->left,var)+"*"+expression(ptr->left)+"^"+to_string(exponent-1)+"))";
                    }
                }
                else{
                    //f'= f*(right*(left'/left) +ln(left)*right')
                    // Assuming ptr->left corresponds to g(x) and ptr->right corresponds to h(x)
                string left_f = expression(ptr->left);
                string right_f = expression(ptr->right);
                string left_f_prime = diff(ptr->left,var);
                string right_f_prime = diff(ptr->right,var);

                return "((" + left_f + "^" + right_f + ")*(" + right_f + "*(" + left_f_prime + "/" + left_f + ") + ln(" + left_f + ")*" + right_f_prime + "))";
                }
            }
        }
    }


    template<typename DataType>
    string calculus_tree<DataType>::diff_function(const int fn,node*ptr,const string&var ){
        switch(fn){
            case EXP: return "("+diff(ptr->left,var)+")*("+expression(ptr)+")";
            case LN: return  "("+diff(ptr->left,var)+")/"+expression(ptr->left);
            case SIN: return "("+diff(ptr->left,var)+")*cos"+expression(ptr->left);
            case COS: return "-1*("+diff(ptr->left,var)+")*sin("+expression(ptr->left)+")";
            case TAN:   {
                string inner = expression(ptr->left);
                return "("+diff(ptr->left,var)+")*(sec"+inner+"^2)" ;
            }
            case SEC: {
                string inner = expression(ptr->left);
                return "("+diff(ptr->left,var)+")*(sec"+inner+"*tan"+inner+")" ;
            }
            case CSC:{
                string inner = expression(ptr->left);
                return "-1*("+diff(ptr->left,var)+")*(csc"+inner+"*cotan"+inner+")" ;
            }
            case COTAN: {
                string inner = expression(ptr->left);
                return "-1*("+diff(ptr->left,var)+")*(cotan"+inner+"^2)" ;
            }
            case SQRT: {
                string inner = expression(ptr->left);
                return  "-0.5*"+diff(ptr->left,var)+inner+"^-1.5";
            }
            case ABS : {
                //assuming it's >0
                return diff(ptr->left,var);
            }
            //f'(x) = g'(x) / (g(x) * ln(b))

            case LOG : return  "("+diff(ptr->left,var)+")/("+expression(ptr->left)+"*ln"+ptr->symbol.substr(3)+")";

            case ASIN: return "(" + diff(ptr->left,var) + ")/sqrt(1-"+expression(ptr->left)+"^2";

            case ACOS: return "-1*(" + diff(ptr->left,var) + ")/sqrt(1-("+expression(ptr->left)+")^2)";
            case ATAN: return "("+diff(ptr->left,var) + ")/(1+("+expression(ptr->left) +")^2)";
            case SINH: return "("+diff(ptr->left,var) + ")*cosh(" + expression(ptr->left) + ")";
            case COSH: return "("+diff(ptr->left,var) + ")*sinh(" + expression(ptr->left) + ")";
            case TANH: return "("+diff(ptr->left,var) + ")*(1-tanh(" + expression(ptr->left) + ")^2)";
            case ASINH: return "("+diff(ptr->left,var) + ")*(1/sqrt(" + expression(ptr->left) + "^2+1))";
            case ACOSH: return "("+diff(ptr->left,var) + ")*(1/sqrt(" + expression(ptr->left) + "^2-1))";
            case ATANH: return "("+diff(ptr->left,var) + ")*(1/(1-" + expression(ptr->left) + "^2))";
            case IMG : return "img"+diff(ptr->left,var) ;
        }
    }


    template<typename DataType>
    string calculus_tree<DataType>::diff(node*ptr,const string&var){
        if(root){
            if(ptr==NULL){
                ptr= root ;
            }
            if(ptr->left||ptr->right){
                if(is_op(ptr->symbol,0)){
                    return diff_op(ptr,var);
                }
                else {
                    int fn_code = is_function(ptr);
                    if(fn_code!=-1){
                        return diff_function(fn_code,ptr,var);
                    }
                }
            }
            else{
                if(is_num(ptr->symbol)||var!=ptr->symbol){
                    return "0";
                }
                else{
                    return "1"  ;
                }
            }
        }
        else{
            return "0" ;
        }
    }

    template<typename DataType>
    calculus_tree<DataType> calculus_tree<DataType>::diff_with(const string&variable){
        unsigned int st = 0 ;
        if(root==NULL){
            string str ="0";
            return calculus_tree(str) ;
        }
        else{
            return calculus_tree(diff(root,variable));
        }
    }

#include <chrono>
int main(){

    /*
    string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))+tan(log2(x+5))+sec(x*asin(1/(x+1)))+csc((x^3+2*x)/4)-cotan(exp(x/2))+
    acos(1/(x+2)^0.5)-atan(x^2/3-ln(x))+exp(sin(pi/6+x))+ln(cos(x^2+exp(x)))+log2(1/(x+3))";

    test4
    string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))^tan(log2(x+5))*sec(x*asin(1/(x+1)))/"
                       "csc((x^3+2*x)/4)^cotan(exp(x/2))*acos(1/(x+2)^0.5)";

    test6 string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))^tan(log2(x+5))^sec(x*asin(1/(x+1)))/"
                       "csc((x^3+2*x)/4)*(cotan(exp(x/2))+acos(1/(x+2)^0.5))";

    string operation = "sin(pi/4+ln(x^2+1))+cos(pi/3-exp(x))*tan(log2(x+5))^(sec(x*asin(1/(x+1)))+5*"
                       "csc((x^3+2*x)/4))^cotan(exp(x/2))*acos(1/(x+2)^0.5)";

    string operation = "1/x+1*((x^2+4*x+1/x^2-1)*log(x+(x^2-1)^0.5)-(x+3)/(x^2-1)^0.5)";


    string operation = "sin(acos(1/(x+5*(exp(2*x)^atan(x^2+3)))))+ln(sec(3*x))-(cos(x^2)/(x+4))";


    string operation = "(cos(acos(1/(x+5*(exp(3*y)^atan(x^3+y^2-4)))))+ln(sec(2*x-cos(y^3)))-(cos(x^2+y)/(tan(y^4-x)+7*(exp(x+y)))))*(asin(x+y)^3)+log(5*exp(cotan(x^3-2*y))+sin(x*ln(cos(3*y))))-(atan(exp(x+y)^tan(x-y))/(sec(x^2+y^2)+3*csc(x+y)))+7*log(x*y)-(cos(exp(x*atan(y^2)))/(sec(ln(x))+cos(y*x^2))))";
        string operation ="1+2-3*6/18^1^8*5/8*25^x^sin(x+2)/(13+224)+7";

    f(x,y)=(sin(acos(1/(x+exp(2*y))))+ln(sec(x^2-y))+cos(x^3)/(tan(y^2-3*x)))*(asin(x+y)^2)-atan(exp(x*y))
    ((((sin(acos((1/(x+exp((2*y))))))^ln(sec(((x^2)-y))))*(cos((x^3))/tan(((y^2)^(3*x)))))*(asin((x+y))^2))-atan(exp((x*y))))

    f(x,y)=
    sin(acos(1/(x+5*exp(y)))^tan(ln(x^8+y^2))^cos(exp(atan(x*y)))/sec(x^4+y^3))*(asin(x+y)^3+ln(sec(5*x-4*y))-tan(cos(x^5-y^3)))/(cotan(x^6+y^6)+exp(tan(ln(x+y^2))))+log(atan(exp(x*y^2)))+sin(ln(cos(exp(x^3+y^2))))+(sec(x^2-y)*exp(cos(x*y))+ln(tan(x^4+y)))+cos(atan(x^5+y^6)+sec(3*x-2*y))*(asin(cos(x^3))+ln(sec(5*y+x^3))-tan(cos(x^5)))+exp(tan(x*ln(cos(2*y)))/(cos(x^2+y)+sec(x*y)))-sin(log(cotan(atan(2*x-y))+cos(ln(exp(x^2*y^3)))))+(sec(2*x-3*y)/tan(exp(x*y)))+cos(exp(2*atan(x^2*y)))/(ln(3*x-2*y)+tan(sec(x+y^2)))*atan(cos(log(x*exp(y^2)))+sec(cos(3*x*ln(y^3))))+exp(ln(tan(x^4-y^5)))*cos(atan(x+ln(y)))-sin(cotan(log(x^6+y^3)))*exp(sec(tan(2*x-y^2)))+cos(ln(x^2)*tan(exp(x*y^3)))/(sin(3*x^2+y^5))+exp(log(atan(x*y^4)+sec(ln(x*y^3))))*(sec(ln(3*x+y))+cos(tan(x*exp(y))))+cos(log(x+tan(exp(y*x^4)))*ln(atan(x*y)))-sin(exp(x^3)*cos(tan(x^2+y^4)))+(cos(ln(x^3*y^3))*exp(ln(cotan(x+y^2))))/(tan(log(x^2+y^2))+sec(cos(3*x*y)))+atan(log(x^2+y^6))+sec(cos(exp(atan(x+y^4))))+sin(atan(exp(x^4-y^3)))*cos(ln(cos(exp(x^2*y))))+log(cotan(atan(2*x-y^2))+sec(ln(exp(x^3+y^2))))-exp(cos(ln(tan(2*x+y)))+tan(sec(x^2+y^3)))+sin(atan(exp(x*y)))/(cos(x^5+y^5))+ln(cotan(atan(x^3+y^6)))*cos(exp(sec(tan(x^2-y^2))))+cos(ln(x^4+y^4))*exp(cotan(ln(2*x-y)))-sin(log(cotan(exp(x^5+y^3)))+sec(tan(2*x+y^6)))+tan(cos(x^6-y^4)*sec(ln(cotan(x+y^2))))/(cos(log(x^4+y^6))+exp(ln(cotan(x+y^5))))+sec(log(x^3+y^6))*cos(atan(exp(x^4+y^3)))-sin(log(cotan(atan(x^3+y^5)))+sec(cos(exp(x+y^6))))+exp(tan(log(x^5+y^2))+sec(atan(x+y^3)))

    */

    string operation = "sin(x^(x+1))";
    calculus_tree<complex<long double>> tree(operation);
    string var = "x";
    cout<<tree.diff(NULL,var);


    cout<<endl;
    system("pause");
}
