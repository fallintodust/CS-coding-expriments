#include <stdio.h>

int a[100005];
int n,cnt;

int sum(int x) {
    return x==1?1:x+sum(x-1);
}

int main(){
    scanf ("%d",&n);
    //for (int i=1;i<=n;++i)
    //    sum+=i;
    cnt=sum(n);
    printf ("%d\n",cnt);
    return 0;
}
