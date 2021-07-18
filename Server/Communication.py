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
        self.commands = {"login": self.login, "register": self.register, "logout": self.logout,
                         "updateData": self.update_data, "otp": self.gen_otp}

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

    def communicate(self):
        """
        user communication's handler
        """
        code = 1
        logging.info("start communicate!")
        while code != self.EXIT:
            enc_data = self.conn_socket.recv(self.MAX_MSG)
            data = self.decrypt(enc_data)
            try:
                code, params = self.parse(data)
                logging.info(f"got from client:\n(code, params) = {code, params}")
            except:
                logging.info("[!] error")
                return  # TODO: return error msg - msg not in format
            else:
                ans = self.commands[code](params)
                logging.info(ans)
                self.conn_socket.send(self.encrypt(ans))

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

    def login(self, params: dict) -> str:
        """
        try login with given username and password

        :param params: dict of username and password
        :return: fit msg
        """
        try:
            account = self.db.login(params["username"], params["hash_password"])
        except ValueError as ve:
            return str(ve)
        else:
            return f"1|user login:{json.dumps(account)}"

    def register(self, params: dict) -> str:
        """
        try create new account with given params
        :param params: new account params' as dict
        :return: fit msg
        """
        otp_secret = Cipher.get_secret()
        try:
            acc_id = self.db.add_new_account(params["FullName"], params["Username"], params["Password"],
                                             params["Email"], params["BirthDay"], params["Gender"], params["Country"],
                                             params["City"], params["Street"], params["HouseNum"], params["IsMarry"],
                                             otp_secret)
        except ValueError as ve:
            return str(ve)
        else:
            return f"1|id:{acc_id}|{otp_secret}"

    def logout(self, _):
        """
        commit logout from current login account
        :param _: unused param
        :return: succeed msg
        """
        self.db.logout()
        return "1"

    def update_data(self, params: dict) -> str:
        """
        try update data of given account data with the new params

        :param params: new params
        :return: fit msg
        """
        if not self.db.is_login:
            return "0|Please login first"
        return "1|account update successfully" if self.db.update_account(params["accNum"], params["FullName"],
                                                                         params["Username"], params["Email"],
                                                                         params["BirthDay"], params["Gender"],
                                                                         params["Country"], params["City"],
                                                                         params["Street"], params["HouseNum"], params[
                                                                             "IsMarry"]) \
            else "0|username or email already in used"

    def gen_otp(self):
        try:
            secret, counter = self.db.get_otp_data()
        except ValueError as e:
            return e
        else:
            value = Cipher.get_otp(secret, counter)
            print(value)


if __name__ == "__main__":
    Communication().start()
