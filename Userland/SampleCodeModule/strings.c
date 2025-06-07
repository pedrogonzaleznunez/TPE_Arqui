#include <myStrings.h>
#include <stddef.h>

int strlen(const char *s) {
    int count = 0;
    
    while (*s++){
        count++;
    }
    
    return count;
}

char * strcpy(char * t, const char * s) {
    char *copy = t;

    while( *s ){
        *copy++ = *s++;
    }
    
    *copy = 0;

    return t;
}

char * strncpy(char * t, const char * s, unsigned int n) {
    char *copy = t;

    while(n--){
        *copy++ = *s;
        if (*s){
            s++;
        }
    }

    return t;
}


char * strcat(char * t, const char * s){
    char *aux = t + strlen(t);

    strcpy(aux, s);

    return t;
}


char * strncat(char * t, const char * s, int n) {
    char *aux = t + strlen(t);

    strncpy(aux, s, n);

    aux += n;
    *aux = 0;

    return t;
}


int strcmp(const char *t, const char *s){
    while( *t && *s && (*t == *s)){
        t++;
        s++;
    }
    return *t - *s;
}

int strncmp(const char * t, const char * s, unsigned int n) {
    int rta = 0;

    while( (*t || *s) && n-- && !rta){
        if( *t != *s ){
            rta = *t > *s ? 1 : -1;
        }
        if( *t ){
            t++;
        }
        if( *s ){
            s++;
        }
    }

    return rta;
}


char * strchr(const char * s, char c){
    char *rta = NULL;

    while(*s && rta == NULL){
        if(*s == c){
            rta = s;
        }
        s++;
    }

    return rta;
}


char * strrchr(const char * s, char c){
    char *rta = NULL;

    while(*s){
        if(*s == c){
            rta = s;
        }
        s++;
    }

    return rta;
}

void trim(char * str){
    char * aux = str;

    while(*aux == ' ' || *aux == '\t'){
        aux++;
    }

    int i = 0;

    while(*aux){
        if(!((*aux == ' ' || *aux == '\t') && (*(aux + 1) == ' ' || *(aux + 1) == '\t'))){
            if(*aux == '\t'){
                str[i++] = ' ';
            } else {
                str[i++] = *aux;
            }
        }
        aux++;
    }

    str[i] = 0;

    return;
}
