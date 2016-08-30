#ifndef HTTPFORM_H
#define HTTPFORM_H

struct HttpForm
{
    HttpForm();
    ~HttpForm();

    void parse(char *, int);
    void clean();

    char *head;
    int status_code;
    int head_length;
    int content_length;
    char *content;

private:

//    HttpForm & operator = (HttpForm  form){
//        free(head);
//        head = form.head;
//        status_code = form.status_code;
//        content_length = form.content_length;
//        content = form.content;
//        return *this;
//    }

};

#endif // HTTPFORM_H
