#ifndef _functions_and_known_constants_h_
#define _functions_and_known_constants_h_
    // Uncomment the following line to enable complex mode

    //#define COMPLEX_MODE 1


    //Includes
    #include <iostream>

    #include <string>



    #ifdef COMPLEX_MODE

        const int function_count =21 ;
        const int keyword_count =26;
        enum  {
            SQRT,ABS,SIN,COS,TAN,
            SEC,CSC,COTAN,ASIN,ACOS,
            ATAN,EXP,LN,SINH,COSH,
            TANH,ASINH,ACOSH,ATANH,IMG,
            LOG,
            PI,E,I,
            INF_ERR,NAN_ERR
        };
        /*
        functions must be followed by parenthese
        for log you just attach base to it
        for ex:log2
        by default it's log10

        pi,i,e are known constants no need to write the value explicitly
        */
        const std::string  key_words[keyword_count]={"sqrt","abs","sin","cos","tan",
                                                "sec","csc","cotan","asin","acos",
                                                "atan","exp","ln","sinh","cosh",
                                                "tanh","asinh","acosh","atanh","img",
                                                //add new functions here
                                                "log",
                                                //add new constants here
                                                "pi","e","i",
                                                "inf","nan"};


    #else
        const int function_count =20 ;
        const int keyword_count =24;

        enum  {
            SQRT,ABS,SIN,COS,TAN,
            SEC,CSC,COTAN,ASIN,ACOS,
            ATAN,EXP,LN,SINH,COSH,
            TANH,ASINH,ACOSH,ATANH,LOG,
            PI,E,
            INF_ERR,NAN_ERR
        };
        const std::string  key_words[keyword_count]={"sqrt","abs","sin","cos","tan",
                                                "sec","csc","cotan","asin","acos",
                                                "atan","exp","ln","sinh","cosh",
                                                "tanh","asinh","acosh","atanh",
                                                //add new functions here
                                                "log",
                                                "pi","e",
                                                //add new constants here
                                                "inf","nan"};

    #endif
    /*
    extracts operand at pos then returns it's enum value if exists
    else it returns -1
    */
    int is_known_function(const std::string &expression,unsigned int& pos ) ;
    /*
    returns true if expression[pos] is an operator
    */
    bool is_op(const std::string &expression,unsigned int pos ) ;
    //checks if var is a number
    //with decimal points and signs
    bool is_num(const std::string  &var);
    //returns enum value if operand starting at pos
    //else -1
    int is_keyword(const std::string &expression,unsigned int  pos);
    //returns enum value if an operand starting at pos if it's a known constant
    //else -1
    int is_known_constant(const std::string &var,unsigned int pos ) ;

#endif
