import threading 
import socket

class UdpHandler: 
    def __init__(self, ip, port): 
        print("UDP handler init\n")
        self.running = True 
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.settimeout(1)

        self.server_address = (ip, port) 
        self._lock = threading.Lock() 
        self._receive_condition = threading.Condition(self._lock) 
        self._latest_response = None 
        self._receive_thread = threading.Thread(target=self._receive_thread, daemon=True)
        self._receive_thread.start() 



    def _receive_thread(self): 
        while self.running: 
            try: 
                data, _ = self.sock.recvfrom(4096) 

                with self._lock: 
                    print(data)
                    self._latest_response = data
                    self._receive_condition.notify()


            except socket.timeout: 
                continue 
            except socket.error as e: 
                print(f"Socket error: {e}")

    def send_command(self, command): 
        with self._lock: 
            print(f"Sending data {command}")
            self.sock.sendto(command, self.server_address)
            self._receive_condition.wait(0.5)
        return self._latest_response