# import binascii
import secrets

from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA


class Cipher:
    END = b'}'
    PAD = b'*'

    def __init__(self):
        """
        c'tor for new Cipher object
        """
        self.shared_key = secrets.token_hex(8).encode()
        self.iv = secrets.token_hex(8).encode()
        self.rsa_pubkey = None

    def cipher_obj(self):
        """
        Creating an AES cipher block for encrypting & decrypting.
        The cipher object is working only once - so we recreating it every encrypting/decrypting.
        :returns: AES cipher block for encrypting/decrypting.
        """
        return AES.new(self.shared_key, AES.MODE_CBC, iv=self.iv)

    def decrypt(self, enc_msg: bytes) -> bytes:
        """
        decrypt given msg with a given shared key.

        :param enc_msg: the encrypted data
        :return:
        """
        return self.unpad(self.cipher_obj().decrypt(enc_msg))

    def encrypt(self, msg: bytes) -> bytes:
        """
        encrypt given msg with a given shared key.

        :param msg: the msg
        :return: encrypt msg
        """
        return self.cipher_obj().encrypt(self.pad(msg, AES.block_size))

    def pad(self, data: bytes, size: int) -> bytes:
        pad_len = size - len(data) % size
        return data + self.PAD * pad_len

    def unpad(self, padded_data):
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

    def enc_int_msg(self, msg: int) -> bytes:
        """
        Get a new conversation public key, encrypted by the PRE_SHARED_KEY

        :param msg: the msg
        :return: encrypted new public key
        """
        aes_encrypting = self.cipher_obj()
        encrypted_pubkey = aes_encrypting.encrypt(pad(msg.to_bytes(256, "big"), AES.block_size))
        return encrypted_pubkey
