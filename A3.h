typedef struct {
    char name[31];
    int count;
} NameCountData;

typedef enum {
    TYPE_NAMECOUNT,
    TYPE_B // there is a possibility to extend with more types in the future
} MessageType;

typedef struct {
    MessageType type;
    size_t size; // Size of the following payload
} MessageHeader;

int check_in(char *a, char *b[]);
void nprinter(char *nused[], int count[]);