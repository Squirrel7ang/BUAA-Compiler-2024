#ifndef _TOKEN_H
#define _TOKEN_H

namespace token {
    enum token_type {
        NUM, 
        ADD, SUB, 
        MUL, DIV, 
        EXP,
        EQU,
    };

    class Token {

        token_type type;
        int lin;
        int col;

    public:
        explicit Token(int lin, int col, token_type type)
            : lin(lin), col(col), type(type) {}
        token_type get_type();
        int get_lin();
        int get_col();
        void set_type();
    };

    class NumberToken : public Token {
        int val;
    public:
        explicit NumberToken(int lin, int col, int val)
            : Token(lin, col, token_type::NUM) {}
        int get_val();
    };

    class SymbolToken : public Token {
        char ch;
    private:
        token_type char_to_token(char ch);
        char get_ch();

    public:
        explicit SymbolToken(int lin, int col, char ch)
            : Token(lin, col, char_to_token(ch)) {}
    };

} // namespace token


#endif // _TOKEN_H