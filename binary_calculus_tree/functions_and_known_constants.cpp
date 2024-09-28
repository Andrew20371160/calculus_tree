#include "functions_and_known_constants.h"


    bool is_op(const string&expression,unsigned int pos ) {
        if(pos<expression.size()){
            switch(expression[pos]){
                case '+':return true ;break;
                case '-':return true ;break;
                case '*':return true ;break;
                case '/':return true ;break;
                case '^':return true ;break;
                case '(':return true ;break;
                case ')':return true ;break;
            }
        }
        return false ;
    }

    bool is_num(const string &var){
        if(var.length()){
            bool found_digit=  false ;
            unsigned int i=0 ;
            unsigned int dot_counter = 0 ;
            while(i<var.length()){
                if(var[i]>='0'&&var[i]<='9'){
                   found_digit= true;
                }
                else if(var[i]=='-'||var[i]=='+'){
                    if(found_digit){
                        return false ;
                    }
                }
                else if(var[i]=='.'){
                    if(dot_counter){
                        return false ;
                    }
                    else {
                        dot_counter =1 ;
                    }
                }
                else{
                    return false ;
                }
                i++;
            }
            return true ;
        }
        return false ;
    }

    int is_known_function(const string&expression ,unsigned int &start){
        if(start<expression.length()){
            unsigned int temp_start = start ;
            unsigned int original_start = start ;
            while(temp_start<expression.length()&&!is_op(expression,temp_start)){
                temp_start++;
            }
            string temp=  expression.substr(original_start, temp_start - original_start);
            if(temp.substr(0,3)=="log"){
                return LOG ;
             }
             for(int i =0 ; i <function_count-1;i++){
                if(temp==key_words[i]){
                    return i;
                }
             }
         }
         return -1 ;
    }

    int is_keyword(const string&expression ,unsigned int pos){
        if(pos<expression.length()){
            unsigned int original_start = pos ;
            while(pos<expression.length()&&!is_op(expression,pos)){
                pos++;
            }
            string temp=  expression.substr(original_start, pos - original_start);
             for(int i =0 ; i <keyword_count;i++){
                if(temp==key_words[i]){
                    return i;
                }
             }
         }
         return -1 ;
    }

    int is_known_constant(const string&expression,unsigned int pos ) {
        if(pos<expression.length()){
            unsigned int original_start = pos ;
            while(pos<expression.length()&&!is_op(expression,pos)){
                pos++;
            }
            string temp=  expression.substr(original_start, pos - original_start);
            for(int i =function_count ; i <keyword_count-2;i++){
                if(temp==key_words[i]){
                    return i;
                }
            }
        }
        return -1 ;
    }
