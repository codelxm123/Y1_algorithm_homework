#include<bits/stdc++.h>
using namespace std;
const int maxn=4e6+10;
vector<int> G[maxn];
//loc[node]�洢����ʺ��Ժ�,�ַ����ս����node�ϵ��ַ������ 
vector<int> loc[maxn];
int sumn,n;
int no[maxn],lowlink[maxn],sccno[maxn],dfs_block,scc_cnt;
int wh[maxn],pos[maxn];
int ch[maxn][2],father[maxn],tot;
string str[maxn];
stack<int> S;
//�ӱ�u->v,����u�����v 
void add_clause(int u,int v){
	G[u].push_back(v);
}
void link(int u,int v){
	add_clause(u,v);
	add_clause(v^1,u^1);  //������� 
}
//tarjan�㷨��������ȱ������� 
void dfs(int u) {
	no[u]=lowlink[u]=++dfs_block;
	S.push(u);
	for (int i=0;i<G[u].size();i++) {
		int v=G[u][i];
		if (!no[v]) {
			dfs(v);
			lowlink[u]=min(lowlink[u],lowlink[v]);
		} else if (!sccno[v]) {
			lowlink[u]=min(lowlink[u],no[v]);	
		}
	}
	if (lowlink[u]==no[u]) {
		scc_cnt++;
		for (;;) {
			int x=S.top();
			S.pop();
			sccno[x]=scc_cnt;
			if (x==u)
				break;
		}
	}
}
void two_sat_solve(int sumn,int n) {
	for (int i=0;i<=sumn;i++) {
		if (!no[i])
			dfs(i);
	}
	//sccno[x]��ʾx���ڵ�ǿ��ͨ������ 
	for (int i=1;i<=n;i++)
		if (sccno[i<<1]==sccno[i<<1|1]) {
			puts("NO");
			return;
		}
	puts("YES");
	for (int i=1;i<=n;i++) {
		//wh[i]��ʾ�ַ���i���ʺų��ֵ�λ��,ֵΪ-1��δ���� 
		if (wh[i]!=-1)
			str[i][wh[i]]=(sccno[i<<1]<sccno[i<<1|1]?'0':'1');
		puts(str[i].c_str());
	}
}
//�����ֵ������룬���ظ�����ʺ��Ժ��ַ����ս����������� 
int trie_insert(const char *s) {
	int u=0;
	for (int i=0;s[i];i++) {
		int c=s[i]^48;  //����s[i]Ϊ'0'��'1'��ȡcֵ0��1
		if (!ch[u][c]) {
			ch[u][c]=++tot;
			father[tot]=u;
		}	
		u=ch[u][c];
	}
	return u;  
}
//�������� 
void dealInput(){
	freopen("binary.in","r",stdin);
	freopen("binary.out","w",stdout);
	cin>>n;
	for (int i=1;i<=n;i++){
		cin>>str[i];
		int whpos=-1;
		int sz=str[i].size();
		for (int j=0;j<sz;j++)
			if (str[i][j]=='?'){
				whpos=j;
				break;
			}
		//��һ�����:�ַ�����û���ʺ�(Ҳ�����һ����0һ����1,�����ս�㻹�Ƕ�Ӧԭ�ַ��������ս�λ��) 
		if (whpos==-1) {
			pos[i<<1]=pos[i<<1|1]=trie_insert(str[i].c_str());
			add_clause(i<<1|1,i<<1);
		}
		//�ڶ������:�ַ��������ʺ� 
		else {
			//��0��� 
			str[i][whpos]='0';
			pos[i<<1]=trie_insert(str[i].c_str());
			//��1���
			str[i][whpos]='1';
			pos[i<<1|1]=trie_insert(str[i].c_str()); 
		}
		wh[i]=whpos;
	}
}
//�������봦�����мӱ߲���,�����ܽ���� 
int deal_edge(){
	int s=(n<<1|1)+1;
	//�������е�node,���Ӹ���father[node]��ѡ1������ַ����㣬��Ӹ���nodeѡ1������ַ����� 
	for (int node=1;node<=tot;node++)
		link(s+(father[node]<<1),s+(node<<1));
	for (int i=1;i<=n;i++) {
		int tnode0=pos[i<<1];
		int tnode1=pos[i<<1|1];
		if (wh[i]==-1) {
			loc[tnode0].push_back(i<<1);   
			link(i<<1,s+(father[tnode0]<<1|1));   //�ַ���i��0����ʺ�,root~father[tnode0]�ϲ�����������ַ����� 
			link(i<<1,s+(tnode0<<1));  //�ַ���i��0����ʺ�,root~tnode0����1������ַ�����
		}else{
			loc[tnode0].push_back(i<<1);   
			link(i<<1,s+(father[tnode0]<<1|1));   
			link(i<<1,s+(tnode0<<1));  
			loc[tnode1].push_back(i<<1|1);     //��1���,������� 
			link(i<<1|1,s+(father[tnode1]<<1|1));  
			link(i<<1|1,s+(tnode1<<1));  
		}
	}
	s+=(tot<<1|1)+1;
	for (int node=1;node<=tot;node++) {
		//noden�洢node������ַ��������Ŀ,��Ȼ��������ֻ�ܴ�����ѡ1�� 
		int noden=loc[node].size();
		//node��û������ַ�����,������ 
		if (!noden)  
			continue;
		for (int i=0;i<noden;i++){
			if (i) {
				link(s+(i-1<<1),s+(i<<1));  //���node��ǰi-1����ѡ��1��,��ǰi����ѡ1�� 
				link(loc[node][i],s+((i-1<<1)|1));  //���node��ѡ�˵�i����,ǰi-1�����治����ѡ 
			}
			link(loc[node][i],s+(i<<1));   //���node��ѡ�˵�i����,��ǰi����ѡ1��
		}
		s+=noden<<1;
	}
	return s;
}
int main() {
	dealInput(); 
	int sumn=deal_edge();
	two_sat_solve(sumn,n);
	return 0;
}
