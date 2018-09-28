#include <bits/stdc++.h>
#define MP make_pair
#define PB push_back
using namespace std;
typedef long long ll;
template<typename T>
inline T read(T&x){
    x=0;int f=0;char ch=getchar();
    while (ch<'0' || ch>'9') f|=(ch=='-'),ch=getchar();
    while (ch>='0' && ch<='9') x=x*10+ch-'0',ch=getchar();
    return x=f?-x:x;
}
const int SIGMA_SIZE=26;
const int N=3e5+10;
const int M=20000+10;
char s[N];
int T,n,i,j,tot,dfs_clock,st[N],ed[N],w[M],fail[N],son[N][SIGMA_SIZE];
ll tag[N<<2];
vector<int>pos[M],G[N];
queue<int>Q;
void ins(char* s,int j){
    int p=1;
    for (int i=0;s[i];++i){
        int idx=s[i]-'a';
        if (!son[p][idx]) son[p][idx]=++tot;
        p=son[p][idx];
        pos[j].PB(p);
    }
}
void getfail(){
	fail[1]=1;
	for (int i=0;i<SIGMA_SIZE;++i){
		if (!son[1][i]) son[1][i]=1;
		else{
			fail[son[1][i]]=1;
			Q.push(son[1][i]);
		}
	}
    while (!Q.empty()){
        int u=Q.front();Q.pop();
        for (int i=0;i<SIGMA_SIZE;++i){
            if (!son[u][i]) son[u][i]=son[fail[u]][i];
            else{
                fail[son[u][i]]=son[fail[u]][i];
                Q.push(son[u][i]);
            }
        }
    }
}
void dfs(int u){
    st[u]=++dfs_clock;
    for (int i=0;i<(int)G[u].size();++i){
        int v=G[u][i];
        dfs(v);
    }
    ed[u]=dfs_clock;
}
#define lson root<<1,l,mid
#define rson root<<1|1,mid+1,r
inline void up(ll&a,ll b){if(a<b)a=b;}
void build(int root,int l,int r){
    tag[root]=0;
    if (l==r) return;
    int mid=l+((r-l)>>1);
    build(lson);
    build(rson);
}
void query(int root,int l,int r,int pos,ll& dp){
    up(dp,tag[root]);
    if (l==r) return;
    int mid=l+((r-l)>>1);
    if (pos<=mid) query(lson,pos,dp);
    else query(rson,pos,dp);
}
void update(int root,int l,int r,int L,int R,ll val){
    if (L<=l && r<=R){
        up(tag[root],val);
        return;
    }
    int mid=l+((r-l)>>1);
    if (L<=mid) update(lson,L,R,val);
    if (mid<R) update(rson,L,R,val);
}
int main(){
    for (read(T);T--;){
        read(n);
        for (tot=i=1;i<=n;++i){
            scanf("%s%d",s,&w[i]);
            if (w[i]<=0) continue;
            pos[i].clear();
            ins(s,i);
        }
        getfail();
        for (i=2;i<=tot;++i) G[fail[i]].PB(i);
        dfs_clock=0,dfs(1);
        build(1,1,tot);
        ll ans=0;
        for (i=1;i<=n;++i){
            if (w[i]<=0) continue;
            ll dp=0;
            int final=0;
            for (j=0;j<(int)pos[i].size();++j){
                int v=pos[i][j];
                query(1,1,tot,st[v],dp);
                if (j+1==(int)pos[i].size()) final=pos[i][j];
            }
            up(ans,dp+=w[i]);
            update(1,1,tot,st[final],ed[final],dp);
        }
        printf("%lld\n",ans);
        for (i=1;i<=tot;++i){
            G[i].clear();
            for (j=0;j<SIGMA_SIZE;++j) son[i][j]=0;
        }
    }
    return 0;
}