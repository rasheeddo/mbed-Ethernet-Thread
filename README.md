# mbed-Ethernet-Thread

This is a sample code to send/receive data packets from/by mbed NUCLEO_F767ZI.

There are two threads `receiver_worker` and `sender_worker` running in the same time.

mbed is set as static-ip by using this block of code in main loop
```
SocketAddress ipAddr;
SocketAddress netmaskAddr;
SocketAddress gatewayAddr;

ipAddr.set_ip_address(mbedIP);
netmaskAddr.set_ip_address(mbedMask);
gatewayAddr.set_ip_address(mbedGateway);


net.disconnect();
net.set_network(ipAddr.get_ip_address(), netmaskAddr.get_ip_address(), gatewayAddr.get_ip_address());
```

where mbedIP, mbedMask, mbedGateway are global variables defined at header of the file.

```
uint16_t RECV_PORT = 5555;
uint16_t SEND_PORT = 6666;
 
// Static IP network variables
static const char* mbedIP       = "192.168.8.20";  //IP 
static const char* mbedMask     = "255.255.255.0";  // Mask
static const char* mbedGateway  = "192.168.8.1";    //Gateway
 
static const char* recvIP = "192.168.8.181";
```

This code are adapted from these two example code, but the API they used were already old.
https://os.mbed.com/questions/73552/Udp-Receive-with-nucleo_f767zi/
https://os.mbed.com/users/DanielKlioc/notebook/udp-static-ip-example-/


***NOTE***

I am using mbed-os 6.10, and there is some issue with printf function as mentioned in [here](https://forums.mbed.com/t/hitchhikers-guide-to-printf-in-mbed-6/12492), so I used one of their solution to add `mbed_app.json` at the root of project directory, and the content inside are [here](./mbed_app.json). 

## Test

Just plug the usb cable of mbed to pc and you can use any serial port monitor tool as you like, for convenient I am using Arduino Serial Monitor. So mbed is sending/receiving in the same time.

We can use python codes from [here](https://github.com/rasheeddo/testSendRecvUDP_mbed) to make sure it works. We need to change ip on `recvIP` according to our PC ip address.