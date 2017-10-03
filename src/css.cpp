
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

#include <algorithm>
#include <vector>
#include <string>
#include <limits>
#include <iostream>

inline bool is_white_space(char c){
  return c == '\n' || c == ' ' || c == '\t' ;
}

const char* skip_space(const char* p){
  while( is_white_space(*p) ) p++ ;
  return p ;
}

const char* parse_rulename(std::string& s, const char *p){
  const char* start = p ;

  // move right to the opening brace
  while( *p != '{' ){  //}
    p++ ;
  }

  // move back to trim space
  const char* end = p - 1;
  while( is_white_space(*end) ) --end ;

  s.assign(start, end+1) ;
  std::replace( s.begin(), s.end(), '\n', ' ') ;

  p++ ;
  return p ;
}

const char* parse_declaration( std::string& s, const char* p){
  const char* start = p ;

  // move right unntil the ':'
  while( *p != ':' ) p++ ;

  // move back to trim space
  const char* end = p - 1;
  while( is_white_space(*end) ) --end ;

  s.assign( start, end + 1) ;
  p++ ; // also skip the ':'
  return p ;
}

const char* skip_string( const char* p){
  const char* q = p ;
  p++ ;
  while( *p != '"' ) p++ ;
  p++ ;

  std::string s ;
  s.assign( q, p ) ;

  return p ;
}

const char* parse_value( std::string& s, const char* p ){
  p = skip_space(p) ;
  const char* start = p ;

  while( true ){

    switch(*p){
    case '}':
    case ';':
      {
        const char* end = p ;
        while( *p == ' ') --end ;
        s.assign( start, end ) ;
        return p ;
      }
    case '"':
      p = skip_string(p) ;
      break ;
    default:
      break ;
    } ;
    p++ ;

  }

  return p ;
}

extern "C" SEXP tokenize_css_impl( SEXP txt ){
  SEXP s1 = STRING_ELT(txt, 0) ;
  const char* p = CHAR(STRING_ELT(txt, 0));

  std::string rule_name ;
  std::string declaration ;
  std::string value ;

  std::vector< std::string > rules ;
  std::vector< std::string > declarations ;
  std::vector< std::string > values ;

  while(true){
    R_CheckUserInterrupt();

    // the rule name
    p = skip_space(p) ;
    if( ! *p ) break ;

    p = parse_rulename(rule_name, p) ;

    // declarations for the rule
    while(true){
      // skip empty space
      p = skip_space(p) ;
      if( *p == '}' ) {
        p++ ;
        break ;
      }

      // here we are at the start of a declaration
      p = parse_declaration(declaration, p) ;

      // and the value
      p = parse_value( value, p ) ;

      // store all 3
      rules.push_back(rule_name) ;
      declarations.push_back(declaration) ;
      values.push_back(value) ;

      char c = *p ;
      p++ ;
      if( c == '}' ) break ;
    }

  }

  int n = rules.size() ;

  SEXP s_rules = PROTECT(Rf_allocVector(STRSXP, n)) ;
  SEXP s_declarations = PROTECT(Rf_allocVector(STRSXP, n)) ;
  SEXP s_values = PROTECT(Rf_allocVector(STRSXP, n)) ;

  for( int i=0; i<n; i++) SET_STRING_ELT(s_rules, i, Rf_mkCharLen(rules[i].c_str(), rules[i].size() )) ;
  for( int i=0; i<n; i++) SET_STRING_ELT(s_declarations, i, Rf_mkCharLen(declarations[i].c_str(), declarations[i].size() )) ;
  for( int i=0; i<n; i++) SET_STRING_ELT(s_values, i, Rf_mkCharLen(values[i].c_str(), values[i].size() )) ;

  SEXP res = PROTECT(Rf_allocVector(VECSXP, 3)) ;
  SET_VECTOR_ELT(res, 0, s_rules) ;
  SET_VECTOR_ELT(res, 1, s_declarations) ;
  SET_VECTOR_ELT(res, 2, s_values) ;

  SEXP colnames = PROTECT(Rf_allocVector(STRSXP, 3)) ;
  SET_STRING_ELT(colnames, 0, Rf_mkChar("rule")) ;
  SET_STRING_ELT(colnames, 1, Rf_mkChar("property")) ;
  SET_STRING_ELT(colnames, 2, Rf_mkChar("value")) ;

  SEXP classes = PROTECT(Rf_allocVector(STRSXP, 3)) ;
  SET_STRING_ELT(classes, 0, Rf_mkChar("tbl_df")) ;
  SET_STRING_ELT(classes, 1, Rf_mkChar("tbl")) ;
  SET_STRING_ELT(classes, 2, Rf_mkChar("data.frame")) ;

  SEXP rownames = PROTECT(Rf_allocVector(INTSXP, 2)) ;
  INTEGER(rownames)[0] = NA_INTEGER ;
  INTEGER(rownames)[1] = -n ;

  Rf_setAttrib(res, Rf_install("names"), colnames ) ;
  Rf_setAttrib(res, Rf_install("class"), classes ) ;
  Rf_setAttrib(res, Rf_install("row.names"), rownames ) ;

  SET_OBJECT(res, 1);
  UNPROTECT(7) ;
  return res ;

}
