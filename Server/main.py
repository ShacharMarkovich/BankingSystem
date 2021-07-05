import socket
import sqlite3
import logging
import sys
table_name = "Accounts"
table_columns = "f_name text, l_name text, balance real, username text, salt text, password_hashed text"
HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)

def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


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
            while True:
                try:
                    s.listen()
                    conn, addr = s.accept()
                    with conn:
                        print('Connected by', addr)
                        while True:
                            data = conn.recv(1024)
                            if not data:
                                break
                            login(data)
                            logging.info(data.decode())
                            conn.sendall(data)
                except(ConnectionResetError):
                    pass

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    logging.basicConfig(stream=sys.stdout, level=logging.INFO)
    con = sqlite3.connect('example.db')
    cur = con.cursor()
    con.commit()
    init_table_connection(con)
    server_listen_loop(con)


