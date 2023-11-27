#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <cstdint>

using namespace std;
using namespace std::chrono;

pid_t pid;
pid_t child_pid;
int children[4];
int status;
milliseconds ms;

void createOrphan(int i){
    ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    cout << "ms:" << ms.count() << " Orphan Child " << i << " Created & Sleeping..." << " (PID: " << getpid() << " - PPID: " << getppid() << ")" << endl;
    sleep(15);
    ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    cout << "ms:" << ms.count() << " Orphan Child " << i << " Awake & Terminates" << " (PID: " << getpid() << " - PPID: " << getppid() << ")" << endl;
    exit(0);

    return;
}

void createZombie(int i){
    ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    cout << "ms:" << ms.count() << " Zombie Child " << i << " Created & Sleeping..." << " (PID: " << getpid() << " - PPID: " << getppid() << ")" << endl;
    sleep(5);
    ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    cout << "ms:" << ms.count() << " Zombie Child " << i << " Awake & Terminates" << " (PID: " << getpid() << " - PPID: " << getppid() << ")" << endl;
    exit(0);
    return;
}

int main(){
    ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    cout << "ms:" << ms.count() << " Parent Sleeping..." << " (PID: " << getpid() << ")" << endl;

    for (int i = 0; i < 4; i++){
        pid = fork();
        if (pid == 0){
            children[i] = getpid();
            if (i == 0 || i == 1)
                createZombie(i);
            else
                createOrphan(i);
        }
        else if (pid < 0){
            cout << "FORK FAILED!" << endl;
        }
    }
    
    sleep(10);
    ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    cout << "ms:" << ms.count() << " Parent " << "Awake & Waiting..." << " (PID: " << getpid() << ")" << endl;
    
    for (int i = 0; i < 4; i++){
        child_pid = waitpid(children[i], &status, WNOHANG);
        if (child_pid < 0){
            perror("WAIT FAILED!");
            return 1;
        }
        if (child_pid == 0){
            continue;
        }
        else if (WIFEXITED(status)){
            ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
            cout << "ms:" << ms.count() << " Child " << i << " with PID " << child_pid << " Exited! " << endl;
        }
    }
    cout << "Parent Terminates" << endl;

    return 0;
}