import hashlib
import secrets

from sqlalchemy import Column, Integer, String, Boolean, Date, Float

from SqlDataBase import SqlDataBase


class Account(SqlDataBase.Base):
    __tablename__ = "Account"
    accId = Column(Integer, primary_key=True, autoincrement=10000)
    full_name = Column(String(25))
    username = Column(String(25))
    hash_password = Column(String(64))
    salt = Column(String(8))
    email = Column(String(25))
    birthday = Column(Date)
    gender = Column(String(25))
    country = Column(String(25))
    city = Column(String(25))
    street = Column(String(25))
    house_num = Column(Integer)
    is_marry = Column(Boolean)
    balance = Column(Float)

    def __init__(self, full_name: str, username: str, hash_password: str, email: str, birthday, gender: str,
                 country: str, city: str, street: str, house_num: int, is_marry: bool):
        """
        c'tor of new Bank Account.

        Example:

        newAcc = Account("fullname", "username", ....)

        session.add(newAcc)

        session.commit()
        """
        self.full_name = full_name
        self.username = username
        self.salt = secrets.token_hex(4)
        self.hash_password = hashlib.sha256((hash_password + self.salt).encode()).hexdigest()
        self.email = email
        self.birthday = birthday
        self.gender = gender
        self.country = country
        self.city = city
        self.street = street
        self.house_num = house_num
        self.is_marry = is_marry
        self.balance = 0.0

    def to_json(self):
        """
        return Account as JSON

        :return: Account as JSON
        """
        return {
            "full_name": self.full_name,
            "username": self.username,
            "hash_password": self.hash_password,
            "email": self.email,
            "birthday": self.birthday,
            "gender": self.gender,
            "country": self.country,
            "city": self.city,
            "street": self.street,
            "house_num": self.house_num,
            "is_marry": self.is_marry,
            "balance": self.balance,
            "accNum": self.accId
        }
