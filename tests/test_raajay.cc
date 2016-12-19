#include "ps/ps.h"
using namespace ps;

void StartServer() {
  if (!IsServer()) return;
  auto server = new KVServer<float>(0);
  server->set_request_handle(KVServerDefaultHandle<float>());
  RegisterExitCallback([server](){ delete server; });
}

void RunWorker() {
  if (!IsWorker()) {
      return;
  }
  LL << "Create a KVWorker";
  KVWorker<float> kv(0);


  // initialize key and value pairs
  int num = 10000;
  std::vector<Key> keys(num);
  std::vector<float> vals(num);

  int rank = MyRank();
  srand(rank + 7); // set some random seed

  for (int i = 0; i < num; ++i) {
    keys[i] = kMaxKey / num * i + rank;
    vals[i] = (rand() % 1000);
  }

  // push
  int repeat = 50;
  std::vector<int> ts; // an array to hold the time stamps of the push operations
  for (int i = 0; i < repeat; ++i) {
    // why are we pushing the same keys and values 50 times?
    ts.push_back(kv.Push(keys, vals));
    // to avoid too frequency push, which leads huge memory usage
    if (i > 10) kv.Wait(ts[ts.size()-10]);
  }
  // Waiting for all pushes to complete
  for (int t : ts) {
      LL << "RAAJAY: Waiting for " << t;
      kv.Wait(t);
  }


  // pull the values for a given set of keys
  std::vector<float> rets; // a placeholder to store the pulled values
  kv.Wait(kv.Pull(keys, &rets));

  // See if the values is the pulled value is the same as the pushed value
  // times the number times the values were pushed.
  float res = 0;
  for (int i = 0; i < num; ++i) {
    res += fabs(rets[i] - vals[i] * repeat);
  }

  CHECK_LT(res / repeat, 1e-5);
  LL << "error: " << res / repeat;
}

int main(int argc, char *argv[]) {
  // setup server nodes
  StartServer();
  // start system
  Start();
  // run worker nodes
  RunWorker();
  // stop system
  Finalize();
  return 0;
}
