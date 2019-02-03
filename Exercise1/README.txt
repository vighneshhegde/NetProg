README FOR EXERCISE 1, NETWORK PROGRAMMING.
02/02/2019
VIGHNESH HEGDE
2015B4A70342P

Default arguments  
n = number of child processes of the parent = 3 
k = number of child processes of each child = 4
l = threshold of termination				= 5
m = number of reptitions of step (b)		= 2


The parent first opens a file for appending.
Then it creates n processes which create k processes each.
All these processes have the file open for writing.
They write their pid on to the file and pause.
Finally, the parent writes its own pid and sends a signal to the processgroup to wake them all up.
The parent executes "wc" to check if all child processes have written. 
When resumed, each processs selects a random line to read from the file and chooses a random number to send signal. 
All signals are handled by the same handler.
The handler for SIGUSR1 is changed at this point.
The signal numbers are used to catch all signals because the signal name to number mapping is architecture dependent.
Assuming SIGUSR1 is mapped to 12.
If the number of signals received by a process is less than the threshold number, it repeats the sending of signals. NOT MORE THAN.
Parent terminates if all children terminate or if it receives signals beyond the threshold.
In the latter case the children become orphaned but continue to execute.
This explains why sometimes the processes are still printing even after the bash prompt appears.
Sometimes all the printf statements don't print to the console for unknon reasons.
