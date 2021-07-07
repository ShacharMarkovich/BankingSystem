import logging
import socket
import sys
import models
from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

from models import *


engine = create_engine('sqlite:///college.db', echo=True)
session = sessionmaker(bind=engine)()
Base = declarative_base()


def insert(acc: models.Account):
    pass


def login(data):
    pass


def server_listen_loop():
    logging.info("Waiting for connection")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as listen_socket:
        listen_socket.bind((HOST, PORT))
        listen_socket.listen()
        while True:
            conn_socket, addr = listen_socket.accept()
            with conn_socket:
                print('Connected by', addr)
                while True:
                    data = conn_socket.recv(MAX_MSG)
                    login(data)
                    logging.info(data.decode())
                    conn_socket.send(b"I got it!")
                    break


def main():
    # Press the green button in the gutter to run the script.
    logging.basicConfig(stream=sys.stdout, level=logging.INFO)
    server_listen_loop()


if __name__ == '__main__':
    main()
