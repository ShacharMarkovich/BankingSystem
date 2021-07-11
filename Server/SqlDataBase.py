import hashlib

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker


class SqlDataBase(object):
    engine = create_engine('sqlite:///college.db', echo=True)
    session = sessionmaker(bind=engine)()
    Base = declarative_base()

    def __init__(self):
        self.login_acc = None

    def validate_username(self, username):
        return self.session.query(models.Account).filter_by(username=username).first() is not None

    def validate_email(self, email):
        return self.session.query(models.Account).filter_by(email=email).first() is not None

    def add_new_account(self, full_name: str, username: str, hash_password: str, email: str, birthday, gender: str,
                        country: str, city: str, street: str, house_num: int, is_marry: bool) -> bool:
        """
        Add new account to DB, first check if exists

        :return: if succeeded
        """
        if self.validate_email(email) and self.validate_username(username):
            new_acc = models.Account(full_name, username, hash_password, email, birthday, gender, country, city, street,
                                     house_num, is_marry)
            self.session.add(new_acc)
            self.session.commit()
            return True

        return False

    def login(self, username: str, password: str):
        """
        try login to account with given data:

        :param username: given username
        :param password: given password
        :return: is succeed - return account as json, else - return None
        """
        account = self.session.query(models.Account).filter_by(username=username).first()
        if account:
            if account.hash_password == hashlib.sha256((password + account.salt).encode()).hexdigest():
                self.login_acc = account
                return account.to_json()
        return None


import models
