# Example Python Code to Insert a Document 

from pymongo import MongoClient 
from pymongo.errors import OperationFailure
from bson.objectid import ObjectId 

class AnimalShelter(object): 
    """ CRUD operations for Animal collection in MongoDB """ 

    def __init__(self): 
        # Initializing the MongoClient. This helps to access the MongoDB 
        # databases and collections. This is hard-wired to use the aac 
        # database, the animals collection, and the aac user. 
        # 
        # You must edit the password below for your environment. 
        # 
        # Connection Variables 
        # 
        USER = 'aacuser' 
        PASS = 'password123' 
        HOST = 'localhost' 
        PORT = 27017 
        DB = 'aac' 
        COL = 'animals' 
        # 
        # Initialize Connection 
        # 
        self.client = MongoClient('mongodb://%s:%s@%s:%d' % (USER,PASS,HOST,PORT)) 
        self.database = self.client['%s' % (DB)] 
        self.collection = self.database['%s' % (COL)] 

    # Create a method to return the next available record number for use in the create method
            
    # Complete this create method to implement the C in CRUD. 
    def create(self, data):
        if data is not None:
            try:
                self.database.animals.insert_one(data)  # data should be dictionary
                return True
            except OperationFailure as e:
                print(f"Insert operation failed: {e}")
                return False
        else:
            raise Exception("Nothing to save, because data parameter is empty")

    # Create method to implement the R in CRUD.
    def read(self, query):
        if query is not None:
            try:
                cursor = self.database.animals.find(query)
                return list(cursor)
            except OperationFailure as e:
                print(f"Read operation failed: {e}")
                return []
        else:
            raise Exception("Nothing to find, because query parameter is empty")

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
            int: Number of documents modified (0 if none)
        """
        if query is None:
            raise ValueError("Query parameter cannot be None")
        
        if new_values is None:
            raise ValueError("New values parameter cannot be None")
        
        try:
            result = self.database.animals.update_many(query, {"$set": new_values})
            return result.modified_count
        except Exception as e:
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
            int: Number of documents deleted (0 if none)
        """
        if query is None:
            raise ValueError("Query parameter cannot be None")
        
        try:
            result = self.database.animals.delete_many(query)
            return result.deleted_count
        except Exception as e:
            print(f"Delete operation failed: {e}")
            return 0