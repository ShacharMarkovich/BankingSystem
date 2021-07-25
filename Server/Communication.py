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
                         "updateData": self.update_data, "otp": self.otp, "withdraw": self.withdraw,
                         "deposit": self.deposit}
        self.otp_wait = False

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
        try:
            while code != self.EXIT:
                enc_data = self.conn_socket.recv(self.MAX_MSG)
                try:
                    data = self.decrypt(enc_data)
                    code, params = self.parse(data)
                except Exception as e:
                    logging.info("[!] error: ", str(e))
                    self.conn_socket.send(self.encrypt("0|message not in format"))
                else:
                    logging.info(f"got from client:\n(code, params) = {code, params}")
                    ans = self.commands[code](params)
                    logging.info(ans)
                    self.conn_socket.send(self.encrypt(ans))
        except:
            pass

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
            print(f"[!] registration returns: 1|id:{acc_id}|{otp_secret}")
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

    def withdraw(self, amount: dict) -> str:
        """
        withdraw amount from current login user

        :param amount: the amount
        :return: fit msg
        """
        if self.db.account is None:
            raise ValueError("0|Please logging first")
        if self.db.account["Balance"] < amount["amount"]:
            raise ValueError("0|You don't have enough money")
        print("withdraw")

        # TODO: TOTP
        self.db.update_account_balance(self.db.account["accNum"], self.db.account["Balance"] - amount["amount"])
        return "1|succeed"

    def deposit(self, amount: dict) -> str:
        """
        deposit amount from current login user

        :param amount: the amount
        :return: fit msg
        """
        if self.db.account is None:
            raise ValueError("0|Please logging first")
        print("deposit")

        # TODO: TOTP
        self.db.update_account_balance(self.db.account["accNum"], self.db.account["Balance"] + amount["amount"])
        return "1|succeed"

    def transfer(self, params):
        if self.db.account["Balance"] < params["amount"]:
            raise ValueError("0|You don't have enough money")

        if not self.db.check_if_exists(params["accNum"]):
            raise ValueError("0|no such account!")

        self.db.update_account_balance(self.db.account["accNum"], self.db.account["Balance"] - params["amount"])
        self.db.update_account_balance(params["accNum"], self.db.account["Balance"] + params["amount"])
        return "1|succeed"

    def otp(self, user_otp) -> str:
        """
        handle TOTP

        :param user_otp: user OTP
        :return: fit msg
        """
        # if not self.otp_wait:
        #    return "0|otp is not necessary now!"
        try:
            secret = self.db.get_otp_data()[0]
        except ValueError as ve:
            return ve
        else:
            otp = Cipher.get_otp(secret)
            logging.info("user OTP:" + user_otp["otp"])
            logging.info("sys OTP:" + otp)
            return "1|good" if otp == user_otp["otp"] else "0|bad"


if __name__ == "__main__":
    Communication().start()
