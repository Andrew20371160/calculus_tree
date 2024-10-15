#include "calculus_tree.h"
    using namespace std ;

    string to_string(const string&src){
        return src;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
        os<<'<';
        for (unsigned int i = 0 ;i<vec.size();i++) {
            os<<vec[i] ;
            if(i<vec.size()-1){
                os<<",\n\n";
            }
        }
        os<<'>';
        return os;
    }
    //constructors
    template<typename DataType>
    calculus_tree<DataType>::calculus_tree(void){
        root=NULL;
    }

    template<typename DataType>
    calculus_tree<DataType> ::calculus_tree(const calculus_tree<DataType>&src_tree){
        root =NULL ;
        if(src_tree.root){
            root = copy_tree(src_tree.root);
        }
    }

    template<typename DataType>
    calculus_tree<DataType> ::calculus_tree(const string&expression){
        unsigned int start= 0;
        root =create_tree(processor.prepare_exp(expression),start) ;
    }

    template<typename DataType>
    calculus_tree<DataType>::~calculus_tree(){
        remove_node(root);
        root= NULL;
    }
    //operators section
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
    template<typename input_datatype>
    calculus_tree<DataType>& calculus_tree<DataType>::operator=(const input_datatype&data){
        if(root){
            remove_node(root);
            root =NULL;
        }
        set_exp(to_string(data));

        return *this ;
    }

    template<typename DataType>
    calculus_tree<DataType> calculus_tree<DataType>:: operator+(const calculus_tree<DataType>&src)const{
        if(root&&src.root){
            calculus_tree<DataType>ret_tree = *this ;
            ret_tree.root->append_parent("+");
            node*temp_src= src.copy_tree(src.root) ;  ret_tree.root->append_next(temp_src);
                ret_tree.root = ret_tree.root->parent;
            return ret_tree ;
        }
        else if(root){
            return *this ;
        }
        else if(src.root){
            return src ;
        }
        else{
            return calculus_tree<DataType>("0") ;
        }
    }

        template<typename DataType>
        calculus_tree<DataType> calculus_tree<DataType>:: operator-(const calculus_tree<DataType>&src)const {
            if(root&&src.root){
                calculus_tree<DataType>ret_tree = *this ;
                ret_tree.root->append_parent("-");
                node*temp_src= src.copy_tree(src.root) ;  ret_tree.root->append_next(temp_src);
                ret_tree.root = ret_tree.root->parent;
                return ret_tree ;
            }
            else if(root){
                return *this ;
            }
            else if(src.root){
                calculus_tree<DataType> ret_tree ;
                node *ret_root=new node("*");
                ret_root->append_child("-1");
                ret_root->right = src.copy_tree(src.root);
                ret_tree.root =ret_root ;
                return ret_tree;
            }
            else{
                return calculus_tree<DataType>("0") ;
            }
        }

        template<typename DataType>
        calculus_tree<DataType> calculus_tree<DataType>:: operator*(const calculus_tree<DataType>&src)const {
            if(root&&src.root){
                calculus_tree<DataType>ret_tree = *this ;
                ret_tree.root->append_parent("*");
                node*temp_src= copy_tree(src.root) ;
                ret_tree.root->append_next(temp_src);
                ret_tree.root = ret_tree.root->parent;
                return ret_tree;
            }
            else{
                return calculus_tree<DataType>("0") ;
            }
        }

        template<typename DataType>
        calculus_tree<DataType> calculus_tree<DataType>:: operator/(const calculus_tree<DataType>&src)const {
            if(root&&src.root){
                calculus_tree<DataType>ret_tree = *this ;
                ret_tree.root->append_parent("/");
                node*temp_src= src.copy_tree(src.root) ;  ret_tree.root->append_next(temp_src);
                ret_tree.root = ret_tree.root->parent;
                return ret_tree;
            }
            else if(root){
                return calculus_tree<DataType>("inf");
            }
            else if(src.root){
                return calculus_tree<DataType>("0");
            }
            else{
                return calculus_tree<DataType>("nan");
            }
        }
        template<typename DataType>

        calculus_tree<DataType> calculus_tree<DataType>:: operator^(const calculus_tree<DataType>&src)const {
             if(root&&src.root){
                calculus_tree<DataType>ret_tree = *this ;
                ret_tree.root->append_parent("^");
                node*temp_src= src.copy_tree(src.root) ;  ret_tree.root->append_next(temp_src);
                ret_tree.root = ret_tree.root->parent;
                return ret_tree;
            }
            else if(root){
                if(abs(stold(root->symbol))>threshold){
                    return calculus_tree<DataType>("1");
                }
                return calculus_tree<DataType>("nan");
            }
            else if(src.root){
                if(abs(stold(src.root->symbol))>threshold){
                    return calculus_tree<DataType>("0");
                    }
                    return calculus_tree<DataType>("nan");
                }
            else{
                return calculus_tree<DataType>("nan");
            }
        }

    //copy and removal and printing section
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
    node * calculus_tree<DataType>::copy_tree(const node*src_root)const{
        if(src_root){
            node *ret_root = NULL ;
            ret_root = new node(src_root->symbol);
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
    void calculus_tree<DataType> ::set_exp(const string &expression){
        if(root){
            remove_tree();
        }
        unsigned int start= 0;
        root = create_tree(processor.prepare_exp(expression),start);
    }


    template<typename DataType>
    void calculus_tree<DataType>::print(node* ptr) const {
        if(root){
            bool found_keyword = false;
            if(ptr == NULL){
                ptr = root;
            }
            if(is_function_tree(ptr)){
                cout<<ptr->symbol;
                found_keyword = true;
            }
            if(ptr->left){
                cout<<"(";
                print(ptr->left);
                if(ptr->right){
                    cout<<ptr->symbol;
                    print(ptr->right);
                }
                cout<<")";
            }
            else if(!found_keyword){
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
            if(is_function_tree(ptr)){
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


    //parsing section

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
        //skip opening parenthese
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
            if(is_op(expression,start)&&expression[start]!=')'&&expression[start]!='('){
                new_op = true ;
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

    template<typename DataType>
    void calculus_tree<DataType>::var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root){
        node *temp = NULL;
        int diff = precedence(op,0)-precedence(last_op->symbol,0);
        if(diff>0||op=="^"){
            temp = new node(op) ;
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
            if(is_known_function(expression,start)!=-1){
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
                            if(is_known_function(expression,start)!=-1){
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
                                temp =new node(extract(expression,start));
                                signed_var=true;
                            }
                            if(temp&&signed_var){
                                node *ret_root =NULL;
                                ret_root=new node("*");
                                ret_root->append_child(op+"1");
                                ret_root->append_child(temp) ;
                                temp= ret_root;
                            }
                        }
                    }
                    return temp ;
                }
                else{
                    return new node(extract(expression,start));
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

            if(ret_root&&start<expression.length()){
                ret_root->append_parent(extract(expression,start));
                ret_root=ret_root->parent;
                last_op = ret_root;
                while(start<expression.length()){
                    bool new_op= false ;
                    //extract the thing
                    block= parse_block(expression,start) ;

                    if(is_op(expression,start)&&expression[start]!='('&&expression[start]!=')'){
                        op = extract(expression,start);
                        new_op= true ;
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
    evaluation functions
    */
    template<typename DataType>
    bool calculus_tree<DataType>:: is_function_tree(node*&ptr)const{
        if(ptr){
            //functions only have left child
            return (ptr->left&&ptr->right==NULL);
        }
         return 0 ;
    }
    template<typename DataType>
    bool calculus_tree<DataType>:: is_constant_tree(node*ptr)const{
       if(ptr){
           return (ptr->left==NULL)&&(ptr->right==NULL);
        }
        return 0  ;
    }

    template<typename DataType>
    bool calculus_tree<DataType>:: is_op_tree(node*ptr)const{
        if(ptr){
            return ptr->left&&ptr->right;
        }
        return false ;
    }



    template<typename DataType>
    string calculus_tree<DataType>::eval_extract(const string&expression,unsigned int &start){
        string var="";
        if(start<expression.length()){
            unsigned int original_start = start ;
            while(start<expression.length()&&expression[start]!='='&&expression[start]!=','){
                start++;
            }
            var  = expression.substr(original_start,start -original_start);
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
                       if(value.length()){
                            if(is_num(value)){
                                variables_and_values.push_back(var);
                                variables_and_values.push_back(value);
                            }
                            else if(is_known_constant(value,0)){

                                variables_and_values.push_back(var);
                                variables_and_values.push_back(to_string(evaluate_constant<DataType>(value)));
                            }
                       }
                       i++;
                   }
                   else{
                        break ;
                   }
                }
                set<string> ind_vars;
                independent_variables_tour(root,ind_vars) ;
                if(ind_vars.size() ==variables_and_values.size()/2){
                    for(unsigned int i = 0 ; i <ind_vars.size();i++){
                        for(list<string>::iterator it = variables_and_values.begin(); it!=variables_and_values.end();++it){
                            if(ind_vars.find(*it) == ind_vars.end()){
                                cout<<"\nUnknown variable ( "<<*it<<" )";
                                return 0;
                            }
                            ++it ;
                        }
                    }
                }
            }
            return evaluate(root,variables_and_values) ;
        }
        return 0 ;
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
                return evaluate_constant<DataType>(ptr->symbol);
            }
            else{
                if(is_op_tree(ptr)){
                    return evaluate_operator(ptr->symbol[0],left_operand,right_operand);
                }
                else{
                    unsigned int temp_start = 0 ;
                    int fn_code = is_known_function(ptr->symbol,temp_start);
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
                            return evaluate_function<DataType>(fn_code,(left_operand),base_log) ;
                        }
                        else{
                            return evaluate_function<DataType>(fn_code,(left_operand),base_log);
                        }
                    }
                    else{
                        cout<<"\nUNDEFINED\n";
                        cout<<ptr->symbol;
                        return 0 ;
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
            return "(("+v1+")*("+v2+"))";
        }
    }

    template<typename DataType>
    string calculus_tree<DataType>::simplify_div(const string&v1,const string &v2){
        if(v1=="0"&&v2=="0"){
            return "nan";
        }
        else if(v1=="0"){
            return "0" ;
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
            return "0" ;
        }
        else if(v1=="0"){
            return "-1*("+v2+")";
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
    string calculus_tree<DataType>::diff_mult(node* ptr, const string& var) {
        string left = "0";
        string right = "0";
        string right_prime = "0";
        string left_prime = "0";

        if (!is_constant_tree(ptr->left)||ptr->left->symbol==var) {
            left_prime = diff(ptr->left, var);
            right = expression(ptr->right);
        }
        if (!is_constant_tree(ptr->right)||ptr->right->symbol==var) {
            right_prime = diff(ptr->right, var);
            left = expression(ptr->left);
        }
        return simplify_add(simplify_mult(left, right_prime), simplify_mult(right, left_prime));
    }


    template<typename DataType>
    string calculus_tree<DataType>::diff_div(node*ptr,const string&var){
        //f =left/right
        //f' = (right*left'-left*right')/right^2
        string left_prime = "0";
        string left ="0";
        string right_prime ="0";
        string right=expression(ptr->right) ;
        if (!is_constant_tree(ptr->left)||ptr->left->symbol==var) {
            left_prime = diff(ptr->left, var);
        }
        if (!is_constant_tree(ptr->right)||ptr->right->symbol==var) {
            right_prime = diff(ptr->right, var);
            left = expression(ptr->left);
        }
        return simplify_div(simplify_sub(simplify_mult(right,left_prime),
                                        simplify_mult(left,right_prime)),"("+right+")^2");
    }


    template<typename DataType>
    string calculus_tree<DataType>::diff_op(node*ptr,const string&var){
        switch(ptr->symbol[0]){
            case '+': return diff_plus_minus(ptr,var);
            case '-': return diff_plus_minus(ptr,var);
            case '*': return diff_mult(ptr,var);
            case '/': return diff_div(ptr,var) ;
            case '^':{
                if(is_num(ptr->right->symbol)){
                    long double  exponent = stold(ptr->right->symbol);
                    if(abs(exponent)<=threshold){
                        return "0" ;
                    }
                    else if(abs(exponent-1)<=threshold){
                        return "(" + diff(ptr->left,var) + ")";
                    }
                    else{
                        return simplify_mult(simplify_mult(ptr->right->symbol,diff(ptr->left,var)),
                                             "("+expression(ptr->left)+")^"+to_string((exponent-1)));
                    }
                }
                else{
                    //f=left^right
                    //lin(f) = right*ln(left)
                    //f'/f = right*(left'/left)+in(left)*right'
                    //f' = left^right*(right*(left'/left)+in(left)*right')

                    string left = "0";
                    string right = "0";
                    string left_prime = "0";
                    string right_prime = "0";
                    if (!is_constant_tree(ptr->left)||ptr->left->symbol==var) {
                        left_prime = diff(ptr->left, var);
                        left = expression(ptr->left) ;
                        right = expression(ptr->right);
                    }
                    if (!is_constant_tree(ptr->right)||ptr->right->symbol==var) {
                        right_prime = diff(ptr->right, var);
                        if(!left.size()){
                            left = expression(ptr->left);
                        }
                    }
                    return simplify_mult("("+left+")^("+right+")",simplify_add(
                                        simplify_mult(right,simplify_div(left_prime,left))
                                        ,simplify_mult("ln("+left+")",right_prime)));

                }
            }
        }
    }

    template<typename DataType>
    string calculus_tree<DataType>::diff_function(const int fn,node*ptr,const string&var ){
        switch(fn){
            case EXP: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    return simplify_mult(diff(ptr->left,var),expression(ptr));
                }
            }break;
            case LN: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    return  simplify_div(diff(ptr->left,var),expression(ptr->left));
                }

            }break;
            case SIN: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"cos"+inner);
                }
            }break;
            case COS:{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult("-1",simplify_mult(diff(ptr->left,var),"sin"+inner));
                }
            }break;
            case TAN:   {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"sec"+inner+"^2") ;
                }

            }break;
            case SEC: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"(sec"+inner+"*tan"+inner+")") ;
                }

            }break;
            case CSC:{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult("-1",simplify_mult(diff(ptr->left,var),"(csc"+inner+"*cotan"+inner+")")) ;
                }

            }break;
            case COTAN: {

                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult("-1",simplify_mult(diff(ptr->left,var),"csc"+inner+"^2")) ;
                }

            }break;
            case SQRT: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return  simplify_mult("0.5",simplify_mult(diff(ptr->left,var),inner+"^-0.5"));
                }

            }break;
            case ABS : {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    //assuming it's >0
                    return "("+diff(ptr->left,var)+")";
                }

            }break;
            //f'(x) = g'(x) / (g(x) * ln(b))

            case LOG : {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    string base = ptr->symbol.substr(3);
                    if(base.length()){
                       return  simplify_div(diff(ptr->left,var),simplify_mult(expression(ptr->left),"ln("+base+")"));
                    }
                    else{
                        return  simplify_div(diff(ptr->left,var),simplify_mult(expression(ptr->left),"ln(10)"));
                    }
                }

            }break;
            case ASIN:{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                     string inner = "("+expression(ptr->left)+")";
                     return simplify_div(diff(ptr->left,var),"sqrt(1-"+inner+"^2)");
                 }

            }break;
            case ACOS:{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                     string inner = "("+expression(ptr->left)+")";
                     return simplify_mult("-1",simplify_div(diff(ptr->left,var),"sqrt(1-"+inner+"^2)"));
                 }

                }break;
            case ATAN:{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    string inner = "("+expression(ptr->left)+")";
                    return simplify_div(diff(ptr->left,var),"(1+"+inner+"^2)");
                }

                }break;
            case SINH: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"cosh" + inner);
                }

                    }break;
            case COSH: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"sinh" + inner);
                }
            }break;
            case TANH: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"(1-tanh" + inner + "^2)");
                }

                }break;
            case ASINH:{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"(1/sqrt(" + inner +"^2+1))");
                }

            }break;
            case ACOSH: {
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                    string inner = "("+expression(ptr->left)+")";

                    return simplify_mult(diff(ptr->left,var),"(1/sqrt(" + inner +"^2-1))");
                }

            }break;
            case ATANH:{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){
                    string inner = "("+expression(ptr->left)+")";
                    return simplify_mult(diff(ptr->left,var),"(1/(1-"+inner+"^2))");

                }
            }break;
            #ifdef COMPLEX_MODE
            case IMG :{
                if(!is_constant_tree(ptr->left)||ptr->left->symbol==var){

                   return "img("+diff(ptr->left,var)+")" ;
                }

            }break;
            #endif // COMPLEX_MODE
        }
        //add new function differentiation here
        return "0" ;
    }


    template<typename DataType>
    string calculus_tree<DataType>::diff(node*ptr,const string&var){
        if(root){
            if(ptr==NULL){
                ptr= root ;
            }
            if(ptr->left||ptr->right){
                if(is_op_tree(ptr)){
                    return diff_op(ptr,var);
                }
                else {
                    unsigned int temp_start = 0 ;
                    int fn_code = is_known_function(ptr->symbol,temp_start);
                    if(fn_code!=-1){
                        return diff_function(fn_code,ptr,var);
                    }
                }
            }
            else{
                if(var!=ptr->symbol){
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
        if(root==NULL){
            string str ="0";
            return calculus_tree(str) ;
        }
        else{
            calculus_tree<DataType>ret_tree ;
            unsigned int temp_start =0;
            if(variable.length()&&!is_num(variable)&&!is_op(variable,0)&&is_keyword(variable,0)==-1){
                ret_tree.root = create_tree(diff(root,variable),temp_start);
                ret_tree.simplify();
                return ret_tree ;
            }
            else{
                cout<<"\nCan't differentiate with respect to : "<<variable<<"\n";
                return calculus_tree("nan");
            }
        }
    }
    template<typename DataType>
    void calculus_tree<DataType>::independent_variables_tour(node*ptr,set<string>&ret_set){
        if(ptr){
           if(ptr->left){
                independent_variables_tour(ptr->left,ret_set);
           }
           if(ptr->right){
                independent_variables_tour(ptr->right,ret_set);
           }
           if(ptr->left==NULL&&ptr->right==NULL){
                if(!is_num(ptr->symbol)&&is_keyword(ptr->symbol,0)==-1){
                    ret_set.insert(ptr->symbol);
                }
           }
        }
    }


    template<typename DataType>
    vector<string> calculus_tree<DataType>::independent_variables(void){
        if(root){
            set<string>ret_set ;
            independent_variables_tour(root,ret_set);
            vector<string> ret_vec(ret_set.size());
            unsigned int  i = 0 ;
            for(set<string>::iterator it = ret_set.begin();it!=ret_set.end();++it){
                ret_vec[i] = *it;
                i++;
            }
            return ret_vec;
        }
        else{
            return vector<string>();
        }
    }

    template<typename DataType>
    vector<calculus_tree<DataType>> calculus_tree<DataType>::gradient(const vector<string>&ind_vars){
        vector<calculus_tree<DataType>> gradient_field ;
        if(root){
            if(!ind_vars.empty()){
                gradient_field.resize(ind_vars.size());
                unsigned int  i = 0 ;
                for(;i<ind_vars.size();i++){
                    unsigned int start = 0 ;
                    gradient_field[i].root = create_tree(diff(root,ind_vars[i]),start);
                }
            }
        }
        return gradient_field;
    }

    template<typename DataType>
    calculus_tree<DataType> calculus_tree<DataType>::laplacian(const vector<string>&ind_vars){
        if(root){
            if(!ind_vars.empty()){
                calculus_tree<DataType>ret_tree;
                vector<calculus_tree<DataType>> gradient_field = gradient(ind_vars);
                string laplac_str = "";
                for(unsigned int i =0;i<gradient_field.size();i++){
                    laplac_str += diff(gradient_field[i].root,ind_vars[i]);
                    laplac_str += "+";
                }
                unsigned int start = 0;
                laplac_str.pop_back();
                ret_tree.root=create_tree(laplac_str,start);
                return ret_tree ;
            }
        }
        return calculus_tree<DataType>();
    }

    template<typename DataType>
    calculus_tree<DataType> calculus_tree<DataType>::divergence(const vector<calculus_tree<DataType>>&gradient_field,
                                                            const vector<string>&independent_variables){
            calculus_tree<DataType>ret_tree;
            if(!gradient_field.empty()){
                if(independent_variables.size()==gradient_field.size()){
                    string div_str = "";
                    for(unsigned int i =0;i<gradient_field.size();i++){
                        div_str += diff(gradient_field[i].root,independent_variables[i]);
                        div_str += "+";
                    }
                    div_str.pop_back();
                    unsigned int start = 0;
                    ret_tree.root= create_tree(div_str,start);

                }
            }
        return ret_tree;
    }

    /*
    i     j     k
    0     1     2
    d/dx d/dy d/dz
    fx    fy   fz
    */
    template<typename DataType>
    vector<calculus_tree<DataType>> calculus_tree<DataType>::curl(const vector<calculus_tree<DataType>>&gradient_field,
                                                            const vector<string>&independent_variables){
        if(gradient_field.size()==3){
            unsigned int start =0 ;
            vector<calculus_tree<DataType>> ret_curl(3);
            ret_curl[0].root = create_tree(simplify_sub(diff(gradient_field[2].root,independent_variables[1])
                                    ,diff(gradient_field[1].root,independent_variables[2])),start);
            start = 0;

            ret_curl[1].root = create_tree(simplify_mult("-1",simplify_sub(diff(gradient_field[2].root,independent_variables[0])
                            ,diff(gradient_field[0].root,independent_variables[2]))),start);
            start = 0;
            ret_curl[2].root = create_tree(simplify_sub(diff(gradient_field[1].root,independent_variables[0])
                                        ,diff(gradient_field[0].root,independent_variables[1])),start);

            return ret_curl;
        }
        return vector<calculus_tree<DataType>>();
    }

    //save and load section
    template<typename DataType>
    bool calculus_tree<DataType>::save(const string&file_path) {
        if(root){
            ofstream file(file_path);
            if (file.is_open()) {
                save_tour(root,file);
                file.close();
                return true;
            }
        }
        return false;
    }

    template<typename DataType>
    void calculus_tree<DataType>::save_tour(node* ptr,ofstream&file) const {
        if(root){
            bool found_keyword = false;
            string ret_exp = "";
            if(ptr == NULL){
                ptr = root;
            }
            if(is_function_tree(ptr)){
                file<<ptr->symbol;
                found_keyword = true;
            }
            if(ptr->left){
                file<<"(";
                save_tour(ptr->left,file);
                if(ptr->right){
                    file<<ptr->symbol;
                    save_tour(ptr->right,file);
                }
                file<<")";
            }
            else if(!found_keyword){
                file<<ptr->symbol;
            }
        }
    }

    template<typename DataType>
    bool calculus_tree<DataType>::load(const string&filePath) {
        ifstream file(filePath);
        if (file.is_open()) {
            if(root){
                remove_tree();
            }
            string line;
            string expression = "";
            while (getline(file, line)) {
                expression += line;
            }
            expression = processor.prepare_exp(expression) ;
            unsigned int start = 0;
            root = create_tree(expression,start);
            file.close();
            return true;
        }
        return false;
    }
    template<typename DataType>
    string  calculus_tree<DataType>::simplify_tree_add(const std::string &v1,const std::string  &v2){
        if(v1.size()||v2.size()){
            bool is_v1_num = is_num(v1);
            bool is_v2_num = is_num(v2);
            if(is_v1_num&&is_v2_num){
                return to_string(stold(v1)+stold(v2));
            }
            else if (is_v1_num)
            {
                if(abs(stold(v1))<=threshold){
                    return v2 ;
                }
            }
            else if(is_v2_num){
                if(abs(stold(v2))<=threshold){
                    return v1 ;
                }
            }
            if(v1=="inf"||v2=="inf"){
                return "inf";
            }
        }
        //can't simplify
        return "";
    }
    template<typename DataType>
    string  calculus_tree<DataType>::simplify_tree_sub(const std::string &v1,const std::string  &v2){
        if(v1.size()||v2.size()){
        bool is_v1_num = is_num(v1);
        bool is_v2_num = is_num(v2);
        if(is_v1_num&&is_v2_num){
            return to_string(stold(v1)-stold(v2));
        }
        //no need to simplify 0-v1 since it would be translated to -1*v1
        else if(is_v2_num){
            if(abs(stold(v2))<=threshold){
                return v1 ;
            }
        }
        else if(v1==v2){
            return "0";
        }
        if(v1=="inf"||v2=="inf"){
            return "inf";
        }
        }
        //can't simplify
        return "";
    }
    template<typename DataType>
    string  calculus_tree<DataType>::simplify_tree_mult(const std::string &v1,const std::string  &v2){
        if(v1.size()||v2.size()){
        bool is_v1_num = is_num(v1);
        bool is_v2_num = is_num(v2);
        if(is_v1_num&&is_v2_num){
            return to_string(stold(v1)*stold(v2));
        }
        else if (is_v1_num){
            if(abs(stold(v1))<=threshold){
                return "0";
            }
            else if(abs(stold(v1)-1)<=threshold){
                return v2;
            }
        }
        else if (is_v2_num){
            if(abs(stold(v2))<=threshold){
                return "0";
            }
            else if(abs(stold(v2)-1)<=threshold){
                return v1;
            }
        }
        else if(v1=="inf"||v2=="inf"){
            return "inf";
        }
        }
        return "";
    }
    template<typename DataType>
    string  calculus_tree<DataType>::simplify_tree_div(const std::string &v1,const std::string  &v2){
        if(v1.size()||v2.size()){
        bool is_v1_num = is_num(v1);
        bool is_v2_num = is_num(v2);
        if(is_v1_num&&is_v2_num){
            if(abs(stold(v2))<=threshold){
                if(abs(stold(v1))<=threshold){
                    return "nan";
                }
                return "inf" ;
            }
            return to_string(stold(v1)/stold(v2));
        }
        else if (is_v2_num){
            if(abs(stold(v2))<=threshold){
                return "inf";
            }
            else if(abs(stold(v2)-1)<=threshold){
                return v1;
            }
        }
        else if (is_v1_num){
            if(abs(stold(v1))<=threshold){
                return "0";
            }
        }
        else if(v1==v2){
            return"1";
        }
        else if(v2=="inf"){
            return "0";
        }
        else if(v1=="inf"){
            return "inf";
        }
        }
        return "";
    }
    template<typename DataType>
    string  calculus_tree<DataType>::simplify_tree_power(const std::string &v1,const std::string  &v2){
        if(v1.size()||v2.size()){
        bool is_v1_num = is_num(v1);
        bool is_v2_num = is_num(v2);
        if(is_v1_num&&is_v2_num){
            if(abs(stold(v2))<=threshold){
                if(abs(stold(v1))<=threshold){
                    return "nan";
                }
                return "1";
            }
            return to_string(pow(stold(v1),stold(v2)));
        }
        else if(is_v2_num){
            long double v2_val = stold(v2);
            if(abs(v2_val-1)<=threshold){
                return v1 ;
            }
            else if(abs(v2_val)<=threshold){
                return "1";
            }
        }
        else if(is_v1_num){
            long double v1_val = stold(v1);
            if(abs(v1_val-1)<=threshold){
                return "1" ;
            }
            else if(abs(v1_val)<=threshold){
                return "0";
            }
        }
        else if(v1=="inf"||v2=="inf"){
            return "inf";
        }
        }
        return "";
    }

    template<typename DataType>
    string calculus_tree<DataType>::simplify_tree(node*ptr){
        if(ptr){
            string left_operand="";
            string right_operand="";
            if(ptr->left){
                left_operand = simplify_tree(ptr->left);
            }
            if(ptr->right){
                right_operand = simplify_tree(ptr->right);
            }
            if(is_op_tree(ptr)){
                string simplification_output ="";
                switch(ptr->symbol[0]){
                    case '+':simplification_output = simplify_tree_add(left_operand,right_operand);break;
                    case '-':simplification_output = simplify_tree_sub(left_operand,right_operand);break;
                    case '*':simplification_output = simplify_tree_mult(left_operand,right_operand);break;
                    case '/':simplification_output = simplify_tree_div(left_operand,right_operand);break;
                    case '^':simplification_output = simplify_tree_power(left_operand,right_operand);break;
                }
                if(simplification_output.size()){
                    //simplification occurred
                    if(ptr->parent){
                        node*temp = ptr->parent;
                        remove_node(ptr);
                        temp->append_child(simplification_output);
                    }
                    else{
                        //root is updated then
                        remove_node(root);
                        root=new node(simplification_output);
                    }
                    return simplification_output ;
                }
            }
            else if(is_function_tree(ptr)){
                if(is_num(left_operand)){
                    unsigned int temp_start = 0 ;
                    int fn_code = is_known_function(ptr->symbol,temp_start);
                    string value ="";
                    DataType base_log = DataType(10);
                    if(fn_code==LOG){
                        if(ptr->symbol.length()>3){
                            base_log = DataType(stold(ptr->symbol.substr(3)));
                        }
                        value =to_string(evaluate_function<DataType>(fn_code,stold(left_operand),base_log)) ;
                    }
                    else{
                        value= to_string(evaluate_function<DataType>(fn_code,stold(left_operand),base_log));
                    }
                    if(ptr->parent){
                        node*temp = ptr->parent;
                        remove_node(ptr);
                        temp->append_child(value);
                    }
                    else{
                        //root is updated then
                        remove_node(root);
                        root=new node(value);
                    }
                    return value ;
                }
            }
        else {
            if(is_known_constant(ptr->symbol,0)!=-1){
                return to_string(evaluate_constant<DataType>(ptr->symbol));
            }
            return ptr->symbol;
        }
    }
    return "";
    }

    template<typename DataType>
    void calculus_tree<DataType>::simplify(void){
        if(root){
            simplify_tree(root);
        }
    }

    template<typename DataType>
    void calculus_tree<DataType>::exchange_variable_tour(node*ptr,const string &old_var,const calculus_tree<DataType>&new_var){
        if(ptr){
           if(ptr->left){
                exchange_variable_tour(ptr->left,old_var,new_var);
           }
           if(ptr->right){
                exchange_variable_tour(ptr->right,old_var,new_var);
           }
           if(ptr->left==NULL&&ptr->right==NULL){
                if(ptr->symbol==old_var){
                    if(ptr->parent){
                        node*parent= ptr->parent;
                        if(parent->left==ptr){
                            remove_node(ptr);
                            parent->left=copy_tree(new_var.root);
                            ptr=parent->left;
                        }
                        else{
                            remove_node(ptr);
                            parent->right=copy_tree(new_var.root);
                            ptr=parent->right;
                        }
                    }
                    else{
                        //updating root
                        remove_node(root);
                        root = copy_tree(new_var.root) ;
                        ptr=root;
                    }
                }
           }
        }
    }
    template<typename DataType>
    calculus_tree<DataType> calculus_tree<DataType>::exchange(const string&var,const string&new_var)const{
        if(is_keyword(var,0)==-1&&!is_num(var)){
            calculus_tree<DataType>temp_tree(new_var);
            calculus_tree<DataType>ret_tree=*this ;
            ret_tree.exchange_variable_tour(ret_tree.root,var,temp_tree);
            return ret_tree;
        }
        return *this;
    }

#include <chrono>

int main(){

    calculus_tree<long double>tree("0*(2x^2+cos(u))+1");
    tree.simplify();
    cout<<tree;
    system("pause");
}
