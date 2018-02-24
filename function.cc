#include<stdio.h>
#include "KS2KSSM.h"
#include <iconv.h>
#include <string.h>

int cho=0,jung=0,jong=0;
int pre_cho=0,pre_jung=0,pre_jong=0;
int i=0;

int cho_position;
int jung_position;
int jong_position;

int my_strlen(char* str)
{
	int idx=0;
	while(str[idx]!='\0') idx++;
	return idx;
}


int spilit_flag(char *string, int num)
{
	if(string[num]==' ' ){return 1;}
	else if(string[num]=='.' ){return 1;}
	else if(string[num]=='\0' ){return 1;}
	else if(string[num]=='\n' ){return 1;}
	else {return 0;}
}


void alphabet(int *cho, int *jung, int *jong, int johab)
{
	int temp;
	temp=johab;
	temp=temp<<17;
	temp=temp>>27;
	temp=temp&0x1F;
	*cho=temp;

	temp=johab;
	temp=temp<<22;
	temp=temp>>27;
	temp=temp&0x1F;
	*jung=temp;

	temp=johab;
	temp=temp<<27;
	temp=temp>>27;
	temp=temp&0x1F;
	*jong=temp;
}

void fraction(char *string, char *onechar,int i)
{
	onechar[0]=string[i];
	onechar[1]=string[i+1];	
	onechar[2]='\0';
}


void conversion_table(int position, int number,char *temp1,char *temp2,char *temp3)
{
	char conv_arr[3][30][5]={{{'.'},{'.'},{'g'},{'G'},{'n'},{'d'},{'D'},{'r'},{'m'},{'b'},{'B'},{'s'},{'S'},{'N'},{'z'},{'Z'},{'c'},{'k'},{'t'},{'p'},{'h'}},
	                         {{'.'},{'.'},{'.'},{'a'},{'E'},{"ja"},{"jE"},{'v'},{'.'},{'.'},{'e'},{"jv"},{"je"},{'o'},{"wa"},
		                     {"wE"},{'.'},{'.'},{'O'},{"jo"},{'u'},{"wv"},{"we"},{"wi"},{'.'},{'.'},{"ju"},{'U'},{"xi"},{'i'}},
	                         {{'.'},{'.'},{'g'},{'G'},{'#'},{'n'},{'#'},{'#'},{'d'},{'l'},{'#'},{'#'},{'#'},{'#'},{'#'},{'#'},
	                          {'#'},{'m'},{'.'},{'b'},{'#'},{'s'},{'S'},{'N'},{'z'},{'c'},{'k'},{'t'},{'p'},{'h'}}};

	char *tmp;
	if(position==1) tmp=temp1;
	else if(position==2) tmp=temp2;
	else tmp=temp3;

	int idx=0;
	while(conv_arr[position-1][number][idx]!='\0'){
		tmp[idx]=conv_arr[position-1][number][idx];
		idx++;
	}
	tmp[idx]='\0';
	return;
}


void word2phone(int *cho, int *jung, int *jong, char *cho_out, char *jung_out, char *jong_out,int *flag)
{
	conversion_table(1,*cho,cho_out,jung_out,jong_out);
	conversion_table(2,*jung,cho_out,jung_out,jong_out);
	conversion_table(3,*jong,cho_out,jung_out,jong_out);
}

int phone_class(int b,int a)
{
	int arr[2][30]={{0,0,1,1,4,1,1,5,4,1,1,2,2,4,3,3,3,1,1,1,2},{0,0,6,6,0,9,0,0,6,10,0,0,0,0,0,0,0,9,0,6,0,7,7,9,8,8,6,6,6,7}};
	return arr[b/3][a];
}

int voiced_phone_class(int a, int b)
{
	if(a==1) if(b==2 | b==5 | b==9 | b==14 | b==20) return 1;
	if(a==3) if(b==5 | b==9 | b==17 | b==23 |b==1) return 2;
	return -1;
}

int string_input(char *string, char *input_string,int start,int width)
{
	int count=0;
	int temp;
	temp=start;
	for(;start<temp+width;)
	{
		string[start]=input_string[count];
		count++;
		start++;
		string[start]=' ';
	}
	return start+1;
}

void string_shift_copy(char *string,int start,int width)
{
	char temp[100];
	int copy_count=0;
	int all;
	int init=start;
	for(;;start++)
	{
		temp[copy_count]=string[start];
		copy_count++;
		if(string[start]=='\0')
		{all=start-init+1; break;}
	}
	copy_count=0;
	for(int iter=init;iter<all+width+init;iter++)
	{ 
		if(iter<init+width)
		{
			string[iter]=' ';	
		}
		else
		{
			string[iter]=temp[copy_count];
			copy_count++;	
		}
	}
}

int insert_phone(char *printed_string, int count)
{
	int temp=0;
	//초성
	if(phone_class(1,cho)==1 | phone_class(1,cho)==2)//폐쇄음,마찰음
	{
		if(cho!=20)
		{
		string_shift_copy(printed_string,cho_position+2,2);
		printed_string[cho_position+2]='H';
		}
		if(voiced_phone_class(3,pre_jong)==2 & voiced_phone_class(1,cho)==1)//유성음화
		{
			printed_string[cho_position+1]='V';
			string_shift_copy(printed_string,cho_position+2,1);
		}
			temp=my_strlen(printed_string);
			printed_string[temp]=' ';
			return temp;
	}
	else if(phone_class(1,cho)==3)//파찰음
	{
		string_shift_copy(printed_string,cho_position+2,4);
		printed_string[cho_position+2]='F';
		printed_string[cho_position+4]='H';
		if(voiced_phone_class(3,pre_jong)==2 & voiced_phone_class(1,cho)==1)//유성음화
	    {
			printed_string[cho_position+1]='V';
			string_shift_copy(printed_string,cho_position+2,1);
	    }
		temp=my_strlen(printed_string);
		printed_string[temp]=' ';
		return temp;
	}
	else if(phone_class(3,pre_jong)==10 & phone_class(1,cho)==5)//ㄹ 설측음
	{
		printed_string[cho_position]='l';
		temp=my_strlen(printed_string);
		printed_string[temp]=' ';
		return temp;
	}
	else if(phone_class(3,pre_jong)==0 & phone_class(1,cho)==5)//ㄹ 탄설음
	{
		printed_string[cho_position]='r';
		printed_string[cho_position+1]='R';
		string_shift_copy(printed_string,cho_position+2,1);
		temp=my_strlen(printed_string);
		printed_string[temp]=' ';
		return temp;
	}	
	
	//종성
	if(phone_class(3,jong)==6 | phone_class(3,jong)==7)//폐쇄음,마찰음
	{
		string_shift_copy(printed_string,jong_position+2,2);
		printed_string[jong_position+2]='H';
		temp=my_strlen(printed_string);
		printed_string[temp]=' ';
		return temp;
	}

	else if(phone_class(3,jong)==8)//파찰음
	{
		string_shift_copy(printed_string,jong_position+2,4);
		printed_string[jong_position+2]='F';
		printed_string[jong_position+4]='H';
		temp=my_strlen(printed_string);
		printed_string[temp]=' ';
		return temp;
	}
	else if(phone_class(3,jong)==10)//ㄹ 설측음
	{
		printed_string[cho_position]='l';
		temp=my_strlen(printed_string);
		printed_string[temp]=' ';
		return temp;
	}
	else
	{return count;}

}

void UTF8toEUCKR(char *outBuf,char *inBuf)
{
	int ret;
	size_t in_size=my_strlen(inBuf);
	size_t out_size=200;
	iconv_t ic=iconv_open("EUC-KR","UTF-8");
	ret=iconv(ic,&inBuf,&in_size,&outBuf,&out_size);
	iconv_close(ic);
}



void convert(char* string,char* printed_string)
{
	int count1=0;
	char empty[5]="_";
    char silence[5]="sil";
	int johab;
	char onechar[3];
	int flag;
	wchar_t wonechar;
    char cho_out[2];
    char jung_out[3];
    char jong_out[3];
	char conv_str[200];
	UTF8toEUCKR(conv_str,string);
	for(int iter=0;iter<my_strlen(conv_str);)
	{   
		if(spilit_flag(conv_str,iter)!=1)
		{
			fraction(conv_str, onechar,iter);
			wonechar= wchar_t(onechar[0]<<8)|((wchar_t)onechar[1]&0x00FF);  
			wchar_t wansung=wonechar;
			johab = (int) KS2KSSM(wansung);
			alphabet(&cho,&jung,&jong, johab);
			if(cho==0 && jung==0 && jong==0) break;
			word2phone(&cho,&jung,&jong, cho_out, jung_out, jong_out,&flag);
			cho_position=count1;
			count1=string_input(printed_string, cho_out,count1,my_strlen(cho_out));
			
			if(cho==13) count1=cho_position;
			jung_position=count1;
			count1=string_input(printed_string, jung_out,count1,my_strlen(jung_out));
			if(jong==1){printed_string[count1]='\0';}
			else
			{
				jong_position=count1;
				count1=string_input(printed_string, jong_out,count1,my_strlen(jong_out));
				printed_string[count1]='\0';
			}
			
			count1=insert_phone(printed_string,count1);
			cho_position=0;jung_position=0;jong_position=0;
			iter=iter+2;	
		}
		else
		{
			count1=string_input(printed_string, empty,count1,my_strlen(empty));
		    iter=iter+1; 
		}	
		pre_cho=cho;
		pre_jung=jung;
		pre_jong=jong;

	}
	
	printed_string[count1-1]='\0';
}

