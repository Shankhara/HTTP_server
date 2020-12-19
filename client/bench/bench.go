package main

import (
	"bufio"
	"flag"
	"fmt"
	"net"
	"sync"
	"time"
)

type Conf struct {
	clients 	int
	requests	int
	keepAlive	bool
	url 		string
	cut         bool
	verbose 	bool
	slowmode	int
}

type Resp struct {
	elapsedTime time.Duration
	status		int
}


func worker(stats chan *Resp, conf Conf, wg *sync.WaitGroup) {
	defer wg.Done()

	strGet := "GET / HTTP/1.1\r\n\r\n"
	for i := 0; i < conf.requests; i++ {
		start := time.Now()
		conn, err := net.Dial("tcp", conf.url)
		if err != nil {
			fmt.Println("Unable to connect to ", conf.url)
			return
		}
		for i, _ := range strGet {
			conn.Write([]byte{strGet[i]})
			if (conf.cut && i > 8) {
			    conn.Close();
			}
			if conf.slowmode > 0 {
				//fmt.Println("Writing", string(strGet[i]), time.Since(start))
				time.Sleep(time.Duration(conf.slowmode) * time.Millisecond)
			}
		}
		status, err := bufio.NewReader(conn).ReadString('\n')
		if err != nil {
			fmt.Println("Unable to read")
		}
		if conf.verbose {
			fmt.Println("RECV: ", status)
		}
		stats <- &Resp{status: 200, elapsedTime: time.Since(start)}
	}
	/*client := &http.Client{}
		resp, err := client.Get(url)
		if err != nil {
			stats <- &Resp{status: 0, elapsedTime: time.Since(start)}
		}else{
			stats <- &Resp{status: resp.StatusCode, elapsedTime: time.Since(start)}
		}
	}*/
}

func spawnWorkers(stats chan *Resp, conf *Conf, wg *sync.WaitGroup) {
	for i := 0; i < conf.clients; i++ {
		wg.Add(1)
		go worker(stats, *conf, wg)
	}
}

func generateStats(stats chan *Resp, over chan bool, verbose bool) {
	var totalTime time.Duration
	var totalRequests int64
	var totalFailedRequests int64
	totalTime = 0
	for {
		select {
			case resp := <- stats:
				if verbose {
					fmt.Println("Recv", resp.elapsedTime, "STATUS", 200)
				}
				totalTime += resp.elapsedTime
				totalRequests++
				if resp.status != 200 {
					totalFailedRequests++
				}
			case <- over:
				averageTime := totalTime.Milliseconds() / totalRequests
				fmt.Println("Requests performed:", totalRequests)
				fmt.Println("AverageTimePerRequest", averageTime, "ms")
				if totalFailedRequests > 0 {
					fmt.Println("Failed requests:", totalFailedRequests , "over", totalRequests, "requests")
				}
		}
	}
}

func main(){
	var wg sync.WaitGroup
	var configuration Conf
	stats := make(chan *Resp)
	done := make(chan bool)
	flag.IntVar(&configuration.clients, "c", 5, "number of concurrent clients")
	flag.IntVar(&configuration.requests, "n", 1, "number of requests performed by each client")
	flag.BoolVar(&configuration.keepAlive, "k", false, "reuse tcp connection")
	flag.BoolVar(&configuration.verbose, "v", false, "verbose")
	flag.BoolVar(&configuration.cut, "i", false, "cut while sending")
	flag.IntVar(&configuration.slowmode, "s", 0, "slowmode: every char is sent with <slowmode> in MS delay")
	flag.StringVar(&configuration.url, "u", "http://127.0.0.1","URL")
	flag.Parse()
	fmt.Printf("Target (%s): %d clients running %d requests (verbose: %t, slowmode: %d)\n",
		configuration.url, configuration.clients, configuration.requests, configuration.keepAlive, configuration.slowmode)
	spawnWorkers(stats, &configuration, &wg)
	go generateStats(stats, done, configuration.verbose)
	wg.Wait()
	done <- true
}