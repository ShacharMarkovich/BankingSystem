import hashlib
import secrets
import sqlite3


class SqlDataBase(object):
    """
    Responsible for maintaining the SQL server session
    """

    def __init__(self):
        """
        c'tor, open DB, add tables if not exists
        """
        self.con = sqlite3.connect('BankingSystem.db')
        self.cur = self.con.cursor()
        self.cur.execute("""
        CREATE TABLE IF NOT EXISTS Account (
            accId INTEGER NOT NULL,
            full_name VARCHAR(25),
            username VARCHAR(25) UNIQUE,
            hash_password VARCHAR(64),
            salt VARCHAR(8),
            email VARCHAR(25) UNIQUE,
            birthday VARCHAR(10),
            gender VARCHAR(25),
            country VARCHAR(25),
            city VARCHAR(25),
            street VARCHAR(25),
            house_num INTEGER,
            is_marry BOOLEAN,
            balance FLOAT,
            PRIMARY KEY (accId),
            CHECK (is_marry IN (0, 1))
            )""")

        self.cur.execute("""
        CREATE TABLE IF NOT EXISTS Secret (
            pkey INTEGER NOT NULL,
            accId INTEGER NOT NULL,
            secret VARCHAR(32) NOT NULL,
            counter INTEGER NOT NULL,
            PRIMARY KEY (pkey),
            FOREIGN KEY (accId) REFERENCES Account(accId)
            )""")

        self.con.commit()
        self.account = None

    def exists(self, username, email):
        """
        check if the given username and email are in the DB, unless they much to the current login account

        :param username: username to check
        :param email: email to check
        :return: whether or not exists
        """
        if self.account is not None and (username == self.account["Username"] or email == self.account["Email"]):
            return False

        for _username, _email in self.cur.execute(
                f"""SELECT username,email FROM Account WHERE username='{username}' or email='{email}'"""):
            if _username == username or _email == email:
                return True
        return False

    def add_new_account(self, full_name: str, username: str, hash_password: str, email: str, birthday, gender: str,
                        country: str, city: str, street: str, house_num: int, is_marry: bool, secret: str) -> int:
        """
        Add new account to DB, first check if exists

        :param: secret key for OTP
        :return: account id if succeeded
        """
        if not self.exists(username, email):
            salt = secrets.token_hex(4)
            hash_password = hashlib.sha256((hash_password + salt).encode()).hexdigest()
            self.cur.execute(f"""INSERT INTO Account(full_name, username, hash_password, salt, email, birthday, gender,
                                                country, city, street, house_num, is_marry, balance)
                                VALUES ('{full_name}', '{username}', '{hash_password}', '{salt}', '{email}', 
                                        '{birthday[:10]}', '{gender}', '{country}', '{city}', '{street}', {house_num},
                                        {is_marry}, 0.0)""")
            self.con.commit()
            for accId in self.cur.execute(f"""SELECT accId FROM Account where username='{username}' LIMIT 1"""):
                self.cur.execute(f"""INSERT INTO Secret(accId, secret, counter) VALUES({accId}, '{secret}', 0)""")
                self.con.commit()
                return accId

    def login(self, username: str, password: str):
        """
        try login to account with given data:

        :param username: given username
        :param password: given password
        :return: is succeed - return account as json, else - return None
        """
        for _salt, real_hash_password in self.cur.execute(
                f"SELECT salt, hash_password from Account where username='{username}' LIMIT 1"):
            if _salt is not None and real_hash_password is not None and real_hash_password == hashlib.sha256(
                    (password + _salt).encode()).hexdigest():
                for accNum, full_name, username, hash_password, salt, email, birthday, gender, country, city, street, house_num, is_marry, balance in self.cur.execute(
                        f"select * from Account where username='{username}' LIMIT 1"):
                    self.account = {"FullName": full_name, "Username": username, "hash_password": hash_password,
                                    "Email": email, "BirthDay": birthday, "Gender": gender, "Country": country,
                                    "City": city, "Street": street, "HouseNum": house_num, "IsMarry": is_marry,
                                    "Balance": balance, "accNum": accNum}
                    return self.account
            return "Unknown error, try again"
        return "user not found"

    def logout(self):
        """
        logout from user
        """
        self.account = None

    @property
    def is_login(self):
        return self.account is not None

    def update_account(self, accNum: int, full_name: str, username: str, email: str, birthday, gender: str,
                       country: str, city: str, street: str, house_num: int, is_marry: bool) -> bool:
        """
        update curr account with new details

        :return: if succeed
        """
        if not self.exists(username, email):
            self.cur.execute(f"""UPDATE Account SET full_name='{full_name}', username='{username}', email='{email}',
                        birthday='{birthday[:10]}', gender='{gender}', country='{country}', city='{city}',
                        street='{street}', house_num={house_num}, is_marry={is_marry}
                        WHERE accID = {accNum}""")
            self.con.commit()
            return True
        return False

    def get_otp_data(self) -> tuple:
        """
        get current login user's HOTP key and counter from DB
        :return: HOTP key and counter
        """
        if self.account is None:
            raise ValueError("not login")
        acc_id = self.account["accNum"]
        for secret, counter in self.cur.execute(f"SELECT secret, counter FROM Secret WHERE accId = {acc_id} LIMIT 1"):
            if secret is not None and counter is not None:
                return secret, counter
        raise ValueError("Unknown error, try again")
