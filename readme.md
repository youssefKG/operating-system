# **Process Management** 
## Process Defintion: 
procees is the unit of work in most systemes. Systems is consist of collection of processes execute user code all this processes may execute concurently.<br>
when a program is load to memory and aready for execution is called process.

## process state: 
a process state is current activity of that process. A process may be in one of the following states:

- **New**: the procees is beign created.<br>
- **Running**: instructions are being executed.<br>
- **waiting**: the process waiting for some event to occur(such as an I/O completion or reception signal).<br>
- **Ready**: the process is waiting to assigned to a processor.<br>
- **Terminated**: the process has finished execution.

## process control block: 
each process is reperesented is the operating system by a **process control block(PCB)**. It contains many pieces of information associated with a specific procees including these: 
- **Process State** : the state may be new, ready, running, waiting, terminated.
- **Process Counter** : the counter indicate the address of the next instruction to be executed for this process.
- **CPU Registers**: the registers vary in number and type, depending on the computer architecture. They include accumulators, index registers, stack pointers, and general-purpose registers, plus any condition-code information. Along with the program counter, this state information must be saved when an interrupt occurs, to allow the process to be continued correctly afterward.
- **CPU scheduling information**: This information includes a process priority, pointers to scheduling queues, and any other scheduling parameters.
- **Memory-management** information. This information may include such
items as the value of the base and limit registers and the page tables, or the
segment tables, depending on the memory system used by the operating
system

## threads Definition:
is the smallest set or sequence of instruction that computer or process can manage or execute.
a process can have mutiple threads

## process representation at linux: 

`
#include <linux/shced>
long state; /* state of process*/ 
long state; /* state of the process */
struct sched entity se; /* scheduling information */
struct task struct *parent; /* this process’s parent */
struct list head children; /* this process’s children */
struct files struct *files; /* list of open files */
struct mm struct *mm; /* address space of this process */
`

## shceduling queues 

- **Job Queue**: all the proceesses in the system.
- **Ready Queue*: all the processes are residing in main memory and are ready
  and waiting to execute are kept on list
- **Device queue**: the list of processes that wainting for particular I/O Device.

A process is initialy put in ready queue it waits there until it is selected for execution, one the process is allocated the cpu is executing , one of several could occur: 

- the process could issue and I/O request and thene be palced in an I/O queue.
- the process could create new child process and wait for the child's termination.
- the process could be removed from the CPU forciply as result of interput and
  be put back in ready queue

The first two cases, the process eventually swithces from the waiting state to
ready state and us the put back to the **ready queue. A process continues this
cycle until it terminates, at which time it is removed from all queues and has
its PCB and resources deallocated.

## Shedulers 
proce migrate among various scheduling queues throughout its lifetime.
### process schdulers in  batch system: 
in batch systme, more prosesses are submitted than can be executed immediately.
these processes are spooled to mass storage device (disk) where they are kept
for later execution. **the long-term-scheduler** selects processes that are
ready in storage device and loads theme to memory for execution. **The
Short-term-scheduler** or **CPU sheduler** select among from among the
processes that are ready to execute and allocate the CPU to one of them.
the **CPU sheduler (the short-term-scheduler)**  must select a process from memory and put in execution
and may execute for only few milliseconds.
the **long-term-sheduler** executes much less frequently; minutes may separate
the creation of one new processthe long term-scheduler control **the degre of
multiprogramming**(the number of process in memory )  if the **degree of
multuprogramming** is stable then average rate of process creation creation
must be equal to average departure rate of processes leaving the system. thus
the long-term shceduler may need to be invoked only when a process leaves the
system.


