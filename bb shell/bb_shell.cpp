#include <iostream>
#include <string>
#include <vector>

#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

vector<string> split2(const string &str, const string &pattern)
{
	char *strc = new char[strlen(str.c_str()) + 1];
	strcpy(strc, str.c_str()); //string转换成C-string
	vector<string> res;
	char *temp = strtok(strc, pattern.c_str());
	while (temp != NULL)
	{
		res.push_back(string(temp));
		temp = strtok(NULL, pattern.c_str());
	}
	delete[] strc;
	return res;
}

int main()
{
	void *dp = dp = dlopen("./libbusybox.so", RTLD_LAZY);
	void *fc = dlsym(dp, "lbb_main");
	typedef void (*bb)(char **argv);
	bb func = (bb)fc;

	cout << "use ctrl+c or quit to exit." << endl;
	int pid;
	/* fork another process */
	while (1)
	{
		string command;
		cout << "busybox >";
		getline(cin, command);
		vector<string> cmd_vec = split2(command, " ");
		if(!command.compare("quit"))
			exit(0);
		

		int arg_count = cmd_vec.size();
		int pid = fork();
		if (pid < 0)
		{
			/* error occurred */
			cout << "forked failed" << endl;
			exit(-1);
		}
		else if (pid == 0)
		{
			/*  child process   */
			const char **argv_bb = (const char **)malloc(sizeof(char *) * arg_count + 1);
			memset(argv_bb, 0, sizeof(char *) * arg_count + 1);
			for (int i = 0; i < arg_count; ++i)
			{
				argv_bb[i] = cmd_vec[i].c_str();
			}
			func((char **)argv_bb);
			free(argv_bb);
		}
		else
		{
			/*    parent process  */
			/* parent will wait for the child to complete*/
			wait(NULL);
			printf("busybox command Complete! \n");
		}
	}


	return 0;
}