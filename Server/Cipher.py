import secrets

import pyotp
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA

totp = pyotp.HOTP('base32secret3232')


class Cipher:
    END = b'}'
    PAD = b'*'

    def __init__(self):
        """
        c'tor for new Cipher object
        """
        self.shared_key = secrets.token_hex(8).encode()
        self.iv = secrets.token_hex(8).encode()

        self.encrypt_obj = self.create_cipher_obj()
        self.decrypt_obj = self.create_cipher_obj()

        self.rsa_pubkey = None

    def create_cipher_obj(self):
        """
        Creating an AES cipher block for encrypting or decrypting.

        :returns: AES cipher block for encrypting/decrypting.
        """
        return AES.new(self.shared_key, AES.MODE_CBC, iv=self.iv)

    def decrypt(self, enc_msg: bytes) -> bytes:
        """
        decrypt given msg with a given shared key.

        :param enc_msg: the encrypted data
        :return: decrypt msg
        """
        return self.unpad(self.create_cipher_obj().decrypt(enc_msg))

    def encrypt(self, msg: bytes) -> bytes:
        """
        encrypt given msg with a given shared key.

        :param msg: the msg
        :return: encrypted msg
        """
        return self.create_cipher_obj().encrypt(self.pad(msg, AES.block_size))

    def pad(self, data: bytes, size: int) -> bytes:
        """
        add the `PAD` char at the given data's end.

        :param data: data to padded
        :param size: block size
        :return: padded data
        """
        pad_len = size - len(data) % size
        return data + self.PAD * pad_len

    def unpad(self, padded_data):
        """
        unpad the padded_data

        :param padded_data: the padded data
        :return: unpadded data
        """
        return padded_data[:padded_data.rfind(self.END) + 1]

    @staticmethod
    def byte2int(num: bytes) -> int:
        """
        staticmethod for convert bytes to int

        :param num: given int number as bytes
        :return: number as int
        """
        return int.from_bytes(num, byteorder='big', signed=False)

    def gen_rsa_pubkey(self, modulus: bytes, exponent: bytes):
        """
        build RSA public key from given modulus and exponent

        :param modulus: the modulus
        :param exponent: the exponent
        """
        self.rsa_pubkey = RSA.construct((self.byte2int(modulus), self.byte2int(exponent)))

    def rsa_encrypt_key(self) -> bytes:
        """
        encrypt the shared key and iv with the RSA public key

        :return: enc msg
        """
        return PKCS1_OAEP.new(self.rsa_pubkey).encrypt(self.shared_key + self.iv)

    @staticmethod
    def get_secret() -> str:
        """
        Generate 32 bit long random base32 string
        """
        return pyotp.random_base32()

    @classmethod
    def get_otp(cls, secret, counter):
        return pyotp.HOTP(secret).at(counter)
