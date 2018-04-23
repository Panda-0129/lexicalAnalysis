//
//  main.c
//  lexicalAnalysis
//
//  Created by 金波 on 10/04/2018.
//  Copyright © 2018 金波. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void str_config(void);
bool isASingle(char);
//int isBSingleChar(char);
int isReservedWord(char*);
int isInteger(char);
int isChar(char);
int isSeparator(char);
int isSpace(char);
//int isComment(char);
void getToken(void);
void setTokenArray(char*, char*);
void out(void);
char* toLower(char*);

char* reservedWord [9] = {"begin", "end", "if", "then", "else","for", "do", "while", "and or not"};
char str [10240];
char token [128];
int cursorOfStr;
int cursorOfToken;

typedef struct _token {
    char str[128];
    char key[128];
}_tokenStruct;

struct _token _tokenArray[10240];

int main(int argc, const char * argv[]) {
    str_config();
    getToken();
    out();
    return 0;
}

void str_config() {
    char ch;
    int i = 0;
    FILE *fp;
    cursorOfStr = 0;
    cursorOfToken = 0;
    fp = fopen("/Users/panda_0129/Documents/Learn/编译原理/lexicalAnalysis/lexicalAnalysis/test.txt", "r+");
    while ((ch=fgetc(fp)) != EOF) {
        str[i] = ch;
        i++;
    }
    //    do {
    //        ch = getchar();
    //        str[i] = ch;
    //        i++;
    //    }while(ch!='#');
    fclose(fp);
}

void getToken() {
    
    while (str[cursorOfStr] != '\0') {
//        空格
        if(isSpace(str[cursorOfStr])){
            token[0] = str[cursorOfStr];
            setTokenArray("Space", token);
            cursorOfStr++;
        }
//        分隔符(';', '{', '}', '(', ')', '#')
        else if(isSeparator(str[cursorOfStr])) {
            token[0] = str[cursorOfStr];
            setTokenArray("Separator", token);
            cursorOfStr++;
        }
//        判断是否为单字符运算符（+，-，*，/，%，;，|, :）
        else if(isASingle(str[cursorOfStr])) {
            token[0] = str[cursorOfStr];
//            :=或:
            if(str[cursorOfStr] == ':') {
                if(str[cursorOfStr + 1] == '=') {
                    token[1] = str[cursorOfStr + 1];
                    setTokenArray("two-ch operator", token);
                    cursorOfStr+=2;
                }
                else {
                    setTokenArray("single-ch operator", token);
                    cursorOfStr++;
                }
            }
//            负数或-
            else if (str[cursorOfStr] == '-') {
                int i = 1;
                if(isInteger(str[cursorOfStr + 1])) {
                    token[i] = str[cursorOfStr + 1];
                    cursorOfStr++;
                    i++;
                    while (isInteger(str[cursorOfStr + 1]) || str[cursorOfStr + 1] == '.') {
                        token[i] = str[cursorOfStr + 1];
                        i++;
                        cursorOfStr++;
                    }
                    setTokenArray("num", token);
                    cursorOfStr++;
                } else {
                    setTokenArray("single-ch operator", token);
                    cursorOfStr++;
                }
            }
            else {
                setTokenArray("single-ch operator", token);
                cursorOfStr++;
            }
        }
//        判断该运算符是否为<，若为<，则继续判断该运算符是单字运算符还是双字运算符
        else if (str[cursorOfStr] == '<') {
//            判断是否为<=
            if(str[cursorOfStr + 1] == '=') {
                token[0] = str[cursorOfStr];
                token[1] = str[cursorOfStr + 1];
                setTokenArray("less equal\t", token);
                cursorOfStr+=2;
            }
//            判断是否为<>
            else if (str[cursorOfStr + 1] == '>') {
                token[0] = str[cursorOfStr];
                token[1] = str[cursorOfStr + 1];
                setTokenArray("11, NE", token);
                cursorOfStr+=2;
            }
            else {
                token[0] = str[cursorOfStr];
                setTokenArray("less than", token);
                cursorOfStr++;
            }
        }
//        有‘==’或’=‘两种情况
        else if(str[cursorOfStr] == '=') {
            token[0] = str[cursorOfStr];
            if(str[cursorOfStr + 1] == '='){
                token[1] = str[cursorOfStr + 1];
                setTokenArray("two-ch operator", token);
                cursorOfStr+=2;
            } else {
                setTokenArray("single-ch operator", token);
                cursorOfStr++;
            }
        }
//        ’>=‘、’>‘两种情况
        else if (str[cursorOfStr] == '>'){
            if (str[cursorOfStr + 1] == '=') {
                token[0] = str[cursorOfStr];
                token[1] = str[cursorOfStr + 1];
                setTokenArray("greater equal\t", token);
                cursorOfStr+=2;
            }
            else {
                token[0] = str[cursorOfStr];
                setTokenArray("greater than\t", token);
                cursorOfStr++;
            }
        }
//        '!'、'!='两种情况
        else if(str[cursorOfStr] == '!') {
            token[0] = str[cursorOfStr];
            if(str[cursorOfStr + 1] == '=') {
                token[1] = str[cursorOfStr + 1];
                setTokenArray("two-ch operator", token);
                cursorOfStr+=2;
            } else {
                setTokenArray("single operator", token);
                cursorOfStr++;
            }
        }
//        数字、以数字开头的字符串（错误）、小数
        else if(isInteger(str[cursorOfStr])){
//            isNum true为数字，false为错误字符串
            bool isNum = true;
            int i = 1;
            token[0] = str[cursorOfStr];
            while (isChar(str[cursorOfStr + 1]) || isInteger(str[cursorOfStr + 1]) || str[cursorOfStr + 1] == '.') {
                if(isChar(str[cursorOfStr + 1])) {
                    isNum = false;
                    token[i] = str[cursorOfStr + 1];
                    i++;
                    cursorOfStr++;
                }
                else if (isInteger(str[cursorOfStr + 1])){
                    token[i] = str[cursorOfStr + 1];
                    i++;
                    cursorOfStr++;
                }
                else if (str[cursorOfStr + 1] == '.') {
                    token[i] = str[cursorOfStr + 1];
                    i++;
                    cursorOfStr++;
                    isNum = true;
                }
            }
            cursorOfStr++;
            if(isNum) {
                setTokenArray("num", token);
            }
            else {
                setTokenArray("error", token);
            }
        }
//        标识符与保留字
        else if(isChar(str[cursorOfStr]) || str[cursorOfStr] == '_') {
            int i = 1;
            token[0] = str[cursorOfStr];
            while (str[cursorOfStr + 1] != ' ') {
                if(str[cursorOfStr + 1] == '\n' || !((isChar(str[cursorOfStr + 1])) || str[cursorOfStr + 1] == '_')) {
                    break;
                }
                token[i] = str[cursorOfStr + 1];
                i++;
                cursorOfStr++;
            }
            char temp[128];
            memset(temp, 0, sizeof(temp));
            for (int j = 0; token[j] != '\0'; j++) {
                temp[j] = token[j];
            }
            if(isReservedWord(temp)) {
                setTokenArray("reserved word\t", token);
            } else {
                setTokenArray("identifier\t", token);
            }
            cursorOfStr++;
        }
//        "字符串"
        else if (str[cursorOfStr] == '"') {
            int i = 1;
            token[0] = str[cursorOfStr];
            while (str[cursorOfStr + 1] != '"') {
                token[i] = str[cursorOfStr + 1];
                i++;
                cursorOfStr++;
            }
            token[i] = str[cursorOfStr + 1];
            cursorOfStr+=2;
            setTokenArray("string", token);
        }
//        注释
        else if(str[cursorOfStr] == '/') {
            token[0] = str[cursorOfStr];
//            单行注释 以换行符结束
            if(str[cursorOfStr + 1] == '/') {
                int i = 1;
                while (str[cursorOfStr + 1] != '\n' && str[cursorOfStr + 1] != '#') {
                    token[i] = str[cursorOfStr + 1];
                    i++;
                    cursorOfStr++;
                }
                cursorOfStr++;
                setTokenArray("comment", token);
            }
//            多行注释 /* */
            else if (str[cursorOfStr + 1] == '*') {
                int i = 1;
                while (str[cursorOfStr + 1] != '/') {
                    token[i] = str[cursorOfStr + 1];
                    i++;
                    cursorOfStr++;
                }
                token[i] = str[cursorOfStr + 1];
                cursorOfStr += 2;
                setTokenArray("comment", token);
            }
            else {
                setTokenArray("single-ch operator", token);
                cursorOfStr++;
            }
        }
        else {
            token[0] = ' ';
//            out("other", token);
            setTokenArray("other", token);
            cursorOfStr++;
        }
        memset(token, 0, sizeof(token));
    }
}

//给结构体数组赋值
void setTokenArray(char* key, char* str) {
    if(strcmp(key, "other") != 0) {
        strcpy(_tokenArray[cursorOfToken].key, key);
        strcpy(_tokenArray[cursorOfToken].str, str);
        cursorOfToken++;
    }
}

//输出结构体数组
void out() {
    FILE* fp;
    fp = fopen("/Users/panda_0129/Documents/Learn/编译原理/lexicalAnalysis/lexicalAnalysis/result.xml", "w+");
    fputs("<?xml version=\"1.0\" ?>\n", fp);
    fputs("<root>\n", fp);
//    printf("Token Type\t\t\t\t\tToken Value\n");
//    printf("---------------------------------------------\n");
    for(int i = 0; i < cursorOfToken; i++) {
//        if(strcmp(_tokenArray[i].key, "Space") == 0 || strcmp(_tokenArray[i].key, "other") == 0) {
//            continue;
//        }
//        if(strlen(_tokenArray[i].key) <= 4){
//            printf("%s\t\t\t\t\t\t\t%s\n", _tokenArray[i].key, _tokenArray[i].str);
//        } else if (strlen(_tokenArray[i].key) <= 9) {
//            printf("%s\t\t\t\t\t\t%s\n", _tokenArray[i].key, _tokenArray[i].str);
//        } else {
//            printf("%s\t\t\t\t%s\n", _tokenArray[i].key, _tokenArray[i].str);
//        }
        fputs("<Token>\n", fp);
        fputs("\t<type>", fp);
        fputs(_tokenArray[i].key, fp);
        fputs("</type>", fp);
        fputs("\n\t<string>", fp);
        fputs(_tokenArray[i].str, fp);
        fputs("</string>\n", fp);
        fputs("</Token>\n", fp);
    }
    fputs("</root>", fp);
    fclose(fp);
}

//判断是否为单字符运算符
bool isASingle(char ch) {
    if(ch == '+' || ch == '-' || ch == '*' || ch == '%' || ch == '|' || ch == ';' || ch == ':') {
        return true;
    } else {
        return false;
    }
}

//是否为字母
int isChar(char ch) {
    if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
        return true;
    }
    
    return false;
}

//是否为数字
int isInteger(char ch) {
    if(ch <= '9' && ch >= '0') {
        return true;
    }
    
    return false;
}

//分隔符
int isSeparator(char ch) {
    if(ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == '#') {
        return true;
    }
    
    return false;
}

//空格
int isSpace(char ch) {
    if(ch == ' ') {
        return true;
    }
    
    return false;
}

//保留字
int isReservedWord(char* ch) {
    for(int i = 0; i < 9; i++) {
        ch = toLower(ch);
        if(strcmp(ch, reservedWord[i]) == 0){
            return true;
        }
    }
    return false;
}

//转换小写
char* toLower(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if(str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] - 'A' + 'a';
        }
    }
    return str;
}
