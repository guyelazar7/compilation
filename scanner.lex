%{
    #include "types.h"
    #include "hw3_output.hpp"
    #include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap

whitespace  ([\r\t\n ])

%%
int                                 yylval=new Node(yytext);    return INT;
byte                                yylval=new Node(yytext);   return BYTE;
b                                   yylval=new Node(yytext);  return B;
bool                                yylval=new Node(yytext);   return BOOL;
and                                 yylval=new Node(yytext);    return AND;
or                                  yylval=new Node(yytext);   return OR;
not                                 yylval=new Node(yytext);return NOT;
true                                yylval=new Node(yytext);return TRUE;
false                               yylval=new Node(yytext);return FALSE;
return                              yylval=new Node(yytext);return RETURN;
if                                  yylval=new Node(yytext);return IF;
else                                yylval=new Node(yytext);return ELSE;
while                               yylval=new Node(yytext);return WHILE;
break                               yylval=new Node(yytext);return BREAK;
continue                            yylval=new Node(yytext);return CONTINUE;
";"                                 yylval=new Node(yytext);return SC;
"("                                 yylval=new Node(yytext);return LPAREN;
")"                                 yylval=new Node(yytext);return RPAREN;
"{"                                 yylval=new Node(yytext);return LBRACE;
"}"                                 yylval=new Node(yytext);return RBRACE;
"="                                 yylval=new Node(yytext);return ASSIGN;
"=="|"!="                           yylval=new Node(yytext);return EQUALS;
"<="|"<"|">="|">"                   yylval=new Node(yytext);   return RELOP;
[-+]                                yylval=new Node(yytext);return PLUS_MINUS;
[*/]                                yylval=new Node(yytext);return MUL_DIV;
[a-zA-Z][a-zA-Z0-9]*                yylval=new Node(yytext); return ID;
[1-9]+[0-9]*|0                      yylval=new Node(yytext);  return NUM;
\"([^\"\n\r\\]|\\[rnt\"\\])+\"      yylval=new Node(yytext); return STRING;
\/\/([^\r\n]*) ;
{whitespace} ;
. {output::errorLex(yylineno);exit(0);};
%%