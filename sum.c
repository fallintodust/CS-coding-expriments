#include <stdio.h>

int sum(int n){
    int cnt=0;
    for (int i=1;i<=n;++i)//n=65536超出限制
        cnt+=i;

    return cnt;
}

int main(){
    int n;
   int ret= scanf ("%d",&n);
    printf ("%d\n",sum(n));
    return 0;
}
