#!/bin/bash
flex --outfile lex_sql.cpp --header-file=lex_sql.h lex_sql.l
`which bison` -d -Wcounterexamples --output yacc_sql.cpp yacc_sql.y
