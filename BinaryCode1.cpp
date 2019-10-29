#include<bits/stdc++.h>
using namespace std;
const int maxn=4e6+10;
vector<int> G[maxn];
//loc[node]存储替代问号以后,字符串终结点在node上的字符串序号 
vector<int> loc[maxn];
int sumn,n;
int no[maxn],lowlink[maxn],sccno[maxn],dfs_block,scc_cnt;
int wh[maxn],pos[maxn];
int ch[maxn][2],father[maxn],tot;
string str[maxn];
stack<int> S;
//加边u->v,即若u则必须v 
void add_clause(int u,int v){
	G[u].push_back(v);
}
void link(int u,int v){
	add_clause(u,v);
	add_clause(v^1,u^1);  //逆否命题 
}
//tarjan算法中深度优先遍历部分 
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
	//sccno[x]表示x属于的强联通分量号 
	for (int i=1;i<=n;i++)
		if (sccno[i<<1]==sccno[i<<1|1]) {
			puts("NO");
			return;
		}
	puts("YES");
	for (int i=1;i<=n;i++) {
		//wh[i]表示字符串i中问号出现的位置,值为-1则未出现 
		if (wh[i]!=-1)
			str[i][wh[i]]=(sccno[i<<1]<sccno[i<<1|1]?'0':'1');
		puts(str[i].c_str());
	}
}
//利用字典树插入，返回各替代问号以后字符串终结点在树中序号 
int trie_insert(const char *s) {
	int u=0;
	for (int i=0;s[i];i++) {
		int c=s[i]^48;  //根据s[i]为'0'或'1'获取c值0或1
		if (!ch[u][c]) {
			ch[u][c]=++tot;
			father[tot]=u;
		}	
		u=ch[u][c];
	}
	return u;  
}
//处理输入 
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
		//第一种情况:字符串中没有问号(也抽象成一个用0一个用1,但是终结点还是对应原字符串插入终结位置) 
		if (whpos==-1) {
			pos[i<<1]=pos[i<<1|1]=trie_insert(str[i].c_str());
			add_clause(i<<1|1,i<<1);
		}
		//第二种情况:字符串中有问号 
		else {
			//用0替代 
			str[i][whpos]='0';
			pos[i<<1]=trie_insert(str[i].c_str());
			//用1替代
			str[i][whpos]='1';
			pos[i<<1|1]=trie_insert(str[i].c_str()); 
		}
		wh[i]=whpos;
	}
}
//结束输入处理后进行加边操作,返回总结点数 
int deal_edge(){
	int s=(n<<1|1)+1;
	//树上所有点node,若从根到father[node]上选1个替代字符串点，则从根到node选1个替代字符串点 
	for (int node=1;node<=tot;node++)
		link(s+(father[node]<<1),s+(node<<1));
	for (int i=1;i<=n;i++) {
		int tnode0=pos[i<<1];
		int tnode1=pos[i<<1|1];
		if (wh[i]==-1) {
			loc[tnode0].push_back(i<<1);   
			link(i<<1,s+(father[tnode0]<<1|1));   //字符串i用0替代问号,root~father[tnode0]上不能再有替代字符串点 
			link(i<<1,s+(tnode0<<1));  //字符串i用0替代问号,root~tnode0上有1个替代字符串点
		}else{
			loc[tnode0].push_back(i<<1);   
			link(i<<1,s+(father[tnode0]<<1|1));   
			link(i<<1,s+(tnode0<<1));  
			loc[tnode1].push_back(i<<1|1);     //用1替代,情况类似 
			link(i<<1|1,s+(father[tnode1]<<1|1));  
			link(i<<1|1,s+(tnode1<<1));  
		}
	}
	s+=(tot<<1|1)+1;
	for (int node=1;node<=tot;node++) {
		//noden存储node上替代字符串点的数目,显然根据题意只能从里面选1个 
		int noden=loc[node].size();
		//node上没有替代字符串点,不处理 
		if (!noden)  
			continue;
		for (int i=0;i<noden;i++){
			if (i) {
				link(s+(i-1<<1),s+(i<<1));  //这个node上前i-1个里选了1个,则前i个里选1个 
				link(loc[node][i],s+((i-1<<1)|1));  //这个node上选了第i个点,前i-1个里面不能再选 
			}
			link(loc[node][i],s+(i<<1));   //这个node上选了第i个点,则前i个里选1个
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
