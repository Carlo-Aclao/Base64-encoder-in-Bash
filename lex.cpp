/*
 * lex.cpp
 *
 * CS280 - Fall 2017
 */

#include <cctype>
#include <map>
using std::map;

#include "tokens.h"

string tokenString( TokenType tt )
{
    switch(tt)
    {
        case IF:       return "IF";
        case THEN:     return "THEN";
        case PRINT:    return "PRINT";
        case TRUE:     return "TRUE";
        case FALSE:    return "FALSE";
        case IDENT:    return "IDENT";
        case ICONST:   return "ICONST";
        case SCONST:   return "SCONST";
        case PLUS:     return "PLUS";
        case MINUS:    return "MINUS";
        case STAR:     return "STAR";
        case SLASH:    return "SLASH";
        case ASSIGN:   return "ASSIGN";
        case EQ:       return "EQ";
        case NEQ:      return "NEQ";
        case LT:       return "LT";
        case LEQ:      return "LEQ";
        case GT:       return "GT";
        case GEQ:      return "GEQ";
        case LOGICAND: return "LOGICAND";
        case LOGICOR:  return "LOGICOR";
        case SC:       return "SC";
        case ERR:      return "ERR";
        case DONE:     return "DONE";
        default:       return "";
    }
};


extern ostream& operator<<(ostream& out, const Token& tok) {
	TokenType tt = tok.GetTokenType();
	out << tokenString(tt);
	if( tt == IDENT || tt == ICONST || tt == SCONST || tt == ERR ) {
		out << "(" << tok.GetLexeme() << ")";
	}
	return out;
}

extern Token getNextToken(istream *in, int *linenum){

	enum LexState { BEGIN, INID, INSTRING, SAWEQ, SAWLT, SAWGT, 
                    SAWAND, SAWOR, SAWNOT, ININT, INCOMMENT } lexstate = BEGIN;
	string lexeme;
	char ch;

	while(in->get(ch)) {

		if( ch == '\n' ) {
			(*linenum)++;
		}

		switch( lexstate ) {
		case BEGIN:
			if( isspace(ch) )
				continue;

			lexeme = ch;

            if(      isalpha(ch) ) { lexstate = INID; }
            else if( isdigit(ch) ) { lexstate = ININT; }
            else if( ch == '"'   ) { lexstate = INSTRING; }
            else if( ch == '='   ) { lexstate = SAWEQ; }
            else if( ch == '>'   ) { lexstate = SAWGT; }
            else if( ch == '<'   ) { lexstate = SAWLT; }
            else if( ch == '&'   ) { lexstate = SAWAND; }
            else if( ch == '|'   ) { lexstate = SAWOR; }
            else if( ch == '!'   ) { lexstate = SAWNOT; }
            else if( ch == '#'   ) { lexstate = INCOMMENT; }
			else {
				TokenType tt = ERR;
				switch( ch ) {
                    case '+': tt = PLUS;   break;
                    case '-': tt = MINUS;  break;
                    case '*': tt = STAR;   break;
                    case '/': tt = SLASH;  break;
                    case '(': tt = LPAREN; break;
                    case ')': tt = RPAREN; break;
                    case ';': tt = SC;     break;
				}
				return Token(tt, lexeme, *linenum);
			}
			break;

		case INID:
			if( isalpha(ch) || isdigit(ch) ) {
				lexeme += ch;
			}
			else {
				if( ch == '\n' )
					(*linenum)--;
				in->putback(ch);
                if( lexeme == "if"    ) return Token( IF,    lexeme, *linenum );
                if( lexeme == "then"  ) return Token( THEN,  lexeme, *linenum );
                if( lexeme == "print" ) return Token( PRINT, lexeme, *linenum );
                if( lexeme == "true"  ) return Token( TRUE,  lexeme, *linenum );
                if( lexeme == "false" ) return Token( FALSE, lexeme, *linenum );
				return Token( IDENT, lexeme, *linenum);
			}
			break;

		case INSTRING:
			lexeme += ch;
			if( ch == '\n' ) {
				return Token(ERR, lexeme, *linenum );
			}
			if( ch == '"' ) {
				lexeme = lexeme.substr(1, lexeme.length()-2);
				return Token(SCONST, lexeme, *linenum );
			}
			break;

		case SAWEQ:
			if( ch == '=' )
				return Token(EQ, lexeme, *linenum);
			else {
				if( ch == '\n' )
					(*linenum)--;
				in->putback(ch);
				return Token(ASSIGN, lexeme, *linenum);
			}
			break;

		case SAWGT:
			if( ch == '=' )
				return Token(GEQ, lexeme, *linenum);
			else {
				if( ch == '\n' )
					(*linenum)--;
				in->putback(ch);
				return Token(GT, lexeme, *linenum);
			}
			break;

		case SAWLT:
			if( ch == '=' )
				return Token(LEQ, lexeme, *linenum);
			else {
				if( ch == '\n' )
					(*linenum)--;
				in->putback(ch);
				return Token(LT, lexeme, *linenum);
			}
			break;

		case SAWAND:
			if( ch == '&' ) return Token(LOGICAND, lexeme, *linenum);
			else            return Token(ERR,      lexeme, *linenum);
			break;

		case SAWOR:
			if( ch == '|' ) return Token(LOGICOR, lexeme, *linenum);
			else            return Token(ERR,     lexeme, *linenum);
			break;

		case SAWNOT:
			if( ch == '=' ) return Token(NEQ, lexeme, *linenum);
			else            return Token(ERR, lexeme, *linenum);
			break;

		case ININT:
			if( isdigit(ch) ) {
				lexeme += ch;
			}
			else if( isalpha(ch) ) {
				lexeme += ch;
				return Token(ERR, lexeme, *linenum);
			}
			else {
				if( ch == '\n' )
					(*linenum)--;
				in->putback(ch);
				return Token(ICONST, lexeme, *linenum);
			}
			break;

		case INCOMMENT:
			if( ch == '\n' ) lexstate = BEGIN;
			break;
		}

	}

	if( in->eof() ) return Token(DONE, "", *linenum);
	return Token(ERR, "some strange I/O error", *linenum);
}

