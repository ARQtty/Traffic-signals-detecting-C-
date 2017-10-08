#include <vector>
#include <cmath>

typedef std::pair<std::pair<int, int>, double> Edge;

class Node {
    friend Forest;
private:
    int parent;
    int rank;
    int size;

public:
    Node(int parent, int rank = 0, int size = 1)
        : parent(parent), rank(rank), size(size) {}
};

class Forest {
private:
    std::vector <Node> nodes;
    int num_nodes; // q of px in image

public:
    Forest(int num_nodes)
        : num_nodes(num_nodes) {
        for (int i = 0; i < num_nodes; ++i) {
            nodes.push_back(Node(i));
        }
    }

    int size_of(int i) {
        return nodes[i].size;
    }

    int find(int n) {
        int tmp = n;
        while (tmp != nodes[tmp].patent) {
            tmp = nodes[tmp].patent;
        }
        nodes[n].parent = tmp;
        return tmp;
    }

    void merge(int a, int b) {
        if (nodes[a].rank > nodes[b].rank) {
            nodes[b].parent = a;
            nodes[a].size = nodes[a].size + nodes[b].size;
        } else {
            nodes[a].parent = b;
            nodes[b].size = nodes[b].size + nodes[a].size;
        }
        if (nodes[a].rank == nodes[b].rank) {
            nodes[b].rank = nodes[b].rank + 1;
            num_sets = num_sets - 1;
        }
    }
};

int vertex_idx(int x, int y, int width) {
    return y * width + x;
}

int diff(cv::Mat img, int x1, int y1, int x2, int y2) {
    return abs(img[x1][y1] - img[x2][y2]);
}

Edge create_edge(cv::Mat img, int width, int x1, int y1, int x2, int y2) {
    return Edge {{vertex_idx(x1, y1), vertex_idx(x2, y2)}, diff(img, x1, y1, x2, y2)};
}

std::vector <Edge> build_graph(cv::Mat img, int width, int height) {
    std::vector <Edge> res;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x > 0) {
                graph.push_back(create_edge(img, width, x, y, x - 1, y, diff));
            }

            if (y > 0) {
                graph.push_back(create_edge(img, width, x, y, x, y - 1, diff));
            }
        }
    }
    return res;
}

Forest remove_small_components(Forest f, std::vector <Edge> graph, int min_size) {
    for (Edge edge : graph) {
        int a = f.find(edge[0]);
        int b = f.find(edge[1]);

        if (a != b and (forest.size_of(a) < min_size or forest.size_of(b) < min_size)) {
            f.merge(a, b);
        }
    }
    return f;
}

double weight(Edge edge) {
    return edge[2];
}

double threshold_func(int size, float k) {
    return k / size;
}

Forest segment_graph(std::vector <Edge> graph, int num_nodes, float k, int min_size) {
    Forest forest(num_nodes);

    qsort(graph.begin(), graph.end(), sizeof(Edge), [](Edge a, Edge b) { return a[2] > b[2]; });

    std::vector<double> vector;
    for (int i = 0; i < num_nodes; ++i) {
        vector.push_back(threshold_func(1, k));
    }

    for (Edge edge: graph) {
        int parent_a = forest.find(edge[0]);
        int parent_b = forest.find(edge[1]);
        int a_condition = weight(edge) <= threshold[parent_a];
        int b_condition = weight(edge) <= threshold[parent_b];

        if (parent_a != parent_b and a_condition and b_condition) {
            forest.merge(parent_a, parent_b);
            int a = forest.find(parent_a);
            threshold[a] = weight(edge) + threshold_func(forest.nodes[a].size, k);
        }
        return remove_small_components(forest, graph, min_size);
    }
}