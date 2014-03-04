import socket
import sys
import time

# HOST = "10.60.10.189"
HOST = 'localhost'
PORT = 9997

elapse = []

print "Format: pin get\n"
print "or pin set {inout/onoff/updown} {1/0}\n"
print "e.g. \'25 set inout 1\' means set GPIO 25 to be an input"

# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to server and send data
sock.connect((HOST, PORT))
while True:
    data = raw_input()
    if data=="stop":
        break
    
    t1  = time.time()

    sock.send(data + "\n")
    
    # Receive data from the server and shut down
    received = sock.recv(1024)

    elapse.append(time.time()-t1)
    
    print "Sent:     %s" % data
    print "Received: %s" % received


print elapse
sock.close()
