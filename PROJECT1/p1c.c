// PROJECT 1 - Alex Groves, Andreas Dilling
// Seems like nano likes to mess with the indentation when viewed
// in certain editors. I hope it looks OK!

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


// This is how we'll handle the hierarchy of processes - each process contains two
// process pointers, one to a child process and one to its next sibling process.
struct process {
        int pid;                // Process ID
	int ppid;		// Parent Process ID
	unsigned long vsize;	// Virtual memory usage
	char comm[256];		// Process name
     	struct process* child;	// First child process pointer
        struct process* next;   // Pointer to next process
};


// This recursive function will print the entire process tree using any process
// as a starting point.
void printProcessChildren(int indent, int pid, struct process *procList[]) {
	struct process *procPointer = procList[pid]; // Pointer to drag along children list
	while(procPointer != NULL) {
		for(int i = 0; i < indent; i++) printf(" "); // indentation!

		// the interesting bit
		printf("%d %s %lu kb\n", procPointer->pid, procPointer->comm, (procPointer->vsize)/1024);

		// if the current child process has children, also output those!
		if(procPointer->child){
			printProcessChildren(indent + 2, procPointer->pid, procList);
		}

		procPointer = procPointer->next; // moving along
	}
}


// function built mostly for the purpose of being isolated from main()
// this helped with organization and debugging.
void processTree()
{
	// yes, this is ugly! realized down the road that there are much more elegant
	// solutions utilizing the tree structure by traversing it from the root.
	// due to time, however, this is the final implementation.
	// Also, we acknowledge this wouldn't work on a system with more than
	// 99,999 processes, but we just picked a reasonable number.
	struct process *procList[100000];  // procList[n] will point to the FIRST CHILD of parent with pid n.
	struct process *procIndex[100000]; // procIndex[n] will point to the process with pid n.

	// giant armies of nothing in these arrays, please
	for(int i = 1; i < 100000; i++) {
		procList[i] = NULL;
		procIndex[i] = NULL;
	}

	// the "root" isn't a real process, but it allows us to map pid 1 to it without adding edge cases
	// the contents of this struct aren't important, though.
	struct process *root = malloc(sizeof(struct process));
	procIndex[0] = root;

	// next, we open all the subdirectories of /proc/ and collect its info.
	struct dirent *de; // pointer to directory entry
	DIR *dr = opendir("/proc");

	// other variables we end up using
	int pid;			// used for the first few operationos on an entry
	char path[256]; 		// used to hold the filepath to each entry
	struct process *newProc;	// used to point to each new process before it's allocated to an array.
	char buffer[256];		// used to build up the string consisting of the process name
	char ppidStr[16];	 	// used for fetching the ppid from its file
	char vsizeStr[16];		// used for fetching the vsize from its file
	struct process *procPointer;	// used while iterating through linked lists of process siblings

	// let's loop through all directory entries
	while ((de = readdir(dr)) != NULL) {
		pid = atoi(de->d_name);	// Collect pid, what we actually want

		// if pid is nonzero, we have a process
		if(pid) {

			//We create ou4 new process in the heap, so we can play with it outside this scope
			newProc = malloc(sizeof(struct process));
			newProc->pid = pid;
			newProc->next = NULL; // OK mr segfault
			newProc->child = NULL;

			// add it to the mega indexing array (yep, still ugly):
			procIndex[pid] = newProc;

			// now, we need info from the proc/n/stat file
			snprintf(path, sizeof(path), "/proc/%d/stat", pid);
			FILE *fp = fopen(path, "r");
			fscanf(fp, "%d", &pid);

			// continue collecting character strings until we reach end of title
			strcpy(buffer, "");
			while(strlen(buffer) != 1) {
				strcat(newProc->comm, strcat(buffer, " "));
				fscanf(fp, "%s", buffer);
			}
			// gettings lots of segfault issues with scanning for %d 
			// directly, so we convert them using atoi
			strcpy(ppidStr, ""); strcpy(vsizeStr, "");
			for(int i = 0; i < 20; i++){
				// if we're on the first iteration, feed into ppidStr
				// the 20th iteration will align and find vsizeStr
				fscanf(fp, "%s", (i == 0 ? ppidStr : vsizeStr));
			}
			newProc->ppid = atoi(ppidStr);   // ppid is an int
			newProc->vsize = atol(vsizeStr); // vsize is a long

			// Here we attach the current entry to its parent both through procList
			// and to the parent directly through its child attribute. The array, again,
			// isn't really necessary if we were to just traverse the tree from root.
			if(!procList[newProc->ppid]){ // we're adding the first link to the children list for this process
				procList[newProc->ppid] = newProc;
				procIndex[newProc->ppid]->child = newProc;
			} else {
				// the first link exists already, so we'll append to the end
				procPointer = procList[newProc->ppid];
				while(procPointer->next != NULL)
					procPointer = procPointer->next;

				procPointer->next = newProc;
			}
		} // would probably de-nest this if had the time
	}
	// print pid 1 manually (kind of ugly, would have liked to bake it into the general algorithm)
	printf("%d %s %lukb\n", procIndex[1]->pid, procIndex[1]->comm, (procIndex[1]->vsize)/1024);

	// finally, call the recursive output function to visualize the tree.
	printProcessChildren(2, 1, procList);
}


int main(int argc, char *argv[])
{
	processTree(); // let's go :D
}
