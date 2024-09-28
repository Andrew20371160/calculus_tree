#ifndef _calculus_tree_h_

#define _calculus_tree_h_
//Includes
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <list>
#include <math.h>
#include <cmath>
#include <complex>
#include <string>
#include <fstream>

using namespace std ;



// Uncomment the following line to enable complex mode

//#define COMPLEX_MODE 1


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
    const int keyword_count =26;
    enum  {
        SQRT,ABS,SIN,COS,TAN,SEC,CSC,COTAN,ASIN,ACOS,ATAN,
        EXP,LN,SINH,COSH,TANH,ASINH,ACOSH,ATANH,IMG,I,LOG,
        PI,E,INF_ERR,NAN_ERR
    };
    /*
    functions must be followed by parenthese
    for log you just attach base to it
    for ex:log2
    by default it's log10

    pi,i,e are known constants no need to write the value explicitly
    */
    const string key_words[keyword_count]={"sqrt","abs","sin","cos","tan",
                                            "sec","csc","cotan","asin","acos",
                                            "atan","exp","ln","sinh","cosh",
                                            "tanh","asinh","acosh","atanh","img",
                                            "log","pi","e","i","inf","nan"};


#else
    const int function_count =20 ;
    const int keyword_count =24;

    enum  {SQRT,ABS,SIN,COS,TAN,SEC,CSC,COTAN,ASIN, ACOS, ATAN, EXP, LN, SINH,
           COSH,TANH,ASINH,ACOSH,ATANH,LOG,PI,E,INF_ERR,NAN_ERR
        };
    const string key_words[keyword_count]={"sqrt","abs","sin","cos","tan",
                                            "sec","csc","cotan","asin","acos",
                                            "atan","exp","ln","sinh","cosh",
                                            "tanh","asinh","acosh","atanh","log",
                                            "pi","e","inf","nan"};
#endif

    /*
    cout overloading for vector
    */
    template<typename T>
    ostream& operator<<(ostream& stream, const vector<T>& vec) ;


/*
the following class is to abstract linking operations
of a node from calculus_tree class
*/
class node
{
    template<typename DataType>
    friend class calculus_tree;
    //a symbol can be 3.5515, v1,x,any_name
    string symbol ;
    //links
    node *parent ;
    node *left ;
    node *right ;
    //a function to allocate memory for a new node
    node*get_node(const string&);
    /*
    this function disconects self or this from it's parent
    but it's children are still with him
    */
    bool disconnect_self(void);
    /*
    where (this) must have a parent
    this function adds a new child to parent of (this)
    or a new sibling to (this)
    */
    bool append_next(const string&);
    /*
    this function adds a child to (this)
    in the order -> left then right
    if it doesn't have place then it doesn't
    */
    bool append_child(const string&);
    /*
        if (this) has a parent then this function
        disconnects (this) from its parent
        then assign a new parent to it
        else it just assigns a new parent to it
    */
    bool append_parent(const string&);
    /*
    previous operations but done one nodes or subtrees
    */
    /*
    this function disconnects src_root from it's parent
    then adds it to children of parent of (this)
    if applicable else it doesn't
    */
    bool append_next(node*&src_root);
    /*
    this function disconnects src_root from it's parent
    then adds it to children of (this)
    if applicable else it doesn't
    */
    bool append_child(node*&src_root);
    /*
    this function disconnects (this) from it's parent
    then adds it to children of src_root if applicable
    */
    bool append_parent(node*&src_root);

    /*
    op becomes parent of (this)
    and parent (this) becomes parent of (op)
    check below diagram
    */
    /*
    p1                                p1
    |     ->exchange_parent(p2)->     |
    this                              p2
                                      |
                                    this
    */
    bool exchange_parent(const string&op) ;

};
/*
calculus tree (class used by the user)
*/
template<typename DataType>
class calculus_tree
{
    private :
        node *root  ;
        /*
        parsing functions
        note: all parsing functions have 2 inputs a string,and starting index (where to start parsing from)
        */

        /*
        this function starts creating a subtree from '(' till ')'
        recursively untill the subtree is generated then returns the root of that
        subtree
        */
        node*parse_parenthese(const string&,unsigned int &start);
        /*
        when finding a function from the keywords
        this function generates a subtree where the root is the function name
        and the child (left child) is the parenthese or expression between them
        */
        node*parse_function(const string&,unsigned int &start);
        /*
        this function switches between parsing a function or parenthese or just a variable
        then retrun generated tree ( or node in case of variable)
        */
        node*parse_block(const string &expression,unsigned int &start);

        /*
        this function parse the whole expression and generates the mathematical tree
        then returns the root of it
        */
        node*create_tree(const string &expression,unsigned int &start);
        /*
        returns precedence of the operator at pos in the expression
        */
        int precedence(const string&expression,unsigned int pos);
        /*
        this is a helper function to create_tree
        based on precedence of the new operator it assigns the place of var
        if higher or '^' then op is parent of var and op is assigned as a right child to last_op
        else then while the precedence is less than or equal to parent's operator precedence
        last_op goes up then exchang it's parent with op
        (of course ret_root is treated to be at max top)
        */
        void var_op_func(const string&op,node*&var,node*&last_op,node*&ret_root);
        /*
        this function extracts an operand (or variable no matter the length) or an operator
        and returns it
        */
        string extract(const string&,unsigned int &start);
        /*
        is function returns -1 if ptr->symbol isn't a function
        else it returns it's code (the enum value)
        */
        int is_function( node*&ptr)const;
        //same for this
        int is_function(const string&expression,unsigned int  ) ;
        /*
        returns true if expression[pos] is an operator
        */
        bool is_op(const string&expression,unsigned int  ) ;
        //checks if var is a number
        //with decimal points and signs
        bool is_num(const string &var);
        //returns true if extracted operand is a keyword (either a function or constant like "pi")
        bool is_keyword(const string&expression ,unsigned int pos);
        //removes src and it's chidren
        //disconnects src from it's parent aswell
        bool remove_node(node*&src) ;

        /*
        evaluation functions
        */
        /*
            this function extracts an operand till '=' , ',' are met
            (used in evaluation function)
        */
        string eval_extract(const string&,unsigned int &start);

        /*
            the name is obvious
        */
        DataType evaluate_operator(char op,const DataType&left_operand,const DataType&right_operand);
        /*
        if ptr->symbol is a kown constant it returns it's value
        else it prints an error msg where the issue is and returns zero at that point
        */
        DataType evaluate_constant(node*);
        /*
        switches through the known function using function code
        then returns the value
        */
        DataType evaluate_function(const int fn,const DataType var,const DataType base);
        /*
        vists children first to get values or expressions results
        then returns the total value of the mathematical tree
        (if tree contains variables like x) it uses the value where evaluating
        it doesn't replace value of x in the tree
        */
        DataType evaluate(node*ptr,const list<string>&vars_and_values);

        /*
        differentiation functions
        */

        /*
        switches through functions using fn then returns differentiation of it
        with respect to var
        */
        string diff_function(const int fn,node*ptr,const string&var );
        /*
            switches through operators to return the differentiation according to the operator
            this function also contains differentiation of '^'
            both cases whether exponent is a constant or an expression are handled
            (accordign to exponent rule)
        */
        string diff_op(node*ptr,const string&var);
        /*
        returns left' +    right'
                      -
        */
        string diff_plus_minus(node*ptr,const string&var);
        /*
        applies multiplication rule to ptr's children and returns the differentiation
        */
        string diff_mult(node*ptr,const string&var);
        /*
        applies division rule to ptr's children and returns the differentiation
        */
        string diff_div(node*ptr,const string&var);
        /*
        post-traversal while differentiating the tree with respect to var
        */
        string diff(node*ptr,const string&var);
        /*
            if v1 or v2 are zeroes these functions simplify the expression accordingly
            they aslo enclose each expression with parenthese
        */
        string simplify_add(const string&v1,const string &v2);
        string simplify_sub(const string&v1,const string &v2);
        string simplify_mult(const string&v1,const string &v2);
        string simplify_div(const string&v1,const string &v2);
        /*
        returns a copy from src root
        */
        node * copy_tree(const node*);
        /*
            fills ret_set with independent variables in the tree
            (e.g. x,y,v133335353...)
        */
        void independent_variables_tour(node*ptr,set<string>&ret_set);

        bool is_constant(const string &var);
        bool is_constant(node*ptr);
        bool is_known_constant(const string&var) ;
        unsigned int token_type(const string&token, int &open_brackets_c);

        bool valid_var_const_token(unsigned int previous_token, const string&token,string&ret_exp);

        bool valid_function_token(unsigned int previous_token, const string&token,string&ret_exp);

        bool valid_open_bracket_token(unsigned int previous_token, const string&token,string&ret_exp);

        bool valid_close_bracket_token(unsigned int previous_token, const string&token,string&ret_exp);

        bool valid_operator_token(unsigned int previous_token, const string&token,string&ret_exp);

        void skip_spaces(const string&expression,unsigned int &start);
        string preprocess_extract(const string&expression,unsigned int &start);

    public:
        /*
        constructors
        */
        calculus_tree(void);
        //generates the calculust tree form expression
        calculus_tree(const string&expression);
        //copy constructor
        calculus_tree(const calculus_tree&);
        ~calculus_tree();

        //assignment operator
        //of input is empty then caller becomes empty aswell
        calculus_tree& operator=(const calculus_tree&);
        /*
            operators for calculus trees
        */
        calculus_tree operator+(const calculus_tree&)const ;
        calculus_tree operator-(const calculus_tree&)const ;
        calculus_tree operator*(const calculus_tree&)const ;
        calculus_tree operator/(const calculus_tree&)const ;
        calculus_tree operator^(const calculus_tree&)const ;

        /*
            this function sets expression of the tree with expression
            (deletes old one if exists)
        */
        void set_exp(const string &expression);

        /*
            removes whole tree
        */
        bool remove_tree(void);
        /*
        this function differeniates the tree with respect to the variable of differentiation
        and returns the resulting tree
        */
        calculus_tree diff_with(const string&variable);
        /*
        returns gradient field of the tree
        differentiates the tree with respect to each eantry of ind_vars
        then returns the resulting vector field
        <df/dx1,df/fx2.......etc>
        */
        vector<calculus_tree<DataType>> gradient(const vector<string>&ind_vars);
        /*
        since curl is defined only in 2d or 3d
        this function returns curl of the gradient field (as a vector )
        first input is the gradient_field (must be 3 entries)
        2nd are the independent variables (you can say dell operator)

        */
        vector<calculus_tree<DataType>> curl(vector<calculus_tree<DataType>>&gradient_field,
                                                            const vector<string>&independent_variables);
        /*
        this function retruns the divergence of a gradient field (as a calculus tree)
        first input is the gradient_field (can be of any size)
        2nd are the independent variables (must be same size as gradient_field)
        input :(fx,fy,fz),(x,y,z)
        */
        calculus_tree divergence(vector<calculus_tree<DataType>>&gradient_field,
                                                            const vector<string>&independent_variables);

        /*
            first it calculates the gradient with respece to independent_variables
            then differentiates each entry with respect to independent_variables
            fxx+fyy+fzz
        */
        calculus_tree laplacian(const vector<string>&ind_vars);

        /*
        returns a vector where each entry is a variable in the tree
        */
        vector<string> independent_variables(void);
        /*
        loads an expression and sets current tree to be that expression
        */
        bool load(const string&filePath);
        //saves current tree into a file (of the path file_path)
        bool save(const string&filePath);

        void print(node*ptr = NULL)const;
        //returns a string of the expression in the current tree
        string expression(node*ptr = NULL)const;
        /*
        if the tree contains variables like x ,y,c1,v2....
        and you want to evaluate the tree
        call it like this ->evaluate_at("x=5.55,y=728,c1=525.2")
        if there are no variables then call it as it is
        evaluate_at()
        */
        DataType evaluate_at(string vars_equal="");
        //you can cout a tree
        friend ostream& operator<<(ostream& os, const calculus_tree& obj) {

                os << obj.expression();
                return os;
        }

        string prepare_exp(const string&exp);
    };


#endif
