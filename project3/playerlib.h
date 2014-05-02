#ifndef PLAYER_H
#define PLAYER_H

/** playFile(int fh)
*	Uses *exclusively* low level operations to read and write from the given file handle and write the output to stdout. 
*	Allowing it to be passed a socket or file handle.
*	@param handle The handle to the file that needs to be displayed.
*	@returns 1 on success, 0 on error, and sets errno appropriately.
**/
int playFile(int handle);


#endif