#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"


uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL

//report which pages have been accessed

int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  //nick
  
  //starting virtual address of first user page
  uint64 buf;
  //number of pages to check
  int num;
  //store result into bitmask
  //bitmask is stored in a user address to a buffer
  uint64 abits;
  
  pagetable_t pagetable = myproc()->pagetable;
  pte_t *pte;
  uint64 clear_access_mask = PTE_A^0xffffffff;
  //printf("mask: %x\n", clear_access_mask);

  //copy from user space to kernel
  argaddr(0, (uint64 *) &buf); //uint *
  argint(1, &num); //int *
  //printf("num: %d\n", num);
  argaddr(2, (uint64 *) &abits); //uint **
  //printf("abits: %d\n", abits);
  //printf("kernel: buf: %d; num: %d; abits: %d\n", buf, num, abits); //debug


  //limit: 64 pages to check
  if (num > 64) {
    return -1;
  }

  uint64 temp = 0;//temporary buffer for storinng output bitmask
  
  uint64 accessed = 0;
  //printf("outside loop, num: %d\n", num);
  for (int i = 0; i < num; i++) {
    //printf("loop: %d\n", i);
    //check every page's access
    pte = walk(pagetable, (uint64) buf + i*PGSIZE, 0); //buf + i*4 is the va of next pte
    //check access bit and record
    accessed = (PTE_A & *pte)>>6; //check
    //printf("accessed: %x\n", *pte);
    temp = temp + (accessed << i); //record
    //clear access bit
    //printf("pte before clear: %x;", *pte);
    //printf("pte after clear: %x\n", *pte&clear_access_mask);
    *pte = *pte&clear_access_mask;
  }

  //printf("abits set correctly: %d\n", accessed == ((1 << 1) | (1 << 2) | (1 << 30))); //debug
  //printf("abits is: %x\n", temp);
  //printf("abits: %d; temp: %d\n", abits, temp);
  //copy data out to user buffer
  //example: p->pagetable, addr, (char *)&st, sizeof(st) //addr is user addr
  copyout(pagetable, (uint64) abits, (char *) &temp, 8);
  //printf("copied out\n");
  
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
