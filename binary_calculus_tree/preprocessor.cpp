    #include "preprocessor.h"

    preprocessor::preprocessor(void){

    }

    void preprocessor::skip_spaces(const string&expression,unsigned int &start){
        while(start<expression.size()&&expression[start]==' '){
            start++;
        }
    }
    //this extracts operand (whatever its length) or operator
    //start goes to second operand or operator

    string preprocessor::preprocess_extract(const string&expression,unsigned int &start){
        string var="";
        if(start<expression.length()){
            skip_spaces(expression,start);
            unsigned int original_start = start;
            if(is_op(expression,start)){
                var+=expression[start];
                start++;
                skip_spaces(expression,start);
            }
            else if(expression[start]>='0'&&expression[start]<='9'){
                while(start<expression.length()&&expression[start]>='0'&&expression[start]<='9'&&!is_op(expression,start)&&expression[start]!=' '){
                    start++;
                }
                if(expression[start]=='.'){
                    //for floating point numbers
                    start++ ;
                }
                while(start<expression.length()&&expression[start]>='0'&&expression[start]<='9'&&!is_op(expression,start)&&expression[start]!=' '){
                    start++;
                }
                var = expression.substr(original_start, start - original_start);
                skip_spaces(expression,start);
            }
            else{
                while(start<expression.length()&&!is_op(expression,start)&&expression[start]!=' '){
                    start++;
                }
                var = expression.substr(original_start, start - original_start);
                skip_spaces(expression,start);
            }
        }
        return var ;
    }



    unsigned int preprocessor::token_type(const string&token,int &open_brackets_c){

        if(is_op(token,0)){
            switch(token[0]){
                case '(' :{
                    open_brackets_c++;
                    return OPEN_BRACKET ;
                }break;
                case ')' :{
                    open_brackets_c--;
                   return CLOSE_BRACKET ;
                }break;
                default :{
                    return OPERATOR ;
                }
            }
        }
        unsigned int temp_start = 0  ;
        if(is_known_function(token,temp_start)!=-1){
            return FUNCTION ;
        }
        else if(token=="."){
            return ERROR ;
        }
        else if(token[0]>='0'&&token[0]<='9'){
            if(is_num(token)){
                return VAR_CONST;
            }
            return ERROR ;
        }
        return VAR_CONST ;
    }

    bool preprocessor::valid_var_const_token(unsigned int previous_token, const string&token,string&ret_exp){
        bool valid = true ;
        //ERROR=-1,VAR_CONST,FUNCTION,OPERATOR,OPEN_BRACKET,CLOSE_BRACKET
        if(previous_token==VAR_CONST){
            ret_exp+="*";
        }
        else if(previous_token==FUNCTION||previous_token==CLOSE_BRACKET){
            cout<<"\nError:can't have ("<<token<<") immediately after a 'function' or '('";
            valid =false ;
        }
        return valid;
    }

    bool preprocessor::valid_function_token(unsigned int previous_token, const string&token,string&ret_exp){
        bool valid = true ;
        //ERROR=-1,VAR_CONST,FUNCTION,OPERATOR,OPEN_BRACKET,CLOSE_BRACKET
        if(previous_token==VAR_CONST||previous_token==CLOSE_BRACKET){
            ret_exp+="*";
        }
        else if(previous_token==FUNCTION){
            cout<<"\nError:can't have a function immediately after a function without parentheses";
            valid =false ;
        }
        return valid;
    }

    bool preprocessor::valid_open_bracket_token(unsigned int previous_token, const string&token,string&ret_exp){
        bool valid = true ;
        //ERROR=-1,VAR_CONST,FUNCTION,OPERATOR,OPEN_BRACKET,CLOSE_BRACKET
        if(previous_token==VAR_CONST||previous_token==CLOSE_BRACKET){
            ret_exp+="*";
        }
        return valid;
    }


    bool preprocessor::valid_close_bracket_token(unsigned int previous_token, const string&token,string&ret_exp){
        bool valid = true ;
        //ERROR=-1,VAR_CONST,FUNCTION,OPERATOR,OPEN_BRACKET,CLOSE_BRACKET
        if(previous_token>VAR_CONST&&previous_token<CLOSE_BRACKET){
            cout<<"\nError:can't have a function or *,(,/,+,-,^ immediately before ')'";
            valid =false;
        }
        return valid;
    }

    bool preprocessor::valid_operator_token(unsigned int previous_token, const string&token,string&ret_exp){
        bool valid = true ;
        //ERROR=-1,VAR_CONST,FUNCTION,OPERATOR,OPEN_BRACKET,CLOSE_BRACKET
        switch(previous_token){
            case FUNCTION:{
                cout<<"\nError:can't have ("<<token<<") immediately after a function without parentheses";
                valid= false ;
            }break ;
            case OPEN_BRACKET :{
                if(!(token[0]=='+'||token[0]=='-')){
                    cout<<"\nError:can't have ("<<token<<") immediately after '('";
                    valid = false;
                }
            }break ;
            case OPERATOR :{
                if(!(token[0]=='+'||token[0]=='-')){
                    cout<<"\nError:can't have ("<<token<<") immediately after '"<<ret_exp[ret_exp.size()-1]<<"'";
                    valid = false;
                }
            }break ;
        }
        return valid;
    }

    string preprocessor::prepare_exp(const string&expression){
        /*
        1-opening brackets must match closing ones
        2-2x,xsin(x),x(2) , x2 ->left*right
        3-xy is considered a variable name not x*y must indicate
        4-a function must be followed by ( even if it's one  operand like sinx
        5-operand is a token and operator is another token
        6-(x+y)(z+5)->(x+y)*(z+5)
        7- +- are the only operators that can be used to start an expression
        */
        unsigned int i = 0  ;
        string ret_exp = "" ;
        string temp = "";
        int open_brackets_c = 0 ;
        int previous_token =-1 ;
        int current_token =-1 ;
        bool error_state = false;
        temp =preprocess_extract(expression,i);
        previous_token = token_type(temp,open_brackets_c) ;
        ret_exp +=temp  ;
        while(i<expression.length()&&open_brackets_c>=0){
            temp = preprocess_extract(expression,i);
            current_token = token_type(temp,open_brackets_c) ;
                if(open_brackets_c>-1){
                    switch(current_token){
                        case FUNCTION:{
                            if(!valid_function_token(previous_token,temp,ret_exp)){
                                error_state=true;
                            }
                        }break ;

                        case OPEN_BRACKET :{
                            if(!valid_open_bracket_token(previous_token,temp,ret_exp)){
                                error_state=true;
                            }
                        }break ;

                        case CLOSE_BRACKET:{
                            if(open_brackets_c>-1){
                                if(!valid_close_bracket_token(previous_token,temp,ret_exp)){
                                    error_state=true;

                                }
                            }
                            else{
                                cout<<"\nError: closing brackets exceeded opening brackets" ;
                                error_state=true;
                            }
                        }break ;

                        case OPERATOR :{
                           if(!valid_operator_token(previous_token,temp,ret_exp)){
                                error_state=true;
                            }
                        }break ;

                        case ERROR :{
                            cout<<"\nError: invalid number "<<temp;
                            error_state=true;
                        }break ;

                        default :{
                            if(!valid_var_const_token(previous_token,temp,ret_exp)){
                                error_state=true;
                            }
                        }
                    }
                    if(error_state){
                        cout<<"\nError at "<<(i-temp.size());
                        return "0" ;
                    }
                }
                else{
                    cout<<"\nError: closing brackets exceeded opening brackets" ;
                    cout<<"\nError at "<<(i-temp.size());
                    return "0";
                }
                ret_exp +=temp  ;
                previous_token = current_token ;
            }
            //ERROR=-1,VAR_CONST,FUNCTION,OPERATOR,OPEN_BRACKET,CLOSE_BRACKET
            if(current_token>VAR_CONST&&current_token<CLOSE_BRACKET){
                cout<<"Error: can't end an expression with ("<<temp<<")";
                cout<<"\nError at "<<(i-temp.size());
                return "0";
            }
        cout<<"\nProcessed\n";
        return ret_exp;
    }
