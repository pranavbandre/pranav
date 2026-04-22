/*
 * ============================================================
 *   LIBRARY BOOK MANAGEMENT SYSTEM
 *   Data Structure: Singly Linked List
 *   Language: C
 * ============================================================
 *
 *  Features:
 *    1. Add a new book
 *    2. Delete a book by ISBN
 *    3. Search a book by ISBN
 *    4. Search books by author
 *    5. Display all books
 *    6. Update book details
 *    7. Issue a book (mark as borrowed)
 *    8. Return a book (mark as available)
 *    9. Display total book count
 *   10. Exit
 */

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

//Constants 
#define MAX_TITLE   100
#define MAX_AUTHOR   60
#define MAX_ISBN     20
#define MAX_GENRE    40

// Node / Book structure
typedef struct Book {
    char   title[MAX_TITLE];
    char   author[MAX_AUTHOR];
    char   isbn[MAX_ISBN];
    char   genre[MAX_GENRE];
    int    year;
    int    isIssued;          // 0 = available, 1 = borrowed
    struct Book *next;
} Book;

//head pointer
Book *head = NULL;
//Utility: clear input buffer
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Utility: safe string input (strips trailing newline)
void readString(const char *prompt, char *dest, int size) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin)) {
        dest[strcspn(dest, "\n")] = '\0';
    }
}

//Utility: print a horizontal rule
void printLine(void) {
    printf("----------------------------------------------------------\n");
}

//Utility: print a single book record
void printBook(const Book *b) {
    printf("  Title   : %s\n", b->title);
    printf("  Author  : %s\n", b->author);
    printf("  ISBN    : %s\n", b->isbn);
    printf("  Genre   : %s\n", b->genre);
    printf("  Year    : %d\n", b->year);
    printf("  Status  : %s\n", b->isIssued ? "Issued (Not Available)" : "Available");
}

//1. Add a new book at the end of the list
void addBook(void) {
    Book *newBook = (Book *)malloc(sizeof(Book));
    if (!newBook) {
        printf("[ERROR] Memory allocation failed.\n");
        return;
    }

    printLine();
    printf("  ADD NEW BOOK\n");
    printLine();

    readString("  Title   : ", newBook->title, MAX_TITLE);
    readString("  Author  : ", newBook->author, MAX_AUTHOR);
    readString("  ISBN    : ", newBook->isbn, MAX_ISBN);
    readString("  Genre   : ", newBook->genre, MAX_GENRE);

    printf("  Year    : ");
    scanf("%d", &newBook->year);
    clearInputBuffer();

    newBook->isIssued = 0;
    newBook->next     = NULL;

    // Check for duplicate ISBN 
    Book *cur = head;
    while (cur) {
        if (strcmp(cur->isbn, newBook->isbn) == 0) {
            printf("[ERROR] A book with ISBN '%s' already exists.\n", newBook->isbn);
            free(newBook);
            return;
        }
        cur = cur->next;
    }

    // Append to list 
    if (!head) {
        head = newBook;
    } else {
        Book *temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newBook;
    }

    printf("[SUCCESS] Book '%s' added successfully.\n", newBook->title);
}

//2. Delete a book by ISBN
void deleteBook(void) {
    if (!head) {
        printf("[INFO] Library is empty. Nothing to delete.\n");
        return;
    }

    char isbn[MAX_ISBN];
    printLine();
    printf("  DELETE BOOK BY ISBN\n");
    printLine();
    readString("  Enter ISBN: ", isbn, MAX_ISBN);

    Book *prev = NULL, *cur = head;

    while (cur) {
        if (strcmp(cur->isbn, isbn) == 0) {
            if (cur->isIssued) {
                printf("[WARNING] Book '%s' is currently issued. Cannot delete.\n", cur->title);
                return;
            }
            if (prev) prev->next = cur->next;
            else       head      = cur->next;

            printf("[SUCCESS] Book '%s' deleted.\n", cur->title);
            free(cur);
            return;
        }
        prev = cur;
        cur  = cur->next;
    }

    printf("[NOT FOUND] No book with ISBN '%s'.\n", isbn);
}

//3. Search a book by ISBN
void searchByISBN(void) {
    char isbn[MAX_ISBN];
    printLine();
    printf("  SEARCH BY ISBN\n");
    printLine();
    readString("  Enter ISBN: ", isbn, MAX_ISBN);

    Book *cur = head;
    while (cur) {
        if (strcmp(cur->isbn, isbn) == 0) {
            printf("[FOUND]\n");
            printBook(cur);
            return;
        }
        cur = cur->next;
    }
    printf("[NOT FOUND] No book with ISBN '%s'.\n", isbn);
}

//4. Search books by author name (partial match)
void searchByAuthor(void) {
    char author[MAX_AUTHOR];
    printLine();
    printf("  SEARCH BY AUTHOR\n");
    printLine();
    readString("  Enter author name (or part): ", author, MAX_AUTHOR);

    int found = 0;
    Book *cur = head;
    while (cur) {
        if (strstr(cur->author, author) != NULL) {
            if (!found) printf("[RESULTS]\n");
            printLine();
            printBook(cur);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found)
        printf("[NOT FOUND] No books by author matching '%s'.\n", author);
}

//5. Display all books

void displayAllBooks(void) {
    if (!head) {
        printf("[INFO] No books in the library.\n");
        return;
    }

    printLine();
    printf("  ALL BOOKS IN LIBRARY\n");
    printLine();

    Book *cur = head;
    int   idx = 1;
    while (cur) {
        printf("  [%d]\n", idx++);
        printBook(cur);
        printLine();
        cur = cur->next;
    }
}

 //6. Update book details by ISBN
void updateBook(void) {
    char isbn[MAX_ISBN];
    printLine();
    printf("  UPDATE BOOK DETAILS\n");
    printLine();
    readString("  Enter ISBN of book to update: ", isbn, MAX_ISBN);

    Book *cur = head;
    while (cur) {
        if (strcmp(cur->isbn, isbn) == 0) {
            printf("  Found: '%s'. Enter new details (press Enter to keep current):\n\n", cur->title);

            char buf[MAX_TITLE];

            printf("  Title [%s]: ", cur->title);
            fgets(buf, MAX_TITLE, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) strcpy(cur->title, buf);

            printf("  Author [%s]: ", cur->author);
            fgets(buf, MAX_AUTHOR, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) strcpy(cur->author, buf);

            printf("  Genre [%s]: ", cur->genre);
            fgets(buf, MAX_GENRE, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (strlen(buf) > 0) strcpy(cur->genre, buf);

            printf("  Year [%d] (0 to keep): ", cur->year);
            int yr;
            scanf("%d", &yr);
            clearInputBuffer();
            if (yr != 0) cur->year = yr;

            printf("[SUCCESS] Book updated.\n");
            return;
        }
        cur = cur->next;
    }
    printf("[NOT FOUND] No book with ISBN '%s'.\n", isbn);
}

//7. Issue a book (mark as borrowed)
void issueBook(void) {
    char isbn[MAX_ISBN];
    printLine();
    printf("  ISSUE BOOK\n");
    printLine();
    readString("  Enter ISBN: ", isbn, MAX_ISBN);

    Book *cur = head;
    while (cur) {
        if (strcmp(cur->isbn, isbn) == 0) {
            if (cur->isIssued) {
                printf("[WARNING] Book '%s' is already issued.\n", cur->title);
            } else {
                cur->isIssued = 1;
                printf("[SUCCESS] Book '%s' has been issued.\n", cur->title);
            }
            return;
        }
        cur = cur->next;
    }
    printf("[NOT FOUND] No book with ISBN '%s'.\n", isbn);
}

 //8. Return a book (mark as available)
void returnBook(void) {
    char isbn[MAX_ISBN];
    printLine();
    printf("  RETURN BOOK\n");
    printLine();
    readString("  Enter ISBN: ", isbn, MAX_ISBN);

    Book *cur = head;
    while (cur) {
        if (strcmp(cur->isbn, isbn) == 0) {
            if (!cur->isIssued) {
                printf("[WARNING] Book '%s' was not issued.\n", cur->title);
            } else {
                cur->isIssued = 0;
                printf("[SUCCESS] Book '%s' has been returned.\n", cur->title);
            }
            return;
        }
        cur = cur->next;
    }
    printf("[NOT FOUND] No book with ISBN '%s'.\n", isbn);
}

//9. Display total book count (total, available, issued)
void displayCount(void) {
    int total = 0, available = 0, issued = 0;
    Book *cur = head;
    while (cur) {
        total++;
        if (cur->isIssued) issued++;
        else               available++;
        cur = cur->next;
    }
    printLine();
    printf("  LIBRARY STATISTICS\n");
    printLine();
    printf("  Total Books   : %d\n", total);
    printf("  Available     : %d\n", available);
    printf("  Issued        : %d\n", issued);
    printLine();
}

//Free all nodes (cleanup)
void freeList(void) {
    Book *cur = head;
    while (cur) {
        Book *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    head = NULL;
}
//Menu
void showMenu(void) {
    printf("\n");
    printLine();
    printf("     LIBRARY BOOK MANAGEMENT SYSTEM\n");
    printLine();
    printf("  1. Add a New Book\n");
    printf("  2. Delete a Book\n");
    printf("  3. Search by ISBN\n");
    printf("  4. Search by Author\n");
    printf("  5. Display All Books\n");
    printf("  6. Update Book Details\n");
    printf("  7. Issue a Book\n");
    printf("  8. Return a Book\n");
    printf("  9. Library Statistics\n");
    printf("  0. Exit\n");
    printLine();
    printf("  Enter choice: ");
}
//Main
int main(void) {
    int choice;

    // Pre-load two sample books for demonstration
    head = (Book *)malloc(sizeof(Book));
    strcpy(head->title,  "The C Programming Language");
    strcpy(head->author, "Brian W. Kernighan & Dennis M. Ritchie");
    strcpy(head->isbn,   "978-0131103627");
    strcpy(head->genre,  "Computer Science");
    head->year     = 1988;
    head->isIssued = 0;
    head->next     = NULL;

    Book *b2 = (Book *)malloc(sizeof(Book));
    strcpy(b2->title,  "Data Structures Using C");
    strcpy(b2->author, "Reema Thareja");
    strcpy(b2->isbn,   "978-0199097166");
    strcpy(b2->genre,  "Computer Science");
    b2->year     = 2014;
    b2->isIssued = 1;   // already issued 
    b2->next     = NULL;
    head->next   = b2;

    do {
        showMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addBook();      break;
            case 2: deleteBook();   break;
            case 3: searchByISBN(); break;
            case 4: searchByAuthor(); break;
            case 5: displayAllBooks(); break;
            case 6: updateBook();   break;
            case 7: issueBook();    break;
            case 8: returnBook();   break;
            case 9: displayCount(); break;
            case 0:
                printf("\n  Goodbye! Freeing memory...\n");
                freeList();
                printf("  All nodes freed. Exiting.\n\n");
                break;
            default:
                printf("[ERROR] Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}