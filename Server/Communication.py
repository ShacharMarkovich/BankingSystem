import json
import logging
import socket
import sys
from typing import Tuple

from Cipher import Cipher
from SqlDataBase import SqlDataBase

logging.basicConfig(stream=sys.stdout, level=logging.INFO)


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
        self.cipher = Cipher()  # gen session (=symmetric) key & iv:
        self.db = SqlDataBase()
        self.commands = {"login": self.login, "register": self.register}

    def communicate(self):
        """
        user communication's handler
        """
        code = 1
        logging.info("start communicate!")
        while code != self.EXIT:
            enc_data = self.conn_socket.recv(self.MAX_MSG)
            print(f"recv: {enc_data, len(enc_data)}")
            data = self.decrypt(enc_data)
            try:
                code, params = self.parse(data)
                print(f"got from client:\n[!](code, params) = {code, params}")
            except:
                print("[!] error")
                pass  # TODO: return error msg - msg not in format
            else:
                ans = self.commands[code](params)
                print(ans)
                to_send = self.encrypt(ans)
                print("[!] len(self.encrypt(ans)) = ", len(to_send))
                self.conn_socket.send(to_send)

    def start(self):
        """
        start the server, open sockets and get new users
        """
        logging.info("Waiting for connection")
        listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            listen_socket.bind(self.ADDR)
        except socket.error:
            logging.error("port is already taken")
            exit()
        else:
            listen_socket.listen()
        while True:
            self.conn_socket, addr = listen_socket.accept()
            logging.info('Connected by ' + str(addr))
            self.keys_exchange()
            self.communicate()
            self.conn_socket.close()

    def keys_exchange(self) -> None:
        """
        handler key symmetric and asymmetric keys with client
        """
        # gen asymmetric public key:
        exponent = self.conn_socket.recv(self.MAX_MSG)
        modulus = self.conn_socket.recv(self.MAX_MSG)
        self.cipher.gen_rsa_pubkey(modulus, exponent)

        # encrypt the session key & iv with the asymmetric public key and send to client:
        enc_shared_key = self.cipher.rsa_encrypt_key()
        self.conn_socket.send(enc_shared_key)

    def encrypt(self, ans: str) -> bytes:
        """
        encrypt the given msg

        :param ans: the given answer
        :return: encrypted answer
        """
        return self.cipher.encrypt(ans.encode())

    def decrypt(self, enc_data: bytes) -> str:
        """
        decrypt the given msg

        :param enc_data: the given msg
        :return: decrypt msg
        """
        return self.cipher.decrypt(enc_data).decode()

    def parse(self, data: str) -> Tuple[str, dict]:
        """
        Parse coming data to cmd code and params

        :param data: data from user
        :return: parse data
        """
        code, params = data.split(self.SEP, 1)
        return code, json.loads(params)

    def login(self, params: dict):
        account = self.db.login(params["username"], params["hash_password"])
        if account:
            print(f"1|{json.dumps(account)}")
            return f"1|{json.dumps(account)}"
        else:
            return "0|username or password are wrong"

    def register(self, params: dict):
        return "1|account added successfully" if self.db.add_new_account(params["FullName"], params["Username"],
                                                                         params["Password"], params["Email"],
                                                                         params["BirthDay"], params["Gender"],
                                                                         params["Country"], params["City"],
                                                                         params["Street"], params["HouseNum"], params[
                                                                             "IsMarry"]) \
            else "0|username or email already in used"


if __name__ == "__main__":
    Communication().start()
