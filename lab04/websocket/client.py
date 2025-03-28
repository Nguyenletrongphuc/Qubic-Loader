import tornado.ioloop
import tornado.websocket

class WebSocketClient:
    def __init__(self, io_loop):
        self.connection = None
        self.io_loop = io_loop
        
    def start(self):
        self.connec_and_read()
        
    def stop(self):
        self.io_loop.stop()
    
    def connec_and_read(self):
        print("Connecting to server")
        tornado.websocket.websocket_connect(url=f"ws://localhost:8888/Websocket/", callback=self.mabe_try_connection,on_message_callback=self.on_message,ping_interval=10,ping_timeout=30,)
    
    def mabe_try_connection(self, future):
        try:
            self.connection = future.result()
        except :
            print("Connection failed")
            self.io_loop.call_later(3, self.connec_and_read)
    
    def on_message(self, message):
        if message is None:
            print("Connection closed")
            self.io_loop.stop()
            return
        print("Received:", message)
        
        self.connection.read_message(callback=self.on_message)

def main():
    io_loop = tornado.ioloop.IOLoop.current()
    client = WebSocketClient(io_loop)
    client.start()
    io_loop.start()
    
if __name__ == "__main__":
    main()