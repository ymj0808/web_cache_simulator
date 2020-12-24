#include<map>
#include<string>
#include<fstream>
#include<iostream>
using namespace std;

void trace_clean(string in_path, string out_path, int threshold) {
	ifstream input(in_path, ios::in);
	ofstream output(out_path, ios::out);
	map<long long, pair<int, int>> counter; // id, <count, size>
	int time, id, size;
	int before_num = 0;
	int after_num = 0;
	while (input >> time >> id >> size) {
		before_num++;
		auto iter = counter.find(id);
		if (iter != counter.end()) {
			iter->second.first++;
			if (iter->second.first >= threshold) {
				output << id << " " << iter->second.second << endl;
				after_num++;
			}
		}
		else {
			counter[id] = make_pair(1, size);
			if (threshold == 0) {
				output << id << " " << size << endl;
				after_num++;
			}
		}
	}
	input.close();
	output.close();
	cout << "before_num: " << before_num << "  after_num: " << after_num << endl;
}

int main(int argc, char* argv[]) {
	const string in_path = argv[1];
	const string out_path = argv[2];
	int threshold = atoi(argv[3]);
	trace_clean(in_path, out_path, threshold);
}