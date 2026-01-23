//#define _BSD_SOURCE     // getpass()
#define _XOPEN_SOURCE   // crypt()

#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include "../tlpi_lib/tlpi_hdr.h"

int main(int args, char* argv[])
{
    char *username, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd* spwd;
    Boolean authOk;
    size_t len;
    long lnmax;

    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if(lnmax == -1)     // If limit is infetrminant
        lnmax = 256;    // make a guess
    
    username = malloc(lnmax);
    if(username == NULL)
        errExit("malloc");
    
    printf("Username: ");
    fflush(stdout);
    if(fgets(username, lnmax, stdin)== NULL)
        exit(EXIT_FAILURE);     // Exit on EOF
    
    len = strlen(username);
    if(username[len-1]== '\n')
        username[len-1] = '\0';
    
    pwd = getpwnam(username);
    if(pwd == NULL)
        fatal("Couldn't get passwd record");
    spwd = getspnam(username);
    if(spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");
    
    if(spwd != NULL)
        pwd->pw_passwd = spwd -> sp_pwdp;
    
    password = getpass("Password: ");

    encrypted = crypt(password, pwd->pw_passwd);
    for(p = password; *p != '\0';)
        *p++ = '\0';
    if(encrypted == NULL)
        errExit("crypt");
    
    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if(!authOk)
    {
        printf("Incorrect password");
        exit(EXIT_FAILURE);
    }

    printf("Successfully suthenticated: UID=%ld\n", (long)pwd->pw_uid);

    // Write youe won functionality
    
    exit(EXIT_SUCCESS);        
}

/*
-lcrypt is added here
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic -I../tlpi_lib 01_check_passwords.c ../tlpi_lib/ugid_functions.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -lcrypt -o 01_check_passwords
*/
