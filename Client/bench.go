package main

import (
	"flag"
	"fmt"
	"net/http"
	"sync"
	"time"
)

type Conf struct {
	clients 	int
	requests	int
	keepAlive	bool
	url 		string
	verbose 	bool
}

type Resp struct {
	elapsedTime time.Duration
	status		int
}


func worker(stats chan *Resp, requests int, url string, keepAlive bool, wg *sync.WaitGroup) {
	defer wg.Done()
	client := &http.Client{}
	for i := 0; i < requests; i++ {
		start := time.Now()
		resp, err := client.Get(url)
		if err != nil {
			
		}
		stats <- &Resp{status: resp.StatusCode, elapsedTime: time.Since(start)}
	}
}

func spawnWorkers(stats chan *Resp, conf *Conf, wg *sync.WaitGroup) {
	for i := 0; i < conf.clients; i++ {
		wg.Add(1)
		go worker(stats, conf.requests, conf.url, conf.keepAlive, wg)
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
	flag.StringVar(&configuration.url, "u", "http://127.0.0.1","URL")
	flag.Parse()
	fmt.Printf("Target (%s): %d clients running %d requests (verbose: %t)\n",
		configuration.url, configuration.clients, configuration.requests, configuration.keepAlive)
	spawnWorkers(stats, &configuration, &wg)
	go generateStats(stats, done, configuration.verbose)
	wg.Wait()
	done <- true
}