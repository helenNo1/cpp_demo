#include <mongoc/mongoc.h>

int main() {
    // Initialize MongoDB C Driver
    mongoc_init();

    // Create a MongoDB client
    mongoc_client_t *client = mongoc_client_new("mongodb://localhost:27017");

    // Check if the client was created successfully
    if (!client) {
        fprintf(stderr, "Failed to create MongoDB client\n");
        return EXIT_FAILURE;
    }

    // Access a MongoDB database and collection
    mongoc_database_t *database = mongoc_client_get_database(client, "testdb");
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "testdb", "testcollection");

    // Insert a document into the collection
    bson_t *doc = BCON_NEW("name", BCON_UTF8("John"), "age", BCON_INT32(30));
    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, NULL)) {
        fprintf(stderr, "Failed to insert document\n");
        return EXIT_FAILURE;
    }

    // Query the inserted document
    bson_t *query = BCON_NEW("name", BCON_UTF8("John"));
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    const bson_t *result;
    while (mongoc_cursor_next(cursor, &result)) {
        char *str = bson_as_canonical_extended_json(result, NULL);
        printf("Query Result: %s\n", str);
        bson_free(str);
    }

    // Clean up resources
    mongoc_cursor_destroy(cursor);
    bson_destroy(query);
    bson_destroy(doc);
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_client_destroy(client);

    // Cleanup MongoDB C Driver
    mongoc_cleanup();

    return EXIT_SUCCESS;
}
