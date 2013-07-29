#ifndef GRAPH_H
#define GRAPH_H

#include <queue>
using std::priority_queue;
using std::vector;
#include <set>
using std::set;
#include <map>
using std::map;

namespace gasper {

	/** 
	*	Dijkstra search algorithm on graphs	with non-negative comparatible edge path cost.
	*
	*	node_t is the type of graph verteces
	*	cost_t is the type of non-negative edge path costs, that have comparator "<" and "-1"
	*/
	template<typename node_t, typename cost_t>
	struct graph_edge {
		node_t begining, end;
		cost_t cost;
		graph_edge(){};
		graph_edge(node_t a, node_t b, node_t c):begining(a),end(b),cost(c){};
	};
	
	template<typename node_t, typename cost_t>
	struct graph_vertex {
		node_t vertex;
		cost_t cost;
		graph_vertex(){};
		graph_vertex(node_t a, cost_t b):vertex(a),cost(b){};
	};
	
	template<typename node_t, typename cost_t>
	bool operator<(const graph_edge<node_t, cost_t>& left, const graph_edge<node_t, cost_t>& right){return left.cost < right.cost;}

	template<typename node_t, typename cost_t>
	bool operator<(const graph_vertex<node_t, cost_t>& left, const graph_vertex<node_t, cost_t>& right){return left.cost < right.cost;}
	
	/**
	*	graph is an array of graph_edge elements that represent graph edges
	*	num_edges is the number of edges in graph
	*	source is the begining vertex for search process
	*/
	template<typename node_t, typename cost_t>
	graph_vertex<node_t, cost_t>* dijkstra(const graph_edge<node_t, cost_t>* graph, const int num_edges, const node_t source)
	{
		vector<graph_edge<node_t, cost_t>> graph_edges;
		priority_queue<graph_vertex<node_t, cost_t>, vector<graph_vertex<node_t, cost_t>>, std::less<vector<graph_vertex<node_t, cost_t>>::value_type>> graph_heap;
		set<node_t> tmp_verteces;
		for (int i = 0; i<num_edges; i++) 
		{
			graph_edges.insert(graph_edges.end(), *(graph+i));
			if (tmp_verteces.find(graph_edges[i].begining) == tmp_verteces.end())
				tmp_verteces.insert(graph_edges[i].begining);
			if (tmp_verteces.find(graph_edges[i].end) == tmp_verteces.end())
				tmp_verteces.insert(graph_edges[i].end);
		}
		for (set<node_t>::iterator i = tmp_verteces.begin(); i!=tmp_verteces.end(); i++)
			graph_heap.push(graph_vertex<node_t, cost_t>(*i, (*i != source)?-1:0));
		
		vector<graph_vertex<node_t, cost_t>> result;
		while (!graph_heap.empty())
		{
			graph_vertex<node_t, cost_t> current_vertex = graph_heap.top();
			graph_heap.pop();
			
			for (int i = 0; i<num_edges; i++)
			{
				if (current_vertex.cost == -1) continue;
				if (graph_edges[i].begining == current_vertex.vertex)
					graph_heap.push(graph_vertex<node_t, cost_t>(graph_edges[i].end, current_vertex.cost+graph_edges[i].cost));
			}
			
			bool flag = false;
			for (int i = 0; i<result.size(); i++)
				if (current_vertex.vertex == result[i].vertex)
				{
					result[i].cost = (current_vertex.cost < result[i].cost && current_vertex.cost != -1) ? current_vertex.cost : result[i].cost;
					flag = true;
				}
			if (!flag) result.insert(result.end(), current_vertex);
		}
		
		graph_vertex<node_t, cost_t>* result_array = new graph_vertex<node_t, cost_t>[result.size()];
		for (int i = 0; i<result.size(); i++)
			result_array[i] = result[i];
		return result_array;
	}
	
	/*
	*	graph is the distance matrix
	*	verteces is the array of verteces as they represented in graph
	*	num_verteces is the number of verteces
	*	source is the begining vertex
	*/
	template<typename node_t, typename cost_t>
	graph_vertex<node_t, cost_t>* dijkstra(cost_t** graph, const node_t* verteces, const int num_verteces, const node_t source)
	{
		int num_edges = 0;
		for (int i = 0; i<num_verteces; i++)
			for (int j = 0; j<num_verteces; j++)
				if (graph[i][j]>0) num_edges++;
		graph_edge<node_t, cost_t>* graph_edges = new graph_edge<node_t, cost_t>[num_edges];
		num_edges = 0;
		for (int i = 0; i<num_verteces; i++)
			for (int j = 0; j<num_verteces; j++)
				if (graph[i][j]>0) 
					graph_edges[num_edges++] = graph_edge<node_t, cost_t>(verteces[i], verteces[j], graph[i][j]);
		return dijkstra<node_t, cost_t>(graph_edges, num_edges, source);
	}
	
	/*
	*	graph is the distance matrix as a one dimensional array
	*	verteces is the array of verteces as they represented in graph
	*	num_verteces is the number of verteces
	*	source is the begining vertex
	*/
	template<typename node_t, typename cost_t>
	graph_vertex<node_t, cost_t>* dijkstra(cost_t* graph, const node_t* verteces, const int num_verteces, const node_t source)
	{
		int num_edges = 0;
		for (int i = 0; i<num_verteces*num_verteces; i++)
			if (graph[i]>0) num_edges++;
		graph_edge<node_t, cost_t>* graph_edges = new graph_edge<node_t, cost_t>[num_edges];
		num_edges = 0;
		for (int i = 0; i<num_verteces; i++)
			for (int j = 0; j<num_verteces; j++)
				if (graph[i*num_verteces+j]>0) 
					graph_edges[num_edges++] = graph_edge<node_t, cost_t>(verteces[i], verteces[j], graph[i*num_verteces+j]);
		return dijkstra<node_t, cost_t>(graph_edges, num_edges, source);
	}

	/** 
	*	A* pathfinding algorithm on graphs
	*/

	template<typename node_t, typename cost_t>
	cost_t standard_heuristic(node_t start, node_t end){return (cost_t)0;}

	template<typename node_t, typename cost_t>
	struct path {
		node_t* path_nodes;
		int length;
		cost_t cost;
		path(){};
		path(node_t* a, int b, cost_t c):path_nodes(a),length(b),cost(c){};
	};

	template<typename node_t, typename cost_t>
	vector<node_t> reconstruct_path(map<node_t, node_t> path, node_t current_vertex)
	{
		vector<node_t> p;
		if (path.find(current_vertex) != path.end())
			p = reconstruct_path<node_t, cost_t>(path, path[current_vertex]);
		p.insert(p.end(), current_vertex);
		return p;
	}

	template<typename node_t, typename cost_t, cost_t (*heuristic)(node_t, node_t)>
	path<node_t, cost_t> A_star(const graph_edge<node_t, cost_t>* graph, const int num_edges, const node_t begining, const node_t end)
	{
		vector<graph_edge<node_t, cost_t>> graph_edges;
		set<node_t> closed_set, open_set;
		map<node_t, node_t> path;
		map<node_t, cost_t> g_score;
		priority_queue<graph_vertex<node_t, cost_t>, vector<graph_vertex<node_t, cost_t>>, std::less<vector<graph_vertex<node_t, cost_t>>::value_type>> f_score;
		vector<node_t> result;

		for (int i = 0; i<num_edges; i++)
			graph_edges.insert(graph_edges.end(), *(graph+i));
		open_set.insert(begining);
		g_score[begining] = 0;
		f_score.push(graph_vertex<node_t, cost_t>(begining, heuristic(begining, end)));

		while (!open_set.empty())
		{
			node_t current_vertex = f_score.top().vertex;
			if (current_vertex == end)
				result = reconstruct_path<node_t, cost_t>(path, end);

			f_score.pop();
			open_set.erase(current_vertex);
			closed_set.insert(current_vertex);

			for (int i = 0; i<num_edges; i++)
				if (graph_edges[i].begining == current_vertex)
				{
					cost_t tent_g_score = g_score[current_vertex] + graph_edges[i].cost;
					node_t adjacent_vertex = graph_edges[i].end; 
					if (closed_set.find(adjacent_vertex) != closed_set.end() && tent_g_score >= g_score[adjacent_vertex]) continue;
					if (open_set.find(adjacent_vertex) == open_set.end() || tent_g_score < g_score[adjacent_vertex])
					{
						path[adjacent_vertex] = current_vertex;
						g_score[adjacent_vertex] = tent_g_score;
						f_score.push(graph_vertex<node_t, cost_t>(adjacent_vertex, tent_g_score + heuristic(adjacent_vertex, end)));
						open_set.insert(adjacent_vertex);
					}
				}
		}

		node_t* result_array;
		if (result.size() == 0) 
			result_array = NULL; 
		else
		{
			result_array = new node_t[result.size()];
			for (int i = 0; i<result.size(); i++)
				result_array[i] = result[i];
		}
		return ::path<node_t, cost_t>(result_array, result.size(), g_score[end]);
	}
	
	template<typename node_t, typename cost_t, cost_t (*heuristic)(node_t, node_t)>
	path<node_t, cost_t> A_star(cost_t** graph, const node_t* verteces, const int num_verteces, const node_t begining, const node_t end)
	{
		int num_edges = 0;
		for (int i = 0; i<num_verteces; i++)
			for (int j = 0; j<num_verteces; j++)
				if (graph[i][j]>0) num_edges++;
		graph_edge<node_t, cost_t>* graph_edges = new graph_edge<node_t, cost_t>[num_edges];
		num_edges = 0;
		for (int i = 0; i<num_verteces; i++)
			for (int j = 0; j<num_verteces; j++)
				if (graph[i][j]>0) 
					graph_edges[num_edges++] = graph_edge<node_t, cost_t>(verteces[i], verteces[j], graph[i][j]);	
		return A_star<node_t, cost_t, heuristic>(graph_edges, num_edges, begining, end);
	}
	
	template<typename node_t, typename cost_t, cost_t (*heuristic)(node_t, node_t)>
	path<node_t, cost_t> A_star(const cost_t* graph, const node_t* verteces, const int num_verteces, const node_t begining, const node_t end)
	{
		int num_edges = 0;
		for (int i = 0; i<num_verteces*num_verteces; i++)
			if (graph[i]>0) num_edges++;
		graph_edge<node_t, cost_t>* graph_edges = new graph_edge<node_t, cost_t>[num_edges];
		num_edges = 0;
		for (int i = 0; i<num_verteces; i++)
			for (int j = 0; j<num_verteces; j++)
				if (graph[i*num_verteces+j]>0) 
					graph_edges[num_edges++] = graph_edge<node_t, cost_t>(verteces[i], verteces[j], graph[i*num_verteces+j]);	
		return A_star<node_t, cost_t, heuristic>(graph_edges, num_edges, begining, end);
	}

}
#endif