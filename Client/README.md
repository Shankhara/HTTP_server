# HTTP CLIENT

**Install Go**

[golang.org/doc/install](https://golang.org/doc/install)


**Build**

``go build bench.go``

**Usage**

``./bench -h``

`Usage of ./bench:`

`  -c int`
`    	number of concurrent clients (default 5)`

`  -k	reuse tcp connection`

`  -n int`
`    	number of requests performed by each client (default 1)`

`  -s int`
`    	slowmode: every char is sent with <slowmode> in MS delay`

` -u string`
`    	URL (default "http://127.0.0.1")`

`  -v	verbose`


**Example**


Bench 200 clients perfoming 5 requests each

``
./bench -c 200 -n 5 -u=hostname:8080
``

Slowmode with 5 clients (adding 200ms between each character sent)

``
./bench -c 5 -n 5 -u localhost:8080 -s 200 -v
``