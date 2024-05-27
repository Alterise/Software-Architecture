## With caching

```
Running 10s test @ http://localhost:8080
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.92ms    2.25ms  59.28ms   99.09%
    Req/Sec   561.18     51.94   626.00     84.80%
  Latency Distribution
     50%    1.72ms
     75%    1.92ms
     90%    2.18ms
     99%    3.93ms
  55897 requests in 10.01s, 13.98MB read
Requests/sec:   5585.99
Transfer/sec:      1.40MB
```

## Without caching

```
Running 10s test @ http://localhost:8080
  10 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     2.73ms    2.11ms  56.10ms   98.99%
    Req/Sec   382.99     33.66   530.00     89.50%
  Latency Distribution
     50%    2.54ms
     75%    2.71ms
     90%    2.95ms
     99%    3.83ms
  38143 requests in 10.01s, 9.55MB read
Requests/sec:   3811.29
Transfer/sec:      0.95MB
```