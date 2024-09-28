#ifndef _preprocessor_h_
#define _preprocessor_h_
#include "functions_and_known_constants.h"
enum{
    ERROR=-1,VAR_CONST,FUNCTION,OPERATOR,OPEN_BRACKET,CLOSE_BRACKET
};

    class preprocessor{
        private:
            unsigned int token_type(const string&token, int &open_brackets_c);

            bool valid_var_const_token(unsigned int previous_token, const string&token,string&ret_exp);

            bool valid_function_token(unsigned int previous_token, const string&token,string&ret_exp);

            bool valid_open_bracket_token(unsigned int previous_token, const string&token,string&ret_exp);

            bool valid_close_bracket_token(unsigned int previous_token, const string&token,string&ret_exp);

            bool valid_operator_token(unsigned int previous_token, const string&token,string&ret_exp);

            void skip_spaces(const string&expression,unsigned int &start);

            string preprocess_extract(const string&expression,unsigned int &start);


        public :
            preprocessor(void) ;
            string prepare_exp(const string&exp);

    };

#endif
