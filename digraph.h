#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <queue>
#include <utility>
#include <stack>
#include <list>

class digraph
{
public:
    typedef std::string T;
    typedef std::unordered_set<T> TSet;

    digraph()
    {
        _n = _m = 0;
    }

    // pre: none
    // post: returns the number of vertices
    std::size_t n() const
    {
        return _n;
    }

    // pre: none
    // post: returns the number of edges
    std::size_t m() const
    {
        return _m;
    }

    std::size_t outdegree(const T & v) const
    {

        assert(_V.count(v) == 1);
        return _t.at(v).size();
    }

    std::size_t indegree(const T & v) const
    {
        std::size_t ans(0);

        assert(_V.count(v) == 1);

        for (auto s: _V)
            if (_t.at(s).count(v) == 1)
                ++ans;

        return ans;
    }

    void add_vertex(const T & v)
    {
        if (_V.count(v) == 1)
            return;

        _t[v] = TSet();
        _V.insert(v);
        ++_n;

    }

    void add_edge(const T & s, const T & d)
    {
        assert(_V.count(s) == 1 && _V.count(d) == 1);

        if ((_t[s].insert(d)).second == true)
            ++_m;

    }

    TSet V() const
    {
        return _V;
    }

    TSet Adj(const T & v) const
    {
        assert(_V.count(v) == 1);
        return _t.at(v);
    }

    void remove_edge(const T & s, const T & d)
    {
        assert(_V.count(s) == 1 && _V.count(d) == 1);

        _m -= _t[s].erase(d);
    }

    void remove_vertex(const T & v)
    {
        assert(_V.count(v) == 1);
        for (auto s: _V)
        {
            _m -= _t[s].erase(v);

        }

        _t.erase(v);
        _V.erase(v);
        --_n;
    }

    digraph reverse() const
    {
        digraph R;

        for (auto v: _V)
            R.add_vertex(v);


        for (auto v: _V)
            for (auto w: Adj(v))
                R.add_edge(w, v);

        return R;
    }

    void rdfs(const T & s,
              std::unordered_map<T, std::size_t> & pre,
              std::unordered_map<T, std::size_t> & post,
              std::size_t &time,
              std::list<T> & l) const
    {
        pre[s] = time++;
        for (auto v: Adj(s))
        {
            if (pre.count(v) == 0)
            {
                rdfs(v, pre, post, time, l);
            }
            else
            {
                if (post.count(v) == 0)
                    ;
                else
                {
                    if (pre[v] > pre[s])
                        ;
                    else
                        ;
                }
            }
        }
        post[s] = time++;
        l.push_front(s);
    }

   std::unordered_map<T, T> dfs(const T & v)
   {
       std::stack<T> s;
       std::unordered_map<T, T> parent;
       std::unordered_map<T, std::size_t> pre, post;

       std::size_t time(1);

       s.push(v);

       while (!s.empty())
       {
           T top = s.top();
           if (pre.find(top) == pre.end()) // has not been discovered
           {
               pre[top] = time++;
               TSet neighbors = Adj(top);
               for (auto n: neighbors)
                   if (pre.find(n) == pre.end())
                   {
                       s.push(n);
                       parent[n] = top;
                   }
           }
           else
           {
               if (post.find(top) == post.end())
                   post[top] = time++;
               s.pop();
           }
       }


       return parent;

   }

    void ts1(const T & v,
             std::unordered_map<T, std::size_t> & pre,
             std::unordered_map<T, std::size_t> & post, 
             std::size_t & time,
             std::stack<T> & st) const
    {
        pre[v] =time++;
        for (auto w: Adj(v))
        {
            if (pre.count(w) == 0)
                ts1(w, pre, post, time, st);
        }
        post[v] = time++;
        st.push(v);

    }
    std::vector<T> ts() const
    {
        std::stack<T> st;

        std::unordered_map<T, std::size_t> pre, post;
        std::size_t time(1);

        for (auto start: _V)
        {
            if (pre.count(start) == 0)
                ts1(start, pre, post, time, st);
        }

        std::vector<T> ans;

        while (!st.empty())
        {
            ans.push_back(st.top());
            st.pop();
        }

        return ans;

    }
    digraph bfs(const T & a, std::unordered_set<T> & visited) const
    {
        assert(_t.find(a) != _t.end());
        std::queue<T> q;
        digraph ans;

        // initialization
        visited.insert(a);
        q.push(a);
        ans.add_vertex(a);

        while (!q.empty())
        {
            T front = q.front();
            q.pop();

            std::size_t count(0);
            for (auto w: Adj(front))
            {
                if (visited.insert(w).second) // w has not been visited
                {
                    q.push(w);
                    ans.add_vertex(w);
                    ans.add_edge(front, w);
                }
            }
        }
        return ans;
    }

   // pre: none
   // post: returns the number of connected components of this graph
    std::size_t nc() const
    {
        std::size_t ans(0);
        std::unordered_set<T> visited;

        for (auto e: V())
        {
            if (visited.find(e) == visited.end())
            {
                bfs(e, visited);
                ++ans;
            }
        }

        return ans;
    }

    std::unordered_map<T, std::size_t> scc() const
    {
        digraph R = reverse();

        std::unordered_map<T, std::size_t> pre, post;
        std::list<T> l;
        std::size_t time(1);

        for (auto s: R.V())
            if (pre.count(s) == 0)
                R.rdfs(s, pre, post, time, l);


        pre.clear();
        post.clear();
        time =1 ;

        std::size_t nc(0);
        std::unordered_map<T, std::size_t> ans;

        for (auto v: l)
        {
            std::list<T> c;
            if (pre.count(v) == 0)
            {

                rdfs(v, pre, post, time, c);
                ++nc;

                std::cout << "Component: ";
                for (auto e: c)
                {
                    std::cout << e << " ";
                    ans[e] = nc;
                }
                std::cout << std::endl;
            }
        }

        return ans;
    }
    std::vector<std::vector<std::size_t>> apsp() const
	{
		std::size_t nV = n(); //number of vertices
		std::vector<std::vector<std::size_t>> dist(nV, std::vector<std::size_t>(nV, nV));	//matrix size nVxnV, filled with nV
		std::size_t i(0);//vector indexies
		std::unordered_map<T, std::size_t> reversed; //unordered map with vertices mapped to an index 0 - (nV - 1)

		std::vector<T> values;
		for(auto e: V())	
		{
			values.push_back(e);
		}
		std::sort(values.begin(),values.end()); //sort vertices in order to match with index
		//insert vertices and indecies in hash table, reversed 
		for(auto e: values)
		{
			reversed.insert({e,i});
			++i;
		}
		//if x is adjacent to vertex e, set distance value to 1
		for(auto e: V())
		{
			for(auto x: Adj(e))
			{
				dist[reversed[e]][reversed[x]] = 1;	
			}
		}
		//set diagonal == 0
		for(std::size_t i = 0; i < nV; ++i)
		{
			for(std::size_t j = 0; j < nV; ++j)
			{
				if(i == j)
				{
					dist[i][j] = 0;
					dist[j][i] = 0;
				}
			}
		}
		//floyd's algorithm
		for(std::size_t k = 0; k < nV; ++k)
		{
			for(std::size_t i = 0; i < nV; ++i)
			{
				for(std::size_t j = 0; j < nV; ++j)
				{

					if(dist[i][j] > dist[i][k] +dist[k][j])
					{
						dist[i][j] = dist[i][k] +dist[k][j];
					}
				}
			}
		}	
	return dist;
	}
private:

    std::unordered_map<T, TSet> _t;     //  adjacency "lists"
    TSet _V;                            //  vector of vertices
    std::size_t _n, _m;                 //  number of vertices and edges
};


std::ostream & operator << (std::ostream & os, const digraph & D)
{
    os << D.n() << " " << D.m() << std::endl;
    for (auto v: D.V())
        os << v << " ";
    os << std::endl;
    for (auto v: D.V())
    {
        os << v << ": ";
        for (auto n: D.Adj(v))
            os << n << " ";
        os << std::endl;
    }
    return os;
}

std::istream & operator >> (std::istream & is, digraph & D)
{
    std::size_t n, m;
    std::string s, d;

    is >> n >> m;

    for (std::size_t i = 0; i < n; ++i)
    {
        is >> s;
        D.add_vertex(s);
    }

    for (std::size_t i = 0; i < m; ++i)
    {
        is >> s >> d;
        D.add_edge(s, d);
    }

    return is;


}





#endif // DIGRAPH_H

