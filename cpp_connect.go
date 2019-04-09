package main

import (
	"fmt"
	"time"
	"bufio"
 	"net"
	"github.com/yarco/swig-playground/build"
)

func blocking_cpp_goroutines() {
	db:=libdb.NewDB()

	c := make(chan bool)
	d := make(chan bool)

	start := time.Now()

	go func() {
		fmt.Println(db.SlowGet(5000))
		c <- true
	}()

	go func() {
		fmt.Println(db.SlowGet(3000))
		d <- true
	}()

	<-c
	<-d

	elapsed := time.Since(start)
	fmt.Printf("Two goroutine -> cpp_blocking method calls took %s \n", elapsed)
}

func sockets() {
	db:=libdb.NewDB()
	db.Listen_many()
	c := make(chan bool)
	d := make(chan bool)

	start := time.Now()

	socket_connect := func(msg string, ch chan<- bool) {
		conn, err := net.Dial("tcp", "127.0.0.1:1234")
		if err != nil {
			fmt.Println(err)
		}
		fmt.Fprintln(conn, msg)
		text, err := bufio.NewReader(conn).ReadString('\n')
		fmt.Println(text)
		ch <- true
	}
	//
	go socket_connect("socket: golang -> cpp -> golang - Success! #1", c)
	go socket_connect("socket: golang -> cpp -> golang - Success! #2", d)

	<-c
	<-d

	elapsed := time.Since(start)
	fmt.Printf("Two socket calls took %s \n", elapsed)
}

func main() {
	//sockets()
	blocking_cpp_goroutines()
}
