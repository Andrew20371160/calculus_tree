#ifndef _calculus_tree_h_

#define _calculus_tree_h_

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
    bool apeend_parent(node*&src_root);
    public :

        node*get_node(const string&);

        bool append_next(const string&);
        bool append_child(const string&);
        bool apeend_parent(const string&);


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
        node*parse_paranthese(const string&,unsigned int &start);
        node*parse_operation(const string&,unsigned int &start);
        //this extracts operand (whatever its length) or operator
        string extract(const string&,unsigned int &start);

        bool remove_node(node*&src) ;
        bool is_op(const string&expression,unsigned int  ) ;
        bool is_keyword(const string&expression,unsigned int  ) ;

        node*parse_expression(const string&,unsigned int &start);

        void remove_root_keep_children(node*&ret_root);
        void fill_children(queue<string>&q ,node*&ret_root);
        node*parse_function(const string&,unsigned int &start);

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
