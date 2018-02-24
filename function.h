#ifndef  _FUNCTION_H
#define  _FUNCTION_H
void Chosung_Jungsung_jongsung(int *cho, int *jung, int *jong, int johab);
int count_string(char *string);
void fraction(char *string, char *onechar,int i);
char* conversion_table(int position, int number,int *flag,char *temp1,char *temp2,char *temp3);
void hangel_to_phoneme(int *cho, int *jung, int *jong, char *cho_out, char *jung_out, char *jong_out,int *flag);
int classfication_function(int b,int a);
int voiced_classfication_function(int a, int b);
int string_input(char *string, char *input_string,int start,int width);
void string_shift_copy(char *string,int start,int width);
int adding_function(char *printed_string, int count);
void convert(char* string,char* printed_string);
#endif