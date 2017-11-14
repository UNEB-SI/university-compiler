#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include "parser.h"

int cont_st = 0;


void prog() {
  // RECOGNIZE GLOBAL DECLARATION OR DEFAULT FUNCTION BODY
  if(isType()){
    getToken();
    strcpy(sb_token.type, token.pr);
    sb_token.scope = GLOBAL;
    strcpy(sb_token.name,token.lexem.value);
    sb_token.zumbi = 0;// we gonna set the cat later
    if(token.type == ID){
      getToken();
      //just for symbol table
      if ((token.type == SN && strcmp(token.signal, ";") == 0) || (token.type == SN && strcmp(token.signal, ",") == 0)) {
        sb_token.cat = VAR;
        verifyRedeclaration(sb_token);
        insert_symbol();
      }
      //DEFAULT DECLARATION
      while(token.type == SN && strcmp(token.signal, ",") == 0) {
        getToken();
        if(token.type == ID) {
          strcpy(sb_token.name,token.lexem.value);
          verifyRedeclaration(sb_token);
          insert_symbol();
          getToken();
        } else {
            sintatic_erro(2);
          exit(-1);
        }
      }
      if(token.type == SN && strcmp(token.signal, ";") == 0) {//final of declaration
        getToken();
        prog();
      }
      //DEFAULT FUNCTION
      else if(token.type == SN && strcmp(token.signal, "(") == 0) {//if it is a function
        getToken();
        sb_token.cat = FUNC;
        verifyRedeclaration(sb_token);
        insert_symbol();
        types_param();
        if(token.type == SN && strcmp(token.signal, ")") == 0) {
          getToken();
          if(token.type == SN && strcmp(token.signal, "{") == 0) {
            getToken();
            while(isType()){
              strcpy(sb_token.type, token.pr);
              getToken();
              //symbol table
              sb_token.scope = LOCAL;
              strcpy(sb_token.name, token.lexem.value);
              sb_token.zumbi = 0;// we gonna set the cat later
              sb_token.cat = VAR;

              if(token.type == ID){
                getToken();
                verifyRedeclaration(sb_token);
                insert_symbol();
                //verify if it is declaration
                while(token.type == SN && strcmp(token.signal, ",") == 0) {
                  getToken();
                  if(token.type == ID) {
                    strcpy(sb_token.name,token.lexem.value);
                    verifyRedeclaration(sb_token);
                    insert_symbol();
                    getToken();
                  } else {
                    sintatic_erro(2);
                    exit(-1);
                  }
                }
                //if it is ; declaration finish
                if(token.type == SN && strcmp(token.signal, ";") == 0) {
                  getToken();
                } else {
                  sintatic_erro(1);
                  exit(-1);
                }
              }
            }
            //verify if has commands
            while(cmd());

            if(token.type == SN && strcmp(token.signal, "}") == 0) {
              getToken();
              // keep all parameters and delete local variables
              insert_zombie();
              exclude_local_symbol();
              prog();
            } else {
              sintatic_erro(6);
              exit(-1);
            }
          } else {
            sintatic_erro(5);
            exit(-1);
          }
        } else {
          sintatic_erro(3);
          exit(-1);
        }

      } else{
        sintatic_erro(13);
        exit(-1);
      }
    } else {
      sintatic_erro(2);
      exit(-1);
    }
  }
  // RECOGNIZE PROTOTYPE
  else if(token.type == PR && strcmp(token.pr, "prototipo") == 0) {
    getToken();
    if(isType()) {
      getToken();
      if(token.type == ID) {
        getToken();
        if(token.type == SN && strcmp(token.signal, "(") == 0) {
          getToken();
          opc_p_types();
          if(token.type == SN && strcmp(token.signal, ")") == 0) {
            getToken();
            while(token.type == SN && strcmp(token.signal, ",") == 0) {
              getToken();
              if(token.type == ID) {
                getToken();
                if(token.type == SN && strcmp(token.signal, "(") == 0) {
                  getToken();
                  opc_p_types();
                  if(token.type == SN  && strcmp(token.signal, ")") == 0) {
                    getToken();
                  } else {
                    sintatic_erro(3);
                    exit(-1);
                  }
                } else {
                  sintatic_erro(4);
                  exit(-1);
                }
              } else {
                sintatic_erro(2);
                exit(-1);
              }
            }
            //END OF PROTOTYPE WITH TYPE
            if (token.type == SN && strcmp(token.signal, ";") == 0) {
              getToken();
              prog();
            } else {
              sintatic_erro(1);
              exit(-1);
            }
          } else {
            sintatic_erro(3);
            exit(-1);
          }
        } else {
          sintatic_erro(4);
          exit(-1);
        }
      } else {
        sintatic_erro(2);
        exit(-1);
      }
    }

    //PROTOTYPE WITHOUT TYPE
    else if(token.type == PR && strcmp(token.pr, "semretorno") == 0) {
      getToken();
      if(token.type == ID){
        getToken();
        if(token.type == SN && strcmp(token.signal, "(") == 0) {
          getToken();
          opc_p_types();
          if(token.type == SN && strcmp(token.signal, ")") == 0) {
            getToken();
            while(token.type == SN && strcmp(token.signal, ",") == 0) {
              getToken();
              if(token.type == ID) {
                getToken();
                if(token.type == SN && strcmp(token.signal, "(") == 0) {
                  getToken();
                  opc_p_types();
                  if(token.type == SN  && strcmp(token.signal, ")") == 0) {
                    getToken();
                  } else {
                    sintatic_erro(3);
                    exit(-1);
                  }
                } else {
                  sintatic_erro(4);
                  exit(-1);
                }
              } else {
                sintatic_erro(2);
                exit(-1);
              }
            }
            //END OF PROTOTYPE WITHOUT TYPE
            if (token.type == SN && strcmp(token.signal, ";") == 0) {
              getToken();
              prog();
            } else {
              sintatic_erro(1);
              exit(-1);
            }
          } else {
            sintatic_erro(3);
          }
        } else {
          sintatic_erro(4);
          exit(-1);
        }
      } else {
        sintatic_erro(2);
        exit(-1);
      }
    }else {
    //  printf("Tô pegando %d\n %s\n", token.type, token.lexem.value);
      sintatic_erro(11);
      exit(-1);
    }
  }
  // RECOGNIZE FUNCTION WITHOUT RETURN
  else if(token.type == PR && strcmp(token.pr, "semretorno") == 0) {
    strcpy(sb_token.type, token.pr);
    getToken();
    sb_token.scope = GLOBAL;
    strcpy(sb_token.name,token.lexem.value);
    sb_token.zumbi = 0;// we gonna set the cat later
    if(token.type == ID) {
      sb_token.cat = FUNC;
      verifyRedeclaration(sb_token);
      insert_symbol();
      getToken();
      if(token.type == SN && strcmp(token.signal, "(") == 0) {//if it is a function
        getToken();
        types_param();
        if(token.type == SN && strcmp(token.signal, ")") == 0) {
          getToken();
          if(token.type == SN && strcmp(token.signal, "{") == 0) {
            getToken();
            while(isType()){
              strcpy(sb_token.type, token.pr);
              getToken();
              if(token.type == ID && (next_token.type == SN && (strcmp(next_token.signal, ",") == 0 || strcmp(next_token.signal, ";") == 0))){
                sb_token.scope = LOCAL;
                strcpy(sb_token.name, token.lexem.value);
                sb_token.zumbi = 0;// we gonna set the cat later
                sb_token.cat = VAR;
                verifyRedeclaration(sb_token);
                insert_symbol();
                getToken();
                //verify if it is declaration
                while(token.type == SN && strcmp(token.signal, ",") == 0) {
                  getToken();
                  if(token.type == ID) {
                    strcpy(sb_token.name, token.lexem.value);
                    verifyRedeclaration(sb_token);
                    insert_symbol();
                    getToken();
                  } else {
                    sintatic_erro(2);
                    exit(-1);
                  }
                }
                //if it is ; declaration finish
                if(token.type == SN && strcmp(token.signal, ";") == 0) {
                  getToken();
                } else {
                  sintatic_erro(1);
                  exit(-1);
                }
              }
            }
            while(cmd());
            if(token.type == SN && strcmp(token.signal, "}") == 0) {
              getToken();
              // keep all parameters and delete local variables
              insert_zombie();
              exclude_local_symbol();
              prog();
            } else {
                sintatic_erro(6);
              exit(-1);
            }
          } else {
            sintatic_erro(4);
            exit(-1);
          }
        } else {
          sintatic_erro(3);
          exit(-1);
        }

      } else{
        sintatic_erro(4);
        exit(-1);
      }
    } else {
      sintatic_erro(7);
      exit(-1);
    }
  }
  else if(token.type == eOF){
    printf("Sucesso na compilação!\n");
    exit(0);
  }
  else{
    sintatic_erro(14);
    exit(-1);
  }
}

/**
* Verify the interval between reserved word to know if it is a type definition
**/
int isType() {
  if(token.type == PR && isReservedWord(token.pr) >= 0 && isReservedWord(token.pr) < 4){
    return 1;
  }
  return 0;
}

void types_param(){
  if(token.type == PR && strcmp(token.signal, "semparam") == 0) {
    getToken();
  } else if(isType()) {
    strcpy(sb_token.type, token.pr);
    getToken();
    sb_token.scope = LOCAL;
    sb_token.cat = PARAN;
    strcpy(sb_token.name,token.lexem.value);
    sb_token.zumbi = 0;// we gonna set the cat later
    if(token.type == ID){
      getToken();
      sb_token.cat = PARAN;
      verifyRedeclaration(sb_token);
      insert_symbol();
      //verify if it is declaration
      while(token.type == SN && strcmp(token.signal, ",") == 0) {
        getToken();
        if(isType()){
          getToken();
          strcpy(sb_token.type, token.pr);
          sb_token.scope = LOCAL;
          sb_token.cat = PARAN;
          strcpy(sb_token.name,token.lexem.value);
          sb_token.zumbi = 0;// we gonna set the cat later
          if(token.type == ID) {
            getToken();
            sb_token.cat = PARAN;
            verifyRedeclaration(sb_token);
            insert_symbol();
          }else {
            sintatic_erro(2);
            exit(-1);
          }
        } else {
            sintatic_erro(12);
          exit(-1);
        }
      }
    } else {
      sintatic_erro(2);
      exit(-1);
    }
  } else {
    sintatic_erro(10);
    exit(-1);
  }
}

int cmd(){
  //SE EXPRESSION
  if(token.type == PR && strcmp(token.pr, "se") == 0){
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      expr();
      if(token.type == SN && strcmp(token.signal,")") == 0){
        getToken();
        if(!cmd()) {
          sintatic_erro(8);
          exit(-1);
        }
        if(token.type == PR && strcmp(token.pr,"senao") == 0){
          getToken();
          if(!cmd()) {
            sintatic_erro(8);
            exit(-1);
          }
        }
        return 1;
      }else{
        sintatic_erro(3);
        exit(-1);
      }
    }else{
      sintatic_erro(4);
      exit(-1);
    }
  }
  //ENQUANTO EXPRESSION
  else if(token.type == PR && strcmp(token.pr,"enquanto") == 0){
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      expr();//add return
      if(token.type == SN && strcmp(token.signal,")") == 0){
        getToken();
        if(!cmd()) {
          sintatic_erro(8);
          exit(-1);
        }
        return 1;
      }else{
        sintatic_erro(3);
        exit(1);
      }
    }else{
      sintatic_erro(4);
      exit(1);
    }
  }
  // PARA EXPRESSION
  else if(token.type == PR && strcmp(token.pr,"para") == 0){
    getToken();
    if(token.type == SN && strcmp(token.signal,"(") == 0){
      getToken();
      atrib();
      if(token.type == SN && strcmp(token.signal,";") == 0){
        getToken();
        expr();
        if(token.type == SN && strcmp(token.signal,";") == 0){
          getToken();
          atrib();
          if(token.type == SN && strcmp(token.signal,")") == 0){
            getToken();
            if(!cmd()){

              exit(-1);
            }
            return 1;
          }else{
            sintatic_erro(3);
            exit(-1);
          }
        }else{
          sintatic_erro(1);
          exit(-1);
        }
      }else{
        sintatic_erro(1);
        exit(-1);
      }
    } else {
      sintatic_erro(4);
      exit(-1);
    }
  }
  // RETORNE EXPRESSION
  else if(token.type == PR && strcmp(token.pr,"retorne") == 0){
    getToken();
    expr();
    if(token.type == SN && strcmp(token.signal,";") == 0){
      getToken();
      return 1;
    }else{
      sintatic_erro(1);
      exit(-1);
    }
  }
  // KEY EXPRESSION
  else if(token.type == SN && strcmp(token.signal,"{") == 0){
    getToken();
    while(cmd());
    if(token.type == SN && strcmp(token.signal,"}") == 0) {
      getToken();
      return 1;
    } else {
      sintatic_erro(6);
      exit(-1);
    }
  }
  // ; EXPRESSION
  else if(token.type == SN && strcmp(token.signal,";") == 0){
    getToken();
  }
  // FUNCTION CALL
  else if(token.type == ID && strcmp(next_token.signal,"(") == 0){
    getToken();
    getToken();
    expr();
    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      expr();
    }
    if(token.type == SN && strcmp(token.signal, ")") == 0){
        getToken();
        if(token.type == SN && strcmp(token.signal, ";") == 0){
          getToken();
          return 1;
        } else {
          sintatic_erro(1);
          exit(-1);
        }
    } else {
      // printf("Sou %d %s\n", token.type, token.signal);
      sintatic_erro(4);
      exit(-1);
    }
  }
  // ATRIB EXPRESSION
  else if(atrib()){
    if(token.type == SN && strcmp(token.signal, ";") == 0) {
      getToken();
      return 1;
    } else {
      sintatic_erro(1);
      exit(-1);
    }
  }

    return 0;

}

void opc_p_types() {
  if(token.type == PR && strcmp(token.signal, "semparam") == 0) {
    getToken();
  } else if(isType()) {
    strcpy(sb_token.type, token.pr);
    getToken();
    if(token.type == ID){
      getToken();
    }
    //verify if it is declaration
    while(token.type == SN && strcmp(token.signal, ",") == 0) {
      getToken();
      //printf("Sai daqui %d\n %s\n", token.type, token.lexem.value);
      if(isType()){
        getToken();
        if(token.type == ID) {
          getToken();
        }
      } else {
        sintatic_erro(11);
        exit(-1);
      }
    }
  } else {
    sintatic_erro(10);
    exit(-1);
  }
}

void expr() {
  expr_simp();
  if(op_rel()){
      expr_simp();
  }

}

void expr_simp(){
  if(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0)){
    getToken();
  }

  termo();

  while(token.type == SN && (strcmp(token.signal,"+") == 0 || strcmp(token.signal,"-") == 0 || strcmp(token.signal,"||") == 0)){
      getToken();
      termo();
  }
}

int op_rel(){ // Não acrescenta token novo
  if(token.type == SN && (strcmp(token.signal,"==") == 0 || strcmp(token.signal,"!=") == 0 || strcmp(token.signal,"<=") == 0 || strcmp(token.signal,"<") == 0 || strcmp(token.signal,">") == 0 || strcmp(token.signal,">=") == 0)){
    getToken();
    return 1;
  }
  return 0;
}

void termo(){
  fator();
  while(token.type == SN && (strcmp(token.signal,"*") == 0 || strcmp(token.signal,"/") == 0 || strcmp(token.signal,"&&") == 0)){
      getToken();
      fator();
  }
}

void fator(){
  // FUNCTION CALL
  if(token.type == ID && next_token.type == SN && strcmp(next_token.signal,"(") == 0) {
    getToken();
    getToken();
    expr();
    while(token.type == SN && strcmp(token.signal,",") == 0){
        getToken();
        expr();
    }
    //close function
    if(token.type == SN && strcmp(token.signal,")") == 0){
      getToken();
    }else{
      sintatic_erro(3);
      exit(-1);
    }
  }
  // CONSTANTS TYPE
  else if(token.type == INTCON || token.type == REALCON || token.type == CARACCON || token.type  == CADEIACON || token.type == ID){
    getToken();
  }
  // EXPRESSION BETWEEN PARENTHESES
  else if(token.type == SN && strcmp(token.signal, "(") == 0) {
    getToken();
    expr();
    if(token.type == SN && strcmp(token.signal, ")") == 0) {
      getToken();
    } else {
      sintatic_erro(3);
      exit(-1);
    }
  }
  //NEGATION OF A FATOR
  else if(token.type == SN && strcmp(token.signal,"!") == 0){
    getToken();
    fator();
  }
}

int atrib(){
  if(token.type == ID){
    getToken();
    if(token.type == SN && strcmp(token.signal,"=") == 0){
      getToken();
      expr();
      return 1;
    }else{
      sintatic_erro(9);
      exit(-1);
    }
  }else {
    return 0;
  }
}
//-----------------------------------------------------

void insert_symbol(){
    symbol_table[cont_st] = sb_token;
    cont_st++;
}

void printf_symbol(){
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    printf("ID: %s Zombie: %d\n", symbol_table[i].name,  symbol_table[i].zumbi);
    i++;
  }
}

void insert_zombie(){
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(symbol_table[i].cat == PARAN) {
      symbol_table[i].zumbi = 1;
    }
    i++;
  }
}

void exclude_local_symbol(){
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(symbol_table[i].scope == LOCAL && !symbol_table[i].zumbi) {
      refix_array(i);
    } else {
      i++;
    }
  }
}

void refix_array(int index) {
  int i;
  for(i = index; i < (sizeof(symbol_table)/sizeof(*symbol_table)) - 1; i++){
    symbol_table[i] = symbol_table[i+1];
  }
}

void verifyRedeclaration(symbol sb) {
  int i = 0;
  while(strcmp(symbol_table[i].name, "") != 0) {
    if(symbol_table[i].scope == sb.scope && strcmp(symbol_table[i].name, sb.name) == 0 && !symbol_table[i].zumbi) {
      printf("Redeclaração de '%s' na linha %d\n", sb.name, line_number);
      exit(-1);
    }
    i++;
  }
}

void sintatic_erro(int flag){
    switch (flag){
        case 1:
            printf("';' Esperado na linha %d\n", line_number);
        break;
        case 2:
            printf("Esperado identificador na linha %d\n", line_number);
        break;
        case 3:
            printf("Esperado ')' na linha %d\n", line_number);
        break;
        case 4:
            printf("Esperado '(' na linha %d\n", line_number);
        break;
        case 5:
            printf("'{' Esperado na linha %d\n", line_number);
        break;
        case 6:
            printf("'}' Esperado na linha %d\n", line_number);
        break;
        case 7:
            printf("Esperado identificador na linha %d\n", line_number);
        break;
        case 8:
            printf("Comando esperado na linha %d\n ", line_number);
        break;
        case 9:
            printf("Esperado sinal '=' na linha %d",line_number);
        break;
        case 10:
            printf("Simbolo não identificado na linha %d\n", line_number);
        break;
        case 11:
            printf("Erro esperado tipo na linha %d\n", line_number);
        break;
        case 12:
            printf("Erro esperado ',' na linha %d\n", line_number);
        break;
        case 13:
            printf("Entrada inválida na linha %d\n", line_number);
        break;
        case 14:
            printf("Esperado fim de arquivo na linha %d\n",line_number);
        break;
    }
}
