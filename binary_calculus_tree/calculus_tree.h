#ifndef _calculus_tree_h_

#define _calculus_tree_h_

#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <list>
#include <math.h>
#include <cmath>
#include <complex>
#include <string>
using namespace std ;




// Uncomment the following line to enable complex mode

#define COMPLEX_MODE 1


/*
NOTE:you must define the calculus tree datatype to be complex<>
(e.g   calculus_tree<complex<long double>> tree();)

complex mode allows you to enter expressions like
5+i*5 or 5+img(5)

img is treated as a function so you can do this -> img(x^2+.......)
i is treated as a constant so you must do this -> expresiion*i or i* expression
*/

#ifdef COMPLEX_MODE
    const int function_count =21 ;
    const int keyword_count =24;

    enum  {
        SQRT,
        ABS,
        SIN,
        COS,
        TAN,
        SEC,
        CSC,
        COTAN,
        ASIN,
        ACOS,
        ATAN,
        EXP,
        LN,
        SINH,
        COSH,
        TANH,
        ASINH,
        ACOSH,
        ATANH,
        IMG,
        I,
        LOG,
        PI,
        E
    };
    const string key_words[keyword_count]={"sqrt","abs","sin","cos","tan",
                                            "sec","csc","cotan","asin","acos",
                                            "atan","exp","ln","sinh","cosh",
                                            "tanh","asinh","acosh","atanh","img",
                                            "log","pi","e","i"};


#else
    const int function_count =20 ;
    const int keyword_count =23;

    enum  {
        SQRT,
        ABS,
        SIN,
        COS,
        TAN,
        SEC,
        CSC,
        COTAN,
        ASIN,
        ACOS,
        ATAN,
        EXP,
        LN,
        SINH,
        COSH,
        TANH,
        ASINH,
        ACOSH,
        ATANH,
        LOG,
        PI,
        E
    };
    const string key_words[keyword_count]={"sqrt","abs","sin","cos","tan",
                                            "sec","csc","cotan","asin","acos",
                                            "atan","exp","ln","sinh","cosh",
                                            "tanh","asinh","acosh","atanh",
                                            "log","pi","e"};
#endif










class node
{
    template<typename DataType>
    friend class calculus_tree;
    string symbol ;

    node *parent ;
    node *left ;
    node *right ;
    bool append_next(node*&src_root);
    bool append_child(node*&src_root);
    bool append_parent(node*&src_root);
    node*get_node(const string&);
    bool disconnect_self(void);
    bool append_next(const string&);
    bool append_child(const string&);
    bool append_parent(const string&);
    //where op is new parent of last op
    //and op is new children to parent of last op
    //and then last op becomes parent of last op
    //last_op->exchange_parent(op);
    bool exchange_parent(const string&op) ;

};
/*
what are operations
*+-/()^
*/

template<typename DataType>
class calculus_tree
{
    private :
        node *root  ;
        //this function parse an expression between 2 paranthese
        //then return it's root
        node*parse_parenthese(const string&,unsigned int &start);
        node*parse_operation(const string&,unsigned int &start);
        //this extracts operand (whatever its length) or operator
        string extract(const string&,unsigned int &start);
        int is_function( node*&ptr)const;

        bool remove_node(node*&src) ;
        bool is_op(const string&expression,unsigned int  ) ;
        int is_function(const string&expression,unsigned int  ) ;
        node*create_tree(const string &expression,unsigned int &start);
        node*parse_block(const string &expression,unsigned int &start);

        void remove_root_keep_children(node*&ret_root);
        void fill_children(queue<string>&q ,node*&ret_root);
        node*parse_function(const string&,unsigned int &start);
        int precedence(const string&expression,unsigned int pos);
        void var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root);
        void var_op_func(const string&op,const string&var,node*&last_op,node*&ret_root);
        DataType evaluate_operator(char op,const DataType&left_operand,const DataType&right_operand);


        DataType evaluate_constant(node*);
        bool is_num(const string &var);
        /*
        since the output could generate a complex number
        (e.g. sqrt(-ve number)
        using complex numbers makes sense
        */
        DataType evaluate_function(const int fn,const DataType var,const DataType);

        DataType evaluate(node*ptr,const list<string>&vars_and_values);

        string diff_function(const int fn,node*ptr,const string&var );
        string diff_op(node*ptr,const string&var);


        string eval_extract(const string&,unsigned int &start);
        node * copy_tree(node*);

        string simplify_add(const string&v1,const string &v2);
        string simplify_sub(const string&v1,const string &v2);
        string simplify_mult(const string&v1,const string &v2);
        string simplify_div(const string&v1,const string &v2);
        string diff_plus_minus(node*ptr,const string&var);
        string diff_mult(node*ptr,const string&var);
        string diff_div(node*ptr,const string&var);
        string diff(node*ptr,const string&var);

        void independent_variables_tour(node*ptr,set<string>&ret_set);

    public:
        calculus_tree(void);
        calculus_tree(const string&expression);
        calculus_tree(const calculus_tree&);
        calculus_tree& operator=(const calculus_tree&);

        ~calculus_tree();

        void set_exp(const string &expression);


        bool remove_tree(void);

        calculus_tree diff_with(const string&variable);

        list<calculus_tree> gradient(void);

        set<string> independent_variables(void);


        string integrate(void)const;
        void print(node*ptr = NULL)const;
        string expression(node*ptr = NULL)const;
        friend ostream& operator<<(ostream& os, const calculus_tree& obj) {

            os << obj.expression();
            return os;
        }
        /*
        if the tree contains variables like x ,y,c1,v2....
        and you want to evaluate the tree
        call it like this ->evaluate_at("x=5.55,y=728,c1=525.2")
        if there are no variables then call it as it is
        evaluate_at
        */
        DataType evaluate_at(string vars_equal="");

};


#endif
