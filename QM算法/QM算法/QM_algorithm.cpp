#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int num_of_paras=0;
int Nq,Nm,Nd,circle=0,num=0;
string group[10][1024];
int res[100];
char *cover,*use;
bool **chart;

int hash_cal(const string &x){
	int sum=0;
	for(int i=0;i<num_of_paras;i++){
		sum=(sum+x[i])*3;
	}
	return sum;
}
string int_to_bit(int x){
	string res="                                               ";
	int count=0;
	for(int i=0;i<num_of_paras;i++){
		res[i]=((x>>(num_of_paras-1-i))  &  0x1);
		if(res[i]==1){
			count++;
		}
	}
	res[num_of_paras]=0; // -个数
	res[num_of_paras+1]=count; // 1个数
	res[num_of_paras+2]=0; // 是否匹配过
	res[num_of_paras+3]='\0'; // 字符串结束
	return res;
}

bool match_process(string& a,string& b,string& c){
	int ins_flag=-1;
	for(int i=0;i<num_of_paras;i++){
		if(a[i]!=b[i]){
			if(ins_flag!=-1){
				return false;
			}
			else{
				ins_flag=i;
			}
		}
	}
	if(ins_flag!=-1){
		c=a;
		c[ins_flag]=3;
		c[num_of_paras+2]=0;
		a[num_of_paras+2]=1;
		b[num_of_paras+2]=1;
		return true;
	}
	return false;
}

bool findMatch(string a,string b){
	for(int i=0;i<num_of_paras;i++){
		if(a[i]!=b[i]){
			if(a[i]==3){
				continue;
			}
			else{
				return false;
			}
		}
	}
	return true;
}
bool check(){
	for(int i=0;i<Nm;i++){
		if(cover[i]>0)
			return false;
	}
	return true;
}
void translate(string a,string &b){
	int j=0;
	for(int i=0;i<num_of_paras;i++){
		if(a[i]==3){
			continue;
		}
		else{
			if(a[i]==0){
				b[j++]='A'+i;
				b[j++]=39;
			}
			if(a[i]==1)
				b[j++]='A'+i;
		}
	}
	b[j]=0;
	b=b.substr(0,j);
}
bool cmp(string a,string b){
	int i=0;
	while(a[i]==b[i]){
		i++;
	}
	if(a[i]==39){
		return false;
	}
	else if(b[i]==39){
		return true;
	}
	else{
		return a[i]<b[i];
	}
}

string QM(int Nm,int* M,int Nd,int* D){
	string res_express[100];
	int res_count=0;
	int i,j,count;
	for(i=0;i<Nm;i++){
		group[0][i]=new char[14];
		group[0][i]=int_to_bit(M[i]);
	}
	for(j=0;j<Nd;j++){
		group[0][i+j]=new char[14];
		group[0][i+j]=int_to_bit(D[j]);
	}
	count=i+j;
	circle=0;
	while(circle<num_of_paras-1){
		num=0;
		circle++;
		int hash[1024],hash_count=0;
		for(i=0;i<count;i++){
			int temp_flag;
			for(j=i+1;j<count;j++){
				temp_flag=0;
				string temp="                                  ";
				if(match_process(group[circle-1][i],group[circle-1][j],temp)){
					for(int p=0;p<hash_count;p++){
						if(hash_cal(temp)==hash[p]){
							temp_flag=1;
							break;
						}
					}
					if(temp_flag==0){
						group[circle][num]=temp;
						hash[hash_count++]=hash_cal(group[circle][num]);
						num++;
					}
					else{
						continue;
					}
				}
			}
			if(group[circle-1][i][num_of_paras+2]==0){
				group[circle][num]=group[circle-1][i];
				num++;
			}
		}
		count=num;
	}
	chart=new bool *[Nm];
	cover=new char[Nm];
	use=new char[num];
	for(i=0;i<num;i++){
		use[i]=0;
	}
	for(i=0;i<Nm;i++){
			chart[i]=new bool[num];
	}
	for(i=0;i<Nm;i++){
		cover[i]=1;
		int flag=0;
		int temp;
		for(j=0;j<num;j++){
			chart[i][j]=findMatch(group[circle][j],group[0][i]);
			if(chart[i][j]==true){
				flag++;
				temp=j;
			}
		}
		if(flag==1&&use[temp]!=1){
			//本质本原
			use[temp]=1;
			res_express[res_count]="         ";
			res[res_count]=temp;
			translate(group[circle][temp],res_express[res_count]);
			//cout<<res[res_count].substr(0,num_of_paras);
			res_count++;
		}
	}
	for(i=0;i<res_count;i++){
		for(j=0;j<Nm;j++){
			if(chart[j][res[i]])
				cover[j]=-1;
		}
	}
	int temp=res_count;
	sort(res_express,res_express+temp,cmp);
	int t_max;
	int max_index;
	while(!check()){
		t_max=0;
		for(i=0;i<num;i++){
			if(use[i]!=0)
				continue;
			int ability=0;
			for(j=0;j<Nm;j++){
				if(cover[j]<=0)
					continue;
				else{
					if(chart[j][i])
						ability++;
				}
			}
			if(ability>t_max){
				t_max=ability;
				max_index=i;
			}
		}
		use[max_index]=1;
		string temp="                      ";
		translate(group[circle][max_index],temp);
		res_express[res_count++]=temp;
		for(i=0;i<Nm;i++){
			if(chart[i][max_index]){
				cover[i]=0;
			}
		}
	}
	sort(res_express+temp,res_express+res_count,cmp);
	string return_string="";
	for(i=0;i<res_count;i++){
		return_string+=res_express[i]+"+";
	}
	if(return_string[return_string.length()-1]=='+'){
		return_string[return_string.length()-1]=0;
	}
	return return_string;
}
int main(void){
	int max=0;
	int st=1;
	cin>>Nq;
	if(Nq==0){
		cout<<"0"<<endl;
		return 0;
	}
	string *result=new string[Nq];
	for(int q=0;q<Nq;q++){
		max=0;
		st=1;
		num_of_paras=0;
		cin>>Nm>>Nd;
		int *M=new int[Nm];
		int *D=new int[Nd];
		for(int m=0;m<Nm;m++){
			cin>>M[m];
			if(M[m]>max)
				max=M[m];
		}
		for(int d=0;d<Nd;d++){
			cin>>D[d];
			if(D[d]>max)
				max=D[d];
		}
		while(st<=max){
			st*=2;
			num_of_paras++;
		}
		result[q]=QM(Nm,M,Nd,D);
	}
	cout<<"于海粟"<<"	"<<"2015010901";
	for(int  i=0;i<Nq;i++){
		cout<<result[i]<<endl;
	}
}