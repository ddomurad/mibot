import socket


TCP_IP = '192.168.0.17'
TCP_PORT = 10402

MESSAGE = "Hello, World! (%s:%s)"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

for i in range(100):
	for j in range(100):
		s.send( bytes(MESSAGE % (i,j), 'UTF-8'))

s.close()

