#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

unsigned int MurMurHash(const void *key, int len) {
  const unsigned int m = 0x5bd1e995;
  const int r = 24;
  const int seed = 97;
  unsigned int h = seed ^ len;
  // Mix 4 bytes at a time into the hash
  const unsigned char *data = (const unsigned char *)key;
  while (len >= 4) {
    unsigned int k = *(unsigned int *)data;
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
    data += 4;
    len -= 4;
  }
  // Handle the last few bytes of the input array
  switch (len) {
  case 3:
    h ^= data[2] << 16;
  case 2:
    h ^= data[1] << 8;
  case 1:
    h ^= data[0];
    h *= m;
  };
  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}

class vnode {
public:
  string vnode_ip;
  unsigned int vnode_hash;
  int rnode_index;
  int vnode_id_unique;
  vnode(string ip, unsigned int hash, int i, int id) {
    vnode_ip = ip;
    vnode_hash = hash;
    rnode_index = i;
    vnode_id_unique = id;
  }

  bool operator<(const vnode &v) { return vnode_hash < v.vnode_hash; }
  bool operator==(const vnode &v) {
    return vnode_id_unique == v.vnode_id_unique;
  }
};

class rnode {
public:
  string rnode_ip;
  int rnode_id;
  vector<vnode> vnodes;
  rnode(string ip, int i) {
    rnode_ip = ip;
    rnode_id = i;
  }
};

int find_nearest_node(unsigned int hash_value,
                      vector<vnode> &list) { // find the nearest virtual node
                                             // for given hash position
  int low = 0;
  int high = list.size() - 1;
  int mid;
  if (hash_value > list[high].vnode_hash) {
    return 0;
  }
  while (low < high) {
    mid = (low + high) / 2;
    if (list[mid].vnode_hash == hash_value) {
      return mid;
    } else if (list[mid].vnode_hash > hash_value) {
      high = mid;
    } else { // this->sorted_node_hash_list[mid]<data_hash
      low = mid + 1;
    }
  }
  return low;
}

void trace_process(string in_path, string ref_path, string out_path,
                   int rnode_num, int vnode_num, int from_rnode, int to_rnode,
                   double ratio) {
  vector<rnode> rnodes;
  int ip_seg_3 = 0;
  int ip_seg_4 = 136;
  int v_id = 0;
  vector<vnode> hash_order_vnode_list;
  for (int i = 0; i < rnode_num; i++) {
    string tmp_ip_r =
        "192.168.0" + to_string(ip_seg_3) + "." + to_string(ip_seg_4);
    rnode node_r(tmp_ip_r, i);
    for (int j = 0; j < vnode_num; j++) {
      string tmp_ip_v = tmp_ip_r + ":" + to_string(j + 1);
      unsigned int tmp_hash_v = MurMurHash(tmp_ip_v.c_str(), tmp_ip_v.length());
      vnode node_v(tmp_ip_v, tmp_hash_v, i, v_id++);
      node_r.vnodes.push_back(node_v);
      hash_order_vnode_list.push_back(node_v);
    }
    rnodes.push_back(node_r);
    ip_seg_3++;
    ip_seg_4++;
  }

  sort(hash_order_vnode_list.begin(), hash_order_vnode_list.end());

  map<long long, int> requestId_to_vnodeId;
  map<long long, int> requestId_to_rnodeId;
  vector<vector<long long>> rnode_request;
  vector<vector<long long>> vnode_request;
  for (int i = 0; i < rnode_num; i++) {
    vector<long long> v;
    rnode_request.push_back(v);
  }
  for (int i = 0; i < vnode_num * rnode_num; i++) {
    vector<long long> v;
    vnode_request.push_back(v);
  }
  int request_num = 0;

  ifstream input;
  ofstream output;
  input.open(in_path);
  long long time, id, size;
  while (input >> time >> id >> size) {
    request_num++;
    string request = to_string(id);
    unsigned request_hash = MurMurHash(request.c_str(), request.length());
    unsigned int vnode_index =
        find_nearest_node(request_hash, hash_order_vnode_list);
    vnode_index++;
    if (vnode_index >= hash_order_vnode_list.size()) {
      vnode_index = 0;
    }
    vnode node = hash_order_vnode_list[vnode_index];
    requestId_to_vnodeId.insert(pair<long long, int>(id, node.vnode_id_unique));
    requestId_to_rnodeId.insert(pair<long long, int>(id, node.rnode_index));
    rnode_request[node.rnode_index].push_back(id);
    vnode_request[node.vnode_id_unique].push_back(id);
  }
  for (int i = 0; i < rnode_num; i++) {
    cout << rnode_request[i].size() << endl;
  }

  input.close();

  /*get request id from original trace, but works unwell to hit rate*/
  /*
  while (input >> time >> id >> size) {
      if (requestId_to_rnodeId[id] == from_rnode && requestId_to_vnodeId[id] <
  vnode_num * requestId_to_rnodeId[id] + move_num) {
          int pos = rand() % rnode_request[to_rnode].size();
          id = rnode_request[to_rnode][pos];
      }
      output << time << " " << id << " " << size << endl;
  }
  */

  /*get request id from ref_trace£¬ 2 options*/
  vector<vector<long long>> ref_rnode_request;
  vector<vector<long long>> ref_vnode_request;
  for (int i = 0; i < rnode_num; i++) {
    vector<long long> v;
    ref_rnode_request.push_back(v);
  }
  for (int i = 0; i < vnode_num * rnode_num; i++) {
    vector<long long> v;
    ref_vnode_request.push_back(v);
  }
  input.open(ref_path);
  while (input >> time >> id >> size) {
    string request = to_string(id);
    unsigned request_hash = MurMurHash(request.c_str(), request.length());
    unsigned int vnode_index =
        find_nearest_node(request_hash, hash_order_vnode_list);
    vnode_index++;
    if (vnode_index >= hash_order_vnode_list.size()) {
      vnode_index = 0;
    }
    vnode node = hash_order_vnode_list[vnode_index];
    ref_rnode_request[node.rnode_index].push_back(id);
    ref_vnode_request[node.vnode_id_unique].push_back(id);
  }
  input.close();

  input.open(in_path);
  output.open(out_path);
  int move_num = (int)vnode_num * ratio;
  /*1.get request id from rnode, don't care vnode*/
  /*
  while (input >> time >> id >> size) {
      if (requestId_to_rnodeId[id] == from_rnode && requestId_to_vnodeId[id] <
  vnode_num * requestId_to_rnodeId[id] + move_num) {
          if(ref_rnode_request[to_rnode].size() > 0){
              int pos = rand() % ref_rnode_request[to_rnode].size();
              id = ref_rnode_request[to_rnode][pos];
          }
      }
      output << time << " " << id << " " << size << endl;
  }
  */

  /*2.get request id from vnode, vnode to vnode*/
  while (input >> time >> id >> size) {
    if (requestId_to_rnodeId[id] == from_rnode &&
        requestId_to_vnodeId[id] <
            vnode_num * requestId_to_rnodeId[id] + move_num) {
      int orig_vId = requestId_to_vnodeId[id];
      int tmp_vId = orig_vId + vnode_num * (to_rnode - from_rnode);
      if (ref_vnode_request[tmp_vId].size() > 0) {
        int pos = rand() % ref_vnode_request[tmp_vId].size();
        id = ref_vnode_request[tmp_vId][pos];
      }
    }
    output << time << " " << id << " " << size << endl;
  }
  /*  batch process  */
  /*
  from_rnode = 0;
  to_rnode = 1;
  int batch_size = 50000;
  int counter = 0;
  while (input >> time >> id >> size) {
      counter++;
      if (counter % batch_size == 0) {
          from_rnode = (from_rnode + 1) % rnode_num;
          to_rnode = (to_rnode + 1) % rnode_num;
      }
      if (requestId_to_rnodeId[id] == from_rnode && requestId_to_vnodeId[id] <
  vnode_num * requestId_to_rnodeId[id] + move_num) {
          int orig_vId = requestId_to_vnodeId[id];
          int tmp_vId = orig_vId + vnode_num * (to_rnode - from_rnode);
          if (ref_vnode_request[tmp_vId].size() > 0) {
              int pos = rand() % ref_vnode_request[tmp_vId].size();
              id = ref_vnode_request[tmp_vId][pos];
          }
      }
      output << time << " " << id << " " << size << endl;
  }
  */
  input.close();
  output.close();
}

int main(int argc, char *argv[]) {
  const string in_path = argv[1];
  const string ref_path = argv[2];
  const string out_path = argv[3];
  int rnode_num = atoi(argv[4]);
  int vnode_num = atoi(argv[5]);
  int from_node = atoi(argv[6]);
  int to_node = atoi(argv[7]);
  double ratio = atof(argv[8]);
  trace_process(in_path, ref_path, out_path, rnode_num, vnode_num, from_node,
                to_node, ratio);
}
