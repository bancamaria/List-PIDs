#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>


struct myProcess {			// info despre proces
	pid_t pid;				// id-ul procesului this
	pid_t* child_pid;		// lista cu pid-urile copiilor
	size_t childNumber;		// numarul de procese copil
	uid_t IDuser;			// id-ul userului apelant
	sigset_t signalSyscall; // stopping signal
	size_t openFilesNumber; // numarul de fisiere deschise de this

	struct dataMemory {
		segsz_t textSize, dataSize, stackSize;	// dimensiunea text segment, data segment, stack
		caddr_t textAddress, dataAddress;		// adresa text segment, adresa data segment	
	} mem;
};


void print_process(struct myProcess *proc) {	// afisari
	printf("Process ID: %d\n", proc->pid);
	printf("Children: ");

	if (proc->childNumber > 0) {	
		int i;
		for (i = 0; i < proc->childNumber; i++) 
			printf("%d ", proc->child_pid[it]);
	} 
	else printf("No children");

	printf("\n");
	printf("Called by user: %d\n", proc->IDuser);
	printf("Interrupting signal: %d\n", proc->signalSyscall);
	printf("Number of opened files: %zu\n", proc->openFilesNumber);
	printf("\nMemory\n");
	printf("Text segment size: %d\n", proc->mem.textSize);
	printf("Text segment virtual address: %p\n", proc->mem.textAddress);
	printf("Data segment size: %d\n", proc->mem.dataSize);
	printf("Data segment virtual address: %p\n", proc->mem.dataAddress);
	printf("Stack size: %d\n", proc->mem.stackSize);
	printf("\n\n\n");
}

struct myProcess* load_process(pid_t pid, struct myProcess *proc) {
	proc = (struct myProcess*) malloc(sizeof(struct myProcess));	// aloc dinamic struct-ul
	int return_signal = syscall(332, pid, proc, 0);					// apelez syscall-ul din syscalls.master
	if (return_signal < 0) {										// error checking
		printf("The process with PID %d could not be found", pid);
		free(proc);
		return NULL;
	}
	proc->child_pid = malloc(sizeof(pid_t) * return_signal);		// aloc dinamic copiii-procese
	syscall(332, pid, proc, 1);										// apelez syscall-ul din syscalls.master
	return proc;
}
	
void DFS(pid_t pid) {								// parcurgerea DFS a descendentilor unui proces
	struct myProcess *proc;
	proc = load_process(pid, proc);			
	if (proc == NULL)								// error checking
		return;
	print_process(proc);
	int i = 0;
	for (i = 0; i < proc->childNumber; i++)			// copiii procesului
		DFS(proc->child_pid[i]);
	free(proc->child_pid);
	free(proc);
}

int main(int argc, char *argv[]) {
	if (argc > 1) 
		DFS(atoi(argv[1]));

	return 0;
}
