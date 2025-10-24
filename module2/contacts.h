#ifndef contacts_h
#define contacts_h 

typedef struct {

    unsigned int id;
    char* name;
    char* surname;
    char* patronymic;
    char* phone_number;
    char* position;

}Contact;

void insertName(Contact*, char*);
void insertSurname(Contact*, char*);
void insertPatronymic(Contact*, char*);
void insertPhoneNumber(Contact*, char*);
void insertPosition(Contact*, char*);
char* scanInput();

#endif