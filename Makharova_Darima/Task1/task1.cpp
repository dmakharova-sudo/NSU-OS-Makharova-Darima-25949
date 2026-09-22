#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <limits.h>
#include <errno.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    const char *options = "ispduU:cC:vV:";
    int c;
    int invalid = 0;
    int iflg = 0, sflg = 0, pflg = 0, uflg = 0, cflg = 0, dflg = 0, vflg = 0;
    string U_arg, C_arg, V_arg;

    cout << "args equals " << argc << endl;

    while ((c = getopt(argc, argv, options)) != -1)
    {
        switch (c)
        {
        case 'i':
            iflg++;
            cout << "Real UID: " << getuid() << " Effective UID: " << geteuid() << endl;
            cout << "Real GID: " << getgid() << " Effective GID: " << getegid() << endl;
            break;

        case 's':
            sflg++;
            if (setpgid(0, 0) == -1)
            {
                cerr << "setpgid failed: " << strerror(errno) << endl;
            }
            else
            {
                cout << "Process set as group leader" << endl;
            }
            break;

        case 'p':
            pflg++;
            cout << "Process ID: " << getpid() << endl;
            cout << "Parent Process ID: " << getppid() << endl;
            cout << "Process Group ID: " << getpgrp() << endl;
            break;

        case 'd':
            dflg++;
            break;

        case 'u':
            uflg++;
            {
                struct rlimit rlim;
                if (getrlimit(RLIMIT_FSIZE, &rlim) == -1)
                {
                    cerr << "getrlimit RLIMIT_FSIZE failed: " << strerror(errno) << endl;
                }
                else
                {
                    cout << "Current ulimit: " << rlim.rlim_cur << " bytes" << endl;
                }
            }
            break;

        case 'U':
            uflg++;
            U_arg = optarg;
            try
            {
                size_t pos;
                long new_ulimit = stol(optarg, &pos);
                if (pos != strlen(optarg))
                {
                    cerr << "Invalid ulimit value: " << optarg << endl;
                    invalid++;
                }
                else
                {
                    struct rlimit rlim;
                    rlim.rlim_cur = new_ulimit;
                    rlim.rlim_max = new_ulimit;
                    if (setrlimit(RLIMIT_FSIZE, &rlim) == -1)
                    {
                        cerr << "setrlimit RLIMIT_FSIZE failed: " << strerror(errno) << endl;
                    }
                    else
                    {
                        cout << "ulimit set to " << new_ulimit << " bytes" << endl;
                    }
                }
            }
            catch (const exception &e)
            {
                cerr << "Invalid ulimit value: " << optarg << endl;
                invalid++;
            }
            break;

        case 'c':
            cflg++;
            {
                struct rlimit rlim;
                if (getrlimit(RLIMIT_CORE, &rlim) == -1)
                {
                    cerr << "getrlimit RLIMIT_CORE failed: " << strerror(errno) << endl;
                }
                else
                {
                    cout << "Core file size limit: " << rlim.rlim_cur << " bytes" << endl;
                }
            }
            break;

        case 'C':
            cflg++;
            C_arg = optarg;
            try
            {
                size_t pos;
                long new_core = stol(optarg, &pos);
                if (pos != strlen(optarg))
                {
                    cerr << "Invalid core file size: " << optarg << endl;
                    invalid++;
                }
                else
                {
                    struct rlimit rlim;
                    rlim.rlim_cur = new_core;
                    rlim.rlim_max = new_core;
                    if (setrlimit(RLIMIT_CORE, &rlim) == -1)
                    {
                        cerr << "setrlimit RLIMIT_CORE failed: " << strerror(errno) << endl;
                    }
                    else
                    {
                        cout << "Core file size set to " << new_core << " bytes" << endl;
                    }
                }
            }
            catch (const exception &e)
            {
                cerr << "Invalid core file size: " << optarg << endl;
                invalid++;
            }
            break;

        case 'v':
            vflg++;
            {
                extern char **environ;
                for (char **env = environ; *env != NULL; env++)
                {
                    cout << *env << endl;
                }
            }
            break;

        case 'V':
            vflg++;
            V_arg = optarg;
            if (putenv(optarg) != 0)
            {
                cerr << "putenv failed: " << strerror(errno) << endl;
            }
            else
            {
                cout << "Environment variable set: " << optarg << endl;
            }
            break;

        case '?':
            cerr << "Invalid option: " << static_cast<char>(optopt) << endl;
            invalid++;
            break;
        }
    }

    cout << "i flag count: " << iflg << endl;
    cout << "s flag count: " << sflg << endl;
    cout << "p flag count: " << pflg << endl;
    cout << "u flag count: " << uflg << endl;
    cout << "c flag count: " << cflg << endl;
    cout << "d flag count: " << dflg << endl;
    cout << "v flag count: " << vflg << endl;

    if (!U_arg.empty())
        cout << "U_arg: " << U_arg << endl;
    if (!C_arg.empty())
        cout << "C_arg: " << C_arg << endl;
    if (!V_arg.empty())
        cout << "V_arg: " << V_arg << endl;

    cout << "Invalid options count: " << invalid << endl;
    cout << "optind equals " << optind << endl;

    if (optind < argc)
    {
        cout << "Non-option arguments: ";
        for (int i = optind; i < argc; ++i)
        {
            cout << argv[i] << " ";
        }
        cout << endl;
    }

    return 0;
}