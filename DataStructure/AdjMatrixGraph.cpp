//#include <iostream>
//#include <vector>
//#include <queue>
//
//template <typename VT, typename ET>
//class AdjMatrixUndirectedGraph : public std::exception {
//public:
//	struct Vertex;
//	typedef Vertex* VertexPtr;
//	struct Edge;
//	typedef Edge* EdgePtr;
//	typedef std::vector<Vertex> VertexList;
//	typedef std::vector<VertexPtr> VertexPtrList;
//	typedef std::vector<Edge> EdgeList;
//	typedef std::vector<EdgePtr> EdgePtrList;
//	typedef std::vector< EdgePtrList > AdjMatrix;
//public:
//	struct Vertex {
//		VT data;
//		int index;
//		EdgePtrList incident_edge_list;
//		Vertex(const VT& data, const int& index) :data(data), index(index) {};
//		~Vertex() {};
//		VT& operator*() { return data; }
//		void set_index(const int& input_index) { index = input_index; }
//		int get_index() { return index; }
//	};
//	struct Edge {
//		VertexPtr left_vertex;
//		VertexPtr right_vertex;
//		ET data;//Edge's data(It can be thought like time from edge to edge).
//		Edge(VertexPtr left, VertexPtr right, const ET& data) :left_vertex(left), right_vertex(right), data(data) {};
//		~Edge() {};
//		ET operator*() const { return data; }
//		bool IsIncidentOn(const Vertex& vertex) const {
//			if (*left_vertex == vertex || *right_vertex == vertex) return true;
//			else return false;
//		}
//	};
//	class Decorator {
//	public:
//		struct VertexWrapper {
//			VertexPtr vertex;
//			bool visit;
//			VertexWrapper(VertexPtr vertex) :vertex(vertex), visit(false) {};
//		};
//	private:
//		std::vector<VertexWrapper> vertex_wrapperes;
//	public:
//		Decorator(std::vector<VertexPtr> vertex_list) {
//			for (auto& itr : vertex_list) {
//				vertex_wrapperes.push_back(VertexWrapper(itr));
//			}
//		}
//		bool get(const VT& data) {
//			for (auto& itr : vertex_wrapperes)
//				if (itr.vertex->operator*() == data) return itr.visit;
//		}
//		void set(const VT& data) {
//			for (auto& itr : vertex_wrapperes) {
//				if (itr.vertex->operator*() == data) {
//					itr.visit = true;
//					break;
//				}
//			}
//		}
//		void Print() {
//			std::cout << "Deco Info" << std::endl;
//			for (auto& itr : vertex_wrapperes)
//				std::cout << itr.vertex->operator*() <<' '<< itr.visit << std::endl;
//			std::cout << "Deco Info End" << std::endl;
//		}
//	};
//public:
//	AdjMatrixUndirectedGraph() :number_of_vertices(0), number_of_edges(0) {
//		adj_matrix_.resize(1004);
//		for (auto& itr : adj_matrix_) {
//			itr.resize(1004, nullptr);
//		}
//		vertex_list_.reserve(1004);
//		edge_list_.reserve(100004);
//	}
//	~AdjMatrixUndirectedGraph() { Clear(); };
//	VertexPtrList Vertices() const { return vertex_list_; }
//	EdgePtrList Edges() const { return edge_list_; }
//	int VerticesNumber() const { return number_of_vertices; }
//	int EdgesNumber() const { return number_of_edges; }
//	typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr InsertVertex(const VT& data);//The arg is a data that will be in vertex.
//	typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr InsertVertexWithIndex(const VT& data,const int& index);//The arg is a data that will be in vertex.
//	typename AdjMatrixUndirectedGraph<VT, ET>::EdgePtr InsertEdge(VertexPtr left_vertex, VertexPtr right_vertex, const ET& data);
//	void EraseVertex(VertexPtr vertex);
//	void EraseEdge(EdgePtr& edge);
//	typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr SearchVertexUsingData(const VT& data) const;
//	typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr SearchVertexUsingIndex(const int& index) const;
//	typename AdjMatrixUndirectedGraph<VT, ET>::EdgePtr SearchEdge(const ET& data) const;
//	typename AdjMatrixUndirectedGraph<VT, ET>::EdgePtrList IncidentEdgeOfVertex(VertexPtr vertex) const;
//	typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtrList IncidentVertexOfVertex(VertexPtr vertex) const;
//	std::vector<VT> DFS(Vertex& start_point) const;
//	std::vector<VT> BFS(Vertex& start_point) const;
//	void Clear();//Clear all lists
//	void PrintInfo() const;
//protected:
//	void DfsUtility(std::vector<VT>& data_list,Decorator& decorator, Vertex& current) const;
//	void BfsUtility(int& level, std::vector<VT>& data_list, Decorator& decorator, VertexPtrList vertex_list) const;
//	void CorrectingIndex();
//private:
//	int number_of_vertices;
//	int number_of_edges;
//	AdjMatrix adj_matrix_;
//	VertexPtrList vertex_list_;
//	EdgePtrList edge_list_;
//};
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr AdjMatrixUndirectedGraph<VT, ET>::InsertVertex(const VT& data)
//{
//	VertexPtr temp = new Vertex{ data, number_of_vertices };
//	vertex_list_.push_back(temp);
//	if (adj_matrix_.capacity() <= number_of_vertices) {
//		adj_matrix_.resize(adj_matrix_.size() * 2);
//		for (auto& itr : adj_matrix_)
//			itr.resize(adj_matrix_.size(), nullptr);
//	}
//	number_of_vertices++;
//	return temp;
//}
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr AdjMatrixUndirectedGraph<VT, ET>::InsertVertexWithIndex(const VT& data, const int& index)
//{
//	VertexPtr temp = new Vertex{ data, index };
//	vertex_list_.push_back(temp);
//	if (adj_matrix_.size() <= number_of_vertices) {
//		adj_matrix_.resize(adj_matrix_.size() * 2);
//		for (auto& itr : adj_matrix_)
//			itr.resize(adj_matrix_.size(), nullptr);
//	}
//	number_of_vertices++;
//	return temp;
//}
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::EdgePtr AdjMatrixUndirectedGraph<VT, ET>::InsertEdge(VertexPtr left_vertex, VertexPtr right_vertex, const ET& data)
//{
//	if (adj_matrix_[left_vertex->index][right_vertex->index] != nullptr)
//		throw std::invalid_argument("There already is edge");
//	EdgePtr temp = new Edge{ left_vertex,right_vertex,data };
//	edge_list_.emplace_back(temp);
//	adj_matrix_[left_vertex->index][right_vertex->index] = temp;
//	adj_matrix_[right_vertex->index][left_vertex->index] = temp;
//	number_of_edges++;
//	left_vertex->incident_edge_list.push_back(temp);
//	right_vertex->incident_edge_list.push_back(temp);
//	return temp;
//}
//
//template<typename VT, typename ET>
//void AdjMatrixUndirectedGraph<VT, ET>::EraseVertex(VertexPtr vertex)
//{
//	EdgePtrList incident_edge_list = IncidentEdgeOfVertex(vertex);
//	for (auto& itr : incident_edge_list) {
//		EraseEdge(itr);
//	}
//	for (int i = 0; i < adj_matrix_.size(); i++) {
//		typename std::vector<EdgePtr>::iterator iter_2 = adj_matrix_[i].begin() + vertex->index;
//		adj_matrix_[i].erase(iter_2);
//	}
//	typename std::vector< std::vector<EdgePtr> >::iterator iter = adj_matrix_.begin() + vertex->index;
//	adj_matrix_.erase(iter);
//	number_of_vertices--;
//	for (size_t i = 0; i < vertex_list_.size(); i++) {
//		if (vertex->data == vertex_list_[i]->data) {
//			typename std::vector<VertexPtr>::iterator iter_2 = vertex_list_.begin() + i;
//			vertex_list_.erase(iter_2);
//			break;
//		}
//	}
//	CorrectingIndex();
//}
//
//template<typename VT, typename ET>
//void AdjMatrixUndirectedGraph<VT, ET>::EraseEdge(EdgePtr& edge)
//{
//	for (size_t i = 0; i < edge_list_.size(); i++) {
//		if (edge_list_[i] == edge) {
//			delete edge_list_[i];
//			typename EdgePtrList::iterator iter = edge_list_.begin() + i;
//			edge_list_.erase(iter);
//			break;
//		}
//	}
//	number_of_edges--;
//}
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr AdjMatrixUndirectedGraph<VT, ET>::SearchVertexUsingData(const VT& data) const
//{
//	for (const auto& itr : vertex_list_) {
//		if (itr->data == data) return itr;
//	}
//	throw std::invalid_argument("There is no such vertex that has the data.");
//}
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtr AdjMatrixUndirectedGraph<VT, ET>::SearchVertexUsingIndex(const int& index) const
//{
//	for (const auto& itr : vertex_list_) {
//		if (itr->index == index) return itr;
//	}
//	throw std::invalid_argument("Error!");
//}
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::EdgePtr AdjMatrixUndirectedGraph<VT, ET>::SearchEdge(const ET& data) const
//{
//	for (const auto& itr : edge_list_) {
//		if (itr->data == data) return itr;
//	}
//	throw std::invalid_argument("There is no such edge that has the data.");
//}
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::EdgePtrList AdjMatrixUndirectedGraph<VT, ET>::IncidentEdgeOfVertex(VertexPtr vertex) const
//{
//	EdgePtrList temp;
//	for (auto& itr : adj_matrix_[vertex->index]) {
//		if (itr != nullptr || itr != EdgePtr()) {
//			temp.push_back(itr);
//		}
//	}
//	return temp;
//}
//
//template<typename VT, typename ET>
//typename AdjMatrixUndirectedGraph<VT, ET>::VertexPtrList AdjMatrixUndirectedGraph<VT, ET>::IncidentVertexOfVertex(VertexPtr vertex) const
//{
//	EdgePtrList temp_edge_list = IncidentEdgeOfVertex(vertex);
//	VertexPtrList temp;
//	for (auto& itr : temp_edge_list) {
//		if (itr->left_vertex == vertex) temp.push_back(itr->right_vertex);
//		else temp.push_back(itr->left_vertex);
//	}
//	return temp;
//}
//
//template<typename VT, typename ET>
//std::vector<VT> AdjMatrixUndirectedGraph<VT, ET>::DFS(Vertex& start_point) const
//{
//	std::vector<VT> temp;
//	Decorator decorator(vertex_list_);
//	DfsUtility(temp, decorator, start_point);
//	return temp;
//
//}
//
//template<typename VT, typename ET>
//std::vector<VT> AdjMatrixUndirectedGraph<VT, ET>::BFS(Vertex& start_point) const
//{
//	std::vector<VT> temp;
//	Decorator decorator(vertex_list_);
//	decorator.set(start_point.operator*());
//	std::vector<VertexPtr> start;
//	start.push_back(&start_point);
//	int level = 0;
//	BfsUtility(level ,temp, decorator, start);
//	return temp;
//}
//
//template<typename VT, typename ET>
//void AdjMatrixUndirectedGraph<VT, ET>::Clear()
//{
//	for (auto& itr : edge_list_)
//		delete itr;
//	edge_list_.clear();
//	for (auto& itr : adj_matrix_)
//		itr.clear();
//	adj_matrix_.clear();
//	for (auto& itr : vertex_list_)
//		delete itr;
//	vertex_list_.clear();
//
//}
//
//template<typename VT, typename ET>
//void AdjMatrixUndirectedGraph<VT, ET>::PrintInfo() const
//{
//	std::cout << "Vertices Info" << std::endl;
//	for (const auto& itr : vertex_list_)
//		std::cout << "Index : " << itr->index << " | Data : " << itr->data << std::endl;
//	std::cout << std::endl;
//	std::cout << "Edge Info" << std::endl;
//	for (const auto& itr : edge_list_)
//		std::cout << "Left : " << itr->left_vertex->data << " | Right : " << itr->right_vertex->data << " | Data : " << itr->data << std::endl;
//	std::cout << std::endl;
//	std::cout << "Matrix Info" << std::endl;
//	for (const auto& itr : adj_matrix_) {
//		for (const auto& itr_2 : itr) {
//			if (itr_2 == nullptr) std::cout << "O ";
//			else std::cout << "Y ";
//		}
//		std::cout << std::endl;
//	}
//}
//
//template<typename VT, typename ET>
//void AdjMatrixUndirectedGraph<VT, ET>::DfsUtility(std::vector<VT>& data_list, Decorator& decorator, Vertex& current) const
//{
//	std::cout << current.operator*();
//	data_list.push_back(current.operator*());
//	VertexPtr temp = &current;
//	decorator.set(current.operator*());
//	VertexPtrList incident_vertex_list = IncidentVertexOfVertex(temp);
//	for (auto& itr : incident_vertex_list) {
//		if (decorator.get(itr->operator*()) == false)
//			DfsUtility(data_list,decorator, *itr);
//	}
//	
//}
//
//template<typename VT, typename ET>
//void AdjMatrixUndirectedGraph<VT, ET>::BfsUtility(int& level ,std::vector<VT>& data_list,Decorator& decorator,VertexPtrList vertex_list) const
//{
//	VertexPtrList next_vertex_list;
//	for (auto& itr : vertex_list) {
//		std::cout << itr->operator*() << ' ';
//	}
//	for (auto& itr : vertex_list) {
//		EdgePtrList incident_edge_list = itr->incident_edge_list;
//		for (auto& itr_2 : incident_edge_list) {
//			if (itr_2->left_vertex == itr && decorator.get(itr_2->right_vertex->operator*()) == false) { 
//				next_vertex_list.push_back(itr_2->right_vertex); 
//				decorator.set(itr_2->right_vertex->operator*());
//			}
//			else if (itr_2->right_vertex == itr && decorator.get(itr_2->left_vertex->operator*()) == false) { 
//				next_vertex_list.push_back(itr_2->left_vertex); 
//				decorator.set(itr_2->left_vertex->operator*());
//			}
//		}
//	}
//	if (next_vertex_list.empty()) return;
//	BfsUtility(++level, data_list, decorator, next_vertex_list);
//}
//
//template<typename VT, typename ET>
//void AdjMatrixUndirectedGraph<VT, ET>::CorrectingIndex()
//{
//	for (int i = 0; i < vertex_list_.size(); i++)
//		vertex_list_[i]->set_index(i);
//}
//
//int main() {
//	AdjMatrixUndirectedGraph<int,int> my_graph;
//	int vertex_num, edge_num, start_num;
//	std::cin >> vertex_num >> edge_num >> start_num;
//	for (int i = 0; i < vertex_num; i++) {
//		int temp_int;
//		std::cin >> temp_int;
//		my_graph.InsertVertex(temp_int);
//	}
//	for (int i = 0; i < edge_num; i++) {
//		int a, b;
//		std::cin >> a >> b;
//		try
//		{
//			my_graph.InsertEdge(my_graph.SearchVertexUsingData(a), my_graph.SearchVertexUsingData(b), i);
//		}
//		catch (const std::exception&)
//		{
//			std::cout << "-1" << std::endl;
//		}
//	}
//	my_graph.BFS(*my_graph.SearchVertexUsingData(start_num));
//
//}