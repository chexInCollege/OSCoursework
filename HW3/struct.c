enum Status {
	INITIALIZING, // 0
	RUNNING,      // 1
	SUSPENDED,    // 2
	READY,        // 3
	TERMINATED    // 4
}

struct Process {
	int pid;		// Process ID
	int instructionPointer; // Instruction Pointer
	int stackPointer;	// Stackk Pointer
	int regAX;		// Register AX
	int regBX;		// Register BX
	int status;		// Process status (RUNNING, READY, etc.)
	struct process* prev;	// Pointer to previous previous
	struct process* next;	// Pointer to next process
}
