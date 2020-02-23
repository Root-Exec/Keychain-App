Welcome to the Keychain tool created by Nick Bonner.

Table of Contents:

Section 1: Overview
Section 2: Set Up
Section 3: Functions

____________________________________________________
Section 1

The purpose of the keychain tool is store passwords
and the names associated with those passwords. It 
has its own style of "encryption". The tool 
automatically encrypts and decrypts the passwords and
their associated name when writing to and from the
source file. Currently, the program exists solely as 
a command line tool. Future plans are to create a GUI
and develop a full program that includes the readme,
the program itself, and the source file. 

____________________________________________________
Section 2

To use the keychain tool, the file titled "Keychain_Source.txt"
needs to be in the same file group as the program. If it is not,
when writing (saving) to the source file, the program will
create it's own "Keychain_Source.txt" file. This new source file
will not include the original passwords, only the saved 
changes because the program searches for the source file and
if it is not found, it will create a new one. 

It is recommended that you create a keychain folder that will
house this readme file, the keychain source file, and the 
program source code. 

The program only has the ability to read off of a .txt file
extension. Any other type of file to hold the names and 
associated passwords will result in undefined behavior.

______________________________________________________
Section 3

The main menu from the program will list out the available
functions that the user may use. The user selects each
function using numbers typed in by the user. The program has
the ability to create a new random password or a user defined 
password, update a password with a random password or
user defined password, deleting a user selected password,
keychain clean up, and exiting the program. 

Each password may be updated from a random password to a user
defined password or vice versa. 

At each user selected function, the program allocates memory
to ready lines from the source file using calloc(). From there,
depending on what the function purpose is, the program will 
iterate through the text file in search of the password name
that the user typed in. If no match is found, the program
loops back to the main menu selection. If a match is found,
the program will run through the function's procedures to delete 
or update the password. 

If an update or delete function is selected all of the above will
occur and an additional "temp.tmp" file will be created in order
to copy the un-updated passwords to, and then the updated password.
The program does not alter the source file in place.

Each function then frees the allocated memory, closes each file,
deletes the old source file, and then renames the temp.tmp file
to "Keychain_Source.txt". 

Once the user no longer needs the program, selecting 9 from the 
main menu screen will close out the program. 

Key Points:

1) The main function does not allocate the memory. Each function
selection that will alter the source file will allocate the 
memory that will be required to change based on the function
that is selected. The main function is only a menu selection
loop that will call each function for execution. 

2) The random password generation is not truly random. It is
still considered "pseudo-random" and not cryptographically 
secure. I would consider it secure in layman's terms and more 
of a deterrent. 

3) Searching for a password is case sensitive. The program
discerns between Amazon and amazon. 

4) Pay attention to the required password and name sizes.
The functions are bounds protected but the result may be
truncated names from what is expected. If that is the case, then
searching from what the user thinks the name is will never produce
a match. 