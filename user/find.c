#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(const char* path, char* des)
{
    char buf[512],*p;
    strcpy(buf,path);
    p = buf + strlen(path);
    *p++ = '/';
    int fd;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
	return;
    }
    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
	return;
	return;
    }
    struct dirent dir;
    int len = sizeof(dir);
    while(read(fd, &dir, len) == len)
    {
	if(dir.inum == 0)
	    continue;
	strcpy(p, dir.name);
	if(stat(buf, &st) < 0)
	{
	    fprintf(2, "cannot stat %s\n", buf);
	    continue;
	}
	switch(st.type)
	{
	    case T_FILE:
		if(!strcmp(dir.name, des))
		    printf("%s\n", buf);
		break;
	    case T_DIR:
		if(strcmp(".", dir.name) && strcmp("..", dir.name))
		    find(buf, des);
		break;
	    default:
		break;
	}
    }
    close(fd);
}


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
	fprintf(2, "usage: find dirName\n");
	exit(1);
    }
    char* des = 0;
    des = (char*)argv[2];
    find(argv[1], des);
    exit(0);
}
