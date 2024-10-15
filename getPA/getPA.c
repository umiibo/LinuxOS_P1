#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

// #define _DEBUG

SYSCALL_DEFINE1(my_get_physical_addresses, void  *, vaddr){

	// copy_to_user(void __user *to, const void *from, unsigned long n)
	// copy_from_user(void *to, const void *from, long n);
	unsigned long va;
    unsigned long pa;
	unsigned long pg_addr=0;
	unsigned long pg_offset=0;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	// Test input
	// #ifdef _DEBUG
		if (vaddr == NULL){
			printk("Error: Invalid vaddr\n");
			return 0;
		}
	// #endif
	va = (unsigned long)vaddr;

	// Traverse Page Table
	pgd = pgd_offset(current->mm, va);
	// #ifdef _DEBUG
		if (pgd_none(*pgd) || pgd_bad(*pgd)){
			printk("Error: Invalid pgd\n");
			return 0;
		}else{
			printk("pgd[0x%lx]: 0x%lx\n", pgd_index((unsigned long)va), pgd_val(*pgd));
		}
	// #endif

	p4d = p4d_offset(pgd, va);
	// #ifdef _DEBUG
		if (p4d_none(*p4d) || p4d_bad(*p4d)){
			printk("Error: Invalid p4d\n");
			return 0;
		}else{
			printk("p4d[0x%lx]: 0x%lx\n", p4d_index(va), p4d_val(*p4d));
		}
	// #endif

	pud = pud_offset(p4d, va);
	// #ifdef _DEBUG
		if (pud_none(*pud) || pud_bad(*pud)){
			printk("Error: Invalid pud\n");
			return 0;
		}else{
			printk("pud[0x%lx]: 0x%lx\n", pud_index(va), pud_val(*pud));
		}
	// #endif

	pmd = pmd_offset(pud, va);
	// #ifdef _DEBUG
		if (pmd_none(*pmd) || pmd_bad(*pmd)){
			printk("Error: Invalid pmd\n");
			return 0;
		}else{
			printk("pmd[%lx]: 0x%lx\n", pmd_index(va), pmd_val(*pmd));
		}
	// #endif

	pte = pte_offset_kernel(pmd, va);
	// #ifdef _DEBUG
		if (pte_none(*pte)){
			printk("Error: Invalid pte\n");
			return 0;
		}else{
			printk("pte[%lx]: 0x%lx\n", pte_index(va), pte_val(*pte));
		}
	// #endif

	// Find Physical Addr. from PTE
	// PTE has the physical frame number
	// Use page_mask
	pg_addr = pte_val(*pte) & PAGE_MASK;
	pg_offset = (unsigned long)va & ~PAGE_MASK;
	pa = pg_addr | pg_offset;

	// #ifdef _DEBUG
		printk("Page Address: 0x%lx, Page Offset: 0x%lx\n", pg_addr, pg_offset);
		printk("Virtual Address: 0x%lx\n", va);
		printk("Physical Address: 0x%lx\n", pa);
	// #endif

	return pa;

}