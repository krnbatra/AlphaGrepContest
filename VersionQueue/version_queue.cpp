#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
 
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

vector< pair<int, pair<int, int> > > v;	// {value, {insertion_id, deletion_id} }

int find_idx(int ptr, int x){
	// 0 to ptr
	int lo = -1, hi = ptr;
	// trace(lo, hi);
	while(hi - lo > 1){
		int mid = (lo + hi) >> 1;
		if(v[mid].second.second > x)
			hi = mid;
		else
			lo = mid;
	}
	return hi;
}

int main(){
	io;
	int n;
	cin >> n;
	int version = 1;
	int ptr = 0;
	for(int i = 1; i <= n; i++){
		char c;
		cin >> c;
		if(c == 'e'){
			int x;
			cin >> x;
			v.push_back({x, {version, INT_MAX}});
			version += 1;
		}else if(c == 'd'){
			v[ptr].second.second = version;
			ptr += 1;
			version += 1;
		}else{
			int x;
			cin >> x;
			int start = find_idx(ptr, x);
			// trace(x, start, ptr);
			for(int i = start; i < v.size(); i++){
				if(v[i].second.first > x)
					break;
				else
					cout << v[i].first << " ";
			}
			cout << "\n";
		}
	}
	return 0;
}