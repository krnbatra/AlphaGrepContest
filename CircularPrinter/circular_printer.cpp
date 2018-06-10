#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

#define TRACE
#ifdef TRACE
#define trace(...) __f(#__VA_ARGS__, __VA_ARGS__)
template <typename Arg1>
void __f(const char* name, Arg1&& arg1){
  cerr << name << " : " << arg1 << std::endl;
}
template <typename Arg1, typename... Args>
void __f(const char* names, Arg1&& arg1, Args&&... args){
  const char* comma = strchr(names + 1, ',');cerr.write(names, comma - names) << " : " << arg1<<" | ";__f(comma+1, args...);
}
#else
#define trace(...)
#endif

#define MOD                 1000000007LL
#define EPS                 1e-9
#define io                  ios_base::sync_with_stdio(false);cin.tie(NULL);

const int N = 1e5 + 5;

string str;
int char_count;
int no_threads;
int k;
int *is_my_turn;
int length;

int str_index;
pthread_t * threads;
pthread_cond_t   condition_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;

void * printString(void * data){
	int index = *((int*)(data));
	for (int i = 0; ; i++){
		pthread_mutex_lock(&condition_mutex);
		while(is_my_turn[index] == 0){
			pthread_cond_wait(&condition_cond, &condition_mutex);
		}
		cout << "Thread" << index + 1 << ": ";
		for(int j = 0; j < char_count; j++){
			cout << str[str_index];
			str_index = (str_index + 1) % length;
		}
		cout << "\n";
		is_my_turn[index] = 0;
		is_my_turn[(index+1) % no_threads] = 1;

		pthread_cond_broadcast(&condition_cond);
        pthread_mutex_unlock( &condition_mutex );
		if(i == k-1) break;
    }
	return NULL;
}

int main(int argc, char **argv){
	str = argv[1];
	char_count = atoi(string(argv[2]).c_str());
	no_threads = atoi(string(argv[3]).c_str());
	k = atoi(string(argv[4]).c_str());
    
    threads = (pthread_t *) malloc(no_threads * sizeof(pthread_t));
    
    length = str.size();
    
    int *arr = (int *) malloc(no_threads * sizeof(int) );
    is_my_turn = (int *) malloc(no_threads * sizeof(int));
	is_my_turn[0] = 1;
	
	for(int i = 1; i < no_threads; i++)
		is_my_turn[i] = 0;

    for(int i = 0; i < no_threads; i++){
        arr[i] = i;
        pthread_create( &threads[i], NULL, printString, (void *)&arr[i]);
    }
	
    for(int i = 0; i < no_threads; i++)
        pthread_join( threads[i], NULL);

    return 0;
}

