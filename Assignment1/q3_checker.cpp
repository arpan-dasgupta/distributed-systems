#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long int lu;
typedef long long int ll;
#define READ_DSC 0

int n, m;
vector<pair<int, int>> edges;

void read_clq(string file_name)
{
    int i, x, y;
    string type, line;
    ifstream fin(file_name.c_str());
    if (!fin.is_open())
    {
        cout << "ERROR opening file" << endl;
        exit(1);
    }

    //eliminate comment lines
    // string comment;
    while (fin.peek() == 'c')
    {
        getline(fin, line);
        // comment=comment+"#"+line+'\n';
    }

    fin >> type; // type of image/matrix (P1)
    fin >> type; // edge
    fin >> n;    // vertexes
    fin >> m;    // edges

    for (i = 0; i < m; i++)
    {
        fin >> type; // e
        if (type == "e")
        {
            fin >> x;
            fin >> y;
            edges.push_back({x - 1, y - 1});
            // cout << x - 1 << " " << y - 1 << endl;
        }
    }
    fin.close();
}

int main(int argc, char **argv)
{
    if (not READ_DSC)
    {
        freopen(argv[1], "r", stdin);
        cin >> n >> m;
        for (int i = 0; i < m; i++)
        {
            int u, v;
            cin >> u >> v;
            edges.push_back({u, v});
        }
    }
    else
    {
        read_clq(string(argv[1]));
    }

    int adj[1000][1000];
    for (int i = 0; i < m; i++)
    {
        for (int j = i + 1; j < m; j++)
        {
            if (edges[i].first == edges[j].first || edges[i].first == edges[j].second || edges[i].second == edges[j].first || edges[i].second == edges[j].second)
            {
                adj[i][j] = adj[j][i] = true;
            }
        }
    }
    int max_deg = 0;
    for (int i = 0; i < m; i++)
    {
        int deg = 0;
        for (int j = 0; j < m; j++)
        {
            deg += adj[i][j];
        }
        max_deg = max(max_deg, deg);
    }

    freopen(argv[2], "r", stdin);
    vector<int> colors(m);

    int max_colors;
    cin >> max_colors;
    // cout << max_colors << " " << argv[2] << " __ \n";
    if (max_colors > max_deg + 1)
    {
        cout << "INVALID 1 : " << max_colors << '\n';
        return 0;
    }

    for (int i = 0; i < m; i++)
    {
        cin >> colors[i];
        if (not cin.good() or colors[i] > max_colors)
        {
            cout << "INVALID 2 : " << max_colors << '\n';
            return 0;
        }
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == j)
                continue;

            bool not_ok = colors[i] == colors[j] && adj[i][j];
            if (not_ok)
            {
                cout << "INVALID!'\n";
            }
        }
    }
    cout << "VALID!\n";

    return 0;
}