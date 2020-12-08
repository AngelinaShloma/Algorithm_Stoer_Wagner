#include <iostream>
#include <vector>
#include <time.h>
#include <cstring>
using namespace std;

int ** GenerateGraph(int n, int m) {
	int lenght = n;
	int **G = new int*[n];
	for (int i = 0; i < n; ++i) G[i] = new int[n];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			G[i][j] = 0;
	}
	if (m == n * (n - 1) / 2.0) {
		for (int i = 0; i < n; ++i)
			for (int j = i + 1; j < n; ++j) {
				G[i][j] = rand() % 100 + 1;
				G[j][i] = G[i][j];
			}
	}
	else {
		int *Not_In_G = new int[n];
		for (int i = 0; i < n; i++)
			Not_In_G[i] = i;
		std::vector<int> In_G;
		In_G.reserve(n);
		int v1, v2, number;
		v1 = rand() % n;
		v2 = rand() % n;
		while (v1 == v2) { v2 = rand() % n; }
		G[Not_In_G[v1]][Not_In_G[v2]] = rand() % 100 + 1;
		G[Not_In_G[v2]][Not_In_G[v1]] = G[Not_In_G[v1]][Not_In_G[v2]];
		In_G.push_back(Not_In_G[v1]);
		In_G.push_back(Not_In_G[v2]);
		for (int i = 0; i < n; ++i)
			if (Not_In_G[i] == v1) {
				for (int j = i; j < n; ++j)
					Not_In_G[j] = Not_In_G[j + 1];
			}
		--n;
		for (int i = 0; i < n; ++i)
			if (Not_In_G[i] == v2) {
				for (int j = i; j < n; ++j)
					Not_In_G[j] = Not_In_G[j + 1];
			}
		--n;
		while (n > 1) {
			number = rand() % (n - 1);
			v1 = Not_In_G[number];
			number = rand() % (In_G.size() - 1);
			v2 = In_G[number];
			G[v1][v2] = rand() % 100 + 1;
			G[v2][v1] = G[v1][v2];
			In_G.push_back(v1);
			for (int i = 0; i < n; ++i) {
				if (Not_In_G[i] == v1) {
					for (int j = i; j < n; ++j)
						Not_In_G[j] = Not_In_G[j + 1];
				}
			}
			--n;
		}
		v1 = Not_In_G[0];
		number = rand() % (In_G.size() - 1);
		v2 = In_G[number];
		G[v1][v2] = rand() % 100 + 1;
		G[v2][v1] = G[v1][v2];
		In_G.push_back(v1);
		m = m - lenght + 1;
		while (m > 0) {
			int i, j;
			i = rand() % lenght;
			j = rand() % lenght;
			while (i == j) { j = rand() % lenght; }
			while (G[i][j] != 0) {
				i = rand() % lenght;
				j = rand() % lenght;
				while (i == j) { j = rand() % lenght; }
			}
			G[i][j] = rand() % 100 + 1;
			G[j][i] = G[i][j];
			--m;
		}
	}
	return G;
}

void Stoer_Wagner(int **G, int n) {
	int best_cost = 10000;
	vector<int> best_cut;
	vector<int> v[n];
	for (int i = 0; i < n; ++i)
		v[i].assign(1, i);
	int w[n];
	bool exist[n], in_a[n];
	memset(exist, true, sizeof exist);
	for (int ph = 0; ph < n - 1; ++ph) {
		memset(in_a, false, sizeof in_a);
		memset(w, 0, sizeof w);
		for (int it = 0, prev; it < n - ph; ++it) {
			int sel = -1;
			for (int i = 0; i < n; ++i)
				if (exist[i] && !in_a[i] && (sel == -1 || w[i] > w[sel]))
					sel = i;
			if (it == n - ph - 1) {
				if (w[sel] < best_cost) {
					best_cost = w[sel];
					best_cut = v[sel];
				}
				v[prev].insert(v[prev].end(), v[sel].begin(), v[sel].end());
				for (int i = 0; i < n; ++i)
					G[prev][i] = G[i][prev] += G[sel][i];
				exist[sel] = false;
			}
			else {
				in_a[sel] = true;
				for (int i = 0; i < n; ++i)
					w[i] += G[sel][i];
				prev = sel;
			}
		}
	}
}

int main() {
	srand(time(NULL));
	int n = 50;
	int m;
	for (int k = 0; k < 10; ++k) {
		m = rand() % (n * (n - 1) / 2) + (n - 1);
		cout << n << ", " << m << endl;
		int **G = new int*[n];
		for (int i = 0; i < n; ++i) G[i] = new int[n];
		G = GenerateGraph(n, m);
		double t = 0.0;
		clock_t start, end;
		int run = 10;
		for (int i = 0; i < run; ++i) {
			start = clock();
			Stoer_Wagner(G, n);
			end = clock();
			t += (double)(end - start) / CLOCKS_PER_SEC;
		}
		t /= run;
		cout << " Time: " << t << " seconds " << endl;
		n += 50;
		t = 0.0;
	}

	n = 100;
	for (int k = 0; k < 10; ++k) {
		m = rand() % (n * (n - 1) / 2) + (n - 1);
		cout << n << ", " << m << endl;
		int **G = new int*[n];
		for (int i = 0; i < n; ++i) G[i] = new int[n];
		G = GenerateGraph(n, m);
		double t = 0.0;
		clock_t start, end;
		int run = 10;
		for (int i = 0; i < run; ++i) {
			start = clock();
			Stoer_Wagner(G, n);
			end = clock();
			t += (double)(end - start) / CLOCKS_PER_SEC;
		}
		t /= run;
		cout << " Time: " << t << " seconds " << endl;
		n += 100;
		t = 0.0;
	}


	return 0;
}