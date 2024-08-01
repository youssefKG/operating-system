# **Process Management** 
## Process Defintion: 
procees is the unit of work in most systemes. Systems is consist of collection of processes execute user code all this processes may execute concurently.<br>
when a program is load to memory and aready for execution is called process.

## process state: 
a process state is current activity of that process. A process may be in one of the following states:

- **New**: the procees is beign created.<br>
- **Running**: instructions are being executed.<br>
- **waitin**: the process waiting for some event to occur(such as an I/O completion or reception signal).<br>
- **Ready**: the process is waiting to assigned to a processor.<br>
- **Terminated**: the process has finished execution.

## process control block: 
each process is reperesented is the operating system by a **process control block(PCB)**. It contains many pieces of information associated with a specific procees including these: 
- **Process State**: the state may be new, ready, running, waiting, terminated.
- **Process Counter**: the counter indicate the address of the next instruction to be executed for this process.
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

