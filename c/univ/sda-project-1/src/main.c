#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>

#include "Terms.h"
#include "AutoComplete.h"


#define MAXQUERYSIZE 128


static void interactiveMode(TermArray *ta, size_t k) {
	char query[MAXQUERYSIZE] = {0};
	char *results[k];
	int len = 0;
	int query_row = 5;
	int ch;
	size_t nbselectedterms;
	double cputime = 0.0;
	clock_t start, end;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE); // Capture backspace and arrow keys

	start = clock();
	AC *ac = acCreate(ta);
	end = clock();
	mvprintw(2, 2, "Preprocessing done in %.6fs.", (double)(end - start) / (double)CLOCKS_PER_SEC);

	while (1)
	{
		// prompt
		move(query_row, 0);
		clrtoeol();
		mvprintw(query_row, 2, "Query: %s", query);

		// run autocomplete
		start = clock();
		nbselectedterms = acComplete(ac, query, k, results);
		end = clock();
		cputime = (double)(end - start) / (double)CLOCKS_PER_SEC;

		// show the terms
		for (size_t i = 1; i <= k; i++)
		{
			move(query_row + i, 0);
			clrtoeol();
			if (i <= nbselectedterms)
				mvprintw(query_row + i, 2, "%s", results[i - 1]);
			else
				mvprintw(query_row + i, 2, "-");
		}
		mvprintw(query_row + k + 2, 2, "CPU time: %.6f", cputime);

		move(query_row, 2 + 7 + len);

		// capture inputs from keyboard
		refresh();
		ch = getch();

		if (ch == '\n')
		{
			break; // Exit on Enter
		}
		else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8)
		{
			if (len > 0)
			{
				query[--len] = '\0';
			}
		}
		else if (isprint(ch) && len < MAXQUERYSIZE - 1)
		{
			query[len++] = (char)ch;
			query[len] = '\0';
		}

		refresh();
	}

	acFree(ac);

	endwin();
}

static void offlineMode(TermArray *ta, size_t k, char *query)
{
	clock_t start, end;

	printf("Preprocessing...");
	start = clock();
	AC *ac = acCreate(ta);
	end = clock();
	printf("done (in %f).\n", (double)(end - start) / (double)CLOCKS_PER_SEC);

	char *results[k];
	start = clock();
	size_t nbselectedterms = acComplete(ac, query, k, results);
	end = clock();
	double cputime = (double)(end - start) / (double)CLOCKS_PER_SEC;

	printf("Query: %s\n", query);
	printf("%ld (/%ld) Selected terms (in %.6fs):\n", nbselectedterms, k, cputime);
	for (size_t i = 0; i < nbselectedterms; i++)
		printf("%s\n", results[i]);

	acFree(ac);
}

int main(int argc, char *argv[])
{

	bool interactive = false;
	char *filename = "";
	char *query = "";
	size_t k = 10;

	if (argc < 4)
	{
		fprintf(stderr, "Usage: %s [-i] file k [query]\n"
						"  - -i: interactive mode\n"
						"  - file: the name of the file containing the terms\n"
						"  - k: the number of results to display (optional)\n"
						"  - query: the query\n  "
						" display the k terms of higher weights that match the query\n",
				argv[0]);
		exit(0);
	}

	srand(42);

	if (strcmp(argv[1], "-i") == 0)
	{
		interactive = true;
		filename = argv[2];
		k = atol(argv[3]);
	}
	else
	{
		interactive = false;
		filename = argv[1];
		k = atol(argv[2]);
		query = argv[3];
	}

	TermArray *termarray = termsLoadFile(filename);
	printf("Read %ld terms from file %s.\n", termarray->length, filename);

	if (interactive)
		interactiveMode(termarray, k);
	else
		offlineMode(termarray, k, query);

	termsFree(termarray);
}
