#ifndef _calculus_tree_h_

#define _calculus_tree_h_
#include <queue>
#include <set>
#include <list>
#include <fstream>
#include "preprocessor.h"
#include "node.h"



    //to enable complex numbers mode head to functions_and_known_constants.h

    /*
    NOTE:you must define the calculus tree datatype to be complex<>
    (e.g   calculus_tree<complex<long double>> tree();)

    complex mode allows you to enter expressions like
    5+i*5 or 5+img(5)

    img is treated as a function so you can do this -> img(x^2+.......)
    i is treated as a constant so you must do this -> expresiion*i or i* expression
    */



/*
cout overloading for vector
*/
template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec) ;




/*
calculus tree (class used by the user)
*/
template<typename DataType>
class calculus_tree
{
    private :
        preprocessor processor;
        node *root  ;
        /*
        parsing functions
        note: all parsing functions have 2 inputs a std::string ,and starting index (where to start parsing from)
        */

        /*
        this function starts creating a subtree from '(' till ')'
        recursively untill the subtree is generated then returns the root of that
        subtree
        */
        node*parse_parenthese(const std::string &,unsigned int &start);
        /*
        when finding a function from the keywords
        this function generates a subtree where the root is the function name
        and the child (left child) is the parenthese or expression between them
        */
        node*parse_function(const std::string &,unsigned int &start);
        /*
        this function switches between parsing a function or parenthese or just a variable
        then retrun generated tree ( or node in case of variable)
        */
        node*parse_block(const std::string  &expression,unsigned int &start);

        /*
        this function parse the whole expression and generates the mathematical tree
        then returns the root of it
        */
        node*create_tree(const std::string  &expression,unsigned int &start);
        /*
        returns precedence of the operator at pos in the expression
        */
        int precedence(const std::string &expression,unsigned int pos);
        /*
        this is a helper function to create_tree
        based on precedence of the new operator it assigns the place of var
        if higher or '^' then op is parent of var and op is assigned as a right child to last_op
        else then while the precedence is less than or equal to parent's operator precedence
        last_op goes up then exchang it's parent with op
        (of course ret_root is treated to be at max top)
        */
        void var_op_func(const std::string &op,node*&var,node*&last_op,node*&ret_root);
        /*
        this function extracts an operand (or variable no matter the length) or an operator
        and returns it
        */
        std::string  extract(const std::string &,unsigned int &start);

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
        std::string  eval_extract(const std::string &,unsigned int &start);



        /*
        vists children first to get values or expressions results
        then returns the total value of the mathematical tree
        (if tree contains variables like x) it uses the value where evaluating
        it doesn't replace value of x in the tree
        */
        DataType evaluate(node*ptr,const  std::list<std::string>& vars_and_values);

        /*
        differentiation functions
        */

        /*
        switches through functions using fn then returns differentiation of it
        with respect to var
        */
        std::string  diff_function(const int fn,node*ptr,const std::string &var );
        /*
            switches through operators to return the differentiation according to the operator
            this function also contains differentiation of '^'
            both cases whether exponent is a constant or an expression are handled
            (accordign to exponent rule)
        */
        std::string  diff_op(node*ptr,const std::string &var);
        /*
        returns left' +    right'
                      -
        */
        std::string  diff_plus_minus(node*ptr,const std::string &var);
        /*
        applies multiplication rule to ptr's children and returns the differentiation
        */
        std::string  diff_mult(node*ptr,const std::string &var);
        /*
        applies division rule to ptr's children and returns the differentiation
        */
        std::string  diff_div(node*ptr,const std::string &var);
        /*
        post-traversal while differentiating the tree with respect to var
        */
        std::string  diff(node*ptr,const std::string &var);
        /*
            if v1 or v2 are zeroes these functions simplify the expression accordingly
            they aslo enclose each expression with parenthese
        */
        std::string  simplify_add(const std::string &v1,const std::string  &v2);
        std::string  simplify_sub(const std::string &v1,const std::string  &v2);
        std::string  simplify_mult(const std::string &v1,const std::string  &v2);
        std::string  simplify_div(const std::string &v1,const std::string  &v2);
        /*
        returns a copy from src root
        */
        node * copy_tree(const node*)const;
        /*
            fills ret_set with independent variables in the tree
            (e.g. x,y,v133335353...)
        */
        void independent_variables_tour(node*ptr, std::set<std::string>& ret_set);
        bool is_constant_tree(node*ptr)const;

        bool is_op_tree(node*ptr)const;

        void save_tour(node* ptr,std::ofstream&file) const ;
        /*
        is function returns -1 if ptr->symbol isn't a function
        else it returns it's code (the enum value)
        */
        bool is_function_tree( node*&ptr)const;
        std::string simplify_tree(node*ptr);
        std::string  simplify_tree_add(const std::string &v1,const std::string  &v2);
        std::string  simplify_tree_sub(const std::string &v1,const std::string  &v2);
        std::string  simplify_tree_mult(const std::string &v1,const std::string  &v2);
        std::string  simplify_tree_div(const std::string &v1,const std::string  &v2);
        std::string  simplify_tree_power(const std::string &v1,const std::string  &v2);

    public:
        /*
        constructors
        */
        calculus_tree(void);
        //generates the calculust tree form expression
        calculus_tree(const std::string &expression);
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
        void set_exp(const std::string  &expression);

        /*
            removes whole tree
        */
        bool remove_tree(void);
        /*
        this function differeniates the tree with respect to the variable of differentiation
        and returns the resulting tree
        */
        calculus_tree diff_with(const std::string &variable);
        /*
        returns gradient field of the tree
        differentiates the tree with respect to each eantry of ind_vars
        then returns the resulting vector field
        <df/dx1,df/fx2.......etc>
        */
        std::vector<calculus_tree<DataType>> gradient(const std::vector<std::string>&ind_vars);
        /*
        since curl is defined only in 2d or 3d
        this function returns curl of the gradient field (as a vector )
        first input is the gradient_field (must be 3 entries)
        2nd are the independent variables (you can say dell operator)

        */
        std::vector<calculus_tree<DataType>> curl(const std::vector<calculus_tree<DataType>>&gradient_field,
                                                            const std::vector<std::string>&independent_variables);
        /*
        this function retruns the divergence of a gradient field (as a calculus tree)
        first input is the gradient_field (can be of any size)
        2nd are the independent variables (must be same size as gradient_field)
        input :(fx,fy,fz),(x,y,z)
        */
        calculus_tree divergence(const std::vector<calculus_tree<DataType>>&gradient_field,
                                                            const std::vector<std::string>&independent_variables);

        /*
            first it calculates the gradient with respece to independent_variables
            then differentiates each entry with respect to independent_variables
            fxx+fyy+fzz
        */
        calculus_tree laplacian(const std::vector<std::string>&ind_vars);

        /*
        returns a vector where each entry is a variable in the tree
        */
        std::vector<std::string> independent_variables(void);
        /*
        loads an expression and sets current tree to be that expression
        */
        bool load(const std::string &filePath);
        //saves current tree into a file (of the path file_path)
        bool save(const std::string &filePath);

        void print(node*ptr = NULL)const;
        //returns a std::string  of the expression in the current tree
        std::string  expression(node*ptr = NULL)const;
        /*
        if the tree contains variables like x ,y,c1,v2....
        and you want to evaluate the tree
        call it like this ->evaluate_at("x=5.55,y=728,c1=525.2")
        if there are no variables then call it as it is
        evaluate_at()
        */
        DataType evaluate_at(std::string vars_equal="");
        //you can cout a tree
        friend std::ostream& operator<<(std::ostream& os, const calculus_tree& obj) {

                os << obj.expression();
                return os;
        }
        void simplify(void);

    };


#endif
