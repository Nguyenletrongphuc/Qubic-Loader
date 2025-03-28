import random
import tornado.ioloop
import tornado.web
import tornado.websocket

class WebSocketServer(tornado.websocket.WebSocketHandler):
    clients = set()
    def open(self):
       WebSocketServer.clients.add(self)

    def on_message(self, message):
        WebSocketServer.clients.remove(self)
        
    @classmethod
    def send_message(cls, message: str):
        print("Sending message to clients")
        for client in cls.clients:
            client.write_message(message)

class RandomWordSelector:
    def __init__(self, words_list):
        self.words_list = words_list
        
    def sample(self):
        return random.choice(self.words_list)
    
def main():
    app = tornado.web.Application([(r"/Websocket/", WebSocketServer)],
                                websocket_ping_interval=10,
                                websocket_ping_timeout=30,)
    app.listen(8888)
    io_loop = tornado.ioloop.IOLoop.current()
    word_selector = RandomWordSelector(["Hello", "World", "Python", "Tornado"])
    periodic_callback = tornado.ioloop.PeriodicCallback(lambda: WebSocketServer.send_message(word_selector.sample()), 3000)
    periodic_callback.start()
    io_loop.start()
    
if __name__ == "__main__":
    main()
                                  
        