#include <bits/stdc++.h>
#define MP make_pair
#define PB emplace_back
using namespace std;
typedef long long ll;
template<typename T>
inline T read(T&x){
	x=0;int f=0;char ch=getchar();
	while (ch<'0' || ch>'9') f|=(ch=='-'),ch=getchar();
	while (ch>='0' && ch<='9') x=x*10+ch-'0',ch=getchar();
	return x=f?-x:x;
}
const int N=1e5+10;
const int INF=2000000000;
int n,i,u,v,q,k,top,dfs_clock,dfn[N],points[N],bel[N],fa[N],son[N],sz[N],dep[N];
bool isVirtual[N];
vector<int>G[N],G2[N];
void dfs(int u,int f){
	fa[u]=f,dep[u]=dep[f]+1,sz[u]=1,son[u]=-1,dfn[u]=++dfs_clock;
	for (auto &v:G[u]){
		if (v==f) continue;
		dfs(v,u);
		sz[u]+=sz[v];
		if (son[u]==-1 || sz[v]>sz[son[u]]) son[u]=v;
	}
}
void dfs2(int u,int f){
	bel[u]=f;
	if (son[u]==-1) return;
	dfs2(son[u],f);
	for (auto &v:G[u]){
		if (v==fa[u] || v==son[u]) continue;
		dfs2(v,v);
	}
}
int lca(int u,int v){
	for (;bel[u]!=bel[v];dep[bel[u]]>dep[bel[v]]?u=fa[bel[u]]:v=fa[bel[v]]);
	return dep[u]>dep[v]?v:u;
}
void addEdge(int u,int v){
	G2[u].PB(v);
	G2[v].PB(u);
}
int dp(int u,int f){
	int ans=0,tot=0;
	for (auto &v:G2[u]){
		if (v==f) continue;
		ans+=dp(v,u);
		tot+=sz[v];
	}
	if (isVirtual[u]){
		ans+=tot;
		sz[u]=1;
	}
	else{
		if (tot>1) ans++;
		if (tot==1) sz[u]=1;
		else sz[u]=0;
	}
	G2[u].clear();
	return ans;
}
inline bool cmp(const int&a,const int&b){return dfn[a]<dfn[b];}
void build(int points[],int k){
	static int stk[N];
	sort(points,points+k,cmp);

	top=0,stk[top++]=0;
	int cnt=k;
	for (int i=0;i<k;i++){
		int u=points[i],f=lca(u,stk[top-1]);
		if (f==stk[top-1]) stk[top++]=u;
		else{
			while (top-2>=0 && dep[stk[top-2]]>=dep[f]){
				addEdge(stk[top-1],stk[top-2]);
				top--;
			}
			if (f!=stk[top-1]){
				addEdge(f,stk[top-1]);
				stk[top-1]=f,points[cnt++]=f,sz[f]=0;
			}
			stk[top++]=u;
		}
	}
	for (int i=top-2;i>=0;i--) addEdge(stk[i],stk[i+1]);
	printf("%d\n",dp(stk[1],0));
	for (G2[0].clear(),i=0;i<cnt;i++) sz[i]=0;
}
int main(){
	read(n);
	for (i=1;i<n;i++){
		read(u),read(v);
		G[u].PB(v);
		G[v].PB(u);	
	}
	dfs(1,0),dfs2(1,1);
	for (read(q);q--;){
		read(k);
		for (i=0;i<k;i++){
			read(points[i]);
			isVirtual[points[i]]=1;
		}
		bool flag=0;
		for (i=0;i<k;i++){
			if (fa[points[i]]!=points[i] && isVirtual[fa[points[i]]]==1){
				flag=1;
				break;
			}
		}
		if (flag) puts("-1");
		else build(points,k);
		for (i=0;i<k;i++) isVirtual[points[i]]=0;
	}
	return 0;
}