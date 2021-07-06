import logging
import socket
import sqlite3
import sys

table_name = "Accounts"
table_columns = "f_name text, l_name text, balance real, username text, salt text, password_hashed text"
HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 12345  # Port to listen on (non-privileged ports are > 1023)


def init_table_connection(c):
    cur = c.cursor()
    cur.execute(f''' SELECT count(name) FROM sqlite_master WHERE type='table' AND name='{table_name}' ''')
    if cur.fetchone()[0] == 1:
        logging.info("Table exists.")
    else:
        cur.execute(f'''CREATE TABLE {table_name} ({table_columns})''')
        cur.execute(f'''INSERT INTO {table_name} (f_name,l_name ,balance,username,salt,password_hashed)
VALUES( "Admin","Admin",9999999999,"Admin","000000","000000");''')
        c.commit()


def login(data):
    pass


def server_listen_loop(con):
    logging.info("Waiting for connection")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        while True:
            try:
                conn, addr = s.accept()
                with conn:
                    print('Connected by', addr)
                    while True:
                        data = conn.recv(1024)
                        if not data:
                            break
                        login(data)
                        logging.info(data.decode())
                        conn.sendall(b"I got it!")
                        break
            except ConnectionResetError:
                pass


def main():
    # Press the green button in the gutter to run the script.
    logging.basicConfig(stream=sys.stdout, level=logging.INFO)
    con = sqlite3.connect('example.db')
    con.commit()
    init_table_connection(con)
    server_listen_loop(con)


if __name__ == '__main__':
    main()
