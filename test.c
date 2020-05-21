#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc > 1) {							// daca nu sunt suficiente argumente
		int childNumber = atoi(argv[1]);	// initializez numarul primului copil
		int i = 0;
		pid_t pid = fork();					// fac primul proces-copil

		if (pid < 0)						// eroare
			return 1;

		else if (pid == 0) {				// daca e primul proces
			printf("%d\n", getpid());		// afiseaza id-ul procesului

			for (i = 0; i < childNumber; i++) {	 // pentru fiecare copil
				pid_t pid = fork();				// creeaza proces-copil
				if (pid < 0)
					return 1;
				else if (pid == 0) {
					sleep(30);					// suspenda procesul apelat pentru cel putin 30s 
					return 0;
				}
				else {
					// parent time
				}
			}

			for (i = 0; i < childNumber; i++)	// pentru fiecare copil asteapta parintele
				wait(NULL);
			sleep(10);							// suspenda procesul apelat pentru cel putin 10s 
			return 0;
		}
	}
	else {
		// parent time
		}
	return 0;
}
