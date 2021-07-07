import json
import logging
import socket
import sys
from typing import Tuple

import Cipher

logging.basicConfig(stream=sys.stdout, level=logging.INFO)
COMMANDS = {}


class Communication:
    """
    Communication's handler class.
    in charge of communicate with users, using external functions for encryption.
    """
    EXIT = 0
    ADDR = ('127.0.0.1', 12345)
    MAX_MSG = 9999
    SEP = '|'

    def __init__(self):
        """
        simple c'tor
        """
        self.conn_socket = None
        self.shared_key = None

    def communicate(self):
        """
        user communication's handler
        """
        code = 1
        while code != self.EXIT:
            enc_data = self.conn_socket.recv(self.MAX_MSG)

            data = self.decrypt(enc_data)
            try:
                code, params = self.parse(data)
            except:
                pass  # TODO: return error msg - msg not in format
            else:
                logging.info(f"got from user: {code}, {data}")
                ans = COMMANDS[code](params)
                self.conn_socket.send(self.encrypt(ans))

    def start(self):
        """
        start the server, open sockets and get new users
        """
        logging.info("Waiting for connection")
        listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            listen_socket.bind(self.ADDR)
        except socket.error:
            print("port is already taken")
            exit()
        else:
            listen_socket.listen()
        while True:
            self.conn_socket, addr = listen_socket.accept()
            logging.info('Connected by ' + addr)
            self.keys_exchange()
            self.communicate()
            self.conn_socket.close()

    def keys_exchange(self) -> None:
        """
        performing secure D.H. key exchange, using long term key.
        """
        enc_user_pubkey = self.conn_socket.recv(self.MAX_MSG)
        self.shared_key = Cipher.gen_secret_key(enc_user_pubkey)

        self.conn_socket.send(Cipher.get_enc_pubkey())

    def decrypt(self, enc_data: bytes) -> str:
        """
        decrypt the given msg

        :param enc_data: the given msg
        :return: decrypt msg
        """
        return Cipher.encrypt_decrypt(self.shared_key, enc_data).decode()

    def parse(self, data: str) -> Tuple[int, dict]:
        """
        Parse coming data to cmd code and params

        :param data: data from user
        :return: parse data
        """
        code, params = data.split(self.SEP, 1)
        return int(code), json.loads(params)

        pass

    def encrypt(self, ans: str) -> bytes:
        """
        encrypt the given msg

        :param ans: the given answer
        :return: encrypted answer
        """
        return Cipher.encrypt_decrypt(self.shared_key, ans.encode())


if __name__ == "__main__":
    Communication().start()
