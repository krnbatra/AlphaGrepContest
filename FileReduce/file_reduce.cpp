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

typedef struct record{
	char type;
	string symbol;
	int price;
	int quantity;
	ll expirydate;
	int strikeprice;
	ll amendtime;
	int id;
	int parentid;
}record;

vector<record> v;
vector<int> adj[N];
map<int, int> mp;
int vis[N], subtree_size[N];
map<int, int> sizes;
ofstream outfile;
set<int> visited_parents;

string parse(string &data, int &idx){
	string ret = "";
	while(idx < data.size() && data[idx] != ','){
		ret += data[idx];
		idx++;
	}
	idx++;
	return ret;
}

void fill_record(record &r, string &data){
	int idx = 0;
	r.type = parse(data, idx)[0];
	r.symbol = parse(data, idx);
	r.price = atoi(parse(data, idx).c_str());
	r.quantity = atoi(parse(data, idx).c_str());
	r.expirydate = atoll(parse(data, idx).c_str());
	r.strikeprice = atoi(parse(data, idx).c_str());
	r.amendtime = atoll(parse(data, idx).c_str());
	r.id = atoi(parse(data, idx).c_str());
	r.parentid = atoi(parse(data, idx).c_str());
	// trace(r.type, r.symbol, r.price, r.quantity, r.expirydate, r.strikeprice, r.amendtime, r.id, r.parentid);
	return;
}

void dfs(int u, int p){
	vis[u] = 1;
	subtree_size[u] = 1;
	for(auto v : adj[u]){
		if(v == p)	continue;
		dfs(v, u);
		subtree_size[u] += subtree_size[v];
	}
	return;
}

void open_file(int idx){
	string file_name = "output_" + to_string(idx) + ".txt";
	// trace(idx, file_name);
	outfile.open(file_name);
}


void print_record(int idx){
	string data = "";
	if(v[idx].type == 'T')
		data += "T";
	else
		data += "P";
	data += "," + v[idx].symbol + "," + to_string(v[idx].price) + "," + to_string(v[idx].quantity) + "," + to_string(v[idx].expirydate) + "," + to_string(v[idx].strikeprice) + "," + to_string(v[idx].amendtime) + "," + to_string(v[idx].id) + "," + to_string(v[idx].parentid);
	// trace(data);
	outfile << data << "\n";
	for(auto v : adj[idx])
		print_record(v);
	return; 
}

int main(int argc, char **argv){
	io;
	string file_name = argv[1];
	int	x = atoi(string(argv[2]).c_str());
	string data;
	// open a file in read mode.
	ifstream infile; 
	infile.open(file_name);
	record r;
	// for correcting index
	v.push_back(r);
	while(infile >> data){
		record r;
		fill_record(r, data);
		v.push_back(r);
	}
	for(int i = 1; i < v.size(); i++){
		mp[v[i].id] = i;
	}
	for(int i = 1; i < v.size(); i++){
		// go to parent id of current record
		int p_id = v[i].parentid;
		if(p_id == 0)	continue;
		// index of parent id
		int idx = mp[p_id];
		if(idx == 0)	continue;
		// this can only be child
		if(v[i].type == 'T') continue;
		// create edge
		adj[idx].push_back(i);
	}
	deque< pair<int, int> > parents;	// storing (size, index) of parents
	for(int i = 1; i < v.size(); i++){
		if(!vis[i]){
			dfs(i, 0);
			parents.push_back({subtree_size[i], i});
		}
	}
	// greater than x
	sort(parents.begin(), parents.end());
	int file_idx = 1;
	for(int i = parents.size() - 1; i >= 0; i--){
		if(parents[i].first >= x){
			// trace(parents[i].first, parents[i].second);
			open_file(file_idx);
			file_idx += 1;
			print_record(parents[i].second);
			outfile.close();
			parents.pop_back();
		}
	}
	// for(auto i : parents)
	// 	trace(i.first, i.second);
	int l = 0, rig = parents.size() - 1;
	int sz = 0;
	bool first = true;
	while(l <= rig){
		if(first){
			open_file(file_idx);
			first = false;
		}
		sz += parents[l].first;
		print_record(parents[l].second);
		visited_parents.insert(parents[l].second);
		l += 1;
		if(l > rig)
			break;
		print_record(parents[rig].second);
		visited_parents.insert(parents[rig].second);
		sz += parents[rig].first;
		// trace(sz);
		if(sz >= x){
			first = true;
			sz = 0;
			outfile.close();
			file_idx += 1;
			rig -= 1;
		}
	}
	for(auto p : parents){
		if(visited_parents.find(p.second) == visited_parents.end()){
			print_record(p.second);
			visited_parents.insert(p.second);
		}
	}
	return 0;
}
