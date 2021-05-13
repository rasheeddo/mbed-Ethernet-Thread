#include "mbed.h"
#include "EthernetInterface.h"
#include "rtos.h"

uint16_t RECV_PORT = 5555;
uint16_t SEND_PORT = 6666;
 
DigitalOut led1(LED1);
DigitalOut led2(LED2);
 
// Static IP network variables
static const char* mbedIP       = "192.168.8.20";  //IP 
static const char* mbedMask     = "255.255.255.0";  // Mask
static const char* mbedGateway  = "192.168.8.1";    //Gateway
 
static const char* recvIP = "192.168.8.181";
  
//Creating Ethernet object
EthernetInterface net;

void receiver_worker(EthernetInterface *net){
	SocketAddress sockAddr;

	UDPSocket recv_sock;
	recv_sock.open(net);

    // Bind the port with same as sender port
	int bind = recv_sock.bind(RECV_PORT);
	printf("bind %d\n", bind);

	// char buffer[256];
	char recv_buffer[2];
	uint16_t recv_int;

	while (true){


		// int ret = recv_sock.recvfrom(&sockAddr, buffer, sizeof(buffer));

		int ret = recv_sock.recvfrom(&sockAddr, recv_buffer, sizeof(recv_buffer));
		memcpy(&recv_int, recv_buffer, sizeof(recv_int));


		if (ret > 0){
			// printf("recv %s \n", buffer);
			// printf("From : %s\n", sockAddr.get_ip_address());

			printf("recv: %d\n", recv_int);
		}
		else{
			printf("error %d\n", ret);
		}

	}

}

void sender_worker(EthernetInterface *net){

	SocketAddress sockAddr;

	// specify where we want to send to
    sockAddr.set_ip_address(recvIP);
    sockAddr.set_port(SEND_PORT);


    UDPSocket send_sock;
    send_sock.open(net);

    /// data packet if string, we could set it like this
    // char out_buffer[] = "very important data";
    /// if data is numbers, then we need to make it as hi-lo byte
    uint16_t data;
    uint16_t i = 0;
    char send_buffer[2];

    while (1){

    	// In case of send string
    	// int ret = sock.sendto(sockAddr, out_buffer, sizeof(out_buffer));

    	// In case of send numbers
    	data = i;
    	printf("send %d\n", data);
    	send_buffer[0] = data >> 8; // hi-byte
    	send_buffer[1] = data&0xFF; // lo-byte
    	int ret = send_sock.sendto(sockAddr, send_buffer, sizeof(send_buffer));

	    // if (ret < 0) {
	    //     printf("Error sending data\n");
	    //     return -1;
	    // }

	    i++;
	    ThisThread::sleep_for(500);
    }

}



Thread receiver_thread;
Thread sender_thread;

int main(){

	SocketAddress ipAddr;
	SocketAddress netmaskAddr;
	SocketAddress gatewayAddr;

	ipAddr.set_ip_address(mbedIP);
	netmaskAddr.set_ip_address(mbedMask);
	gatewayAddr.set_ip_address(mbedGateway);


	net.disconnect();
	net.set_network(ipAddr.get_ip_address(), netmaskAddr.get_ip_address(), gatewayAddr.get_ip_address());

	// After setup ip, then start connecting the network
    printf("UDP Socket example\n");
    if (0 != net.connect()) {
        printf("Error connecting\n");
        return -1;
    }

	receiver_thread.start(callback(receiver_worker, &net));
	sender_thread.start(callback(sender_worker, &net));

	while (true){

		led2 = !led2;
		ThisThread::sleep_for(500);
		led1 = !led1;
	}

	net.disconnect();

	return 0;
}