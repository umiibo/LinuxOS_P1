#include <stdio.h>
                
void *my_get_physical_addresses(void *vaddr)
{
  return syscall(449, vaddr);
}


int a[2000000]; 
                 
int main()
{ 
    int      loc_a;
    void     *phy_add;  

    phy_add=my_get_physical_addresses(&a[0]);
    printf("global element a[0]:\n");  
    printf("Offest of logical address:[%p]   Physical address:[%p]\n", &a[0], phy_add);              
    printf("========================================================================\n"); 
    phy_add=my_get_physical_addresses(&a[1999999]);
    printf("global element a[1999999]:\n");  
    printf("Offest of logical address:[%p]   Physical address:[%p]\n", &a[1999999], phy_add);              
    printf("========================================================================\n"); 
     
}
