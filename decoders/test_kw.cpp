#include <iostream>
#include <vector>
using namespace std;

int main() {
	freopen("test.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int n, m;
	cin >> n >> m;
	vector<vector<double>> logprobs(n, vector<double>(m));
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			cin >> logprobs[i][j];
	freopen("base.txt", "r", stdin);
	cin >> n;
	vector <int> base(n);
	for (int i = 0; i < n; ++i)
		cin >> base[i];

	freopen("check.txt", "r", stdin);
	cin >> n;
	vector <int> check(n);
	for (int i = 0; i < n; ++i)
		cin >> check[i];

	cout << logprobs.size() << ' ' << base.size() << ' ' << check.size();
	
}