#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

int cmp_rtcdate(struct rtcdate date1, struct rtcdate date2) {
    if (date1.year < date2.year) return -1;
    if (date1.year > date2.year) return +1;
    if (date1.month < date2.month) return -1;
    if (date1.month > date2.month) return +1;
    if (date1.day < date2.day) return -1;
    if (date1.day > date2.day) return +1;
    if (date1.hour < date2.hour) return -1;
    if (date1.hour > date2.hour) return +1;
    if (date1.minute < date2.minute) return -1;
    if (date1.minute > date2.minute) return +1;
    if (date1.second < date2.second) return -1;
    if (date1.second > date2.second) return +1;
    return 0;
}

int
sys_sleep_until(void)
{
  struct rtcdate *arrival;
  struct rtcdate current;
  if (argptr(0, (char **)&arrival, sizeof(arrival)) < 0)
    return -1;
  cmostime(&current);
  if (cmp_rtcdate(*arrival, current) < 0)
    return -1;
  acquire(&tickslock);
  while(cmp_rtcdate(*arrival, current) > 0){
    cmostime(&current);
    if(myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_getdate(void) {
  struct rtcdate *dp;
  if (argptr(0, (char **)&dp, sizeof(dp)) < 0)
    return -1;
  cmostime(dp);
  return 0;
}
