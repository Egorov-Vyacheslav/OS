#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int ls_la(struct dirent * dirr)
{
	char * buffer;
	buffer = calloc(sizeof(char)*(strlen(dirr->d_name)) + 2, sizeof(char));
	buffer[0] = '.';
	buffer[1] = '/';

	strcpy(&(buffer[2]), dirr->d_name);

	struct stat S;
	memset(&S, 0, sizeof(struct stat));
	stat(buffer, &S);
	printf((S.st_mode & S_IFDIR)?"d":"-");
	printf((S.st_mode & S_IRUSR)?"r":"-");
	printf((S.st_mode & S_IWUSR)?"w":"-");
	printf((S.st_mode & S_IXUSR)?"x":"-");
	printf((S.st_mode & S_IRGRP)?"r":"-");
	printf((S.st_mode & S_IWGRP)?"w":"-");
	printf((S.st_mode & S_IXGRP)?"x":"-");
	printf((S.st_mode & S_IROTH)?"r":"-");
	printf((S.st_mode & S_IWOTH)?"w":"-");
	printf((S.st_mode & S_IXOTH)?"x":"-");
	printf(" %1ld", S.st_nlink);
	struct passwd * pwd = getpwuid(S.st_uid);
	printf(" %s", pwd->pw_name);

	struct group * grp = getgrgid(S.st_gid);
	printf(" %s", grp->gr_name);
	printf(" %5d ", (int)S.st_size);

  time_t It = S.st_ctime;
	struct tm * time_ptr = localtime(&It);
	printf(" %02d.%02d", time_ptr->tm_mday, time_ptr->tm_mon+1);
	printf(" %02d:%02d ", time_ptr->tm_hour, time_ptr->tm_min);
	printf("%s", dirr->d_name);
	printf("\n");

	return 0;
}

int main()
{
	int n;
	struct dirent ** entry;
	if(-1 == (n = scandir("./", &entry, NULL, alphasort)))
		return 1;

	printf("%d files in directory\n", n);
	int dir_size = n-1;

	while(n--){
		ls_la(entry[dir_size - n]);
	}
	return 0;
}
