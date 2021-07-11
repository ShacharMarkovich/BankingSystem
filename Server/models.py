from sqlalchemy import Column, Integer, String, Boolean, Date, Float, Table, create_engine, MetaData
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

engine = create_engine('sqlite:///college.db', echo=True)
session = sessionmaker(bind=engine)()
meta = MetaData()
Base = declarative_base()

"""
the Accounts table in the DB
"""
Account = Table('Account', meta,
                Column('accId', Integer, primary_key=True, autoincrement=10000),
                Column('full_name', String(25)),
                Column('username', String(25)),
                Column('hash_password', String(64)),
                Column('salt', String(8)),
                Column('email', String(25)),
                Column('birthday', Date),
                Column('gender', String(25)),
                Column('country', String(25)),
                Column('city', String(25)),
                Column('street', String(25)),
                Column('house_num', Integer),
                Column('is_marry', Boolean),
                Column('balance', Float)
                )
