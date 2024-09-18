#ifndef _calculus_general_tree_

#define _calculus_general_tree_

#include <iostream>
#include <queue>
#include <stack>

#include <string>
using namespace std ;

class node
{
    friend class calculus_tree;
    string symbol ;

    node *parent ;
    node *children ;
    node *next ;
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
        int  is_function( node*&ptr)const;

        bool remove_node(node*&src) ;
        bool is_op(const string&expression,unsigned int  ) ;
        int is_function(const string&expression,unsigned int  ) ;
        node*parse(const string &expression,unsigned int &start);
        node*parse_block(const string &expression,unsigned int &start);

        node*parse_expression(const string&,unsigned int &start);

        void remove_root_keep_children(node*&ret_root);
        void fill_children(queue<string>&q ,node*&ret_root);
        node*parse_function(const string&,unsigned int &start);
        int precedence(const string&expression,unsigned int pos);
        void var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root);
        void var_op_func(const string&op,const string&var,node*&last_op,node*&ret_root);

    public:
        calculus_tree(void);
        calculus_tree(const string&expression);
        ~calculus_tree();

        bool remove_tree(void);

        string differentiate(void)const;
        string integrate(void)const;
        void print(node*ptr = NULL)const;
        string expression(node*ptr = NULL)const;
        friend std::ostream& operator<<(ostream& os, const calculus_tree& obj) {

            os << obj.expression();
            return os;
        }
};


#endif
