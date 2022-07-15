#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

struct passwd *getpwnam(const char *name)
{
    struct passwd *ptr;

    setpwent();
    while ((ptr = getpwent()) != NULL) {
        if (strcmp(name, ptr->pw_name) == 0) {
            break;
        }
    }
    endpwent();
    return ptr;
}

int main()
{
    struct passwd *user = getpwnam("echo");
    printf("name: %s\n", user->pw_name);
    printf("passwd: %s\n", user->pw_passwd);
    printf("%d\n", user->pw_uid);
    printf("%s\n", user->pw_dir);
    printf("%d\n", user->pw_gid);
    printf("%s\n", user->pw_gecos);
}