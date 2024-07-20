#include <stdio.h>
bool IsPrime(int m)
{
    int i;
    for(i = 2; i < m; i++)
    {
        if(m % i == 0)
        break;
    } 
    if(i == m)
    	return true;
    else 
    	return false;
}
void TraversaVal(int n)
{
    int i;
    for(i = 2; i < n; i++)
    {
        if(IsPrime(i))
            printf("%d\n", i);
    }
}
int main(int argc, char* argv[])
{
    int val;
    scanf("%d", &val);
    TraversaVal(val);
    return 0;
}