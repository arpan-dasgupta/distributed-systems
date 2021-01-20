#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long int lu;
typedef long long int ll;

int main(int argc, char **argv)
{
    freopen(argv[1], "r", stdin);
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
    }

    freopen(argv[2], "r", stdin);
    vector<int> colors(m);
    for (int i = 0; i < m; i++)
    {
        cin >> colors[m];
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i != j && colors[i] == colors[j] && (edges[i].first == edges[j].first || edges[i].second == edges[j].first || edges[i].first == edges[j].second || edges[i].second == edges[j].second))
            {
                cout << "INVALID!'\n";
            }
        }
    }
    cout << "VALID!\n";

    return 0;
}