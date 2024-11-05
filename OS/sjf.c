#include <stdio.h>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
};

struct Process* queue;
int first = 0;
int last = 0;

void insert(struct Process* queue, int first, int last, struct Process p) {
    queue[last++] = p;
    int i = last-1;
    while(i != first && queue[i].remaining_time < queue[i-1].remaining_time) {
        struct Process temp = queue[i-1];
        queue[i-1] = queue[i];
        queue[i] = temp;
        i--;
    }
}

struct Process new_Process(int id, int at, int bt) {
    struct Process p = {id, at, bt, bt};
    return p;
}

int main() {
    queue = (int*) malloc(sizeof(struct Process) * 100);


}