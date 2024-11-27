#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include<algorithm>
#include<cmath>
#define GREEN(STRING) std::cout<<"\033[32m"<<STRING<<"\033[m"

using namespace std;

struct map {
	int length;
	int width;
	int** data;
};

struct Node {
	Node* parent;
	pair<int, int> pos;
	int g;
	int h;
	bool status=true;
	int f() const{
		return g + h;
	}
	Node(bool t) {
		this->status = t;
	}
	Node(int g, int h, pair<int, int> pos, Node* parent=nullptr) {
		this->g = g;
		this->h = h;
		this->pos = pos;
		this->parent = parent;
	}
	bool operator<(Node const& t) const {
		return f() > t.f();
	}
};

pair<int, int> startPoint;
pair<int, int> endPoint;
int dx[4] = { 0,0,-1,1 };
int dy[4] = { 1,-1,0,0 };
map mainMap;

map createMap(int length, int width, double p) {
	random_device rd;
	mt19937 gen(rd());
	bernoulli_distribution dis(p);
	int** data;
	data = new int*[length];
	for (int i = 0; i < length; i++) {
		data[i] = new int[width];
	}
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			if (dis(gen)) data[i][j] = 1;
			else data[i][j] = 0;
		}
	}
	map m;
	m.data = data;
	m.length = length;
	m.width = width;
	return m;
}

void displayMap(map m) {
	for (int i = 0; i < m.length; i++) {
		for (int j = 0; j < m.width; j++) {
			cout << m.data[i][j] << " ";
		}
		cout << endl;
	}
}

int calculateH(pair<int, int> pos) {
	return abs(endPoint.first - pos.first) + abs(endPoint.second - pos.second);
}

bool isValid(int x, int y) {
	return x >= 0 && x < mainMap.length && y >= 0 && y < mainMap.width && mainMap.data[x][y] == 0;
}

Node findRoute(map m) {
	priority_queue<Node> box;
	vector<vector<bool>>closedMap;
	closedMap.resize(m.length);
	for (int i = 0; i < m.length; i++) {
		closedMap[i].resize(m.width);
	}
	for (int i = 0; i < m.length; i++) {
		for (int j = 0; j < m.width; j++) {
			closedMap[i][j] = false;
		}
	}
	box.emplace(0, calculateH(startPoint), startPoint, nullptr);
	while (!box.empty()&&mainMap.data[startPoint.first][startPoint.second]!=1) {
		Node current = box.top();
		box.pop();
		if (current.pos.first == endPoint.first && current.pos.second == endPoint.second) return current;
		if (closedMap[current.pos.first][current.pos.second]) continue;
		closedMap[current.pos.first][current.pos.second] = true;
		for (int i = 0; i < 4; i++) {
			int newX = current.pos.first+dx[i];
			int newY = current.pos.second+dy[i];
			pair<int, int> newPos;
			newPos.first = newX;
			newPos.second = newY;
			if (isValid(newX, newY) && !closedMap[newX][newY]) {
				box.emplace(current.g + 1, calculateH(newPos), newPos, new Node(current));
			}
		}
	}
	Node empty(false);
	return empty;
}

void displayRoute(vector<pair<int, int>> route) {
	if (route.empty()) cout << "can not find route";
	else {
		for (int i = 0; i < mainMap.length; i++) {
			for (int j = 0; j < mainMap.width; j++) {
				if (mainMap.data[i][j] == 1) GREEN("# ");
				else if (find(route.begin(), route.end(), make_pair(i, j)) != route.end()) cout << "★" << " ";
				else cout << "  ";
			}
			cout << endl;
		}
	}
}

void reconstructRoute(Node current,vector<pair<int,int>>& route) {
	if (current.status) {
		while (current.parent != nullptr) {
			route.push_back(current.pos);
			current = *current.parent;
		}
		route.push_back(make_pair(0, 0));
		displayRoute(route);
	}
	else cout << "can not find route";
}

int main() {
	mainMap = createMap(30, 30, 0.25);
	displayMap(mainMap);
	startPoint = make_pair(0, 0);
	endPoint = make_pair(28, 18);
	Node route = findRoute(mainMap);
	vector<pair<int, int>> path;
	reconstructRoute(route, path);
	return 0;
}