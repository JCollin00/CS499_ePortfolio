"""
CRUD Operations Module for MongoDB - Animal Shelter Application
Course: CS 499 - Computer Science Capstone
Enhancement One: Software Design and Engineering
"""

from pymongo import MongoClient
from pymongo.errors import OperationFailure, PyMongoError
from bson.objectid import ObjectId

class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    def __init__(self, username="aacuser", password="password123", host="localhost", port=27017, db="aac", col="animals"):
        """
        Initialize the MongoClient and establish connection to the database.
        Parameters are configurable to prevent hardcoding sensitive credentials.
        """
        self.USER = username
        self.PASS = password
        self.HOST = host
        self.PORT = port
        self.DB = db
        self.COL = col

        try:
            # Initialize MongoDB Connection
            self.client = MongoClient(f'mongodb://{self.USER}:{self.PASS}@{self.HOST}:{self.PORT}')
            self.database = self.client[self.DB]
            self.collection = self.database[self.COL]
        except Exception as e:
            print(f"Error initializing database connection: {e}")
            raise

    # ============================================================
    # CREATE method - implements the C in CRUD
    # ============================================================
    def create(self, data):
        """
        Insert a document into the animals collection.

        Args:
            data (dict): Dictionary containing animal document fields

        Returns:
            bool: True if insert was successful, False otherwise
        """
        if data is not None and isinstance(data, dict):
            try:
                result = self.collection.insert_one(data)
                return True if result.inserted_id else False
            except OperationFailure as e:
                print(f"Insert operation failed due to database authority/operation error: {e}")
                return False
            except PyMongoError as e:
                print(f"MongoDB Error during insert: {e}")
                return False
        else:
            raise ValueError("Data parameter must be a non-empty dictionary.")

    # ============================================================
    # READ method - implements the R in CRUD
    # ============================================================
    def read(self, query=None):
        """
        Query documents from the animals collection.

        Args:
            query (dict): MongoDB filter criteria (defaults to empty dict to return all)

        Returns:
            list: List of matching documents, or empty list on failure
        """
        if query is None:
            query = {}

        if isinstance(query, dict):
            try:
                cursor = self.collection.find(query)
                return list(cursor)
            except OperationFailure as e:
                print(f"Read operation failed due to database error: {e}")
                return []
            except PyMongoError as e:
                print(f"MongoDB Error during query: {e}")
                return []
        else:
            raise ValueError("Query parameter must be a dictionary.")

    # ============================================================
    # UPDATE method - implements the U in CRUD
    # ============================================================
    def update(self, query, new_values):
        """
        Update documents in the animals collection.

        Args:
            query (dict): MongoDB filter to find documents to update
            new_values (dict): New values to set on matching documents

        Returns:
            int: Number of documents modified (0 if none or failure)
        """
        if query is None or not isinstance(query, dict):
            raise ValueError("Query parameter must be a valid dictionary.")

        if new_values is None or not isinstance(new_values, dict):
            raise ValueError("New values parameter must be a valid dictionary.")

        try:
            result = self.collection.update_many(query, {"$set": new_values})
            return result.modified_count
        except PyMongoError as e:
            print(f"Update operation failed: {e}")
            return 0

    # ============================================================
    # DELETE method - implements the D in CRUD
    # ============================================================
    def delete(self, query):
        """
        Delete documents from the animals collection.

        Args:
            query (dict): MongoDB filter to find documents to delete

        Returns:
            int: Number of documents deleted (0 if none or failure)
        """
        if query is None or not isinstance(query, dict):
            raise ValueError("Query parameter must be a valid dictionary.")

        try:
            result = self.collection.delete_many(query)
            return result.deleted_count
        except PyMongoError as e:
            print(f"Delete operation failed: {e}")
            return 0