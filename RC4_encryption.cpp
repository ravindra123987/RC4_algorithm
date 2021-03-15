#include<bits/stdc++.h>
using namespace std;
#define NUM 256


void char_to_ascii(string s,vector<int> &v){
	int n=s.length();
	for(int i=0;i<n;i++)
	  v[i]=int((char)s[i]);
}

string ascii_to_char(vector<int> v){
	string s="";
	int n=v.size();
	for(int i=0;i<n;i++){
		s+=char(v[i]);
	}
	return s;
}

void initialise(vector<int> key,vector<int> &main_permute,vector<int> &temp_permute){
	int keyLen=key.size();
	for(int i=0;i<NUM;i++){
		main_permute[i] = i;
		temp_permute[i] = key[i%keyLen];
	}
}

void KSA(vector<int> &main_permute,vector<int> temp_permute){
	int i,j=0,k;
	for(i=0;i<NUM;i++){
		j=(j+main_permute[i]+temp_permute[i]) % NUM;
		
		k=main_permute[i];
		main_permute[i]=main_permute[j];
		main_permute[j]=k;
	}
}

void pseudo_random_generator(vector<int> &result_key,vector<int> &main_permute){
	int i=0,j=0,it,k;
	int pt_len=result_key.size();
	
	for(it=0;it<pt_len;it++){
		i=(i+1)%NUM;
		j=(j+main_permute[i])%NUM;
		
		k=main_permute[i];
		main_permute[i]=main_permute[j];
		main_permute[j]=k;
		
		result_key[it]=main_permute[(main_permute[i]+main_permute[j])%NUM];
	}
}

vector<int> XOR(vector<int> result_key,vector<int> pt){
	int len=result_key.size();
	vector<int> ct(len);
	
	for(int i=0;i<len;i++){
		ct[i]=result_key[i]^pt[i];
	} 
	
	return ct;
}


string deciToHexaUtil(int n)
{	string s;
    char hexaDeciNum[2]={'0','0'};
    int i = 0;
    while (n != 0) {
        int temp = 0;
        temp = n % 16;
        if (temp < 10) {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] = temp + 55;
            i++;
        } 
        n = n / 16;
    }
    for (int j = 1; j >= 0; j--)
        s+=hexaDeciNum[j];
        
    return s;
}

string deciToHexa(vector<int> ct_ascii){
	string ct="";
	int n=ct_ascii.size();
	for(int i=0;i<n;i++){
		ct+=deciToHexaUtil(ct_ascii[i]);
	}
	return ct;
}

vector<int> func_encryption(string plain_text,string secret_key){
	
	vector<int> main_permute(NUM);
	vector<int> temp_permute(NUM);
	
	string processed_pt= plain_text;
	string processed_sk= secret_key;
	
	vector<int> key_ascii(processed_sk.length());
	char_to_ascii(processed_sk,key_ascii);	
		
	vector<int> pt_ascii(processed_pt.length());
	vector<int> ct_ascii;
	
	char_to_ascii(processed_pt,pt_ascii);	
	
	initialise(key_ascii,main_permute,temp_permute);
	
	KSA(main_permute,temp_permute);
	
	vector<int> result_key(processed_pt.length());
	
	pseudo_random_generator(result_key,main_permute);
	
	ct_ascii=XOR(result_key,pt_ascii);
	
	return ct_ascii;
}

vector<int> func_decryption(vector<int> ct_ascii,string secret_key){
	vector<int> main_permute(NUM);
	vector<int> temp_permute(NUM);
	
	vector<int> pt_ascii;
	
	string processed_sk= secret_key;
	
	vector<int> key_ascii(processed_sk.length());
	char_to_ascii(processed_sk,key_ascii);	
	
	
	initialise(key_ascii,main_permute,temp_permute);
	
	KSA(main_permute,temp_permute);
	
	vector<int> result_key(ct_ascii.size());
	
	pseudo_random_generator(result_key,main_permute);
	
	pt_ascii=XOR(result_key,ct_ascii);
	
	return pt_ascii;
}



int main(){
	
	string plain_text,secret_key;
	//cout<<"Enter plain text :";
	getline(cin,plain_text);
	//cout<<"Enter secret key :";
	getline(cin,secret_key);
		
	vector<int> ct_ascii;
	
	ct_ascii=func_encryption(plain_text,secret_key);		
	string ct_hexa=deciToHexa(ct_ascii);	
	cout<<ct_hexa;
	
	vector<int> pt_ascii = func_decryption(ct_ascii,secret_key);
	
	string pt_decrypted = ascii_to_char(pt_ascii);
	cout<<"\n"<<pt_decrypted;
}
