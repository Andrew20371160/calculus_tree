#ifndef _calculus_tree_h_

#define _calculus_tree_h_



// Uncomment the following line to enable complex mode
//NOTE:you must define the calculus tree datatype to be complex<>
//(e.g   calculus_tree<complex<long double>> tree();)

#define COMPLEX_MODE 1


#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <math.h>
#include <cmath>
#include <complex>
#include <string>
using namespace std ;

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
        node*parse(const string &expression,unsigned int &start);
        node*parse_block(const string &expression,unsigned int &start);

        void remove_root_keep_children(node*&ret_root);
        void fill_children(queue<string>&q ,node*&ret_root);
        node*parse_function(const string&,unsigned int &start);
        int precedence(const string&expression,unsigned int pos);
        void var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root);
        void var_op_func(const string&op,const string&var,node*&last_op,node*&ret_root);
        DataType evaluate_operator(char op,const DataType&left_operand,const DataType&right_operand);

        bool is_num(const string &var);
        /*
        since the output could generate a complex number
        (e.g. sqrt(-ve number)
        using complex numbers makes sense
        */
        DataType evaluate_function(const int fn,const DataType var,const DataType);

        DataType evaluate(node*ptr,const list<string>&vars_and_values);

        string eval_extract(const string&,unsigned int &start);

    public:
        calculus_tree(void);
        calculus_tree(const string&expression);
        ~calculus_tree();

        bool remove_tree(void);

        string differentiate(void)const;
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
