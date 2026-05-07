/*
 *	Get the pathname of the non-colorized command
 *	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *	@(#)	[MB] cr_which.c	Version 1.1 du 26/05/07 - 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define	X		fprintf(stderr, "%s(%d)\n", __FILE__, __LINE__);

/******************************************************************************

					MAIN

******************************************************************************/
int main(int argc, char *argv[])
{
	int				 _retcode = 0, _i, _found = 0;
	char				*_path_var = "PATH", *_path_val,
					*_canonical_path, *_previous_path = NULL,
					*_delim = ":", *_dir, _pathname[256], *_cmd;
	struct stat		 _stat[2];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s command\n", argv[0]);
		exit(1);
	}

	// Get PATH variable from environment
	// (strdup() is used in order not to modify the PATH variable in environment)
	if ((_path_val = strdup(getenv(_path_var))) == NULL) {
		fprintf(stderr, "%s: PATH variable not found in the environment !\n", argv[0]);
		exit(1);
	}

	_cmd		= argv[1];
	for (_dir = strtok(_path_val, _delim); _dir != NULL; _i++) {
		sprintf(_pathname, "%s/%s", _dir, _cmd);
		// fprintf(stderr, "pathname           = [%s]\n", _pathname);
		_canonical_path	= realpath(_pathname, NULL);

		if (access(_canonical_path, F_OK) < 0) {
			// fprintf(stderr, "     [%s] does not exist\n", _pathname);
		}
		else {
			// fprintf(stderr, "canonical pathname = [%s]\n", _canonical_path);
			if (access(_canonical_path, X_OK) == 0) {
				// fprintf(stderr, " ==> [%s] is executable\n", _canonical_path);
				if (stat(_canonical_path, &_stat[_found]) < 0) {
					// fprintf(stderr, "Cannot stat \"%s\" !\n", _canonical_path);
				}
				else {
					// fprintf(stderr, "_found = %d\n", _found);
					if (_found > 0) {
						if ((_stat[0].st_dev == _stat[1].st_dev)
						&&  (_stat[0].st_ino == _stat[1].st_ino)) {
							// The second executable is the same as the first
							// fprintf(stderr, " --> Same executable as the previous one\n");
						}
						else {
							// Second executable found
							// fprintf(stderr, " ==> Second executable found\n");
							// fprintf(stderr, " ==> [%s] is executable\n", _canonical_path);
							if (_previous_path != NULL) {
								free(_previous_path);
							}
							_previous_path		= strdup(_canonical_path);
							break;
						}
					}
					else {
						// First executable found
						_found++;
						if (_previous_path != NULL) {
							free(_previous_path);
						}
						_previous_path		= strdup(_canonical_path);
					}
				}
			}
			else {
				// fprintf(stderr, "[%s] is NOT executable\n", _canonical_path);
			}
		}
		_dir				= strtok(NULL, _delim);
	}

	if (_found) {
		printf("%s\n", _previous_path);
	}
	else {
		fprintf(stderr, "%s: \"%s\" : command not found !\n", argv[0], _cmd);
		exit(1);
	}

	return 0;
}
