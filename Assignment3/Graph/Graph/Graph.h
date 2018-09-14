//
//  Graph.h
//  Graph
//
//  Created by Ethan Xu on 12/9/17.
//  Copyright © 2017 Ethan Xu. All rights reserved.
//

#ifndef Graph_h
#define Graph_h

#include <set>
#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace gdwg {
    
    template <typename N, typename E>
    class Graph {
        
    private:
        class Node {
            friend class Edge;
            friend Graph;
            
        private:
            N val;
            unsigned outDegree;
            
        public:
            Node() {}
            
            Node(const N& val) {
                this->val = val;
                outDegree = 0;
            }
            
            Node(const N& val, unsigned outDegree) {
                this->val = val;
                this->outDegree = outDegree;
            }
            
            bool operator<(const Node& n) const {
                return val < n.val;
            }
        };
        
        class Edge {
            friend Graph;
            
        private:
            N startVal;
            N endVal;
            E w;
            
        public:
            Edge() {}
            
            Edge(const N& startVal, const N& endVal, const E& w) {
                this->startVal = startVal;
                this->endVal = endVal;
                this->w = w;
            }
            
            bool operator<(const Edge& edge) const {
                return std::tie(startVal, endVal, w) < std::tie(edge.startVal, edge.endVal, edge.w);
            }
        };
        
        std::set<Node> nodes;
        std::set<Edge> edges;
        typename std::set<Node>::iterator gIterator;
        
        Node updateNodeOutDegree(const typename std::set<Node>::iterator& it, unsigned outDegree) {
            Node newNode = Node(it->val, outDegree);
            nodes.erase(it);
            nodes.insert(newNode);
            return newNode;
        }
        
        Node updateNodeVal(const typename std::set<Node>::iterator& it, const N& val) {
            Node newNode = Node(val, it->outDegree);
            nodes.erase(it);
            nodes.insert(newNode);
            return newNode;
        }
        
    public:
        Graph() {};
        Graph(const Graph&);
        Graph(Graph&&);
        Graph& operator=(const Graph& g);
        Graph& operator=(Graph&& g);
        bool addNode(const N&);
        bool addEdge(const N&, const N&, const E&);
        bool replace(const N&, const N&);
        void mergeReplace(const N&, const N&);
        void deleteNode(const N&) noexcept;
        void deleteEdge(const N&, const N&, const E&) noexcept;
        void clear() noexcept;
        bool isNode(const N&) const;
        bool isConnected(const N&, const N&) const;
        void printNodes() const;
        void printEdges(const N&) const;
        void begin() const;
        bool end() const;
        void next() const;
        const N& value() const;
    };
    
    template <typename N, typename E>
    Graph<N, E>::Graph(const Graph& g) {
        for (auto node: g.nodes) {
            nodes.insert(Node(node.val, node.outDegree));
        }
        for (auto edge: edges) {
            edges.insert(Edge(edge.startVal, edge.endVal, edge.w));
        }
    };
    
    template <typename N, typename E>
    Graph<N, E>::Graph(Graph&& g) {
        nodes = std::move(g.nodes);
        edges = std::move(g.edges);
    };
    
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(const Graph& g) {
        for (auto node: g.nodes) {
            nodes.insert(Node(node.val, node.outDegree));
        }
        for (auto edge: edges) {
            edges.insert(Edge(edge.startVal, edge.endVal, edge.w));
        }
        return *this;
    };
    
    template <typename N, typename E>
    Graph<N, E>& Graph<N, E>::operator=(Graph&& g) {
        nodes = std::move(g.nodes);
        edges = std::move(g.edges);
        return *this;
    };
    
    template <typename N, typename E>
    bool Graph<N, E>::isNode(const N& val) const {
        return nodes.find(Node(val)) != nodes.end();
    };
    
    template <typename N, typename E>
    bool Graph<N, E>::addNode(const N& val) {
        Node node = Node(val);
        if (nodes.find(node) != nodes.end()) {
            return false;
        } else {
            nodes.insert(node);
            return true;
        }
    };
    
    template <typename N, typename E>
    void Graph<N, E>::printNodes() const {
        struct outDegreeComparator {
            bool operator()(const Node& a, const Node& b) const {
                if (a.outDegree != b.outDegree) {
                    return a.outDegree < b.outDegree;
                } else {
                    return a.val < b.val;
                }
            }
        };
        std::set<Node, outDegreeComparator> sortedByOutDegreeNodes;
        for (auto node: nodes) {
            sortedByOutDegreeNodes.insert(node);
        }
        for (auto node: sortedByOutDegreeNodes) {
            std::cout << node.val << std::endl;
        }
    }
    
    template <typename N, typename E>
    bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
        typename std::set<Node>::iterator srcIt = nodes.find(Node(src));
        if (srcIt == nodes.end()) {
            throw std::runtime_error("Start node does not exist in this graph!");
        }
        if (!isNode(dst)) {
            throw std::runtime_error("End node does not exist in this graph!");
        }
        Edge targetEdge = Edge(src, dst, w);
        if (edges.find(targetEdge) == edges.end()) {
            updateNodeOutDegree(srcIt, srcIt->outDegree + 1);
            edges.insert(targetEdge);
            return true;
        } else {
            return false;
        }
    }
    
    template <typename N, typename E>
    void Graph<N, E>::printEdges(const N& val) const {
        typename std::set<Node>::iterator srcIt = nodes.find(Node(val));
        if (srcIt == nodes.end()) {
            throw std::runtime_error("Start node does not exist in this graph!");
        }
        std::cout << "Edges attached to Node " << srcIt->val << std::endl;
        if (srcIt->outDegree == 0) {
            std::cout << "(null)" << std::endl;
        } else {
            struct edgeComparator {
                bool operator()(const Edge& a, const Edge& b) const {
                    if (a.w != b.w) {
                        return a.w < b.w;
                    } else {
                        return std::tie(a.startVal, a.endVal) < std::tie(b.startVal, b.endVal);
                    }
                }
            };
            std::set<Edge, edgeComparator> sortedByWeightEdges;
            for (auto edge: edges) {
                if (edge.startVal == val) {
                    sortedByWeightEdges.insert(edge);
                }
            }
            for (auto edge: sortedByWeightEdges) {
                std::cout << edge.endVal << " " <<edge.w << std::endl;
            }
        }
    }
    
    template <typename N, typename E>
    bool Graph<N, E>::isConnected(const N& src, const N& dst) const {
        if (!isNode(src)) {
            throw std::runtime_error("Start node does not exist in this graph!");
        }
        if (!isNode(dst)) {
            throw std::runtime_error("End node does not exist in this graph!");
        }
        for (auto edge: edges) {
            if (edge.startVal == src && edge.endVal == dst) {
                return true;
            }
        }
        return false;
    }
    
    template <typename N, typename E>
    bool Graph<N, E>::replace(const N& oldData, const N& newData) {
        typename std::set<Node>::iterator oldIt = nodes.find(Node(oldData));
        if (oldIt == nodes.end()) {
            throw std::runtime_error("The node containing the old data does not exist in this graph!");
        }
        if (isNode(newData)) {
            return false;
        } else {
            updateNodeVal(oldIt, newData);
            for (auto edge: edges) {
                if (edge.startVal == oldData) {
                    edge.startVal = newData;
                }
                if (edge.endVal == oldData) {
                    edge.endVal = newData;
                }
            }
            return true;
        }
    }
    
    template <typename N, typename E>
    void Graph<N, E>::deleteNode(const N& val) noexcept {
        typename std::set<Node>::iterator nodeIt = nodes.find(Node(val));
        if (nodeIt != nodes.end()) {
            typename std::set<Edge>::iterator edgeIt = edges.begin();
            while (edgeIt != edges.end()) {
                if (edgeIt->startVal == val || edgeIt->endVal == val) {
                    std::cout<<edgeIt->startVal<<"口"<<edgeIt->endVal<<std::endl;
                    if (edgeIt->endVal == val) {
                        typename std::set<Node>::iterator updateNodeIt = nodes.find(Node(edgeIt->startVal));
                        updateNodeOutDegree(updateNodeIt, updateNodeIt->outDegree - 1);
                    }
                    edges.erase(edgeIt++);
                } else {
                    edgeIt++;
                }
            }
            nodes.erase(nodeIt);
        }
    }
    
    template <typename N, typename E>
    void Graph<N, E>::deleteEdge(const N& src, const N& dst, const E& w) noexcept {
        typename std::set<Edge>::iterator edgeIt = edges.find(Edge(src, dst, w));
        if (edgeIt != edges.end()) {
            edges.erase(edgeIt);
            typename std::set<Node>::iterator nodeIt = nodes.find(Node(src));
            updateNodeOutDegree(nodeIt, nodeIt->outDegree - 1);
        }
    }
    
    template <typename N, typename E>
    void Graph<N, E>::mergeReplace(const N& oldData, const N& newData) {
        typename std::set<Node>::iterator oldIt = nodes.find(Node(oldData));
        if (oldIt == nodes.end()) {
            throw std::runtime_error("The node containing the old data does not exist in this graph!");
        }
        if (!isNode(newData)) {
            throw std::runtime_error("The node containing the new data does not exist in this graph!");
        }
        //reconnect edges
        std::vector<Edge> toAddEdges;
        typename std::set<Edge>::iterator edgeIt = edges.begin();
        while (edgeIt != edges.end()) {
            if ((edgeIt->startVal == oldData && edgeIt->endVal == newData) || (edgeIt->startVal == newData && edgeIt->endVal == oldData)) {
                toAddEdges.push_back(Edge(newData, newData, edgeIt->w));
                edges.erase(edgeIt++);
            } else {
                Edge targetEdge;
                bool hit = false;
                if (edgeIt->startVal == oldData) {
                    targetEdge = Edge(newData, edgeIt->endVal, edgeIt->w);
                    hit = true;
                } else if (edgeIt->endVal == oldData) {
                    targetEdge = Edge(edgeIt->startVal, newData, edgeIt->w);
                    hit = true;
                }
                if (hit) {
                    if (edges.find(targetEdge) == edges.end()) {
                        toAddEdges.push_back(targetEdge);
                    }
                    edges.erase(edgeIt++);
                } else {
                    edgeIt++;
                }
            }
        }
        for (auto edge: toAddEdges) {
            edges.insert(edge);
        }
        
        //reset nodes outDegree
        nodes.erase(oldIt);
        for (auto node: nodes) {
            node.outDegree = 0;
        }
        for (auto edge: edges) {
            typename std::set<Node>::iterator nodeIt = nodes.find(Node(edge.startVal));
            updateNodeOutDegree(nodeIt, nodeIt->outDegree + 1);
        }
    }
    
    template <typename N, typename E>
    void Graph<N, E>::begin() const {
        gIterator = nodes.begin();
    }
    
    template <typename N, typename E>
    bool Graph<N, E>::end() const {
        return gIterator == nodes.cend();
    }
    
    template <typename N, typename E>
    void Graph<N, E>::next() const {
        gIterator++;
    }
    
    template <typename N, typename E>
    const N& Graph<N, E>::value() const {
        return gIterator->val;
    }
    
    //#include "Graph.tem"
}

#endif /* Graph_h */
