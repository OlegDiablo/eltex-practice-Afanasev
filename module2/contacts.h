typedef struct {

    char* name;
    char* surname;
    char* patronymic;
    char** phone_number;
    char* position;

}Contact;

void insertName(Contact*, char*);
void insertSurname(Contact*, char*);
void insertPatronymic(Contact*, char*);
void insertPhoneNumber(Contact*, char*, int);
void insertPosition(Contact*, char*);
char* scanInput();
void printContact(Contact*);
